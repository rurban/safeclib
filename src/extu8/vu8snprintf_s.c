/*------------------------------------------------------------------
 * vu8snprintf_s.c
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
 * @def vu8snprintf_s(dest,dmax,fmt,ap)
 * @brief
 *    The truncating vu8snprintf_s function composes a utf-8 string
 *    with the same content that would be printed if format was used
 *    on printf. Instead of being printed, the content is stored in
 *    dest. It is guaranteed that dest will be NUL-terminated.
 *
 * @param[out]  dest  pointer to utf-8 string that will be written into.
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   fmt   format-control string.
 * @param[in]   ap    optional arguments
 *
 * @pre \c fmt shall not be a null pointer.
 * @pre \c dest shall not be a null pointer.
 * @pre \c dmax shall not be zero.
 * @pre \c dmax shall not be greater than \c RSIZE_MAX_STR and size of dest.
 * @pre \c fmt  shall not contain the conversion specifier \c %n.
 *
 * @return  If the buffer \c dest is too small for the formatted text,
 *          including the terminating null, then the buffer is truncated
 *          and zero terminated.
 * @retval  Number of bytes not including the terminating null
 *          byte, or a negative error number if a runtime
 *          constraints violation or an encoding error occurred.
 *
 * @retval  -ESNULLP    when \c dest/fmt is NULL pointer
 * @retval  -ESZEROL    when \c dmax == 0
 * @retval  -ESLEMAX    when \c dmax > \c RSIZE_MAX_STR
 * @retval  -EOVERFLOW  when \c dmax > size of dest
 * @retval  -EINVAL     when \c fmt contains %n
 *
 * @see
 *    vu8sprintf_s(), u8snprintf_s()
 */
#ifdef FOR_DOXYGEN
int vu8snprintf_s(char8_t *restrict dest, rsize_t dmax,
                  const char *restrict fmt, va_list ap)
#else
EXPORT int _vu8snprintf_s_chk(char8_t *restrict dest, rsize_t dmax,
                              const size_t destbos, const char *restrict fmt,
                              va_list ap)
#endif
{
    return _vsnprintf_s_chk((char *)dest, dmax, destbos, fmt, ap);
}
