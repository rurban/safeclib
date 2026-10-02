/*------------------------------------------------------------------
 * u8chr_s.c
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
 * @def u8chr_s(dest,dmax,ch,resultp)
 * @brief
 *    Finds the first occurrence of the byte ch (after conversion to
 *    char8_t as if by (char8_t)ch) in the null-terminated utf-8 string
 *    pointed to by dest. The terminating NUL byte is considered to be
 *    a part of the string and can be found when searching for '\0'.
 *    ch is a single byte (0-255), not a decoded Unicode codepoint; to
 *    search for an ASCII character this is exactly what you want, since
 *    ASCII bytes never appear as part of a multi-byte UTF-8 sequence.
 *
 * @param[in]  dest    pointer to utf-8 string to search
 * @param[in]  dmax    restricted maximum byte-length of dest
 * @param[in]  ch      byte value to search for (0-255)
 * @param[out] resultp pointer to char8_t* in dest
 *
 * @pre  Neither dest nor resultp shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 * @pre  ch shall not be greater than 255
 *
 * @return  The error code of the result. On EOK, see resultp.
 * @retval  EOK        when successfully byte found.
 * @retval  ESNOTFND   when ch not found in dest
 * @retval  ESNULLP    when dest or resultp is the NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, or ch > 255
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 *
 * @see
 *    u8rchr_s(), strchr_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8chr_s(const char8_t *restrict dest, rsize_t dmax, const int ch,
                char8_t **restrict resultp)
#else
EXPORT errno_t _u8chr_s_chk(const char8_t *restrict dest, rsize_t dmax,
                            const int ch, char8_t **restrict resultp,
                            const size_t destbos)
#endif
{
    CHK_SRC_NULL("u8chr_s", resultp)
    *resultp = NULL;

    CHK_DEST_NULL("u8chr_s")
    CHK_DMAX_ZERO("u8chr_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8chr_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8chr_s", destbos)
    }

    if (unlikely(ch > 255)) {
        invoke_safe_str_constraint_handler("u8chr_s: ch exceeds max",
                                           (void *)dest, ESLEMAX);
        return (ESLEMAX);
    }

    return _u8chr_s_uchk(dest, dmax, ch, resultp);
}

#ifndef FOR_DOXYGEN
/* The constraints are already checked by _u8chr_s_chk or proven at
   compile-time. GH #48 */
EXPORT errno_t _u8chr_s_uchk(const char8_t *dest, rsize_t dmax, const int ch,
                             char8_t **resultp) {
    *resultp = (char8_t *)strchr((const char *)dest, ch);

    if (!*resultp)
        return (ESNOTFND);
    else if ((long)((char8_t *)*resultp - dest) > (long)dmax) {
        *resultp = NULL;
        return (ESNOTFND);
    }
    return (EOK);
}
#endif
