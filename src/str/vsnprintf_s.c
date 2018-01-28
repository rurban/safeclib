/*------------------------------------------------------------------
 * vsnprintf_s_s.c
 *
 * August 2017, Reini Urban
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

#if defined(_WIN32) && defined(HAVE_VSNPRINTF_S)
#else
#ifdef SAFECLIB_ENABLE_UNSAFE

/**
 * @brief
 *    The truncating \c vsnprintf_s function composes a string with
 *    same test that would be printed if format was used on \c
 *    printf. Instead of being printed, the content is stored in dest.
 *    Warning: Unlike the safe variant \c vsprintf_s, \c vsnprintf_s does not
 *    guarantee that the buffer will be null-terminated unless
 *    the buffer size is zero.
 *    More than dmax - 1 characters might be written!
 *
 * @note
 *    POSIX specifies that \c errno is set on error. However, the safeclib
 *    extended \c ES* errors do not set \c errno, only when the underlying
 *    system \c vsnprintf call fails, \c errno is set.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.12 The vsnprintf_s function (p: 600)
 *    http://en.cppreference.com/w/c/io/vfprintf
 *    * only included in safeclib with \c --enable-unsafe
 *
 * @param[out]  dest  pointer to string that will be written into.
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   fmt   format-control string.
 * @param[in]   ap    optional arguments
 *
 * @pre Neither \c dest nor \c fmt shall be a null pointer.
 * @pre \c dmax shall not be greater than \c RSIZE_MAX_STR.
 * @pre \c dmax shall not equal zero.
 * @pre \c dmax shall be greater than <tt>strnlen_s(dest, dmax)</tt>.
 * @pre \c fmt  shall not contain the conversion specifier \c %n.
 * @pre None of the arguments corresponding to \c %s is a null pointer. (not yet)
 * @pre No encoding error shall occur.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  On success the total number of characters written is returned.
 * @return  On failure a negative number is returned.
 * @return  If the buffer \c dest is too small for the formatted text,
 *          including the terminating null, then the buffer is set to an
 *          empty string by placing a null character at \c dest[0], and the
 *          invalid parameter handler is invoked. Unlike \c vsnprintf,
 *          \c vsprintf_s guarantees that the buffer will be null-terminated
 *          unless the buffer size is zero.
 *
 * @retval  -ESNULLP when \c dest/fmt is NULL pointer
 * @retval  -ESZEROL when \c dmax = 0
 * @retval  -ESLEMAX when \c dmax > \c RSIZE_MAX_STR
 * @retval  -EINVAL  when fmt contains %n
 *
 * @see
 *    sprintf_s(), vsprintf_s()
 *
 */

EXPORT int
vsnprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, va_list ap)
{

    int ret = -1;
    const char *p;

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("vsnprintf_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return -(ESLEMAX);
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("vsnprintf_s: dest is null",
                   NULL, ESNULLP);
        return -(ESNULLP);
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vsnprintf_s: fmt is null",
                   NULL, ESNULLP);
        return -(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("vsnprintf_s: dmax is 0",
                   NULL, ESZEROL);
        return -(ESZEROL);
    }

    if (unlikely((p = strnstr(fmt, "%n", RSIZE_MAX_STR)))) {
        /* at the beginning or if inside, not %%n */
        if ((p-fmt == 0) || *(p-1) != '%') {
            invoke_safe_str_constraint_handler("vsnprintf_s: illegal %n",
                                               NULL, EINVAL);
            return -(EINVAL);
        }
    }

    errno = 0;
    ret = vsnprintf(dest, (size_t)dmax, fmt, ap);

    if (unlikely(ret < 0)) {
        char errstr[128] = "vsnprintf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, -ret);
    }

    return ret;
}

#endif /* SAFECLIB_ENABLE_UNSAFE */
#endif /* MINGW64 */
