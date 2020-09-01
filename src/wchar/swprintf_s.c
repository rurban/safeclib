/*------------------------------------------------------------------
 * swprintf_s.c
 *
 * September 2017, Reini Urban
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

/* TODO:
any of the arguments corresponding to %s is a null pointer.
*/

/**
 * @def
 * @brief
 *    The \c swprintf_s function composes a wide string
 *    with same test that would be printed if format was used on \c
 *    wprintf. Instead of being printed, the content is stored in
 *    dest.
 *    With \c SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by \c swprintf_s in the
 *    array of dmax wide characters pointed to by \c dest are nulled when
 *    \c swprintf_s returns.
 *
 * @note
 *    POSIX specifies that \c errno is set on error. However, the safeclib
 *    extended \c ES* errors do not set \c errno, only when the underlying
 *    system \c vswprintf call fails, \c errno is set.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.1.4 The swprintf_s function (p: 630-631)
 *    http://en.cppreference.com/w/c/io/fwprintf
 *
 * @param[out]  dest  pointer to wide string that will be written into
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   fmt   format-control wide string
 * @param[in]   ...   optional arguments
 *
 * @pre Neither \c dest nor \c fmt shall be a null pointer.
 * @pre \c dmax shall not be greater than \c RSIZE_MAX_WSTR and size of dest.
 * @pre \c dmax shall not equal zero.
 * @pre \c dmax shall be greater than <tt>wcsnlen_s(dest, dmax)</tt>.
 * @pre \c fmt shall not contain the conversion specifier \c %n
 * @pre None of the arguments corresponding to \c %s is a null pointer. (not
 * yet)
 * @pre No encoding error shall occur.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @note While narrow strings provide \c snprintf, which makes it possible to
 *       determine the required output buffer size, there is no equivalent for
 *       wide strings (until C11's \c swprintf_s), and in order to determine
 *       the buffer size, the program may need to call \c swprintf, check the
 *       result value, and reallocate a larger buffer, trying again until
 *       successful.
 *
 * @return  If no runtime-constraint violation occurred, the swprintf_s
 *          function returns the number of wide characters written in the
 *          array, not counting the terminating null wide character. If an
 *          encoding error occurred or if n or more wide characters are
 *          requested to be written, swprintf_s returns a negative value.
 * @return  If the buffer \c dest is too small for the formatted text,
 *          including the terminating null, then the buffer is set to an
 *          empty string by placing a null wide character at \c dest[0], and
 * the invalid parameter handler is invoked.
 *
 * @retval  >0         on success
 * @retval  -ESNULLP   when \c dest/fmt is NULL pointer
 * @retval  -ESZEROL   when \c dmax = 0
 * @retval  -ESLEMAX   when \c dmax > \c RSIZE_MAX_WSTR
 * @retval  -EOVERFLOW when \c dmax > size of dest
 * @retval  -ESNOSPC   when return value exceeds dmax
 * @retval  -EINVAL    when \c fmt contains \c %n
 * @retval  -1         if an encoding error occurred or if n or more wide
 * characters are requested to be written.
 * @retval  0          on some other error
 *
 * @see
 *    vswprintf_s(), snwprintf_s(), vsnprintf_s()
 */
#if defined FOR_DOXYGEN
int swprintf_s(wchar_t *restrict dest, rsize_t dmax,
               const wchar_t *restrict fmt, ...)
#elif defined(SAFECLIB_HAVE_C99) && !defined(TEST_MSVCRT)
EXPORT int _swprintf_s_chk(wchar_t *restrict dest, rsize_t dmax,
                           const size_t destbos, const wchar_t *restrict fmt,
                           ...)
#else
EXPORT int swprintf_s(wchar_t *restrict dest, rsize_t dmax,
                      const wchar_t *restrict fmt, ...)
#endif
{
    va_list ap, ap2;
    wchar_t *p;
    int ret = -1;
    const size_t destsz = dmax * sizeof(wchar_t);
#if !(defined(SAFECLIB_HAVE_C99) && !defined(TEST_MSVCRT))
    const size_t destbos = BOS_UNKNOWN;
#endif

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("swprintf_s: dest is null", NULL,
                                           ESNULLP);
        return -(ESNULLP);
    }
    if (unlikely(dmax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("swprintf_s: dmax exceeds max",
                                           (void *)dest, ESLEMAX);
        return -(ESLEMAX);
    }
    if (destbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(dest, destsz);
    } else {
        if (unlikely(destsz > destbos)) {
            invoke_safe_str_constraint_handler("swprintf_s: dmax exceeds dest",
                                               (void *)dest, EOVERFLOW);
            return -(EOVERFLOW);
        }
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("swprintf_s: fmt is null",
                                           (void *)dest, ESNULLP);
        return -(ESNULLP);
    }
    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("swprintf_s: dmax is 0",
                                           (void *)dest, ESZEROL);
        return -(ESZEROL);
    }

    /* fmt + args might be empty which is then valid
    if (unlikely(dmax == 1)) {
        invoke_safe_str_constraint_handler("swprintf_s: dmax is 0",
                   (void*)dest, ESNOSPC);
        errno = -(ESNOSPC);
        return 0;
    }*/

#if defined(HAVE_WCSSTR) || !defined(SAFECLIB_DISABLE_EXTENSIONS)
    if (unlikely((p = wcsstr((wchar_t *)fmt, L"%n")))) {
        if ((p - fmt == 0) || *(p - 1) != L'%') {
            invoke_safe_str_constraint_handler("swprintf_s: illegal %n",
                                               (void *)dest, EINVAL);
            return -(EINVAL);
        }
    }
#elif defined(HAVE_WCSCHR)
    if (unlikely((p = wcschr(fmt, flen, L'n')))) {
        /* at the beginning or if inside, not %%n */
        if (((p - fmt >= 1) && *(p - 1) == L'%') &&
            ((p - fmt == 1) || *(p - 2) != L'%')) {
            invoke_safe_str_constraint_handler("swprintf_s: illegal %n",
                                               (void *)dest, EINVAL);
            return -(EINVAL);
        }
    }
#else
#error need wcsstr or wcschr
#endif

    errno = 0;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    ret = vswprintf(dest, dmax, fmt, ap);
    va_end(ap);

    /* check for ESNOSPC or some other error */
    if (unlikely(ret == -1)) {
        if (unlikely(dmax == 1))
            goto nospc;
        errno = 0;
        if (likely(dmax < 512)) { /* stacksize 2k */
            static wchar_t tmp[512];
            va_start(ap2, fmt);
            ret = vswprintf(tmp, 512, fmt, ap2);
            va_end(ap2);
        } else {
            wchar_t *tmp = (wchar_t *)malloc(dmax * sizeof(wchar_t));
            va_start(ap2, fmt);
            ret = vswprintf(tmp, dmax, fmt, ap2);
            va_end(ap2);
            free(tmp);
        }
        if (ret > 0) {
        nospc:
            errno = 0; /* EOVERFLOW */
            handle_werror(dest, dmax, "swprintf_s: len exceeds dmax", ESNOSPC);
            return -(ESNOSPC);
        }
    }

    if (unlikely(ret >= (int)dmax)) {
        goto nospc;
    } else if (unlikely(ret < 0)) {
        char errstr[128] = "swprintf_s: ";
        strcat(errstr, strerror(errno));
        handle_werror(dest, dmax, errstr, -ret);
    }

    return ret;
}
