/*------------------------------------------------------------------
 * wcsnlen_s.c
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
 * @brief
 *    The wcsnlen_s function computes the length of the wide string pointed
 *    to by dest.
 *
 * @remark SPECIFIED IN
 *    ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest  pointer to wide string
 * @param  dmax  maximum length of wide string
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall not equal zero.
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR.
 *
 * @note     On mingw with \c MINGW_HAS_SECURE_API this API is forceinline'd
 *           and the native \c wcsnlen_s with the msvcrt has a different dmax limit:
 *           \c INT_MAX.
 *
 * @return   The function returns the wide string length, excluding  the terminating
 *           null character.  If \c dest is NULL, then \c wcsnlen_s returns 0.
 *           Otherwise, the \c wcsnlen_s function returns the number of wide characters
 *           that precede the terminating null character. If there is no null
 *           character in the first \c dmax characters of dest then \c wcsnlen_s returns
 *           \c dmax. At most the first \c dmax characters of dest are accessed
 *           by \c wcsnlen_s.
 *
 * @see
 *    strnlen_s(), strnterminate_s()
 */

EXPORT rsize_t
wcsnlen_s (const wchar_t *dest, rsize_t dmax)
{
    const wchar_t *z;
    rsize_t orig_dmax = dmax;

    if (unlikely(dest == NULL)) {
        return RCNEGATE(0);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("wcsnlen_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(0);
    }

    if (unlikely(dmax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcsnlen_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(0);
    }

#if 0 && defined(HAVE_WMEMCHR) /* rather inline it */
    z = wmemchr(dest, 0, dmax);
    if (z) dmax = z - dest;
    return dmax;
#else
    for (z = dest; dmax && *dest != 0; dmax--, dest++);
    return dmax ? (rsize_t)(dest - z) : orig_dmax;
#endif
}
#ifdef __KERNEL__
EXPORT_SYMBOL(wcsnlen_s);
#endif /* __KERNEL__ */

#endif /* HAVE_WCHAR_H */
