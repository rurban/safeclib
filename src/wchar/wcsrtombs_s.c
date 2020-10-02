/*------------------------------------------------------------------
 * wcsrtombs_s.c
 *
 * August 2017, Reini Urban
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

#if (defined(TEST_MSVCRT) && defined(HAVE_WCSRTOMBS_S)) || !defined(HAVE_WCHAR_H)
#else

/**
 * @def wcsrtombs_s(retvalp,dest,dmax,srcp,len,ps)
 * @brief
 *    Does not permit the \c ps parameter (the pointer to the conversion state)
 *    to be a null pointer.
 *    The restartable \c wcsrtombs_s function converts a sequence of
 *    wide characters from the array whose first element is pointed to by
 *    \c *srcp to to its narrow multibyte representation from the current
 *    LC_CTYPE locale that begins in the conversion state described by \c *ps.
 *    If \c dest is not null, converted characters are
 *    stored in the successive elements of \c dest. No more than \c len bytes
 *    written to the destination array. Each wide character is
 *    converted as if by a call to \c wcrtomb.  \c wcsrtombs_s
 *    clobbers the destination array from the terminating null and
 *    until \c dmax. In extension to \c wcstombs_s you can re-use the
 *    state via \c ps.
 *
 *    The conversion stops if:
 *
 *    - The wide null character \c L'\0' was converted and stored.
 *      The bytes stored in this case are the unshift sequence (if necessary)
 *      followed by \c '\0', \c *srcp is set to \c NULL and \c *ps represents
 *      the initial shift state.
 *
 *    - A \c wchar_t was found that does not correspond to a valid character
 *      in the current LC_CTYPE locale. \c *srcp is set to point at the first
 *      unconverted wide character.
 *
 *    - the next multibyte character to be stored would exceed \c len.
 *      \c *srcp is set to point at the beginning of the first unconverted
 *      wide character. This condition is not checked if \c dst==NULL.
 *
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the array of dmax
 *    characters pointed to by dest are nulled. Also in the error cases for
 *    srcp = NULL, *srcp = NULL, ESNOSPC and EILSEQ.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.3.2.2 The wcsrtombs_s function (p: 649-650)
 *    http://en.cppreference.com/w/c/string/wide/wcsrtombs
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retvalp pointer to a \c size_t object where the result will be
 * stored
 * @param[out]  dest  pointer to character array where the result will be
 * stored
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   srcp  pointer to the wide string that will be copied to \c dest
 * @param[in]   len   maximum number of bytes to be written to \c dest
 * @param[in]   ps    pointer to the conversion state object
 *
 * @pre retvalp, ps, srcp, or *srcp shall not be a null pointer.
 * @pre dmax and len shall not be greater than \c RSIZE_MAX_STR
 *      (unless dest is null).
 * @pre dmax shall not equal zero (unless dest is null).
 * @pre dmax shall be greater than \c len.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 * @note On the msvcrt with a NULL dest pointer, the retvalp length is limited
 *       by the srcp \c len. In other libc's \c len is ignored.
 *
 * @return  If there is a runtime-constraint violation, then if \c dest
 *          is not a null pointer and \c dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then \c wcsrtombs_s nulls \c dest.
 *          Then the number of bytes excluding terminating zero that were,
 *          or would be written to \c dest, is stored in \c *retvalp.
 * @note    Under the Windows sec_api the *retvalp result is +1.
 *
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retvalp, ps, srcp or *srcp are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, unless dest is NULL
 * @retval  EOVERFLOW  when dmax or len > size of dest (optionally, when the
 *                     compiler knows the object_size statically), unless dest
 *                     is NULL
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESOVRLP    when *srcp and dest overlap
 * @retval  ESNOSPC    when there is no null character in the first dmax
 *                     multibyte characters in the *srcp array and len is
 *                     greater than dmax (unless dest is null)
 * @retval  EILSEQ     if returned by wctomb()
 *
 * @see
 *    wcrtomb_s(), wcstombs_s()
 */
#ifdef FOR_DOXYGEN
errno_t wcsrtombs_s(size_t *restrict retvalp, char *restrict dest,
                    rsize_t dmax, const wchar_t **restrict srcp,
                    rsize_t len, mbstate_t *restrict ps)
#else
EXPORT errno_t _wcsrtombs_s_chk(size_t *restrict retvalp, char *restrict dest,
                                rsize_t dmax, const wchar_t **restrict srcp,
                                rsize_t len, mbstate_t *restrict ps,
                                const size_t destbos)
#endif
{
    size_t l;
    errno_t rc;

    CHK_SRC_NULL("wcsrtombs_s", retvalp)
    *retvalp = 0;
    CHK_SRC_NULL("wcsrtombs_s", ps)

    /* GLIBC asserts with len=0 and wrong state. darwin and musl is fine.
       wine returns 0 early. */
    if (dest) {
        CHK_DMAX_ZERO("wcsrtombs_s")
        if (destbos == BOS_UNKNOWN) {
            if (unlikely(dmax > RSIZE_MAX_WSTR || len > RSIZE_MAX_WSTR)) {
                invoke_safe_str_constraint_handler("wcsrtombs_s"
                                                   ": dmax/len exceeds max",
                                                   (void *)dest, ESLEMAX);
                return RCNEGATE(ESLEMAX);
            }
            BND_CHK_PTR_BOUNDS(dest, destbos);
        } else {
            if (unlikely(dmax > destbos || len > destbos)) {
                if (unlikely(dmax > RSIZE_MAX_WSTR || len > RSIZE_MAX_WSTR)) {
                    handle_error(dest, destbos,
                                 "wcsrtombs_s"
                                 ": dmax/len exceeds max",
                                 ESLEMAX);
                    return RCNEGATE(ESLEMAX);
                } else {
                    handle_error(dest, destbos,
                                 "wcsrtombs_s"
                                 ": dmax/len exceeds dest",
                                 EOVERFLOW);
                    return RCNEGATE(EOVERFLOW);
                }
            }
#ifdef HAVE_WARN_DMAX
            if (unlikely(dmax != destbos)) {
                handle_str_bos_chk_warn("wcsrtombs_s", (char *)dest, dmax,
                                        destbos);
                RETURN_ESLEWRNG;
            }
#endif
        }
    }
    if (unlikely(srcp == NULL || *srcp == NULL)) {
        if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
            memset(dest, 0, dmax);
#else
            dest[0] = '\0';
#endif
        }
        invoke_safe_str_constraint_handler("wcsrtombs_s: srcp/*srcp is null",
                                           (void *)dest, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dest == (char *)srcp)) {
        invoke_safe_str_constraint_handler(
            "wcsrtombs_s: dest overlapping objects", (void *)dest, ESOVRLP);
        return RCNEGATE(ESOVRLP);
    }

    l = *retvalp = wcsrtombs(dest, srcp, len, ps);

    if (likely(l > 0 && l < dmax)) {
#ifdef SAFECLIB_STR_NULL_SLACK
        if (dest) {
            memset(&dest[l], 0, dmax - l);
        }
#endif
        rc = EOK;
    } else {
        /* errno is usually EILSEQ */
        rc = (l <= RSIZE_MAX_STR) ? ESNOSPC : errno;
        if (dest) {
            /* the entire srcp must have been copied, if not reset dest
             * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
             */
            handle_error(dest, dmax,
                         rc == ESNOSPC ? "wcsrtombs_s: not enough space for src"
                                       : "wcsrtombs_s: illegal sequence",
                         rc);
        }
    }

    return RCNEGATE(rc);
}

#endif /* HAVE_WCHAR_H or !TEST_MSVCRT */
