/*------------------------------------------------------------------
 * wcstombs_s.c
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

#ifdef HAVE_WCHAR_H

#if defined(__CYGWIN__) && defined(__x86_64)
#define wcstombs(dest, src, len) \
    wcsrtombs((dest), (const wchar_t ** restrict)&(src), (len), &st)
#endif


/**
 * @brief
 *    The \c wcstombs_s function converts a sequence of
 *    wide characters from the array whose first element is pointed to by
 *    \c src to to its narrow multibyte representation from the current
 *    LC_CTYPE locale.
 *    If \c dest is not null, converted characters are
 *    stored in the successive elements of \c dest. No more than \c len bytes
 *    are written to the destination array. Each wide character is
 *    converted as if by a call to \c wcrtomb.  \c wcstombs_s
 *    clobbers the destination array from the terminating null and
 *    until \c dmax.
 *
 *    The conversion stops if: 
 *
 *    - The wide null character \c L'\0' was converted and stored. 
 *      The bytes stored in this case are the unshift sequence (if necessary)
 *      followed by \c '\0'.
 *
 *    - A \c wchar_t was found that does not correspond to a valid character
 *      in the current LC_CTYPE locale.
 *
 *    - the next multibyte character to be stored would exceed \c len.
 *      This condition is not checked if \c dst==NULL.
 *
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the array of dmax
 *    characters pointed to by dest are nulled. Also in the error cases for
 *    src = NULL, ESNOSPC and EILSEQ.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.6.5.2 The wcstombs_s function (p: 612-613)
 *    http://en.cppreference.com/w/c/string/byte/wcstombs
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retval the number of characters converted
 * @param[out]  dest   buffer for the resulting converted multibyte
 *                     character string
 * @param[in]   dmax   The size in bytes of dest
 * @param[in]   src    wide string that will be converted to \c dest
 * @param[in]   len    number of bytes to be stored in \c dest, not
 *                     including the terminating null character.
 *
 * @pre \c retval and \c src shall not be a null pointer.
 * @pre dmax and len shall not be greater than \c RSIZE_MAX_STR
 *      (unless \c dest is null).
 * @pre \c dmax shall not equal zero (unless \c dest is null).
 * @pre \c dmax shall be greater than \c len.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, then if \c dest
 *          is not a null pointer and \c dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then \c wcstombs_s nulls \c dest.
 *          Then the number of bytes excluding terminating zero that were,
 *          or would be written to \c dest, is stored in \c *retval.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retval or src are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, unless dest is NULL
 * @retval  ESOVRLP    when src and dest overlap
 * @retval  ESNOSPC    when there is no null character in the first dmax
 *                     multibyte characters in the src array and len is
 *                     greater than dmax (unless dest is null)
 * @retval  EILSEQ     if returned by wcstombs()
 *
 * @see
 *    mbstowc_s()
 */
EXPORT errno_t
wcstombs_s (size_t *restrict retval,
             char *restrict dest, rsize_t dmax,
             const wchar_t *restrict src, rsize_t len)
{
    size_t l;
    errno_t rc;
#if defined(__CYGWIN__) && defined(__x86_64)
    mbstate_t st;
#endif

    if (unlikely(retval == NULL)) {
        invoke_safe_str_constraint_handler("wcstombs_s: retval is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely((dmax == 0) && dest)) {
        invoke_safe_str_constraint_handler("wcstombs_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely((dmax > RSIZE_MAX_STR) && dest)) {
        invoke_safe_str_constraint_handler("wcstombs_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(dest == (char*)src)) {
        return RCNEGATE(ESOVRLP);
    }

    if (unlikely(src == NULL)) {
        if (dest)
            handle_error(dest, dmax, "wcstombs_s: src is null",
                         ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    /* l is the strlen, excluding NULL */
    l = *retval = wcstombs(dest, src, len);

    if (likely((ssize_t)l > 0 && (rsize_t)l < dmax)) {
        if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
            memset(&dest[l], 0, dmax-l);
#else
            /* wcstombs only ensures null-termination when len is big enough.
             * Above we ensured l < dmax, otherwise ESNOSPC. */
            dest[l] = '\0';
#endif
        }
        rc = EOK;
    } else {
        /* errno is usually EILSEQ */
        rc = ((ssize_t)l > 0) ? ESNOSPC : errno;
        if (dest) {
            /* the entire src must have been copied, if not reset dest
             * to null the string with SAFECLIB_STR_NULL_SLACK. */
            handle_error(dest, dmax,
                         rc == ESNOSPC ? "wcstombs_s: not enough space for src"
                                       : "wcstombs_s: illegal sequence",
                         rc);
        }
    }

    return RCNEGATE(rc);
}
EXPORT_SYMBOL(wcstombs_s)

#endif /* HAVE_WCHAR_H */
