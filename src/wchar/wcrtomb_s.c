/*------------------------------------------------------------------
 * wcrtomb_s.c
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

#if (defined(TEST_MSVCRT) && defined(HAVE_WCRTOMB_S)) || !defined(HAVE_WCHAR_H)
#else

/**
 * @brief
 *    Does not permit the \c ps parameter (the pointer to the conversion state)
 *    to be a null pointer.
 *    The restartable \c wcrtomb_s function converts a single
 *    wide character to its narrow multibyte representation from the current
 *    LC_CTYPE locale that begins in the conversion state described by \c *ps.
 *    If \c dest is not null, the converted multibyte characters are
 *    stored in \c dest. Max. \c MB_CUR_MAX will be written to \c dest.
 *
 *    If \c dest is a null pointer, the call is equivalent to
 *    <tt>wcrtomb_s(&retval, buf, sizeof buf, L'\0', ps)</tt> with
 *    internal variables \c retval and \c buf (whose size is greater than
 *    \c MB_CUR_MAX).
 *
 *    If wc is the null wide character L'\0', a null byte is stored,
 *    preceded by any shift sequence necessary to restore the initial
 *    shift state and the conversion state parameter *ps is updated to
 *    represent the initial shift state.
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
 *    terminating null character (if any) written in the
 *    array of dmax characters pointed to by dest are nulled.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.3.1.1 The wcrtomb_s function (p: 647-648)
 *    http://en.cppreference.com/w/c/string/wide/wcrtomb
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retval pointer to a \c size_t object where the result will be stored
 * @param[out]  dest  pointer to bytes where the result will be stored
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   wc    the wide character to convert
 * @param[in]   ps    pointer to the conversion state object
 *
 * @pre retval and ps shall not be a null pointer.
 * @pre dmax shall not be greater than \c RSIZE_MAX_STR
 *      (unless dest is null).
 * @pre dmax shall not equal zero (unless dest is null).
 * @pre dmax must be zero if dest is null.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return Returns zero on success and non-zero on failure, in which
 *         case, \c dest[0] is set to '\0' (unless dest is null or dmax is zero or
 *         greater than RSIZE_MAX_STR) and \c *retval is set to (size_t)-1 (unless
 *         retval is null).
 *
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retval or ps are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, unless dest is NULL
 * @retval  ESNOSPC    when dmax is smaller than the number of required bytes
 *
 * @see
 *    wctomb_s()
 */
EXPORT errno_t
wcrtomb_s(size_t *restrict retval,
          char *restrict dest, rsize_t dmax,
          wchar_t wc, mbstate_t *restrict ps)
{
    size_t len;
    errno_t rc;

    if (unlikely(retval == NULL)) {
        invoke_safe_str_constraint_handler("wcrtomb_s: retval is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(ps == NULL)) {
        invoke_safe_str_constraint_handler("wcrtomb_s: ps is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    /* GLIBC asserts with len=0 and wrong state. darwin and musl is fine. */
    if (unlikely((dmax == 0) && dest)) {
        invoke_safe_str_constraint_handler("wcrtomb_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely((dmax > RSIZE_MAX_STR) && dest)) {
        invoke_safe_str_constraint_handler("wcrtomb_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    len = *retval = wcrtomb(dest, wc, ps);

    if (likely(len < dmax)) {
        if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
            memset(&dest[len], 0, dmax - len);
#else
            dest[len] = '\0';
#endif
        }
        rc = EOK;
    } else {
        /* errno is usually EILSEQ */
        rc = (len <= RSIZE_MAX_STR) ? ESNOSPC : errno;
        if (dest) {
            /* the entire src must have been copied, if not reset dest
             * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
             */
            handle_error(dest, dmax,
                         rc == ESNOSPC ? "wcrtomb_s: not enough space for src"
                                       : "wcrtomb_s: illegal sequence",
                         rc);
        }
    }

    return RCNEGATE(rc);
}

#endif /* HAVE_WCHAR_H or !TEST_MSVCRT */
