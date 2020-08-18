/*------------------------------------------------------------------
 * sprintf_s.c
 *
 * November 2014, Charlie Lenahan
 * April 2017, Reini Urban
 * February 2018, Reini Urban
 *
 * Copyright (c) 2014 by Charlie Lenahan
 * Copyright (c) 2017,2018 by Reini Urban
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

#if defined(TEST_MSVCRT) && defined(HAVE_SPRINTF_S)
#else

/**
 * @def sprintf_s(dest,dmax,fmt,...)
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
 * @retval -ESNULLP when \c fmt is NULL pointer
 *         -ESNULLP when \c dest is NULL and dmax > 0
 *         -ESZEROL when \c dmax = 0 and dest is not NULL
 *         -ESLEMAX when \c dmax > \c RSIZE_MAX_STR or dmax > size of dest
 *         -EOVERFLOW when \c dmax > size of dest
 *         -ESNOSPC when return value exceeds dmax unless dmax is zero and dest
 * is NULL -EINVAL  when \c fmt contains \c %n
 *
 * @retval -1  if an encoding error or a runtime constraint violation in the
 *             libc function \c vsnprintf occured.
 *
 * @note The C11 standard was most likely wrong with changing the return value
 * 0 on errors. All other functions and existing C11 implementations do return
 * -1, so we return negative error codes. See the
 * http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1141.pdf revision for their
 * rationale. sprintf_s does not set \c errno.
 *
 * @see
 *    vsprintf_s(), snprintf_s()
 */

#ifdef SAFECLIB_HAVE_C99
EXPORT int _sprintf_s_chk(char *restrict dest, const rsize_t dmax,
                          const size_t destbos, const char *restrict fmt, ...)
#else
EXPORT int sprintf_s(char *restrict dest, rsize_t dmax,
                     const char *restrict fmt, ...)
#endif
{
    int ret = -1;
    va_list ap;
    const char *p;
#ifndef SAFECLIB_HAVE_C99
    const size_t destbos = BOS_UNKNOWN;
#endif

    if (unlikely(dmax && dest == NULL)) {
        invoke_safe_str_constraint_handler("sprintf_s: dest is null", NULL,
                                           ESNULLP);
        return -ESNULLP;
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("sprintf_s: fmt is null", dest,
                                           ESNULLP);
        return -ESNULLP;
    }

    if (unlikely(dest && dmax == 0)) {
        invoke_safe_str_constraint_handler("sprintf_s: dmax is 0", dest,
                                           ESZEROL);
        return -ESZEROL;
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("sprintf_s: dmax exceeds max", dest,
                                           ESLEMAX);
        return -ESLEMAX;
    }
    if (destbos == BOS_UNKNOWN) {
        if (dmax) {
            BND_CHK_PTR_BOUNDS(dest, dmax);
        }
    } else {
        if (unlikely(dmax > destbos)) {
            return -(handle_str_bos_overload("sprintf_s: dmax exceeds dest",
                                             dest, destbos));
        }
    }

    if (unlikely((p = strnstr(fmt, "%n", RSIZE_MAX_STR)))) {
        /* at the beginning or if inside, not %%n */
        if ((p - fmt == 0) || *(p - 1) != '%') {
            invoke_safe_str_constraint_handler("sprintf_s: illegal %n",
                                               (char *restrict)fmt, EINVAL);
            return -1; /* EINVAL */
        }
    }

    /* TODO: in order to check for NULL fmt args, need va_copy.
    va_copy(ap2, ap);
    void *ptr = va_arg(*ap2, void*);
    if (ptr == NULL)
        return -(ESNULLP);
    va_end(ap2);
    */

    va_start(ap, fmt);
    /* FIXME: gcc 4.3 GCC_DIAG_IGNORE(-Wmissing-format-attribute) */
#if defined(_WIN32) && defined(HAVE_VSNPRINTF_S)
    /* to detect illegal format specifiers */
    ret = vsnprintf_s(dest, (size_t)dmax, (size_t)dmax, fmt, ap);
    /*#elif defined(HAVE___VSNPRINTF_CHK) */
    /* glibc allows %n from readonly strings, freebsd/darwin ignores flag. */
    /*ret = __vsnprintf_chk(dest, (size_t)dmax, 2, (size_t)dmax, fmt, ap);*/
#else
    ret = vsnprintf((char *)dest, (size_t)dmax, fmt, ap);
#endif
    /* GCC_DIAG_RESTORE */
    va_end(ap);

    if (unlikely(dmax && ret >= (int)dmax)
#ifdef HAVE_MINGW32
        || (ret == -1 && errno == ERANGE)
#endif
    ) {
        handle_error(dest, dmax, "sprintf_s: len exceeds dmax", ESNOSPC);
#ifdef HAVE_MINGW32
        errno = 0;
#endif
        return -ESNOSPC; /* different to the standard (=0),
                            but like all other implementations */
    }

    if (unlikely(ret <= 0)) {
        char errstr[128] = "sprintf_s: ";
        strcat(errstr, strerror(errno));
        handle_error(dest, dmax, errstr, -ret);
    }

    return ret;
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_sprintf_s_chk);
#endif /* __KERNEL__ */

#endif /* TEST_MSVCRT */
