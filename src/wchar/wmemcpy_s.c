/*------------------------------------------------------------------
 * wmemcpy_s
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2017 Reini Urban
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

#include "safeclib_private.h"
#ifdef HAVE_WCHAR_H

#include "safe_mem_constraint.h"
#include "mem_primitives_lib.h"

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

/**
 * @brief 
 *    This function copies at most smax wchar_t's from src to dest, up to
 *    dmax.
 *
 * @remark EXTENSION TO
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.2.1.3 The wmemcpy_s function (p: 641)
 *    http://en.cppreference.com/w/c/string/wide/wmemcpy_s
 *    * ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest   pointer to the memory that will be replaced by src.
 * @param[in]  dmax   maximum length of the resulting dest, in wchar_t
 * @param[in]  src    pointer to the memory that will be copied to dest
 * @param[in]  smax   number of wide characters to copy
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   Neither dmax nor smax shall be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_MEM16.
 * @pre   smax shall not be greater than dmax.
 * @pre   Copying shall not take place between regions that overlap.
 *    
 * @return  If there is a runtime-constraint violation, the memcpy_s function
 *          stores zeros in the ﬁrst dmax bytes of the region pointed to
 *          by dest if dest is not a null pointer and smax is valid.
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dst/src is NULL POINTER
 * @retval  ESZEROL     when dmax/smax = ZERO
 * @retval  ESLEMAX     when dmax/smax > RSIZE_MAX_MEM16
 * @retval  ESNOSPC     when dmax < smax
 * @retval  ESOVRLP     when src memory overlaps dst
 *
 * @see 
 *    memcpy_s(), memcpy16_s(), wmemmove_s(), memmove16_s()
 *
 */

EXPORT errno_t
wmemcpy_s (wchar_t *dest, rsize_t dmax, const wchar_t *src, rsize_t smax)
{
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

    if (unlikely(dmax > RSIZE_MAX_MEM16 || smax > RSIZE_MAX_MEM16)) {
        invoke_safe_mem_constraint_handler("wmemcpy_s: dmax/smax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(smax == 0)) {
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemcpy_s: smax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(smax > dmax)) {
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemcpy_s: smax exceeds dmax",
                   NULL, ESNOSPC);
        return (RCNEGATE(ESNOSPC));
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
    if (unlikely( ((dest > src) && (dest < (src+smax))) ||
                  ((src > dest) && (src < (dest+dmax))) )) {
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemcpy_s: overlap undefined",
                   NULL, ESOVRLP);
        return (RCNEGATE(ESOVRLP));
    }

    /*
     * now perform the copy
     */
    wmem_move((wmem_type*)dest, (wmem_type*)src, (uint32_t)smax);

    return (RCNEGATE(EOK));
}
EXPORT_SYMBOL(wmemcpy_s)

#endif /* HAVE_WCHAR_H */
