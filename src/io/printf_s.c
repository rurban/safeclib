/*------------------------------------------------------------------
 * printf_s.c
 *
 * September 2017, Reini Urban
 * November 2021, Reini Urban
 *
 * Copyright (c) 2017, 2021 by Reini Urban
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
 *    The printf_s function composes a string via the format string
 *    and writes it to the STDOUT file.
 *
 * @note
 *    POSIX specifies that \c errno is set on error. However, the safeclib
 *    extended \c ES* errors do not set \c errno, only when the underlying
 *    system \c vprintf call fails, \c errno is set.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.3 The printf_s function (p: 593-594)
 *    http://en.cppreference.com/w/c/io/fprintf
 *
 * @param[in]  fmt    format-control string
 * @param[in]  ...    optional arguments
 *
 * @pre fmt shall not be a null pointer.
 * @pre fmt shall not contain the conversion specifier %n.
 * @pre None of the arguments corresponding to %s is a null pointer.
 * @pre No encoding error shall occur.
 *
 * @return  On success the total number of characters written is returned.
 * @return  On failure a negative number is returned.
 * @retval  -ESNULLP when fmt is NULL pointer
 * @retval  -EINVAL  when fmt contains %n
 * @retval  -1       on some other error. errno might be set then.
 */

EXPORT int printf_s(const char *restrict fmt, ...) {
    va_list va;
    int ret;
    const char *p;
    char buffer[1];

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vsnprintf_s: fmt is null",
                                           NULL, ESNULLP);
        return -ESNULLP;
    }
    // catch %n early, before it outputs anything
    if (unlikely((p = strnstr(fmt, "%n", RSIZE_MAX_STR)))) {
        /* at the beginning or if inside, not %%n */
        if ((p - fmt == 0) || *(p - 1) != '%') {
            invoke_safe_str_constraint_handler("vsnprintf_s: illegal %n", NULL,
                                               EINVAL);
            return -(EINVAL);
        }
    }
    
    va_start(va, fmt);
    ret = safec_vsnprintf_s(safec_out_char, "printf_s", buffer, (rsize_t)-1, fmt, va);
    va_end(va);

    if (unlikely(ret < 0 && errno != 0)) {
        char errstr[128] = "printf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, -ret);
    }

    return ret;
}
