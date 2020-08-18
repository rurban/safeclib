/*------------------------------------------------------------------
 * snprintf_s.c
 *
 * August 2017, Reini Urban
 * February 2018, Reini Urban
 *
 * Copyright (c) 2017, 2018 by Reini Urban
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

#ifdef SAFECLIB_ENABLE_UNSAFE

/* TODO:
any of the arguments corresponding to %s is a null pointer
*/

/**
 * @def snprintf_s(dest,dmax,fmt,...)
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
 * @pre None of the arguments corresponding to \c %s is a null pointer. (not
 * yet)
 * @pre No encoding error shall occur.
 *
 * @return  If the buffer \c dest is too small for the formatted text,
 *          including the terminating null, then the buffer is truncated
 *          and null terminated.
 * @retval  Number of characters not including the terminating null
 *          character, or a negative error number if a runtime
 *          constraints violation or an encoding error occurred.
 *
 * @retval  -ESNULLP    when \c dest/fmt is NULL pointer
 * @retval  -ESZEROL    when \c dmax == 0
 * @retval  -ESLEMAX    when \c dmax > \c RSIZE_MAX_STR
 * @retval  -EOVERFLOW  when \c dmax > size of dest
 * @retval  -EINVAL     when \c fmt contains %n
 *
 * @see
 *    sprintf_s(), vsnprintf_s(), snwprintf_s()
 */

#ifdef SAFECLIB_HAVE_C99
EXPORT int _snprintf_s_chk(char *restrict dest, rsize_t dmax,
                           const size_t destbos, const char *restrict fmt, ...)
#else
EXPORT int snprintf_s(char *restrict dest, rsize_t dmax,
                      const char *restrict fmt, ...)
#endif
{
    va_list ap;
    const char *p;
    int ret = -1;
#ifndef SAFECLIB_HAVE_C99
    const size_t destbos = BOS_UNKNOWN;
#endif

    if (unlikely(dest == NULL ||
                 fmt == NULL)) { /* for !dmax size-calc use sprintf_s */
        invoke_safe_str_constraint_handler("snprintf_s: dest/fmt is null", NULL,
                                           ESNULLP);
        return -(ESNULLP);
    }
    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("snprintf_s: dmax is zero", dest,
                                           ESZEROL);
        return -ESZEROL;
    }
    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("snprintf_s: dmax exceeds max", dest,
                                           ESLEMAX);
        return -ESLEMAX;
    }
    if (destbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        if (unlikely(dmax > destbos)) {
            return -(handle_str_bos_overload("snprintf_s: dmax exceeds dest",
                                             dest, destbos));
        }
    }

    if (unlikely((p = strnstr(fmt, "%n", RSIZE_MAX_STR)))) {
        /* at the beginning or if inside, not %%n */
        if ((p - fmt == 0) || *(p - 1) != '%') {
            invoke_safe_str_constraint_handler("snprintf_s: illegal %n", dest,
                                               EINVAL);
            return -(EINVAL);
        }
    }

    errno = 0;
    va_start(ap, fmt);
    ret = vsnprintf(dest, (size_t)dmax, fmt, ap);
    va_end(ap);

    if (unlikely(ret < 0)) {
        char errstr[128] = "snprintf_s: ";
#ifdef HAVE_MINGW32
        if (errno == ERANGE)
            strcat(errstr, "len exceeds dmax");
        else
#endif
            strcat(errstr, strerror(errno));
        handle_error(dest, dmax, errstr, -ret);
        return ret;
    }
    /* manual truncation */
#ifdef SAFECLIB_STR_NULL_SLACK
    /* oops, ret would have been written if dmax was ignored */
    if ((rsize_t)ret > dmax) {
        dest[dmax - 1] = '\0';
    } else {
        memset(&dest[ret], 0, dmax - ret);
    }
#else
    dest[dmax - 1] = '\0';
#endif

    return ret;
}

#endif /* SAFECLIB_ENABLE_UNSAFE */
