/*------------------------------------------------------------------
 * u8ifcu8_s.c
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
 * @def u8ifcu8_s(dest,dmax,src,slen,substring)
 * @brief
 *    Locates the first occurrence of the pre-normalized utf-8
 *    identifier substring src in the pre-normalized utf-8 identifier
 *    dest, comparing fold-cased.
 * @details
 *    Because both operands are guaranteed to already be in NFC form
 *    (char8i_t, cf. u8icpy_s()), no extra normalization work is done
 *    before the fold-cased search; see u8fcu8_s() for the exact
 *    fold-casing semantics and its NFC-canonicalization caveat.
 *
 * @param[in]   dest       pointer to utf-8 identifier string to be searched
 * @param[in]   dmax       restricted maximum byte-length of dest string
 * @param[in]   src        pointer to the utf-8 identifier sub string
 * @param[in]   slen       the maximum number of bytes to use from src
 * @param[out]  substring  the returned substring pointer, into dest
 *
 * @pre  Neither dest, src nor substring shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  slen shall not be 0.
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_STR and size of
 * dest/src.
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dest/src/substring is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  EILSEQ     when dest/src contains illegal utf-8
 * @retval  ESNOTFND   when substring not found
 *
 * @see
 *    u8fcu8_s(), u8icmp_s(), u8iu8i_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8ifcu8_s(char8i_t *dest, rsize_t dmax, const char8i_t *src,
                  rsize_t slen, char8i_t **substring)
#else
EXPORT errno_t _u8ifcu8_s_chk(char8i_t *dest, rsize_t dmax, const char8i_t *src,
                              rsize_t slen, char8i_t **substring,
                              const size_t destbos, const size_t srcbos)
#endif
{
    return _u8fcu8_s_chk((char8_t *)dest, dmax, (const char8_t *)src, slen,
                         (char8_t **)substring, destbos, srcbos);
}
