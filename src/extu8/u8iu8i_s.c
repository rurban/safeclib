/*------------------------------------------------------------------
 * u8iu8i_s.c
 *
 * September 2026
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
 * @def u8iu8i_s(dest,dmax,src,slen,substringp)
 * @brief
 *    Locates the first occurrence of the pre-normalized utf-8
 *    identifier substring src in the pre-normalized utf-8 identifier
 *    dest.
 * @details
 *    Because both operands are guaranteed to already be in NFC form
 *    (char8i_t, cf. u8icpy_s()), and canonically equivalent NFC
 *    strings are byte-identical, a literal byte-sequence search (the
 *    same search u8u8_s()/u8str_s() perform) gives the correct
 *    canonical result without any normalization work at search time.
 *
 * @param[in]   dest       pointer to utf-8 identifier string to be
 *                         searched for the substring
 * @param[in]   dmax       restricted maximum byte-length of dest string
 * @param[in]   src        pointer to the utf-8 identifier sub string
 * @param[in]   slen       the maximum number of bytes to use from src
 * @param[out]  substringp the returned substring pointer
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  slen shall not be 0, when *src != 0 and src != dest
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_STR and size of
 * dest/src.
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dest/src/substring is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0, unless *src = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESNOTFND   when substring not found
 *
 * @see
 *    u8u8_s(), u8str_s(), u8icmp_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8iu8i_s(char8i_t *dest, rsize_t dmax, const char8i_t *src,
                 rsize_t slen, char8i_t **substringp)
#else
EXPORT errno_t _u8iu8i_s_chk(char8i_t *dest, rsize_t dmax, const char8i_t *src,
                             rsize_t slen, char8i_t **substringp,
                             const size_t destbos, const size_t srcbos)
#endif
{
    return _u8str_s_chk((char8_t *)dest, dmax, (const char8_t *)src, slen,
                        (char8_t **)substringp, destbos, srcbos);
}
