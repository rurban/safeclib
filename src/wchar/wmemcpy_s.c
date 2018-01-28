/*------------------------------------------------------------------
 * wmemcpy_s
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
 * January 2018, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2017,2018 Reini Urban
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#ifdef FOR_DOXYGEN
#include "safe_mem_lib.h"
#else
#include "safeclib_private.h"
#include "mem/mem_primitives_lib.h"
#endif

#ifdef HAVE_WCHAR_H

#if SIZEOF_WCHAR_T == 2
#define wmem_type uint16_t
#define wmem_set  mem_prim_set16
#define wmem_move mem_prim_move16
#elif SIZEOF_WCHAR_T == 4
#define wmem_type uint32_t
#define wmem_set  mem_prim_set32
#define wmem_move mem_prim_move32
#else
#error sizeof(wchar_t)?
#endif

#if defined(TEST_MSVCRT) && defined(HAVE_WMEMCPY_S)
#else

/**
 * @brief
 *    This function copies at most count wchar_t's from src to dest, up to
 *    dmax. If count is zero, the function does nothing.
 *
 * @remark EXTENSION TO
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.2.1.3 The wmemcpy_s function (p: 641)
 *    http://en.cppreference.com/w/c/string/wide/wmemcpy
 *    * ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest   pointer to the memory that will be replaced by src.
 * @param[in]  dmax   maximum length of the resulting dest, in wchar_t
 * @param[in]  src    pointer to the memory that will be copied to dest
 * @param[in]  count  number of wide characters to copy
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   dmax shall not be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_WMEM.
 * @pre   count shall not be greater than dmax.
 * @pre   Copying shall not take place between regions that overlap.
 *
 * @return  If there is a runtime-constraint violation, the memcpy_s function
 *          stores zeros in the first dmax bytes of the region pointed to
 *          by dest if dest is not a null pointer and count is valid.
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest or src is a NULL POINTER
 * @retval  ESZEROL     when dmax = ZERO
 * @retval  ESLEMAX     when dmax/count > RSIZE_MAX_WMEM
 * @retval  ESNOSPC     when dmax < count
 * @retval  ESOVRLP     when src memory overlaps dst
 *
 * @see
 *    memcpy_s(), memcpy16_s(), wmemmove_s(), memmove16_s()
 *
 */

EXPORT errno_t
wmemcpy_s (wchar_t *dest, rsize_t dmax, const wchar_t *src, rsize_t count)
{
    if (unlikely(count == 0)) {
        return (RCNEGATE(EOK));
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("wmemcpy_s: dest is NULL",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("wmemcpy_s: dmax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(dmax > RSIZE_MAX_WMEM)) {
        invoke_safe_mem_constraint_handler("wmemcpy_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(count > dmax)) {
        errno_t rc = count > RSIZE_MAX_WMEM ? ESLEMAX : ESNOSPC;
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemcpy_s: count exceeds dmax",
                   NULL, rc);
        return (RCNEGATE(rc));
    }

    if (unlikely(src == NULL)) {
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemcpy_s: src is NULL",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    /*
     * overlap is undefined behavior, do not allow
     */
    if (unlikely( ((dest > src) && (dest < (src+count))) ||
                  ((src > dest) && (src < (dest+dmax))) )) {
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemcpy_s: overlap undefined",
                   NULL, ESOVRLP);
        return (RCNEGATE(ESOVRLP));
    }

    /*
     * now perform the copy
     */
    wmem_move((wmem_type*)dest, (wmem_type*)src, (uint32_t)count);

    return (RCNEGATE(EOK));
}

#endif /* TEST_MSVCRT */
#endif /* HAVE_WCHAR_H */
