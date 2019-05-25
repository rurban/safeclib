/*------------------------------------------------------------------
 * vsscanf_s.c
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
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#endif

/* TODO:
any of the arguments corresponding to %s is a null pointer.
*/

/**
 * @brief
 *    The \c vsscanf_s function reads a formatted string, and writes
 *    to a list of arguments.
 *    Reaching the end of the string is equivalent to reaching the
 *    end-of-file condition for \c fscanf.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.14 The vsscanf_s function (p: 602)
 *    http://en.cppreference.com/w/c/io/vfscanf
 *
 * @param[in]   buffer pointer to a null-terminated string to read from
 * @param[in]   fmt    format-control string.
 * @param[out]  ap     arguments to write to
 *
 * @pre Neither \c buffer nor \c fmt shall be a null pointer.
 * @pre \c fmt shall not contain the conversion specifier \c %n
 * @pre None of the arguments corresponding to \c %s is a null pointer. (not
 * yet)
 * @pre No encoding error shall occur.
 * @pre \c %c, \c %s, and \c %[ conversion specifiers each expect two
 *      arguments (the usual pointer and a value of type \c rsize_t
 *      indicating the size of the receiving array, which may be 1
 *      when reading with a \c %c into a single character) and
 *      except that the following errors are detected at runtime and
 *      call the currently installed constraint handler function. (not yet)
 *
 * @warning The current implementation just does some basic argument
 *      checks and then calls the native \c vsscanf() libc
 *      function. Thus the \c %s null pointer check and the two-arg
 *      versions of \c %c, \c %s, and \c %[ are not yet implemented.
 *
 * @return Number of receiving arguments successfully assigned, or \c EOF
 *         if read failure occurs before the first receiving argument
 *         was assigned or if there is a runtime constraint
 *         violation.
 *
 * @retval  > 0  on success, the number of arguments assigned
 * @retval  EOF  on error
 *
 * @see
 *    vvsscanf_s(), swscanf_s(), vsnprintf_s()
 *
 */

EXPORT int vsscanf_s(const char *restrict buffer, const char *restrict fmt,
                     va_list ap) {
#if defined(HAVE_STRSTR)
    char *p;
#endif
    int ret;

    if (unlikely(buffer == NULL)) {
        invoke_safe_str_constraint_handler("vsscanf_s: buffer is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vsscanf_s: fmt is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

#if defined(HAVE_STRSTR)
    if (unlikely((p = strstr((char *)fmt, "%n")))) {
        if ((p - fmt == 0) || *(p - 1) != '%') {
            invoke_safe_str_constraint_handler("vsscanf_s: illegal %n", NULL,
                                               EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#elif defined(HAVE_STRCHR)
    if (unlikely((p = strchr(fmt, flen, 'n')))) {
        /* at the beginning or if inside, not %%n */
        if (((p - fmt >= 1) && *(p - 1) == '%') &&
            ((p - fmt == 1) || *(p - 2) != '%')) {
            invoke_safe_str_constraint_handler("vsscanf_s: illegal %n", NULL,
                                               EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#endif

    errno = 0;
    ret = vsscanf(buffer, fmt, ap);

    if (unlikely(ret < 0)) { /* always -1 EOF */
        char errstr[128] = "vsscanf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, errno);
    }

    return ret;
}
