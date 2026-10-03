/*------------------------------------------------------------------
 * fprintf_s.c
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
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @brief
 *    The fprintf_s function composes a string via the format string
 *    and writes it to a FILE buffer.
 *
 * @note
 *    POSIX specifies that \c errno is set on error. However, the safeclib
 *    extended \c ES* errors do not set \c errno, only when the underlying
 *    system \c vfprintf call fails, \c errno is set.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.1 The fprintf_s function (p: 591)
 *    http://en.cppreference.com/w/c/io/fprintf
 *
 * @param[out] stream output file stream to write to
 * @param[in]  fmt    format-control string.
 * @param[in]  ...    optional arguments
 *
 * @pre Neither stream nor fmt shall be a null pointer.
 * @pre fmt  shall not contain the conversion specifier %n.
 * @pre None of the arguments corresponding to %s is a null pointer.
 * @pre No encoding error shall occur.
 *
 * @return  On success the total number of characters written is returned.
 * @return  On failure a negative number is returned.
 * @retval  -ESNULLP when stream or fmt is NULL pointer
 * @retval  -EINVAL  when fmt contains %n
 * @retval  -1       on some other error. errno may be set then.
 *
 */

EXPORT int fprintf_s(FILE *restrict stream, const char *restrict fmt, ...) {
    va_list ap;
    int ret;
    const char *p;
    out_fct_wrap_type wrap;

    if (unlikely(stream == NULL)) {
        invoke_safe_str_constraint_handler("fprintf_s: stream is null", NULL,
                                           ESNULLP);
        return -(ESNULLP);
    }
    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("fprintf_s: fmt is null", NULL,
                                           ESNULLP);
        return -(ESNULLP);
    }
    if (unlikely((p = strnstr(fmt, "%n", RSIZE_MAX_STR)))) {
        /* at the beginning or if inside, not %%n */
        if ((p - fmt == 0) || *(p - 1) != '%') {
            invoke_safe_str_constraint_handler("fprintf_s: illegal %n", NULL,
                                               EINVAL);
            return -(EINVAL);
        }
    }

    errno = 0;
    wrap.arg = stream;
    va_start(ap, fmt);
    ret = safec_vsnprintf_s(safec_out_fchar, "fprintf_s", (char*)&wrap, (rsize_t)-1, fmt, ap);
    va_end(ap);

    if (unlikely(ret < 0 && errno != 0)) {
        char errstr[128] = "fprintf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, -ret);
    }

    return ret;
}
