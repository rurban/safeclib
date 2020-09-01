/*------------------------------------------------------------------
 * wcsstr_s.c
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
 * @def wcsstr_s(dest,dmax,src,slen,substringp)
 * @brief
 *    The \b wcsstr_s() function locates the first occurrence of the
 *    wide substring pointed to by \c src which would be located in the
 *    wide string pointed to by \c dest.
 *
 * @remark EXTENSION TO
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest        wide string to be searched for the substring
 * @param[in]   dmax        restricted maximum length of dest
 * @param[in]   src         pointer to the wide sub string
 * @param[in]   slen        the maximum number of wide characters to use from
 * src
 * @param[out]  substringp  the returned substring pointer
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dmax/ shall not be 0
 * @pre  slen shall not be 0, when *src != 0 and src != dest
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_WSTR and size of
 * dest/src.
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dest/src/substringp is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_WSTR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOTFND   when substring not found
 *
 * @see
 *     wcsstr(), strstr_s(), memcmp32_s()
 */
#ifdef FOR_DOXYGEN
errno_t wcsstr_s(wchar_t *restrict dest, rsize_t dmax,
                 const wchar_t *restrict src, rsize_t slen,
                 wchar_t **restrict substringp)
#else
EXPORT errno_t _wcsstr_s_chk(wchar_t *restrict dest, rsize_t dmax,
                             const wchar_t *restrict src, rsize_t slen,
                             wchar_t **restrict substringp,
                             const size_t destbos, const size_t srcbos)
#endif
{
    rsize_t len;
    rsize_t dlen;
    const size_t destsz = dmax * sizeof(wchar_t);
    int i;

    CHK_SRC_NULL("wcsstr_s", substringp)
    *substringp = NULL;

    CHK_DEST_NULL("wcsstr_s")
    CHK_SRC_NULL("wcsstr_s", src)
    CHK_DMAX_ZERO("wcsstr_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("wcsstr_s", RSIZE_MAX_WSTR)
        BND_CHK_PTR_BOUNDS(dest, destsz);
        BND_CHK_PTR_BOUNDS(dest, slen * sizeof(wchar_t));
    } else {
        CHK_DESTW_OVR("wcsstr_s", destsz, destbos)
    }

    /*
     * src points to a string with zero length, or
     * src equals dest, return dest
     */
    if (unlikely(*src == '\0' || dest == src)) {
        *substringp = dest;
        return RCNEGATE(EOK);
    }

    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("wcsstr_s: slen is 0", (void *)dest,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }
    if (unlikely(slen > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcsstr_s: slen exceeds max",
                                           (void *)dest, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen * sizeof(wchar_t));
    } else {
        if (unlikely(slen * sizeof(wchar_t) > srcbos)) {
            invoke_safe_str_constraint_handler("wcsstr_s: slen exceeds src",
                                               (void *)dest, EOVERFLOW);
            return RCNEGATE(EOVERFLOW);
        }
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

            /* move to the next char */
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
