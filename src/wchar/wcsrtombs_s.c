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

#ifdef HAVE_WCHAR_H

/**
 * @brief
 *    Does not permit the \c ps parameter (the pointer to the conversion state)
 *    to be a null pointer.
 *    The restartable \c wcsrtombs_s function converts a sequence of
 *    wide characters from the array whose first element is pointed to by
 *    \c *src to to its narrow multibyte representation from the current
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
 *      followed by \c '\0', \c *src is set to \c NULL and \c *ps represents
 *      the initial shift state.
 *
 *    - A \c wchar_t was found that does not correspond to a valid character
 *      in the current LC_CTYPE locale. \c *src is set to point at the first
 *      unconverted wide character.
 *
 *    - the next multibyte character to be stored would exceed \c len.
 *      \c *src is set to point at the beginning of the first unconverted
 *      wide character. This condition is not checked if \c dst==NULL.
 *
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the array of dmax
 *    characters pointed to by dest are nulled. Also in the error cases for
 *    src = NULL, *src = NULL, ESNOSPC and EILSEQ.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.3.2.2 The wcsrtombs_s function (p: 649-650)
 *    http://en.cppreference.com/w/c/string/wide/wcsrtombs
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retval pointer to a \c size_t object where the result will be stored
 * @param[out]  dest  pointer to character array where the result will be stored
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   src   pointer to the wide string that will be copied to \c dest
 * @param[in]   len   number of bytes available in \c dest
 * @param[in]   ps    pointer to the conversion state object
 *
 * @pre retval, ps, src, or *src shall not be a null pointer.
 * @pre dmax and len shall not be greater than \c RSIZE_MAX_STR
 *      (unless dest is null).
 * @pre dmax shall not equal zero (unless dest is null).
 * @pre dmax shall be greater than \c len.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, then if \c dest
 *          is not a null pointer and \c dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then \c wcsrtombs_s nulls \c dest.
 *          Then the number of bytes excluding terminating zero that were,
 *          or would be written to \c dest, is stored in \c *retval.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retval, ps, src or *src are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, unless dest is NULL
 * @retval  ESOVRLP    when src and dest overlap
 * @retval  ESNOSPC    when there is no null character in the first dmax
 *                     multibyte characters in the *src array and len is
 *                     greater than dmax (unless dest is null)
 * @retval  EILSEQ     if returned by wctomb()
 *
 * @see
 *    wcrtomb_s(), wcstombs_s()
 */

EXPORT errno_t
wcsrtombs_s (size_t *restrict retval,
             char *restrict dest, rsize_t dmax,
             const wchar_t **restrict src, rsize_t len,
             mbstate_t *restrict ps)
{
    size_t l;
    errno_t rc;

    if (unlikely(retval == NULL)) {
        invoke_safe_str_constraint_handler("wcsrtombs_s: retval is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(ps == NULL)) {
        invoke_safe_str_constraint_handler("wcsrtombs_s: ps is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    /* GLIBC asserts with len=0 and wrong state. darwin and musl is fine. */
    if (unlikely((dmax == 0) && dest)) {
        invoke_safe_str_constraint_handler("wcsrtombs_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely((dmax > RSIZE_MAX_STR) && dest)) {
        invoke_safe_str_constraint_handler("wcsrtombs_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(dest == (char*)src)) {
        return RCNEGATE(ESOVRLP);
    }

    if (unlikely(src == NULL)) {
        if (dest)
            handle_error(dest, dmax, "wcsrtombs_s: src is null",
                         ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(*src == NULL)) {
        if (dest)
            handle_error(dest, dmax, "wcsrtombs_s: *src is null",
                         ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    l = *retval = wcsrtombs(dest, src, len, ps);

    if (likely((ssize_t)l > 0 && l < dmax)) {
#ifdef SAFECLIB_STR_NULL_SLACK
        if (dest)
            memset(&dest[l], 0, dmax-l);
#endif
        rc = EOK;
    } else {
        /* errno is usually EILSEQ */
        rc = ((ssize_t)l > 0) ? ESNOSPC : errno;
        if (dest) {
            /* the entire src must have been copied, if not reset dest
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
EXPORT_SYMBOL(wcsrtombs_s)

#endif /* HAVE_WCHAR_H */
