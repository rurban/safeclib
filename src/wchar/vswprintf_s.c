/*------------------------------------------------------------------
 * vswprintf_s.c
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
#include "safeclib_private.h"

/* TODO:
any of the arguments corresponding to %s is a null pointer
*/

/**
 * @brief 
 *    The vswprintf_s function composes a wide string with same test that 
 *    would be printed if format was used on wprintf. Instead of being 
 *    printed, the content is stored in dest.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by vswprintf_s in the
 *    array of dmax wide characters pointed to by dest are nulled when
 *    vswprintf_s returns.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.1.9 The vswprintf_s function (p: 634-635)
 *    http://en.cppreference.com/w/c/io/vfwprintf
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
 * @pre fmt  shall not contain the conversion specifier \c %n
 * @pre None of the arguments corresponding to \c %s is a null pointer (not yet)
 * @pre No encoding error shall occur.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @return  On success the total number of wide characters written is returned.
 * @return  On failure a negative number is returned, and possibly errno set to
 *          EINVAL or EOVERFLOW.
 * @return  If the buffer dest is too small for the formatted text,
 *          including the terminating null, then the buffer is set to an
 *          empty string by placing a null wide character at dest[0], and the
 *          invalid parameter handler is invoked. Unlike vsnprintf,
 *          vswprintf_s guarantees that the buffer will be null-terminated
 *          unless the buffer size is zero.
 *
 * @retval  -ESLEMAX when dmax > RSIZE_MAX_WSTR
 * @retval  -ESNULLP when dest/fmt is NULL pointer
 * @retval  -ESZEROL when dmax = 0
 * @retval  -EINVAL  when fmt contains %n
 * @retval  -ESNOSPC with C11 when return value exceeds dmax
 * @retval  -1       when return value exceeds dmax or some other error.
 *
 * @see
 *    sprintf_s(), vsnprintf_s()
 *
 */

EXPORT int
vswprintf_s(wchar_t *restrict dest, rsize_t dmax,
            const wchar_t *restrict fmt, va_list ap)
{
    wchar_t *p;
    int ret = -1;
#ifndef HAVE_VSNWPRINTF_S
    va_list ap2;
#endif

    if (unlikely(dmax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("vswprintf_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("vswprintf_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vswprintf_s: fmt is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("vswprintf_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

#if defined(HAVE_WCSSTR) || !defined(SAFECLIB_DISABLE_EXTENSIONS)
    if (unlikely((p = wcsstr((wchar_t*)fmt, L"%n")))) {
        if ((p-fmt == 0) || *(p-1) != L'%') {
            invoke_safe_str_constraint_handler("vswprintf_s: illegal %n",
                   NULL, EINVAL);
            return RCNEGATE(EINVAL);
        }
    }
#elif defined(HAVE_WCSCHR)
    if (unlikely((p = wcschr(fmt, flen, L'n')))) {
        /* at the beginning or if inside, not %%n */
        if (((p-fmt >= 1) && *(p-1) == L'%') &&
            ((p-fmt == 1) || *(p-2) != L'%')) {
            invoke_safe_str_constraint_handler("vswprintf_s: illegal %n",
                                               NULL, EINVAL);
            return RCNEGATE(EINVAL);
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
    if (unlikely(ret ==  -1)) {
        if (likely(dmax < 512)) { /* stacksize 2k */
            static wchar_t tmp[512];
            if (dmax == 1) goto nospc;
            ret = vswprintf(tmp, 512, fmt, ap2);
        } else {
            wchar_t *tmp = (wchar_t *)malloc(RSIZE_MAX_WSTR * sizeof(wchar_t));
            ret = vswprintf(tmp, RSIZE_MAX_WSTR, fmt, ap2);
            free(tmp);
        }
        if (ret > 0) {
          nospc:
            invoke_safe_str_constraint_handler("vswprintf_s: len exceeds dmax",
                                               NULL, ESNOSPC);
            *dest = 0;
            return RCNEGATE(ESNOSPC);
        }
    }
#endif

    if (unlikely(ret >= (int)dmax)) {
        goto nospc;
    } else if (unlikely(ret < 0)) {
#ifndef HAVE_VSNWPRINTF_S
        char errstr[128] = "vswprintf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, -ret);
#endif
        *dest = 0;
    }

    return ret;
}
EXPORT_SYMBOL(vswprintf_s)
