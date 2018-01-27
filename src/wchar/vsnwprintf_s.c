/*------------------------------------------------------------------
 * vsnwprintf_s.c
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

#define __STDC_WANT_LIB_EXT1__ 1
#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

#if !defined(HAVE_VSNWPRINTF_S) && defined(SAFECLIB_ENABLE_UNSAFE)

/* TODO:
any of the arguments corresponding to %s is a null pointer.
*/

/**
 * @brief
 *    The truncating \c vsnwprintf_s function composes a wide string
 *    with same test that would be printed if format was used on \c
 *    wprintf. Instead of being printed, the content is stored in
 *    dest.
 *    With \c SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by \c vsnwprintf_s in the
 *    array of dmax wide characters pointed to by \c dest are nulled when
 *    \c vsnwprintf_s returns.
 *    Warning: Unlike the safe variant \c vswprintf_s, \c vsnwprintf_s does not
 *    guarantee that the buffer will be null-terminated unless
 *    the buffer size is zero.
 *
 * @note
 *    POSIX specifies that \c errno is set on error. However, the safeclib
 *    extended \c ES* errors do not set \c errno, only when the underlying
 *    system \c vswprintf or \c vsnwprintf call fails, \c errno is set.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.1.8 The vsnwprintf_s function (p: 634-635)
 *    http://en.cppreference.com/w/c/io/vfwprintf
 *    * only included in safeclib with \c --enable-unsafe
 *
 * @param[out]  dest  pointer to wide string that will be written into.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   fmt   format-control wide string.
 * @param[in]   ap    optional arguments
 *
 * @pre Neither dest nor fmt shall be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_WSTR.
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than wcsnlen_s(dest, dmax).
 * @pre fmt  shall not contain the conversion specifier %n
 * @pre None of the arguments corresponding to %s is a null pointer. (not yet)
 * @pre No encoding error shall occur.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @note While narrow strings provide \c snprintf, which makes it
 * possible to determine the required output buffer size, there is no
 * equivalent for wide strings (until C11's \c snwprintf_s), and in order
 * to determine the buffer size, the program may need to call
 * \c swprintf, check the result value, and reallocate a larger buffer,
 * trying again until successful.
 *
 * @note \c snwprintf_s, unlike \c swprintf_s, will truncate the result to
 * fit within the array pointed to by buffer, even though truncation
 * is treated as an error by most bounds-checked functions.
 *
 * @return  On success the total number of wide characters written is returned.
 * @return  On failure a negative number is returned.
 * @return  If the buffer dest is too small for the formatted text,
 *          including the terminating null, then the buffer is set to an
 *          empty string by placing a null wide character at dest[0], and the
 *          invalid parameter handler is invoked. Unlike \c vsnwprintf,
 *          \c vswprintf_s guarantees that the buffer will be null-terminated
 *          unless the buffer size is zero.
 *
 * @retval  -ESLEMAX when dmax > RSIZE_MAX_WSTR
 * @retval  -ESNULLP when dest/fmt is NULL pointer
 * @retval  -ESZEROL when dmax = 0
 * @retval  -EINVAL  when fmt contains %n
 * @retval  -1       on some other error. errno is set then
 *
 * @see
 *    vswprintf_s(), snwprintf_s(), vsnprintf_s()
 *
 */

EXPORT int
vsnwprintf_s(wchar_t *restrict dest, rsize_t dmax,
            const wchar_t *restrict fmt, va_list ap)
{
    wchar_t *p;
    int ret = -1;
#ifndef HAVE_VSNWPRINTF_S
    va_list ap2;
#endif

    if (unlikely(dmax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("vsnwprintf_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return -(ESLEMAX);
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("vsnwprintf_s: dest is null",
                   NULL, ESNULLP);
        return -(ESNULLP);
    }

    if (unlikely(fmt == NULL)) {
        *dest = L'\0';
        invoke_safe_str_constraint_handler("vsnwprintf_s: fmt is null",
                   NULL, ESNULLP);
        return -(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        *dest = L'\0';
        invoke_safe_str_constraint_handler("vsnwprintf_s: dmax is 0",
                   NULL, ESZEROL);
        return -(ESZEROL);
    }

#if defined(HAVE_WCSSTR) || !defined(SAFECLIB_DISABLE_EXTENSIONS)
    if (unlikely((p = wcsstr((wchar_t*)fmt, L"%n")))) {
        if ((p-fmt == 0) || *(p-1) != L'%') {
            *dest = L'\0';
            invoke_safe_str_constraint_handler("vsnwprintf_s: illegal %n",
                   NULL, EINVAL);
            return -(EINVAL);
        }
    }
#elif defined(HAVE_WCSCHR)
    if (unlikely((p = wcschr(fmt, flen, L'n')))) {
        /* at the beginning or if inside, not %%n */
        if (((p-fmt >= 1) && *(p-1) == L'%') &&
            ((p-fmt == 1) || *(p-2) != L'%')) {
            *dest = L'\0';
            invoke_safe_str_constraint_handler("vsnwprintf_s: illegal %n",
                                               NULL, EINVAL);
            return -(EINVAL);
        }
    }
#else
    #error need wcsstr or wcschr
#endif

    errno = 0;
    /* C11 solves the ESNOSPC problem */
#ifdef HAVE_VSNWPRINTF_S
    ret = vsnwprintf_s(dest, dmax, fmt, ap);
#else
    va_copy(ap2, ap);
    ret = vswprintf(dest, dmax, fmt, ap);

    /* check for ESNOSPC or some other error */
    if (unlikely(ret == -1)) {
        if (likely(dmax < 512)) { /* stacksize 2k */
            static wchar_t tmp[512];
            if (unlikely(dmax == 1)) {
                *dest = L'\0';
                return 1;
            }
            ret = vswprintf(tmp, 512, fmt, ap2);
        } else {
            wchar_t *tmp = (wchar_t *)malloc(dmax * sizeof(wchar_t));
            ret = vswprintf(tmp, dmax, fmt, ap2);
            free(tmp);
        }
        va_end(ap2);
        /* this will bump ret to > 0 */
    }

    /* manual truncation */
    if (unlikely(ret >= (int)dmax)) {
        dest[dmax-1] = L'\0';
    } else if (unlikely(ret < 0)) {
        char errstr[128] = "vsnwprintf_s: ";
        strcat(errstr, strerror(errno));
        *dest = L'\0';
        /* dest[dmax-1] = L'\0'; */
        invoke_safe_str_constraint_handler(errstr, NULL, -ret);
    }
#endif

    return ret;
}
#ifdef __KERNEL__
EXPORT_SYMBOL(vsnwprintf_s);
#endif /* __KERNEL__ */

#endif /* !defined(HAVE_VSNWPRINTF_S) && defined(SAFECLIB_ENABLE_UNSAFE) */
