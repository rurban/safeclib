/*------------------------------------------------------------------
 * wcscat_s.c
 *
 * September 2017, Reini Urban
 *
 * Copyright (c) 2017 by Reini Urban
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

#ifdef HAVE_WCHAR_H

/**
 * @def wcscat_s(dest,dmax,src)
 * @brief
 *    The wcscat_s function appends a copy of the wide string pointed
 *    to by src (including the terminating null character) to the
 *    end of the wide string pointed to by dest. The initial wide character
 *    from src overwrites the null character at the end of dest.
 * @details
 *    All elements following the terminating null wide character (if
 *    any) written by wcscat_s in the array of dmax characters
 *    pointed to by dest take unspeciﬁed values when wcscat_s
 *    returns.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with
 *    0.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.2.2.1 The wcscat_s function (p: 642-643)
 *    http://en.cppreference.com/w/c/string/wide/wcscat
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest      pointer to wide string that will be extended by src
 *                        if dmax allows. The wide string is null terminated.
 *                        If the resulting concatenated wide string is less
 *                        than dmax, the remaining slack space is nulled.
 * @param[in]   dmax      restricted maximum wchar_t length of the resulting
 * dest, including the null
 * @param[in]   src       pointer to the wide string that will be concatenaed
 *                        to string dest
 *
 * @pre  Neither dest nor src shall be a null pointer
 * @pre  dmax shall not equal zero
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR and size of dest
 * @pre  dmax shall be greater than wcsnlen_s(src,m).
 * @pre  Copying shall not take place between objects that overlap
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest is
 *          not a null pointer and dmax is greater than zero and not
 *          greater than RSIZE_MAX_WSTR, then wcscat_s nulls dest.
 * @retval  EOK        when successful operation, all the wide characters from
 * src were appended to dest and the result in dest is null terminated.
 * @retval  ESNULLP    when dest or src is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESUNTERM   when dest not terminated in the first dmax wide
 * characters
 * @retval  ESOVRLP    when src overlaps with dest
 *
 * @see
 *    wcscat_s(), strcpy_s(), strncpy_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t wcscat_s(wchar_t *restrict dest, rsize_t dmax, const wchar_t *restrict src)
#else
EXPORT errno_t _wcscat_s_chk(wchar_t *restrict dest, rsize_t dmax,
                             const wchar_t *restrict src,
                             const size_t destbos)
#endif
{
    rsize_t orig_dmax;
    wchar_t *orig_dest;
    const wchar_t *overlap_bumper;
    const size_t destsz = dmax * sizeof(wchar_t);

    CHK_DEST_NULL("wcscat_s")
    CHK_DMAX_ZERO("wcscat_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("wcscat_s", RSIZE_MAX_WSTR)
        BND_CHK_PTR_BOUNDS(dest, destsz);
    } else {
        CHK_DESTW_OVR("wcscat_s", destsz, destbos)
    }
    CHK_SRCW_NULL_CLEAR("wcscat_s", src)

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        /* Find the end of dest */
        while (*dest != L'\0') {

            if (unlikely(dest == overlap_bumper)) {
                handle_werror(orig_dest, orig_dmax,
                              "wcscat_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_werror(orig_dest, orig_dmax,
                              "wcscat_s: "
                              "dest unterminated",
                              ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_werror(orig_dest, orig_dmax,
                              "wcscat_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            *dest = *src;
            if (unlikely(*dest == L'\0')) {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear any data */
                if (dmax > 0x20)
                    memset(dest, 0, dmax * sizeof(wchar_t));
                else {
                    while (dmax) {
                        *dest = L'\0';
                        dmax--;
                        dest++;
                    }
                }
#endif
                return RCNEGATE(EOK);
            }

            dmax--;
            dest++;
            src++;
        }
    } else {
        overlap_bumper = dest;

        /* Find the end of dest */
        while (*dest != L'\0') {

            /*
             * NOTE: no need to check for overlap here since src comes first
             * in memory and we're not incrementing src here.
             */
            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_werror(orig_dest, orig_dmax,
                              "wcscat_s: "
                              "dest unterminated",
                              ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_werror(orig_dest, orig_dmax,
                              "wcscat_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            *dest = *src;
            if (*dest == L'\0') {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear any data */
                if (dmax > 0x20)
                    memset(dest, 0, dmax * sizeof(wchar_t));
                else {
                    while (dmax) {
                        *dest = L'\0';
                        dmax--;
                        dest++;
                    }
                }
#endif
                return RCNEGATE(EOK);
            }

            dmax--;
            dest++;
            src++;
        }
    }

    /*
     * the entire src was not copied, so null the string
     */
    handle_werror(orig_dest, orig_dmax,
                  "wcscat_s: not enough "
                  "space for src",
                  ESNOSPC);

    return RCNEGATE(ESNOSPC);
}

#endif /* HAVE_WCHAR_H */
