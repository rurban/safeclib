/*------------------------------------------------------------------
 * fu8printf_s.c
 *
 * September 2026
 *
 * Copyright (c) 2026 by Reini Urban
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
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @brief
 *    The fu8printf_s function composes a string via the format string
 *    and writes it, as utf-8, to a FILE stream.
 *
 * @param[out] stream output file stream to write to
 * @param[in]  fmt    format-control string.
 * @param[in]  ...    optional arguments
 *
 * @pre Neither stream nor fmt shall be a null pointer.
 * @pre fmt  shall not contain the conversion specifier %n.
 *
 * @return  On success the total number of bytes written is returned.
 * @return  On failure a negative number is returned.
 * @retval  -ESNULLP when stream or fmt is NULL pointer
 * @retval  -EINVAL  when fmt contains %n
 * @retval  -1       on some other error. errno may be set then.
 *
 * @see
 *    vfu8printf_s(), fprintf_s()
 */
EXPORT int fu8printf_s(FILE *restrict stream, const char *restrict fmt, ...) {
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vfprintf_s(stream, fmt, ap);
    va_end(ap);

    return ret;
}
