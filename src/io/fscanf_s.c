/*------------------------------------------------------------------
 * fscanf_s.c
 *
 * September 2017, Reini Urban
 * February 2022, Reini Urban
 *
 * Copyright (c) 2017,2022 by Reini Urban
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
 %s null pointer check
 the two-arg versions of \c %c, \c %s, and \c %[.
*/

/**
 * @brief
 *    The \c fscanf_s function reads a formatted string from a buffered
 *    FILE stream, and writes to a list of arguments.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.2 The fscanf_s function (p: 592-593)
 *    http://en.cppreference.com/w/c/io/fscanf
 *
 * @param[in]   stream pointer to a FILE stream to read from
 * @param[in]   fmt    format-control string.
 * @param[out]  ...    arguments to write to
 *
 * @pre Neither \c stream nor \c fmt shall be a null pointer.
 * @pre \c fmt shall not contain the conversion specifier \c %n
 * @pre None of the arguments corresponding to \c %s is a null pointer.
 * @pre No encoding error shall occur.
 * @pre \c %c, \c %s, and \c %[ conversion specifiers each expect two
 *      arguments (the usual pointer and a value of type \c rsize_t
 *      indicating the size of the receiving array, which may be 1
 *      when reading with a \c %c into a single character) and
 *      except that the following errors are detected at runtime and
 *      call the currently installed constraint handler function.
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
 *    vfscanf_s(), fwscanf_s(), scanf_s()
 *
 */

EXPORT int fscanf_s(FILE *restrict stream, const char *restrict fmt, ...) {
    va_list ap;
    int ret;
#if defined(HAVE_STRSTR)
    char *p;
#endif

    if (unlikely(stream == NULL)) {
        invoke_safe_str_constraint_handler("fscanf_s: stream is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("fscanf_s: fmt is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

#if defined(HAVE_STRSTR)
    if (unlikely((p = strstr((char *)fmt, "%n")))) {
        if ((p - fmt == 0) || *(p - 1) != '%') {
            invoke_safe_str_constraint_handler("fscanf_s: illegal %n", NULL,
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
            invoke_safe_str_constraint_handler("fscanf_s: illegal %n", NULL,
                                               EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#endif

    errno = 0;
    va_start(ap, fmt);
    ret = safec_vfscanf_s(stream, "fscanf_s", fmt, ap);
    //ret = vfscanf(stream, fmt, ap);
    va_end(ap);

    if (unlikely(ret < 0)) { /* always -1 EOF */
        char errstr[128] = "fscanf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, errno);
    }

    return ret;
}
