/*------------------------------------------------------------------
 * memcmp16_s.c - Compares memory
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
#endif

/**
 * @def memcmp16_s(dest,dlen,src,slen,diff)
 * @brief
 *    Compares memory in uint16_t slices until they differ, and their
 *    difference is returned in diff.  If the block of memory is the
 *    same, diff=0.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest   pointer to memory to compare against
 * @param  dlen   maximum length of dest, in uint16_t's
 * @param  src    pointer to the source memory to compare with dest
 * @param  slen   number of uint16_t's from source to be compared
 * @param  *diff  pointer to the diff which is an integer greater
 *                than, equal to or less than zero according to
 *                whether the object pointed to by dest is
 *                greater than, equal to or less than the object
 *                pointed to by src.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   Neither dlen nor slen shall be 0.
 * @pre   dlen shall not be greater than RSIZE_MAX_MEM16 and sizeof(dest)/2
 * @pre   slen shall not be greater than RSIZE_MAX_MEM16 and sizeof(src)/2
 * @pre   slen shall not be greater than dlen and sizeof(dest)/2
 *
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dlen/slen = ZERO
 * @retval  ESLEMAX     when dlen/slen > RSIZE_MAX_MEM16
 * @retval  EOVERFLOW   when 2*dlen/slen > size of dest/src (optionally, when
 * the compiler knows the object_size statically)
 * @retval  ESLEWRNG    when 2*dlen/slen != sizeof(dest/src) and
 * --enable-error-dmax
 * @retval  ESNOSPC     when dlen < slen
 *
 * @see
 *    memcmp_s(), memcmp32_s()
 *
 */

#ifdef FOR_DOXYGEN
errno_t memcmp16_s(const uint16_t *dest, rsize_t dlen,
                   const uint16_t *src, rsize_t slen, int *diff)
#else
EXPORT errno_t _memcmp16_s_chk(const uint16_t *dest, rsize_t dlen,
                               const uint16_t *src, rsize_t slen, int *diff,
                               const size_t destbos, const size_t srcbos)
#endif
{
    size_t dmax; /* in bytes */
    size_t smax; /* in bytes */

    /* must be able to return the diff */
    if (unlikely(diff == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp16_s: diff is null",
                                           (void *)dest, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }
    *diff = -1; /* default diff */

    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp16_s: dest is null", NULL,
                                           ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(src == NULL)) {
        invoke_safe_mem_constraint_handler("memcmp16_s: src is null",
                                           (void *)dest, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dlen == 0)) {
        invoke_safe_mem_constraint_handler("memcmp16_s: dlen is 0",
                                           (void *)dest, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    dmax = dlen * 2;
    smax = slen * 2;
    if (destbos == BOS_UNKNOWN) {
        if (unlikely(dmax > RSIZE_MAX_MEM16)) {
            invoke_safe_mem_constraint_handler("memcmp16_s: dlen exceeds max",
                                               (void *)dest, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, smax);
    } else {
        if (unlikely(dmax > destbos)) {
            if (unlikely(dmax > RSIZE_MAX_MEM16)) {
                invoke_safe_mem_constraint_handler(
                    "memcmp16_s: dlen exceeds max", (void *)dest, ESLEMAX);
                return (RCNEGATE(ESLEMAX));
            } else {
                invoke_safe_mem_constraint_handler(
                    "memcmp16_s: dlen exceeds dest", (void *)dest, EOVERFLOW);
                return (RCNEGATE(EOVERFLOW));
            }
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(dmax != destbos)) {
            handle_mem_bos_chk_warn("memcmp16_s", (void *)dest, dmax, destbos);
            RETURN_ESLEWRNG;
        }
#endif
    }

    if (unlikely(slen == 0)) {
        invoke_safe_mem_constraint_handler("memcmp16_s: slen is 0",
                                           (void *)dest, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_MEM16)) {
            invoke_safe_mem_constraint_handler("memcmp16_s: slen exceeds max",
                                               (void *)src, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
        BND_CHK_PTR_BOUNDS(src, smax);
    } else {
        if (unlikely(smax > srcbos)) {
            if (unlikely(slen > RSIZE_MAX_MEM16)) {
                invoke_safe_mem_constraint_handler(
                    "memcmp16_s: slen exceeds max", (void *)src, ESLEMAX);
                return (RCNEGATE(ESLEMAX));
            } else {
                invoke_safe_mem_constraint_handler(
                    "memcmp16_s: slen exceeds src", (void *)src, EOVERFLOW);
                return (RCNEGATE(EOVERFLOW));
            }
        }
    }

    if (unlikely(slen > dlen)) {
        invoke_safe_mem_constraint_handler("memcmp16_s: slen exceeds dlen",
                                           (void *)dest, ESNOSPC);
        return (RCNEGATE(ESNOSPC));
    }

    /*
     * no need to compare the same memory
     */
    if (unlikely(dest == src)) {
        *diff = 0;
        return (RCNEGATE(EOK));
    }

    /*
     * now compare sp to dp
     */
    *diff = 0;
    while (dlen != 0 && slen != 0) {
        if (*dest != *src) {
            *diff = *dest - *src; /* in units of int16 */
            break;
        }

        dlen--;
        slen--;

        dest++;
        src++;
    }

    return (RCNEGATE(EOK));
}
