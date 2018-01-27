/*------------------------------------------------------------------
 * mbsrtowcs_s.c
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
 *    Does not permit the \c ps parameter (the pointer to the conversion state) to
 *    be a null pointer.
 *    The restartable \c mbsrtowcs_s function converts a null-terminated
 *    multibyte character sequence from the current LC_CTYPE locale to wchar,
 *    which begins in the conversion state described by \c *ps, from the
 *    array whose first element is pointed to by \c *src to its wide
 *    character representation. If dest is not null, converted
 *    characters are stored in the successive elements of the
 *    \c wchar_t array pointed to by \c dest. No more than \c len wide
 *    characters are written to the destination array. Each multibyte
 *    character is converted as if by a call to \c mbrtowc.
 *    \c mbsrtowcs_s clobbers the destination array from the terminating
 *    null and until \c dmax. In extension to \c mbstowc_s you can re-use the
 *    state via \c ps.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with
 *    0.
 *
 *    The conversion stops if:
 *
 *    - The multibyte null character was converted and stored. \c *src is
 *      set to \c NULL and \c *ps represents the initial shift state.
 *
 *    - An invalid multibyte character (according to the current C
 *      locale) was encountered. \c *src is set to point at the
 *      beginning of the first unconverted multibyte character.
 *
 *    - the next wide character to be stored would exceed \c len.
 *      \c *src is set to point at the beginning of the first unconverted
 *      multibyte character. This condition is not checked if
 *      \c dest==NULL.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.3.2.1 The mbsrtowcs_s function (p: 648-649)
 *    http://en.cppreference.com/w/c/string/multibyte/mbsrtowcs
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retval  pointer to a \c size_t object where the result will be stored
 * @param[out]  dest  pointer to wide character array where the results will be stored
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   src   pointer to the string that will be copied to \c dest
 * @param[in]   len   maximal number of wide characters to be copied to \c dest
 * @param[in]   ps    pointer to the conversion state object
 *
 * @pre retval, ps, src, or *src shall not be a null pointer.
 * @pre dmax and len shall not be greater than \c RSIZE_MAX_WSTR
 *      (unless dest is null).
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than <tt>wcsnlen_s(src, dmax)</tt>.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 * @note On the msvcrt with a NULL dest pointer, the retval length is limited
 *       by the src \c len. In other libc's \c len is ignored.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then \c mbsrtowcs_s nulls dest.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retval, ps, src or *src are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR, unless dest is NULL
 * @retval  ESOVRLP    when src and dest overlap
 * @retval  ESNOSPC    when there is no null character in the first dmax
 *                     multibyte characters in the *src array and len is
 *                     greater than dmax (unless dest is null)
 *
 * @see
 *    mbstowc_s()
 */
EXPORT errno_t
mbsrtowcs_s (size_t *restrict retval,
             wchar_t *restrict dest, rsize_t dmax,
             const char **restrict src, rsize_t len,
             mbstate_t *restrict ps)
{
    wchar_t *orig_dest;
    mbstate_t orig_ps;
    errno_t rc;

    if (unlikely(retval == NULL)) {
        invoke_safe_str_constraint_handler("mbsrtowcs_s: retval is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(ps == NULL)) {
        invoke_safe_str_constraint_handler("mbsrtowcs_s: ps is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(src == NULL)) {
        if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
            /* null string to clear data */
            memset_s((void*)dest, dmax, 0, len * sizeof(wchar_t));
#else
            *dest = '\0';
#endif
        }
        invoke_safe_str_constraint_handler("mbsrtowcs_s: src is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(*src == NULL)) {
        invoke_safe_str_constraint_handler("mbsrtowcs_s: *src is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely((dmax == 0) && dest)) {
        invoke_safe_str_constraint_handler("mbsrtowcs_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely((dmax > RSIZE_MAX_WSTR) && dest)) {
        invoke_safe_str_constraint_handler("mbsrtowcs_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely((char*)dest == (char*)src)) {
        return RCNEGATE(ESOVRLP);
    }

    /* hold base of dest in case src was not copied */
    orig_dest = dest;
    memcpy(&orig_ps, ps, sizeof(orig_ps));

    *retval = mbsrtowcs(dest, src, len, ps);

    if (likely(*retval < dmax)) {
        if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
            memset(&dest[*retval], 0, (dmax-*retval)*sizeof(wchar_t));
#else
            dest[*retval] = L'\0';
#endif
        }
        rc = EOK;
    } else {
        if (dest) {
            size_t tmp = 0;
            errno = 0;
            /* with NULL either 0 or -1 is returned */
            if (*retval > RSIZE_MAX_WSTR) { /* else ESNOSPC */
                tmp = mbsrtowcs(NULL, src, len - 1, &orig_ps);
            }
            rc = (tmp == 0) ? ESNOSPC : errno;
            /* the entire src must have been copied, if not reset dest
             * to null the string. (only with SAFECLIB_STR_NULL_SLACK) */
            handle_werror(orig_dest, dmax,
                         !tmp ? "mbsrtowcs_s: not enough space for src"
                              : "mbsrtowcs_s: illegal sequence",
                         rc);
        }
        else {
            rc = ((size_t)*retval == 0) ? EOK : errno;
        }
    }

    return RCNEGATE(rc);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(mbsrtowcs_s);
#endif /* __KERNEL__ */

#endif /* HAVE_WCHAR_H */
