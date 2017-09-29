/*------------------------------------------------------------------
 * snprintf_s.c
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

#include "safeclib_private.h"

#ifdef SAFECLIB_ENABLE_UNSAFE

/* TODO:
any of the arguments corresponding to %s is a null pointer
*/

/** 
 * @brief
 *    The truncating \c snprintf_s function composes a string with
 *    same test that would be printed if format was used on \c
 *    printf. Instead of being printed, the content is stored in dest.
 *    More than dmax - 1 characters might be written, so this variant is \b unsafe!
 *    Always use \b sprintf_s instead.
 *    The resulting character string will be terminated with a null character,
 *    unless \c dmax is zero. If \c dmax is zero, nothing is written and
 *    \c dest may be a null pointer, however the return value (number
 *    of bytes that would be written) is still calculated and
 *    returned.
 *    Warning: Unlike the safe variant \c sprintf_s, \c snprintf_s does not
 *    guarantee that the buffer will be null-terminated unless
 *    the buffer size is zero.
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
 *    * only included in safeclib with \c --enable-unsafe
 *
 * @param[out]  dest  pointer to string that will be written into.
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   fmt   format-control string.
 * @param[in]   ...   optional arguments
 *
 * @pre Neither \c dest nor \c fmt shall be a null pointer.
 * @pre \c dmax shall not be greater than \c RSIZE_MAX_STR.
 * @pre \c dmax shall not equal zero.
 * @pre \c dmax shall be greater than <tt>strnlen_s(dest, dmax)</tt>.
 * @pre \c fmt  shall not contain the conversion specifier \c %n.
 * @pre None of the arguments corresponding to \c %s is a null pointer. (not yet)
 * @pre No encoding error shall occur.
 *
 * @return Number of characters not including the terminating null
 *         character (which is always written as long as buffer is not
 *         a null pointer and \c dmax is not zero and not greater than
 *         \c RSIZE_MAX_STR), which would have been written to \c dest if
 *         \c dmax was ignored, or a negative value if a runtime
 *         constraints violation or an encoding error occurred.
 *
 * @retval  -ESNULLP when \c dest/fmt is NULL pointer
 * @retval  -ESZEROL when \c dmax = 0
 * @retval  -ESLEMAX when \c dmax > \c RSIZE_MAX_STR
 * @retval  -EINVAL  when \c fmt contains %n
 *
 * @see
 *    sprintf_s(), vsnprintf_s()
 *
 */

EXPORT int
snprintf_s(char * restrict dest, rsize_t dmax, const char * restrict fmt, ...)
{
    va_list ap;
    const char *p;
    int ret = -1;

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("snprintf_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return -(ESLEMAX);
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("snprintf_s: dest is null",
                   NULL, ESNULLP);
        return -(ESNULLP);
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("snprintf_s: fmt is null",
                   NULL, ESNULLP);
        return -(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("snprintf_s: dmax is 0",
                   NULL, ESZEROL);
        return -(ESZEROL);
    }

    if (unlikely((p = strnstr(fmt, "%n", RSIZE_MAX_STR)))) {
        /* at the beginning or if inside, not %%n */
        if ((p-fmt == 0) || *(p-1) != '%') {
            invoke_safe_str_constraint_handler("snprintf_s: illegal %n",
                                               NULL, EINVAL);
            return -(EINVAL);
        }
    }

    errno = 0;
    va_start(ap, fmt);
    ret = vsnprintf(dest, (size_t)dmax, fmt, ap);
    va_end(ap);

    if (unlikely(ret < 0)) {
        char errstr[128] = "snprintf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, -ret);
    }

    return ret;
}
EXPORT_SYMBOL(snprintf_s)

#endif /* SAFECLIB_ENABLE_UNSAFE */
