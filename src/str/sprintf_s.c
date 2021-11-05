/*------------------------------------------------------------------
 * sprintf_s.c
 *
 * November 2021, Reini Urban
 *
 * Copyright (c) 2021 by Reini Urban
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
 * @def sprintf_s(dest, dmax, fmt, ...)
 * @brief
 *    The \b sprintf_s function composes a string with the same content that
 *    would be printed if format was used on printf. Instead of being
 *    printed, the content is stored in dest.
 *    If dmax is zero, nothing is written and dest may be a null pointer,
 *    however the return value (number of bytes that would be written) is
 *    still calculated and returned.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.6 The sprintf_s function (p: 595-596)
 *    http://en.cppreference.com/w/c/io/fprintf
 *
 * @param[out] dest  storage location for output buffer.
 * @param[in]  dmax  maximum number of characters to store in buffer.
 * @param[in]  fmt   format-control string.
 * @param[in]  ...   optional arguments
 *
 * @pre fmt shall not be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR or the sizeof(dest).
 * @pre dmax shall not equal zero if dest is not null.
 * @pre fmt  shall not contain the conversion specifier %n.
 * @pre None of the arguments corresponding to %s is a null pointer. (not yet)
 * @pre No encoding error shall occur.
 *
 * @return  If no runtime-constraint violation occurred, the \c sprintf_s
 * function returns the number of characters written in the array, not counting
 *          the terminating null character. If an error occurred,
 *          \c sprintf_s returns a negative value. (deviating from C11)
 * @return  If dest is valid and too small for the formatted text,
 *          including the terminating null, then the buffer is cleared, and the
 *          invalid parameter handler is invoked.
 *
 * @retval -ESNULLP    when \c fmt is NULL pointer, or
 *                     when \c dest is NULL and dmax > 0, or
 *                     when a \c %s argument is NULL
 * @retval -ESZEROL    when \c dmax = 0 and dest is not NULL
 * @retval -ESLEMAX    when \c dmax > \c RSIZE_MAX_STR or dmax > size of dest
 * @retval -EOVERFLOW  when \c dmax > size of dest
 * @retval -ESNOSPC    when return value exceeds dmax unless dmax is zero and dest
 *                     is NULL
 * @retval -EINVAL     when \c fmt contains \c %n
 *
 * @note The C11 standard was most likely wrong with changing the return value to
 * 0 on errors. All other functions and existing C11 implementations do return
 * -1, so we return negative error codes. See the
 * http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1141.pdf revision for their
 * rationale. sprintf_s does not set \c errno.
 *
 * @see
 *    vsprintf_s(), snprintf_s()
 */
#if defined FOR_DOXYGEN
int sprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, ...)
#elif defined SAFECLIB_HAVE_C99
EXPORT int _sprintf_s_chk(char *restrict dest, const rsize_t dmax,
                          const size_t destbos, const char *restrict fmt, ...)
#else
EXPORT int sprintf_s(char *restrict dest, rsize_t dmax,
                     const char *restrict fmt, ...)
#endif
{
    va_list va;
    int ret;
    va_start(va, fmt);
#if defined SAFECLIB_HAVE_C99
    ret = _vsnprintf_s_chk(dest, dmax, destbos, fmt, va);
#else
    ret = vsnprintf_s(dest, dmax, fmt, va);
#endif    
    va_end(va);
    return ret;
}

#ifdef __KERNEL__
EXPORT_SYMBOL(_sprintf_s_chk);
#endif /* __KERNEL__ */
