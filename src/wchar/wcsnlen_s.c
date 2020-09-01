/*------------------------------------------------------------------
 * wcsnlen_s.c
 *
 * September 2017, Reini Urban
 * May 2019, Reini Urban
 *
 * Copyright (c) 2017,2019 by Reini Urban
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
 * @def wcsnlen_s(str,smax)
 * @brief
 *    The wcsnlen_s function computes the length of the wide string pointed
 *    to by str, providing limited support for non-null terminated strings.
 *
 * @remark SPECIFIED IN
 *    ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  str   pointer to wide string
 * @param  smax  maximum length of wide string, incl. the final null
 *
 * @pre  str shall not be a null pointer.
 * @pre  smax shall not equal zero.
 * @pre  smax shall not be greater than RSIZE_MAX_WSTR and size of str
 *       (inc. final null).
 *
 * @note   On mingw with \c MINGW_HAS_SECURE_API this API is forceinline'd
 *         and the native \c wcsnlen_s with the msvcrt has a different smax
 *         limit: \c INT_MAX.
 * @note   Unlike specified in the C11 spec the runtime-constraint handlers are
 *         called.
 *
 * @return The function returns the wide string length, excluding the
 * terminating null character.  If \c str is NULL, then \c wcsnlen_s returns
 * 0. Otherwise, the \c wcsnlen_s function returns the number of wide
 * characters that precede the terminating null character. If there is no null
 * character in the first \c smax characters of str then \c wcsnlen_s
 * returns \c smax. At most the first \c smax characters of str are accessed
 * by \c wcsnlen_s.
 *
 * @see
 *    strnlen_s(), strnterminate_s()
 */
#ifdef FOR_DOXYGEN
rsize_t wcsnlen_s(const wchar_t *str, rsize_t smax)
#else
EXPORT rsize_t _wcsnlen_s_chk(const wchar_t *str, rsize_t smax, size_t strbos)
#endif
{
    const wchar_t *z;
    rsize_t orig_smax = smax;

    if (unlikely(str == NULL)) {
        return RCNEGATE(0);
    }
    if (unlikely(smax == 0)) {
        invoke_safe_str_constraint_handler("wcsnlen_s: smax is 0", (void *)str,
                                           ESZEROL);
        return RCNEGATE(0);
    }
    if (unlikely(smax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcsnlen_s: smax exceeds max",
                                           (void *)str, ESLEMAX);
        return RCNEGATE(0);
    }
#if defined(HAVE_WARN_DMAX) || defined(HAVE_ERROR_DMAX) ||                     \
    defined(HAVE___BND_CHK_PTR_BOUNDS)
    if (strbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(str, smax * sizeof(wchar_t));
#if defined(HAVE_WARN_DMAX) || defined(HAVE_ERROR_DMAX)
    } else {
        if (unlikely(smax * sizeof(wchar_t) != strbos)) {
            handle_str_bos_chk_warn("wcsnlen_s", (char *)str, smax,
                                    strbos / sizeof(wchar_t));
            RETURN_ESLEWRNG;
        }
#endif
    }
#endif

#if 0 && defined(HAVE_WMEMCHR) /* rather inline it */
    z = wmemchr(str, 0, smax);
    if (z) smax = z - str;
    return smax;
#else
    if (strbos != BOS_UNKNOWN) {
        /* Dont touch past strbos */
        for (z = str; smax && *str != 0; smax--, str++) {
            strbos -= sizeof(wchar_t);
            if (unlikely(strbos <= 0))
                return smax ? (rsize_t)(str - z) : orig_smax;
        }
    } else {
        for (z = str; smax && *str != 0; smax--, str++)
            ;
    }
    return smax ? (rsize_t)(str - z) : orig_smax;
#endif
}

#endif /* HAVE_WCHAR_H */
