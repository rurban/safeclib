/*------------------------------------------------------------------
 * timingsafe_bcmp.c - Compares memory
 *
 * Copyright (c) 2010 Damien Miller.  All rights reserved.
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @brief
 *    Compare all memory bytes. Return 0 or not zero.
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
 * @param  n     number of bytes to compare in both buffers
 *
 * @pre     Neither b1 nor b2 shall be a null pointer.
 *
 * @retval  0 if the buffers are equal, or not zero if they are not equal.
 *
 * @see 
 *    memcmp_s(), timingsafe_memcmp()
 *
 */

EXPORT int
timingsafe_bcmp (const void *b1, const void *b2, size_t n)
{
    const unsigned char *p1 = (const unsigned char *)b1, *p2 = (const unsigned char *)b2;
    int ret = 0;

    for (; n > 0; n--)
        ret |= *p1++ ^ *p2++;
    return (ret != 0);
}
EXPORT_SYMBOL(timingsafe_bcmp)
