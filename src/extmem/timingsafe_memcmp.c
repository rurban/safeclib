/*------------------------------------------------------------------
 * timingsafe_memcmp.c - Compares memory
 *
 * Copyright (c) 2014 Google Inc
 * Copyright (c) 2017 Reini Urban
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *------------------------------------------------------------------
 */

#ifdef FOR_DOXYGEN
#include "safe_mem_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def timingsafe_memcmp(b1,b2,len)
 * @brief
 *    Compare all memory bytes. Return their difference sign
 *    \c (-1,0,1).
 *
 *    The running time is independent of the byte sequences compared, making
 *    them safe to use for comparing secret values such as cryptographic
 *    MACs. In contrast, bcmp(3) and memcmp(3) may short-circuit after finding
 *    the first differing byte.
 *
 * @remark from OpenBSD
 *
 * @param  b1    pointer to memory to compare against
 * @param  b2    pointer to the source memory to compare with b1
 * @param  len   number of bytes to compare in both buffers
 *
 * @pre    Neither b1 nor b2 shall be a null pointer.
 *
 * @retval  -1, 0, or 1
 * @retval  -ESLEMAX  if len > RSIZE_MAX_MEM or > sizeof(b1) or > sizeof(b2)
 *
 * @see
 *    memcmp_s(), timingsafe_bcmp()
 */

#ifdef FOR_DOXYGEN
int timingsafe_memcmp(const void *b1, const void *b2, size_t len)
#else
EXPORT int _timingsafe_memcmp_chk(const void *b1, const void *b2, size_t len,
                                  const size_t destbos, const size_t srcbos)
#endif
{
    const unsigned char *p1 = (const unsigned char *)b1,
                        *p2 = (const unsigned char *)b2;
    size_t i;
    int res = 0, done = 0;

    if (destbos == BOS_UNKNOWN) {
        if (unlikely(len > RSIZE_MAX_MEM)) {
            invoke_safe_mem_constraint_handler("timingsafe_memcmp"
                                               ": n exceeds max",
                                               (void *)b1, ESLEMAX);
            return -ESLEMAX;
        }
        BND_CHK_PTR_BOUNDS(b1, len);
    } else {
        if (unlikely(len > destbos)) {
            invoke_safe_mem_constraint_handler("timingsafe_memcmp"
                                               ": n exceeds b1",
                                               (void *)b1, ESLEMAX);
            return -ESLEMAX;
        }
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(b2, len);
    } else {
        if (unlikely(len > srcbos)) {
            invoke_safe_mem_constraint_handler("timingsafe_memcmp"
                                               ": n exceeds b2",
                                               (void *)b2, ESLEMAX);
            return -ESLEMAX;
        }
    }

    for (i = 0; i < len; i++) {
        /* lt is -1 if p1[i] < p2[i]; else 0. */
        int lt = (p1[i] - p2[i]) >> CHAR_BIT;

        /* gt is -1 if p1[i] > p2[i]; else 0. */
        int gt = (p2[i] - p1[i]) >> CHAR_BIT;

        /* cmp is 1 if p1[i] > p2[i]; -1 if p1[i] < p2[i]; else 0. */
        int cmp = lt - gt;

        /* set res = cmp if !done. */
        res |= cmp & ~done;

        /* set done if p1[i] != p2[i]. */
        done |= lt | gt;
    }

    return (res);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_timingsafe_memcmp_chk);
#endif
