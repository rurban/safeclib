/*------------------------------------------------------------------
 * strcasestr_s.c
 *
 * November 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def strcasestr_s(dest,dmax,src,slen,substring)
 * @brief
 *    The strcasestr_s() function locates the first occurrence of
 *    the substring pointed to by src which would be located in
 *    the string pointed to by dest.  The comparison is case
 *    insensitive.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]  dest       pointer to string to be searched for the substring
 * @param[in]  dmax       restricted maximum length of dest string
 * @param[in]  src        pointer to the substring
 * @param[in]  slen       maximum length of src string
 * @param[out] substring  returned pointer to the substring
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  Neither dmax nor slen shall equal zero.
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_STR and size of
 * dest/src
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dst/src/substring is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOTFND   when substring not found
 *
 * @see
 *    strstr_s(), strprefix_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strcasestr_s(char *dest, rsize_t dmax, const char *src,
                     rsize_t slen, char **substring)
#else
EXPORT errno_t _strcasestr_s_chk(char *dest, rsize_t dmax, const char *src,
                                 rsize_t slen, char **substring,
                                 const size_t destbos, const size_t srcbos)
#endif
{
    rsize_t len;
    rsize_t dlen;
    int i;

    CHK_SRC_NULL("strcasestr_s", substring)
    *substring = NULL;

    CHK_DEST_NULL("strcasestr_s")
    CHK_SRC_NULL("strcasestr_s", src)
    CHK_DMAX_ZERO("strcasestr_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strcasestr_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, slen);
    } else {
        CHK_DEST_OVR("strcasestr_s", destbos)
    }

    if (unlikely(slen > dmax)) {
        errno_t rc = slen > RSIZE_MAX_STR ? ESLEMAX : ESNOTFND;
        invoke_safe_str_constraint_handler("strcasestr_s: slen exceeds dmax",
                                           (void *)dest, rc);
        return RCNEGATE(rc);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen);
    } else {
        if (unlikely(slen > srcbos)) {
            invoke_safe_str_constraint_handler("strcasestr_s: slen exceeds src",
                                               (void *)src, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
    }
    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("strcasestr_s: slen is 0",
                                           (void *)dest, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    /*
     * src points to a string with zero length, or
     * src equals dest, return dest
     */
    if (unlikely(*src == '\0' || dest == src)) {
        *substring = dest;
        return (EOK);
    }

    while (*dest && dmax) {
        i = 0;
        len = slen;
        dlen = dmax;

        while (dest[i] && dlen) {

            /* not a match, not a substring */
            if (toupper((unsigned char)dest[i]) !=
                toupper((unsigned char)src[i])) {
                break;
            }

            /* move to the next char */
            i++;
            len--;
            dlen--;

            if (src[i] == '\0' || !len) {
                *substring = dest;
                return (EOK);
            }
        }
        dest++;
        dmax--;
    }

    /*
     * substring was not found, return NULL
     */
    *substring = NULL;
    return RCNEGATE(ESNOTFND);
}
