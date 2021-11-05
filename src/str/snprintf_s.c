/*------------------------------------------------------------------
 * snprintf_s.c
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
 * @def snprintf_s(dest, dmax, fmt, ...)
 * @brief
 *    The truncating \c snprintf_s function composes a string with
 *    same test that would be printed if format was used on \c
 *    printf. Instead of being printed, the content is stored in dest.
 *    It is guaranteed that dest will be null-terminated.
 *
 * @note
 *    POSIX specifies that \c errno is set on error. However, the safeclib
 *    extended \c ES* errors do not set \c errno, only when the underlying
 *    system \c vsnprintf call fails, \c errno is set.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.6 The snprintf_s function (p: 594-595)
 *    http://en.cppreference.com/w/c/io/fprintf
 *
 * @param[out]  dest  pointer to string that will be written into.
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   fmt   format-control string.
 * @param[in]   ...   optional arguments
 *
 * @pre \c fmt shall not be a null pointer.
 * @pre \c dest shall not be a null pointer.
 * @pre \c dmax shall not be zero.
 * @pre \c dmax shall not be greater than \c RSIZE_MAX_STR and size of dest.
 * @pre \c fmt  shall not contain the conversion specifier \c %n.
 * @pre None of the arguments corresponding to \c %s is a null pointer.
 * @pre No encoding error shall occur.
 *
 * @return  If the buffer \c dest is too small for the formatted text,
 *          including the terminating null, then the buffer is truncated
 *          and null terminated.
 * @retval  Number of characters not including the terminating null
 *          character, or a negative error number if a runtime
 *          constraints violation or an encoding error occurred.
 *
 * @retval  -ESNULLP    when \c dest/fmt is NULL pointer, or
 *                      when a \c %s argument is NULL
 * @retval  -ESZEROL    when \c dmax == 0
 * @retval  -ESLEMAX    when \c dmax > \c RSIZE_MAX_STR
 * @retval  -EOVERFLOW  when \c dmax > size of dest
 * @retval  -EINVAL     when \c fmt contains %n
 *
 * @see
 *    sprintf_s(), vsnprintf_s(), snwprintf_s()
 */
#if defined FOR_DOXYGEN
int snprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, ...)
#elif defined SAFECLIB_HAVE_C99
EXPORT int _snprintf_s_chk(char *restrict dest, rsize_t dmax,
                           const size_t destbos, const char *restrict fmt, ...)
#else
EXPORT int snprintf_s(char *restrict dest, rsize_t dmax,
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
