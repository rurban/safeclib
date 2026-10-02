/*------------------------------------------------------------------
 * vu8sscanf_s.c
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
#include "io/safec_file.h"
#include "u8_private.h"
#endif

/**
 * @brief
 *    The vu8sscanf_s function reads a formatted utf-8 string, and
 *    writes to a list of arguments. Like u8sscanf_s(), the \c %c,
 *    \c %s, and \c %[ conversion specifiers here *require* their
 *    Annex K companion \c rsize_t destination-size argument: it is
 *    always consumed and enforced.
 *
 * @param[in]   dest   pointer to a NUL-terminated utf-8 string to read from
 * @param[in]   fmt    format-control string.
 * @param[out]  ap     arguments to write to; every \c %c, \c %s, and
 *                      \c %[ is followed by an \c rsize_t giving the
 *                      size of its destination array
 *
 * @pre Neither \c dest nor \c fmt shall be a null pointer.
 * @pre \c dest shall not contain an illegal or truncated UTF-8 sequence.
 * @pre \c %c, \c %s, and \c %[ conversion specifiers each expect two
 *      arguments (the usual pointer and a value of type \c rsize_t
 *      indicating the size of the receiving array, which may be 1
 *      when reading with a \c %c into a single character).
 *
 * @return Number of receiving arguments successfully assigned, or \c EOF
 *         if read failure occurs before the first receiving argument
 *         was assigned or if there is a runtime constraint
 *         violation.
 *
 * @retval  > 0  on success, the number of arguments assigned
 * @retval  EOF  on error, or when \c dest contains illegal UTF-8
 *               or a \c %c/\c %s/\c %[ destination is too small
 *               (errno set to \c EILSEQ resp. \c ESNOSPC)
 *
 * @see
 *    u8sscanf_s(), vsscanf_s()
 */
EXPORT int vu8sscanf_s(const char8_t *restrict dest, const char *restrict fmt,
                       va_list ap) {
    int ret;
    _SAFEC_FILE sf = {
        .buf = (unsigned char *)(void *)dest,
        .cookie = (void *)dest,
        .read = safec_string_read,
        .lock = -1
    };

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("vu8sscanf_s: dest is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vu8sscanf_s: fmt is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

    if (unlikely(!u8_is_valid(dest, RSIZE_MAX_STR))) {
        invoke_safe_str_constraint_handler(
            "vu8sscanf_s: illegal UTF-8 sequence in dest", NULL, EILSEQ);
        errno = EILSEQ;
        return EOF;
    }

#if defined(HAVE_STRSTR)
    {
        const char *p;
        if (unlikely((p = strstr(fmt, "%n")))) {
            if ((p - fmt == 0) || *(p - 1) != '%') {
                invoke_safe_str_constraint_handler("vu8sscanf_s: illegal %n",
                                                   NULL, EINVAL);
                errno = EINVAL;
                return EOF;
            }
        }
    }
#endif

    errno = 0;
    ret = safec_vfscanf_s(&sf, "vu8sscanf_s", fmt, ap, 1);

    if (unlikely(ret < 0)) { /* always -1 EOF */
        errno_t saved_errno = errno;
        char errstr[128] = "vu8sscanf_s: ";
        strcat(errstr, strerror(saved_errno));
        invoke_safe_str_constraint_handler(errstr, NULL, saved_errno);
        /* strerror() is not guaranteed to leave errno untouched, esp.
           for out-of-range (safeclib ES*) codes it doesn't recognize */
        errno = saved_errno;
    }

    return ret;
}
