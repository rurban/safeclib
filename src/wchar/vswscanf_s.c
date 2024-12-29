/*------------------------------------------------------------------
 * vswscanf_s.c
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @brief
 *    The \b vswscanf_s function reads a formatted wide string.
 *    Reaching the end of the string is equivalent to reaching the
 *    end-of-file condition for \c fwscanf.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.1.10 The vswscanf_s function (p: 635-636)
 *    http://en.cppreference.com/w/c/io/vfwscanf
 *
 * @param[in]   src    pointer to a null-terminated wide string to read from
 * @param[in]   fmt    format-control wide string.
 * @param[in]   ap     arguments to write to
 *
 * @pre Neither \c src nor \c fmt shall be a null pointer.
 * @pre \c fmt shall not contain the conversion specifier \c %n
 * @pre None of the arguments corresponding to \c %s is a null pointer.
 * @pre No encoding error shall occur.
 * @pre \c %c, \c %s, and \c %[ conversion specifiers each expect two
 *      arguments (the usual pointer and a value of type \c rsize_t
 *      indicating the size of the receiving array, which may be 1
 *      when reading with a \c %lc into a single wide character) and
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
 *    vswscanf_s(), snwprintf_s(), vsnprintf_s()
 *
 */

EXPORT int vswscanf_s(const wchar_t *restrict src, const wchar_t *restrict fmt,
                      va_list ap) {
    wchar_t *p;
    int ret;
    in_fct_wrap_type wrap;

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("vswscanf_s: src is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }
#ifdef HAVE_MINGW32
    if (unlikely(!*src)) {
        return EOF;
    }
#endif

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vswscanf_s: fmt is null",
                                           (void *)src, ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

#if defined(HAVE_WCSSTR) || !defined(SAFECLIB_DISABLE_EXTENSIONS)
    if (unlikely((p = wcsstr((wchar_t *)fmt, L"%n")))) {
        if ((p - fmt == 0) || *(p - 1) != L'%') {
            invoke_safe_str_constraint_handler("vswscanf_s: illegal %n",
                                               (void *)src, EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#elif defined(HAVE_WCSCHR)
    if (unlikely((p = wcschr(fmt, flen, L'n')))) {
        /* at the beginning or if inside, not %%n */
        if (((p - fmt >= 1) && *(p - 1) == L'%') &&
            ((p - fmt == 1) || *(p - 2) != L'%')) {
            invoke_safe_str_constraint_handler("vswscanf_s: illegal %n",
                                               (void *)src, EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#else
#error need wcsstr or wcschr
#endif

    errno = 0;
    wrap.arg = src;
    //ret = vswscanf(src, fmt, ap);
    ret = safec_vscanf_s(safec_in_wbuf, "vswscanf_s", &wrap, fmt, ap);

    if (unlikely(ret < 0)) { /* always -1 EOF */
        char errstr[128] = "vswscanf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, (void *)src, errno);
    }

    return ret;
}
