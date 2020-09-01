/*------------------------------------------------------------------
 * wcsncpy_s.c
 *
 * September 2017, Reini Urban
 * January 2018, Reini Urban
 *
 * Copyright (c) 2017, 2018 by Reini Urban
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

#if (defined(TEST_MSVCRT) && defined(HAVE_WCSNCPY_S)) || !defined(HAVE_WCHAR_H)
#else

/**
 * @def wcsncpy_s(dest,dmax,src,slen)
 * @brief
 *    The \b wcsncpy_s function copies the wide string pointed to by src
 *    (including the terminating null character) into the wide string
 *    pointed to by dest.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by wcsncpy_s in the
 *    array of dmax characters pointed to by dest are nulled when
 *    wcsncpy_s returns.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.2.1.2 The wcsncpy_s function (p: 639)
 *    http://en.cppreference.com/w/c/string/wide/wcsncpy
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to wide string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the wide string that will be copied to dest
 * @param[in]   slen  maximum number of wide characters to copy
 *
 * @pre Neither dest nor src shall be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_WSTR.
 * @pre dmax shall not equal zero.
 * @pre If slen is either greater than or equal to dmax, then dmax should
 *      be more than wcsnlen_s(src,dmax) to avoid truncation.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest and
 *          dmax are valid, then wcsncpy_s nulls dest.
 * @retval  EOK        successful operation, when slen == 0 or the wide
 * characters in src were copied into dest and the result is null terminated.
 * @retval  ESNULLP    when dest/src is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESOVRLP    when buffers overlap
 * @retval  ESNOSPC    when src > dest
 *
 * @see
 *    wcscpy_s(), strncpy_s(), wmemcpy_s(), wmemmove_s()
 */
#ifdef FOR_DOXYGEN
errno_t wcsncpy_s(wchar_t *restrict dest, rsize_t dmax,
                  const wchar_t *restrict src, rsize_t slen)
#else
EXPORT errno_t _wcsncpy_s_chk(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict src, rsize_t slen,
                              const size_t destbos, const size_t srcbos)
#endif
{
    rsize_t orig_dmax;
    wchar_t *orig_dest;
    const wchar_t *overlap_bumper;
    const size_t destsz = dmax * sizeof(wchar_t);

    if (unlikely(slen == 0 && dest && dmax)) {
        *dest = L'\0';
        return EOK;
    }
    CHK_DEST_NULL("wcsncpy_s")
    CHK_DMAX_ZERO("wcsncpy_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("wcsncpy_s", RSIZE_MAX_WSTR)
        BND_CHK_PTR_BOUNDS(dest, destsz);
    } else {
        CHK_DESTW_OVR_CLEAR("wcsncpy_s", destsz, destbos)
    }
    CHK_SRCW_NULL_CLEAR("wcsncpy_s", src)
    if (unlikely(slen > RSIZE_MAX_WSTR)) {
        handle_werror(dest, wcslen(dest), "wcsncpy_s: slen exceeds max",
                      ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen * sizeof(wchar_t));
    } else {
        if (unlikely(slen * sizeof(wchar_t) > srcbos)) {
            handle_werror(dest, wcsnlen_s(dest, dmax),
                          "wcsncpy_s: slen exceeds src", EOVERFLOW);
            return RCNEGATE(EOVERFLOW);
        }
    }

    /* hold base in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_werror(orig_dest, orig_dmax,
                              "wcsncpy_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            if (unlikely(slen == 0)) {
                /* Copying truncated to slen chars.  Note that the TR says to
                 * copy slen chars plus the null char.  We null the slack.
                 */
#ifdef SAFECLIB_STR_NULL_SLACK
                if (dmax > 0x20)
                    memset(dest, 0, dmax * sizeof(wchar_t));
                else {
                    while (dmax) {
                        *dest = L'\0';
                        dmax--;
                        dest++;
                    }
                }
#else
                *dest = L'\0';
#endif
                return RCNEGATE(EOK);
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
            slen--;
            dest++;
            src++;
        }
    } else {
        overlap_bumper = dest;

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_werror(orig_dest, orig_dmax,
                              "wcsncpy_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            if (unlikely(slen == 0)) {
                /* Copying truncated to slen chars.  Note that the TR says to
                 * copy slen chars plus the null char.  We null the slack.
                 */
#ifdef SAFECLIB_STR_NULL_SLACK
                if (dmax > 0x20)
                    memset(dest, 0, dmax * sizeof(wchar_t));
                else {
                    while (dmax) {
                        *dest = L'\0';
                        dmax--;
                        dest++;
                    }
                }
#else
                *dest = L'\0';
#endif
                return RCNEGATE(EOK);
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
            slen--;
            dest++;
            src++;
        }
    }

    /*
     * the entire src must have been copied, if not reset dest
     * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
     */
    handle_werror(orig_dest, orig_dmax,
                  "wcsncpy_s: not "
                  "enough space for src",
                  ESNOSPC);
    return RCNEGATE(ESNOSPC);
}

#endif /* HAVE_WCHAR_H or !TEST_MSVCRT */
