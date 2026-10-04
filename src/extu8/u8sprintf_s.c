/*------------------------------------------------------------------
 * u8sprintf_s.c
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
 * @def u8sprintf_s(dest, dmax, fmt, ...)
 * @brief
 *    The u8sprintf_s function composes a utf-8 string with the same
 *    content that would be printed if format was used on printf.
 *    Instead of being printed, the content is stored in dest.
 * @details
 *    fmt and the variadic arguments are plain ASCII/byte data (as
 *    with all C printf format strings); only dest is utf-8. Since
 *    UTF-8 is byte-compatible with the underlying formatter, this is
 *    otherwise identical to sprintf_s().
 *
 * @param[out] dest  storage location for output buffer.
 * @param[in]  dmax  maximum number of bytes to store in buffer.
 * @param[in]  fmt   format-control string.
 * @param[in]  ...   optional arguments
 *
 * @pre fmt shall not be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR or the sizeof(dest).
 * @pre dmax shall not equal zero if dest is not null.
 * @pre fmt  shall not contain the conversion specifier %n.
 *
 * @return  If no runtime-constraint violation occurred, the number of
 *          bytes written in the array, not counting the terminating
 *          NUL. If an error occurred, a negative value.
 *
 * @retval -ESNULLP    when \c fmt is NULL pointer, or
 *                     when \c dest is NULL and dmax > 0
 * @retval -ESZEROL    when \c dmax = 0 and dest is not NULL
 * @retval -ESLEMAX    when \c dmax > \c RSIZE_MAX_STR or dmax > size of dest
 * @retval -EOVERFLOW  when \c dmax > size of dest
 * @retval -ESNOSPC    when return value exceeds dmax unless dmax is zero and
 *                     dest is NULL
 * @retval -EINVAL     when \c fmt contains \c %n
 *
 * @see
 *    vu8sprintf_s(), u8snprintf_s(), sprintf_s()
 */
#ifdef FOR_DOXYGEN
int u8sprintf_s(char8_t *restrict dest, rsize_t dmax, const char *restrict fmt,
                ...)
#else
EXPORT int _u8sprintf_s_chk(char8_t *restrict dest, const rsize_t dmax,
                            const size_t destbos, const char *restrict fmt, ...)
#endif
{
    va_list va;
    int ret;
    va_start(va, fmt);
    ret = _vsnprintf_s_chk((char *)dest, dmax, destbos, fmt, va);
    va_end(va);
    return ret;
}

#ifdef __KERNEL__
EXPORT_SYMBOL(_u8sprintf_s_chk);
#endif /* __KERNEL__ */
