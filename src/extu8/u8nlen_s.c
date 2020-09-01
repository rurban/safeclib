/*------------------------------------------------------------------
 * u8nlen_s.c
 *
 * September 2020, Reini Urban
 *
 * Copyright (c) 2020 by Reini Urban
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

/**
 * @def u8nlen_s(str,smax)
 * @brief
 *    The u8nlen_s function computes the byte-length of the utf-8 string pointed
 *    to by str, stopping at smax, providing limited support for non-null terminated
 *    strings.
 *
 * @param  str   pointer to utf-8 string
 * @param  smax  maximum byte-length of utf-8 string, incl. the final \0.
 *
 * @pre  str shall not be a null pointer.
 * @pre  smax shall not equal zero.
 * @pre  smax shall not be greater than RSIZE_MAX_STR and size of str
 *       (inc. final null).
 *
 * @return The function returns the utf-8 string length, excluding the
 * terminating null character.  If \c str is NULL, then \c u8nlen_s returns
 * 0. Otherwise, the \c u8nlen_s function returns the number of wide
 * characters that precede the terminating null character. If there is no null
 * character in the first \c smax characters of str then \c u8nlen_s
 * returns \c smax. At most the first \c smax characters of str are accessed
 * by \c u8nlen_s.
 *
 * @see
 *    u8len_s(), strnlen_s(), wcsnlen_s()
 */
#ifdef FOR_DOXYGEN
rsize_t u8nlen_s(const char *str, rsize_t smax)
#else
EXPORT rsize_t _u8nlen_s_chk(const char *str, rsize_t smax, size_t strbos)
#endif
{
    const char *z;
    rsize_t orig_smax = smax;

    if (unlikely(str == NULL)) {
        return RCNEGATE(0);
    }
    if (unlikely(smax == 0)) {
        invoke_safe_str_constraint_handler("u8nlen_s: smax is 0", (void *)str,
                                           ESZEROL);
        return RCNEGATE(0);
    }
    if (unlikely(smax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("u8nlen_s: smax exceeds max",
                                           (void *)str, ESLEMAX);
        return RCNEGATE(0);
    }
#if defined(HAVE_WARN_DMAX) || defined(HAVE_ERROR_DMAX) ||                     \
    defined(HAVE___BND_CHK_PTR_BOUNDS)
    if (strbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(str, smax);
#if defined(HAVE_WARN_DMAX) || defined(HAVE_ERROR_DMAX)
    } else {
        if (unlikely(smax != strbos)) {
            handle_str_bos_chk_warn("u8nlen_s", (char *)str, smax, strbos);
            RETURN_ESLEWRNG;
        }
#endif
    }
#endif

#if 0 && defined(HAVE_MEMCHR) /* rather inline it */
    z = memchr(str, 0, smax);
    if (z) smax = z - str;
    return smax;
#else
    if (strbos != BOS_UNKNOWN) {
        /* Dont touch past strbos */
      for (z = str; smax && *str != 0; smax--, str++, strbos--) {
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
