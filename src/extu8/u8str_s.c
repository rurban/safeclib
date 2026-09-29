/*------------------------------------------------------------------
 * u8str_s.c
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
 * @def u8str_s(dest,dmax,src,slen,substringp)
 * @brief
 *    The u8str_s() function locates the first occurrence of the utf-8
 *    substring pointed to by src in the utf-8 string pointed to by
 *    dest. The search is a literal byte-sequence match, not
 *    normalizing; since UTF-8 is self-synchronizing (no encoded
 *    codepoint is a byte-suffix of another codepoint's encoding), a
 *    byte-sequence match between two well-formed UTF-8 strings always
 *    lands on codepoint boundaries.
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
 *    u8u8_s(), u8spn_s(), u8cspn_s(), u8pbrk_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8str_s(char8_t *dest, rsize_t dmax, const char8_t *src,
                rsize_t slen, char8_t **substringp)
#else
EXPORT errno_t _u8str_s_chk(char8_t *restrict dest, rsize_t dmax,
                            const char8_t *restrict src, rsize_t slen,
                            char8_t **restrict substringp,
                            const size_t destbos, const size_t srcbos)
#endif
{
    rsize_t len;
    rsize_t dlen;
    int i;

    CHK_SRC_NULL("u8str_s", substringp)
    *substringp = NULL;

    CHK_DEST_NULL("u8str_s")
    CHK_SRC_NULL("u8str_s", src)
    CHK_DMAX_ZERO("u8str_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8str_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, slen);
    } else {
        CHK_DEST_OVR("u8str_s", destbos)
    }

    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("u8str_s: slen exceeds dmax",
                                               (void *)src, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
        BND_CHK_PTR_BOUNDS(src, slen);
    } else {
        if (unlikely(slen > srcbos)) {
            if (unlikely(slen > RSIZE_MAX_STR)) {
                invoke_safe_str_constraint_handler(
                    "u8str_s: slen exceeds dmax", (void *)src, ESLEMAX);
                return RCNEGATE(ESLEMAX);
            } else {
                invoke_safe_str_constraint_handler("u8str_s: slen exceeds src",
                                                   (void *)src, EOVERFLOW);
                return RCNEGATE(EOVERFLOW);
            }
        }
    }
    if (unlikely(slen > dmax)) { /* now check the actual lengths */
        len = strlen((const char *)src);
        dlen = strlen((const char *)dest);
        if (len > dmax || len > dlen)
            return RCNEGATE(ESNOTFND);
    }

    /*
     * src points to a string with zero length, or
     * src equals dest, return dest
     */
    if (unlikely(*src == '\0' || dest == src)) {
        *substringp = dest;
        return RCNEGATE(EOK);
    }
    /* allow slen=0 with src="" */
    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("u8str_s: slen is 0", (void *)src,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    while (*dest && dmax) {
        i = 0;
        len = slen;
        dlen = dmax;

        while (src[i] && dlen) {

            /* not a match, not a substring */
            if (dest[i] != src[i]) {
                break;
            }

            /* move to the next byte */
            i++;
            len--;
            dlen--;

            if (src[i] == '\0' || !len) {
                *substringp = dest;
                return RCNEGATE(EOK);
            }
        }
        dest++;
        dmax--;
    }

    /*
     * substring was not found, return NULL
     */
    *substringp = NULL;
    return RCNEGATE(ESNOTFND);
}
