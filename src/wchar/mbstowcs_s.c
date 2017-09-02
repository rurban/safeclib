/*------------------------------------------------------------------
 * mbstowcs_s.c
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

#include "config.h"
#ifdef HAVE_WCHAR_H

#include "safeclib_private.h"
#include "safe_str_constraint.h"
#include "safe_str_lib.h"
#include <wchar.h>

/**
 * @brief
 *    The \c mbstowcs_s function converts a null-terminated multibyte
 *    character sequence from the current LC_CTYPE locale to wchar, which
 *    begins in the conversion state described by \c *ps, from the
 *    array whose first element is pointed to by \c *src to its wide
 *    character representation. If dest is not null, converted
 *    characters are stored in the successive elements of the
 *    \c wchar_t array pointed to by \c dest. No more than \c len wide
 *    characters are written to the destination array. Each multibyte
 *    character is converted as if by a call to \c mbrtowc.
 *    \c mbstowcs_s clobbers the destination array from the terminating
 *    null and until \c dmax.
 *
 *    The conversion stops if: 
 *
 *    - The multibyte null character was converted and stored.
 *
 *    - An invalid multibyte character (according to the current C
 *      locale) was encountered.
 *
 *    - the next wide character to be stored would exceed \c len.
 *      This condition is not checked if \c dest==NULL.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.6.5.1 The mbstowcs_s function (p: 611-612)
 *    http://en.cppreference.com/w/c/string/byte/mbstowcs
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retval	pointer to a \c size_t object where the result will be stored
 * @param[out]  dest  pointer to wide character array where the results will be stored
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   src   pointer to the string that will be copied to \c dest
 * @param[in]   len   number of wide characters available in \c dest
 *
 * @pre retval and src shall not be a null pointer.
 * @pre dmax and len shall not be greater than \c RSIZE_MAX_WSTR
 *      (unless dest is null).
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than <tt>wcsnlen_s(src, dmax)</tt>.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then \c mbstowcs_s nulls dest.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retval or src are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dst is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR, unless dst is NULL
 * @retval  ESOVRLP    when src and dest overlap
 * @retval  ESNOSPC    when there is no null character in the first dmax
 *                     multibyte characters in the src array and len is
 *                     greater than dmax (unless dest is null)
 * @see
 *    mbsrtowc_s()
 *
 */
errno_t
mbstowcs_s (size_t *restrict retval,
             wchar_t *restrict dest, rsize_t dmax,
             const char *restrict src, rsize_t len)
{
    wchar_t *orig_dest;

    if (unlikely(retval == NULL)) {
        invoke_safe_str_constraint_handler("mbstowcs_s: retval is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(src == NULL)) {
        if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
            /* null string to clear data */
            memset_s((void*)dest,dmax,0,len);
#else
            *dest = '\0';
#endif
        }
        invoke_safe_str_constraint_handler("mbstowcs_s: src is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely((dmax == 0) && dest)) {
        invoke_safe_str_constraint_handler("mbstowcs_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely((dmax > RSIZE_MAX_WSTR) && dest)) {
        invoke_safe_str_constraint_handler("mbstowcs_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely((char*)dest == src)) {
        return RCNEGATE(ESOVRLP);
    }

    /* hold base of dest in case src was not copied */
    orig_dest = dest;

    *retval = mbstowcs(dest, src, len);

    if (likely((ssize_t)*retval > 0 && *retval < dmax)) {
        return EOK;
    } else {
        /* errno is usually EILSEQ */
        errno_t rc = ((ssize_t)*retval > 0) ? ESNOSPC : errno;
        if (dest) {
            /* the entire src must have been copied, if not reset dest
             * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
             */
            handle_error((char*)orig_dest, dmax,
                         rc == ESNOSPC ? "mbstowcs_s: not enough space for src"
                                       : "mbstowcs_s: illegal sequence",
                         rc);
        }
        return RCNEGATE(rc);
    }

    return RCNEGATE(ESNOSPC);
}
EXPORT_SYMBOL(mbstowcs_s)

#endif /* HAVE_WCHAR_H */
