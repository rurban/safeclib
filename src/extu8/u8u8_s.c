/*------------------------------------------------------------------
 * u8u8_s.c
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
 * @def u8u8_s(dest,dmax,src,slen,substringp)
 * @brief
 *    The u8u8_s() function locates the first occurrence of the utf-8
 *    substring pointed to by src in the utf-8 string pointed to by
 *    dest.
 * @details
 *    Performs the same literal byte-sequence search as u8str_s().
 *    UTF-8 is self-synchronizing, so a literal match between
 *    well-formed UTF-8 strings always lands on codepoint boundaries.
 *
 * @param[in]   dest       pointer to utf-8 string to be searched for the
 *                         substring
 * @param[in]   dmax       restricted maximum byte-length of dest string
 * @param[in]   src        pointer to the utf-8 sub string
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
 *    u8str_s(), u8norm_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8u8_s(char8_t *dest, rsize_t dmax, const char8_t *src, rsize_t slen,
               char8_t **substringp)
#else
EXPORT errno_t _u8u8_s_chk(char8_t *dest, rsize_t dmax, const char8_t *src,
                           rsize_t slen, char8_t **substringp,
                           const size_t destbos, const size_t srcbos)
#endif
{
    return _u8str_s_chk(dest, dmax, src, slen, substringp, destbos, srcbos);
}
