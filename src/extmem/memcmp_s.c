/*------------------------------------------------------------------
 * memcmp_s.c - Compares memory
 *
 * October 2008, Bo Berry
 * February 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2017-2018 by Reini Urban
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
#include "mem/safe_mem_constraint.h"
#endif

/**
 * @def memcmp_s(dest,dmax,src,slen,diff)
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
 * @param  slen   length of the source memory block
 * @param  *diff  pointer to the diff which is an integer greater
 *                than, equal to or less than zero according to
 *                whether the object pointed to by dest is
 *                greater than, equal to or less than the object
 *                pointed to by src.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   Neither dmax nor slen shall be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_MEM or size of dest
 * @pre   slen shall not be greater than dmax or size of src
 *
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dst/src is NULL POINTER
 * @retval  ESZEROL     when dmax/slen = ZERO
 * @retval  ESLEMAX     when dmax/slen > RSIZE_MAX_MEM
 * @retval  EOVERFLOW   when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG    when dmax/slen != size of dest/src and
 * --enable-error-dmax
 * @retval  ESNOSPC     when dmax < slen
 *
 * @see
 *    memcmp16_s(), memcmp32_s()
 *
 */

#ifdef FOR_DOXYGEN
errno_t memcmp_s(const void *dest, rsize_t dmax, const void *src,
                 rsize_t slen, int *diff)
#else
EXPORT errno_t _memcmp_s_chk(const void *dest, rsize_t dmax, const void *src,
                             rsize_t slen, int *diff, const size_t destbos,
                             const size_t srcbos)
#endif
{
    const uint8_t *dp;
    const uint8_t *sp;

    dp = (uint8_t *)dest;
    sp = (uint8_t *)src;

    /* must be able to return the diff */
    if (unlikely(diff == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp_s: diff is null", NULL,
                                           ESNULLP);
        return (RCNEGATE(ESNULLP));
    }
    *diff = -1; /* default diff */

    if (unlikely(dp == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp_s: dest is null", NULL,
                                           ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(sp == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp_s: src is null",
                                           (void *)dest, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("memcmp_s: dmax is 0", (void *)dest,
                                           ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (destbos == BOS_UNKNOWN) {
        if (unlikely(dmax > RSIZE_MAX_MEM)) {
            invoke_safe_mem_constraint_handler("memcmp_s: dmax exceeds max",
                                               (void *)dest, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, slen);
    } else {
        if (unlikely(dmax > destbos)) {
            if (unlikely(dmax > RSIZE_MAX_MEM)) {
                invoke_safe_mem_constraint_handler("memcmp_s: dmax exceeds max",
                                                   (void *)dest, ESLEMAX);
                return (RCNEGATE(ESLEMAX));
            } else {
                invoke_safe_mem_constraint_handler(
                    "memcmp_s: dmax exceeds dest", (void *)dest, EOVERFLOW);
                return (RCNEGATE(EOVERFLOW));
            }
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(dmax != destbos)) {
            handle_mem_bos_chk_warn("memcmp_s", (void *)dest, dmax, destbos);
            RETURN_ESLEWRNG;
        }
#endif
    }

    if (unlikely(slen == 0)) {
        invoke_safe_mem_constraint_handler("memcmp_s: slen is 0", NULL,
                                           ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_MEM)) {
            invoke_safe_mem_constraint_handler("memcmp_s: slen exceeds max",
                                               (void *)src, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
        BND_CHK_PTR_BOUNDS(src, slen);
    } else {
        if (unlikely(slen > srcbos)) {
            if (unlikely(slen > RSIZE_MAX_MEM)) {
                invoke_safe_mem_constraint_handler("memcmp_s: slen exceeds max",
                                                   (void *)dest, ESLEMAX);
                return (RCNEGATE(ESLEMAX));
            } else {
                invoke_safe_mem_constraint_handler("memcmp_s: slen exceeds src",
                                                   (void *)src, EOVERFLOW);
                return (RCNEGATE(EOVERFLOW));
            }
        }
    }

    if (unlikely(slen > dmax)) {
        invoke_safe_mem_constraint_handler("memcmp_s: slen exceeds dmax",
                                           (void *)dest, ESNOSPC);
        return (RCNEGATE(ESNOSPC));
    }

    /* no need to compare the same memory */
    if (unlikely(dp == sp)) {
        *diff = 0;
        return (RCNEGATE(EOK));
    }

    /* now compare sp to dp */
    *diff = 0;
    while (dmax > 0 && slen > 0) {
        if (*dp != *sp) {
            /***  *diff = *dp - *sp; ***/
            *diff = *dp < *sp ? -1 : 1;
            break;
        }

        dmax--;
        slen--;

        dp++;
        sp++;
    }

    return (RCNEGATE(EOK));
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_memcmp_s_chk);
#endif
