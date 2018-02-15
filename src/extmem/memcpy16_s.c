/*------------------------------------------------------------------
 * memcpy16_s
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
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

#ifdef FOR_DOXYGEN
#include "safe_mem_lib.h"
#else
#include "safeclib_private.h"
#include "mem/mem_primitives_lib.h"
#endif

/**
 * @brief
 *    This function copies at most count uint16_t's from src to dest, up to
 *    dmax bytes.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest   pointer to the memory that will be replaced by src.
 * @param[in]  dmax   maximum length of the resulting dest, in bytes
 * @param[in]  src    pointer to the memory that will be copied to dest
 * @param[in]  count  number of uint16_t's to be copied
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   dmax shall not be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_MEM.
 * @pre   count shall not be greater than dmax/2.
 * @pre   Copying shall not take place between regions that overlap.
 *
 * @return  If there is a runtime-constraint violation, the memcpy_s function
 *          stores zeros in the first dmax bytes of the region pointed to
 *          by dest if dest and dmax are valid.
 * @retval  EOK         when operation is successful or count = 0
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dmax = ZERO
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_MEM
 * @retval  ESLEMAX     when count > RSIZE_MAX_MEM16
 * @retval  ESNOSPC     when count*2 > dmax
 * @retval  ESOVRLP     when src memory overlaps dest
 *
 * @see
 *    wmemcpy_s(), memcpy_s(), memcpy32_s(), wmemmove_s(), memmove16_s()
 *
 */
EXPORT errno_t
memcpy16_s (uint16_t *dest, rsize_t dmax, const uint16_t *src, rsize_t count)
{
    const uint16_t  *sp;

    if (unlikely(count == 0)) {
        /* Since C11 count=0 is allowed */
        return EOK;
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("memcpy16_s: dest is NULL",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("memcpy16_s: dmax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(dmax > RSIZE_MAX_MEM)) {
        invoke_safe_mem_constraint_handler("memcpy16_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(count > dmax/2)) {
        errno_t rc = count > RSIZE_MAX_MEM16 ? ESLEMAX : ESNOSPC;
        mem_prim_set(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy16_s: count*2 exceeds dmax",
                   NULL, rc);
        return (RCNEGATE(rc));
    }

    if (unlikely(src == NULL)) {
        mem_prim_set(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy16_s: src is NULL",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    /*
     * overlap is undefined behavior, do not allow
     */
    if (unlikely( ((dest > src) && (dest < (src+count))) ||
                  ((src > dest) && (src < (dest+dmax/2))) )) {
        mem_prim_set(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy16_s: overlap undefined",
                   NULL, ESOVRLP);
        return (RCNEGATE(ESOVRLP));
    }
    BND_CHK_PTR_BOUNDS(dest, count);
    BND_CHK_PTR_BOUNDS(src, count);

    sp = src;

    /*
     * now perform the copy
     */
    mem_prim_move16(dest, sp, count);

    return (RCNEGATE(EOK));
}
