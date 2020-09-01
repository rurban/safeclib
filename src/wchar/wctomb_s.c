/*------------------------------------------------------------------
 * wctomb_s.c
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

#if (defined(TEST_MSVCRT) && defined(HAVE_WCSTOMBS_S)) || !defined(HAVE_WCHAR_H)
#else

#if defined(__CYGWIN__) && defined(__x86_64)
#define wctomb(dest, wc) wcrtomb((dest), (wc), &st)
#endif

/**
 * @def wctomb_s(retvalp,dest,dmax,wc)
 * @brief
 *    The \c wctomb_s function converts a single
 *    wide character to its narrow multibyte representation from the current
 *    LC_CTYPE locale.
 *    If \c dest is not null, the converted multibyte characters are
 *    stored in \c dest. Max. \c MB_CUR_MAX will be written to \c dest.
 *
 *    If \c dest is a null pointer, the call is equivalent to
 *    <tt>wcrtomb_s(&retval, buf, sizeof buf, L'\0', ps)</tt> with
 *    internal variables \c retval and \c buf (whose size is greater than
 *    \c MB_CUR_MAX).
 *
 *    If the environment macro __STDC_ISO_10646__ is defined, the
 *    values of type wchar_t are the same as the short identifiers of
 *    the characters in the Unicode required set (typically UTF-32
 *    encoding); otherwise, it is implementation-defined. In any case,
 *    the multibyte character encoding used by this function is
 *    specified by the currently active C locale.
 *
 *    If the environment macro __STDC_MB_MIGHT_NEQ_WC__ is defined,
 *    then for members of the basic character set multibyte-character
 *    encoding might not equal wide-character encoding
 *    (non-ASCII-based systems, such as EBCDIC-based systems, may use
 *    Unicode for their wide character encoding and still be
 *    conforming). ASCII-based systems with __STDC_ISO_10646__ defined
 *    leave __STDC_MB_MIGHT_NEQ_WC__ undefined.
 *
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the array of dmax
 *    characters pointed to by dest are nulled. Also in the error cases for
 *    ESNOSPC and EILSEQ.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.6.4.1 The wctomb_s function (p: 610-611)
 *    http://en.cppreference.com/w/c/string/multibyte/wctomb
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retvalp  pointer to a \c size_t object where the result will be
 * stored
 * @param[out]  dest     pointer to bytes where the result will be stored
 * @param[in]   dmax     restricted maximum length of \c dest
 * @param[in]   wc       the wide character to convert
 *
 * @pre retvalp shall not be a null pointer.
 * @pre dmax shall not be greater than \c RSIZE_MAX_STR and size of dest
 *      (unless dest is null).
 * @pre dmax shall not equal zero (unless dest is null).
 * @pre dmax must be zero if dest is null.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return Returns zero on success and non-zero on failure, in which
 *         case, \c dest[0] is set to '\0' (unless dest is null or dmax is
 * invalid) and \c *retvalp is set to (size_t)-1 (unless retvalp is null).
 *
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retvalp is a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, unless dest is NULL
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOSPC    when dmax is smaller than the number of required bytes
 * @retval  EILSEQ     if returned by wctomb()
 *
 * @see
 *    wcrtomb_s()
 */
#ifdef FOR_DOXYGEN
errno_t wctomb_s(int *restrict retvalp, char *restrict dest,
                 rsize_t dmax, wchar_t wc)
#else
EXPORT errno_t _wctomb_s_chk(int *restrict retvalp, char *restrict dest,
                             rsize_t dmax, wchar_t wc, const size_t destbos)
#endif
{
    int len;
    errno_t rc;
#if defined(__CYGWIN__) && defined(__x86_64)
    mbstate_t st;
#endif

    CHK_SRC_NULL("wctomb_s", retvalp)
    /* GLIBC asserts with len=0 and wrong state. darwin and musl is fine. */
    if (dest) {
        CHK_DMAX_ZERO("wctomb_s")
        if (destbos == BOS_UNKNOWN) {
            CHK_DMAX_MAX("wctomb_s", RSIZE_MAX_WSTR)
            BND_CHK_PTR_BOUNDS(dest, dmax);
        } else {
            CHK_DEST_OVR("wctomb_s", destbos)
        }
    } else {
        if (unlikely(dmax != 0)) {
            invoke_safe_str_constraint_handler("wctomb_s"
                                               ": dmax is not 0",
                                               (void *)dest, ESNULLP);
            return RCNEGATE(ESNULLP);
        }
    }

    len = *retvalp = wctomb(dest, wc);

    if (likely(len > 0 && (rsize_t)len < dmax)) {
#ifdef SAFECLIB_STR_NULL_SLACK
        if (dest)
            memset(&dest[len], 0, dmax - len);
#endif
        rc = EOK;
    } else {
        /* errno is usually EILSEQ */
        rc = (len > 0) ? ESNOSPC : errno;
        if (dest) {
            /* the entire src must have been copied, if not reset dest
             * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
             */
            handle_error(dest, dmax,
                         rc == ESNOSPC ? "wctomb_s: not enough space for src"
                                       : "wctomb_s: illegal sequence",
                         rc);
        }
    }

    return RCNEGATE(rc);
}

#endif /* HAVE_WCHAR_H or !TEST_MSVCRT */
