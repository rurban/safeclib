/*------------------------------------------------------------------
 * vsprintf_s.c
 *
 * August 2017, Reini Urban
 * February 2018, Reini Urban
 * November 2021, Reini Urban
 *
 * Copyright (c) 2017,2018,2021 by Reini Urban
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
 * @def vsprintf_s(dest,dmax,fmt,va_args)
 * @brief
 *    The \b vsprintf_s function composes a string with the same content that
 *    would be printed if format was used on \c printf. Instead of being
 *    printed, the content is stored in dest.
 *    If dmax is zero, nothing is written and dest may be a null pointer,
 *    however the return value (number of bytes that would be written) is
 *    still calculated and returned.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.13 The vsprintf_s function (p: 601)
 *    http://en.cppreference.com/w/c/io/vfprintf
 *
 * @param[out]  dest  pointer to string that will be written into.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   fmt   format-control string.
 * @param[in]   ap    optional arguments
 *
 * @pre fmt shall not be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR or the size of dest.
 * @pre dmax shall not equal zero if dest is not null.
 * @pre fmt  shall not contain the conversion specifier %n
 * @pre None of the arguments corresponding to %s is a null pointer (not yet)
 * @pre No encoding error shall occur.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  On success the total number of characters without the terminating
 *          \0 is returned.
 * @return  On failure a negative number is returned  (deviating from C11).
 * @return  If dest is valid and too small for the formatted text,
 *          including the terminating null, then the buffer is cleared, and the
 *          invalid parameter handler is invoked.
 *
 * @retval -ESNULLP    when \c fmt is NULL pointer, or
 *                     when \c dest is NULL and dmax > 0, or
 *                     when a \c %s argument is NULL
 * @retval -ESZEROL    when \c dmax = 0 and dest is not NULL
 * @retval -ESLEMAX    when \c dmax > \c RSIZE_MAX_STR
 * @retval -EOVERFLOW  when \c dmax > size of dest
 * @retval -ESNOSPC    when return value exceeds dmax unless dmax is zero and dest
 *                     is NULL
 * @retval -EINVAL     when \c fmt contains \c %n
 *
 * @note The C11 standard was most likely wrong with changing the return value
 * 0 on errors. All other functions and existing C11 implementations do return
 * -1, so we return negative error codes. See the
 * http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1141.pdf revision for their
 * rationale. vsprintf_s does not set \c errno.
 *
 * @see
 *    sprintf_s(), vsnprintf_s()
 */

#ifdef FOR_DOXYGEN
int vsprintf_s(char *restrict dest, const rsize_t dmax,
               const char *restrict fmt, va_list ap)
#else
EXPORT int _vsprintf_s_chk(char *restrict dest, const rsize_t dmax,
                           const size_t destbos, const char *restrict fmt,
                           va_list ap)
#endif
{
    int ret;
    ret = _vsnprintf_s_chk(dest, dmax, destbos, fmt, ap);

    if (unlikely(dmax && ret >= (int)dmax)
#ifdef HAVE_MINGW32
        || (ret == -1 && errno == ERANGE)
#endif
    ) {
        handle_error(dest, dmax, "vsprintf_s: len exceeds dmax", ESNOSPC);
#ifdef HAVE_MINGW32
        errno = 0;
#endif
        return -ESNOSPC; /* different to the standard (=0),
                            but like all other implementations */
    }

    return ret;
}
