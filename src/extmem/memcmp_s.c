/*------------------------------------------------------------------
 * memcmp_s.c - Compares memory
 *
 * October 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 Cisco Systems
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
#include "safe_mem_constraint.h"
#endif

/**
 * @brief
 *    Compares memory until they differ, and their difference sign
 *    \c (-1,0,1) is returned in \c diff.  If the block of memory is the same,
 *    \c *diff=0.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest   pointer to memory to compare against
 * @param  src    pointer to the source memory to compare with dest
 * @param  dmax   maximum length of dest, in bytes
 * @param  smax   length of the source memory block
 * @param  *diff  pointer to the diff which is an integer greater
 *                than, equal to or less than zero according to
 *                whether the object pointed to by dest is
 *                greater than, equal to or less than the object
 *                pointed to by src.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   Neither dmax nor smax shall be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_MEM.
 * @pre   smax shall not be greater than dmax.
 *
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dst/src is NULL POINTER
 * @retval  ESZEROL     when dmax/smax = ZERO
 * @retval  ESLEMAX     when dmax/smax > RSIZE_MAX_MEM
 * @retval  ESNOSPC     when dmax < smax
 *
 * @see
 *    memcmp16_s(), memcmp32_s()
 *
 */

EXPORT errno_t
memcmp_s (const void *dest, rsize_t dmax,
          const void *src,  rsize_t smax, int *diff)
{
    const uint8_t *dp;
    const uint8_t *sp;

    dp = (uint8_t*) dest;
    sp = (uint8_t*) src;

    /*
     * must be able to return the diff
     */
    if (unlikely(diff == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp_s: diff is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }
    *diff = -1;  /* default diff */

    if (unlikely(dp == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(sp == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp_s: src is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("memcmp_s: dmax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(dmax > RSIZE_MAX_MEM || smax > RSIZE_MAX_MEM)) {
        invoke_safe_mem_constraint_handler("memcmp_s: dmax/smax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(smax == 0)) {
        invoke_safe_mem_constraint_handler("memcmp_s: smax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(smax > dmax)) {
        invoke_safe_mem_constraint_handler("memcmp_s: smax exceeds dmax",
                   NULL, ESNOSPC);
        return (RCNEGATE(ESNOSPC));
    }

    /*
     * no need to compare the same memory
     */
    if (unlikely(dp == sp)) {
        *diff = 0;
        return (RCNEGATE(EOK));
    }

    /*
     * now compare sp to dp
     */
    *diff = 0;
    while (dmax > 0 && smax > 0) {
        if (*dp != *sp) {
            /***  *diff = *dp - *sp; ***/
            *diff = *dp < *sp ? -1 : 1;
            break;
        }

        dmax--;
        smax--;

        dp++;
        sp++;
    }

    return (RCNEGATE(EOK));
}
#ifdef __KERNEL__
EXPORT_SYMBOL(memcmp_s);
#endif /* __KERNEL__ */
