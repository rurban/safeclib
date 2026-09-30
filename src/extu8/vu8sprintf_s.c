/*------------------------------------------------------------------
 * vu8sprintf_s.c
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
 * @def vu8sprintf_s(dest,dmax,fmt,ap)
 * @brief
 *    The vu8sprintf_s function composes a utf-8 string with the same
 *    content that would be printed if format was used on printf.
 *    Instead of being printed, the content is stored in dest.
 *
 * @param[out]  dest  pointer to utf-8 string that will be written into.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   fmt   format-control string.
 * @param[in]   ap    optional arguments
 *
 * @pre fmt shall not be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR or the size of dest.
 * @pre dmax shall not equal zero if dest is not null.
 * @pre fmt  shall not contain the conversion specifier %n
 *
 * @return  On success the total number of bytes without the terminating
 *          \0 is returned.
 * @return  On failure a negative number is returned.
 *
 * @retval -ESNULLP    when \c fmt is NULL pointer, or
 *                     when \c dest is NULL and dmax > 0
 * @retval -ESZEROL    when \c dmax = 0 and dest is not NULL
 * @retval -ESLEMAX    when \c dmax > \c RSIZE_MAX_STR
 * @retval -EOVERFLOW  when \c dmax > size of dest
 * @retval -ESNOSPC    when return value exceeds dmax unless dmax is zero and
 *                     dest is NULL
 * @retval -EINVAL     when \c fmt contains \c %n
 *
 * @see
 *    u8sprintf_s(), vu8snprintf_s()
 */
#ifdef FOR_DOXYGEN
int vu8sprintf_s(char8_t *restrict dest, const rsize_t dmax,
                 const char *restrict fmt, va_list ap)
#else
EXPORT int _vu8sprintf_s_chk(char8_t *restrict dest, const rsize_t dmax,
                             const size_t destbos, const char *restrict fmt,
                             va_list ap)
#endif
{
    int ret;
    ret = _vsnprintf_s_chk((char *)dest, dmax, destbos, fmt, ap);

    if (unlikely(dmax && ret >= (int)dmax)
#ifdef HAVE_MINGW32
        || (ret == -1 && errno == ERANGE)
#endif
    ) {
        handle_error((char *)dest, dmax, "vu8sprintf_s: len exceeds dmax",
                     ESNOSPC);
#ifdef HAVE_MINGW32
        errno = 0;
#endif
        return -ESNOSPC;
    }

    return ret;
}
