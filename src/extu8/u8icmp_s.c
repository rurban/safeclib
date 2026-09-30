/*------------------------------------------------------------------
 * u8icmp_s.c
 *
 * September 2026, Reini Urban
 *
 * Copyright (c) 2026 by Reini Urban
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
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def u8icmp_s(dest,dmax,src,resultp)
 * @brief
 *    Compares two pre-normalized (NFC) utf-8 identifier strings,
 *    byte by byte.
 * @details
 *    Because both operands are guaranteed to already be in NFC form
 *    (char8i_t, cf. u8icpy_s()), a plain byte comparison of the
 *    utf-8 sequences gives the correct canonical result without any
 *    normalization work at compare time.
 *
 * @param[in]   dest       pointer to utf-8 identifier string to compare
 * @param[in]   dmax       restricted maximum byte-length of string dest
 * @param[in]   src        pointer to the utf-8 identifier string to be
 *                         compared to dest
 * @param[out]  resultp    pointer to int result, greater than 0,
 *                         equal to 0 or less than 0, if the string pointed
 *                         to by dest is greater than, equal to or less
 *                         than the string pointed to by src respectively.
 *
 * @pre   Neither dest, src nor resultp shall be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @return  The error code of the result. On EOK, see resultp.
 * @retval  EOK        when comparison is complete and the result is returned
 *                     in resultp
 * @retval  ESNULLP    when dest, src or resultp is the NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    u8cmp_s(), u8icpy_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8icmp_s(const char8i_t *dest, rsize_t dmax, const char8i_t *src,
                 int *resultp)
#else
EXPORT errno_t _u8icmp_s_chk(const char8i_t *dest, rsize_t dmax,
                             const char8i_t *src, int *resultp,
                             const size_t destbos, size_t srcbos)
#endif
{
    size_t slen;

    CHK_SRC_NULL("u8icmp_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("u8icmp_s")
    CHK_SRC_NULL("u8icmp_s", src)
    CHK_DMAX_ZERO("u8icmp_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8icmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8icmp_s", destbos)
    }

    slen = 0;
    while (*dest && *src && dmax) {

        if (*dest != *src) {
            break;
        }

        dest++;
        src++;
        dmax--;
        slen++;
        /* sentinel srcbos -1 = ULONG_MAX */
        if (unlikely(slen >= srcbos)) {
            invoke_safe_str_constraint_handler("u8icmp_s"
                                               ": src unterminated",
                                               (void *)src, ESUNTERM);
            return RCNEGATE(ESUNTERM);
        }
    }
    *resultp = *dest - *src;
    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_u8icmp_s_chk);
#endif /* __KERNEL__ */
