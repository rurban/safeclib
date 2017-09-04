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

#include "config.h"
#ifdef HAVE_WCHAR_H

#include "safeclib_private.h"
#include "safe_str_constraint.h"
#include "safe_str_lib.h"
#include <wchar.h>

/**
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
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.6.4.1 The wctomb_s function (p: 610-611)
 *    http://en.cppreference.com/w/c/string/multibyte/wctomb
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retval pointer to a \c size_t object where the result will be stored
 * @param[out]  dest  pointer to bytes where the result will be stored
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   wc    the wide character to convert
 *
 * @pre retval shall not be a null pointer.
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
 * @retval  ESNULLP    when retval is a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, unless dest is NULL
 * @retval  ESNOSPC    when dmax is smaller than the number of required bytes
 *
 * @see
 *    wcrtomb_s()
 */
EXPORT errno_t
wctomb_s(int *restrict retval,
         char *restrict dest, rsize_t dmax,
         wchar_t wc)
{
    char *orig_dest;

    if (unlikely(retval == NULL)) {
        invoke_safe_str_constraint_handler("wctomb_s: retval is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    /* GLIBC asserts with len=0 and wrong state. darwin and musl is fine. */
    if (unlikely((dmax == 0) && dest)) {
        invoke_safe_str_constraint_handler("wctomb_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely((dmax > RSIZE_MAX_STR) && dest)) {
        invoke_safe_str_constraint_handler("wctomb_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    /* hold base of dest in case src was not copied */
    orig_dest = dest;

    *retval = wctomb(dest, wc);

    if (likely(*retval > 0 && *retval < (int)dmax)) {
        return EOK;
    } else {
        /* errno is usually EILSEQ */
        errno_t rc = (*retval > 0) ? ESNOSPC : errno;
        if (dest) {
            /* the entire src must have been copied, if not reset dest
             * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
             */
            handle_error(orig_dest, dmax,
                         rc == ESNOSPC ? "wctomb_s: not enough space for src"
                                       : "wctomb_s: illegal sequence",
                         rc);
        }
        return RCNEGATE(rc);
    }

    return EOK;
}
EXPORT_SYMBOL(wctomb_s)

#endif /* HAVE_WCHAR_H */
