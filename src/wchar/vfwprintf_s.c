/*------------------------------------------------------------------
 * vfwprintf_s.c
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

/* TODO:
any of the arguments corresponding to %s is a null pointer
*/

/**
 * @brief
 *    The vfwprintf_s function prints formatted output to a wide stream.
 *
 * @note
 *    POSIX specifies that \c errno is set on error. However, the safeclib
 *    extended \c ES* errors do not set \c errno, only when the underlying
 *    system \c vfwprintf call fails, \c errno is set.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.1.6 The vfwprintf_s function (p: 632)
 *    http://en.cppreference.com/w/c/io/vfwprintf
 *
 * @param[in]   stream  output wide stream to write to
 * @param[in]   fmt     format-control wide string.
 * @param[in]   ap      optional arguments
 *
 * @pre Neither stream nor fmt shall be a null pointer.
 * @pre fmt shall not contain the conversion specifier \c %n
 * @pre None of the arguments corresponding to \c %s is a null pointer (not
 * yet)
 * @pre No encoding error shall occur.
 *
 * @return  On success the total number of wide characters written is returned.
 * @return  On failure a negative number is returned, and possibly errno set to
 *          EINVAL or EOVERFLOW.
 *
 * @retval  -ESNULLP when stream or fmt is a NULL pointer
 * @retval  -EINVAL  when fmt contains %n
 * @retval  -1       some other error. errno: EINVAL or EOVERFLOW
 *
 * @see
 *    vfwscanf_s(), fwprintf_s()
 *
 */

EXPORT int vfwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt,
                       va_list ap) {
    wchar_t *p;
    int ret;

    if (unlikely(stream == NULL)) {
        invoke_safe_str_constraint_handler("vfwprintf_s: stream is null", NULL,
                                           ESNULLP);
        return -(ESNULLP);
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vfwprintf_s: fmt is null", NULL,
                                           ESNULLP);
        return -(ESNULLP);
    }

#if defined(HAVE_WCSSTR) || !defined(SAFECLIB_DISABLE_EXTENSIONS)
    if (unlikely((p = wcsstr((wchar_t *)fmt, L"%n")))) {
        if ((p - fmt == 0) || *(p - 1) != L'%') {
            invoke_safe_str_constraint_handler("vfwprintf_s: illegal %n", NULL,
                                               EINVAL);
            return -(EINVAL);
        }
    }
#elif defined(HAVE_WCSCHR)
    if (unlikely((p = wcschr(fmt, flen, L'n')))) {
        /* at the beginning or if inside, not %%n */
        if (((p - fmt >= 1) && *(p - 1) == L'%') &&
            ((p - fmt == 1) || *(p - 2) != L'%')) {
            invoke_safe_str_constraint_handler("vfwprintf_s: illegal %n", NULL,
                                               EINVAL);
            return -(EINVAL);
        }
    }
#else
#error need wcsstr or wcschr
#endif

    errno = 0;
    ret = vfwprintf(stream, fmt, ap);

    if (unlikely(ret < 0)) {
        char errstr[128] = "vfwprintf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, -ret);
    }

    return ret;
}
