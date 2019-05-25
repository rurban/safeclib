/*------------------------------------------------------------------
 * freopen_s.c
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

/* conflicting API */
#if (defined(TEST_MSVCRT) && defined(HAVE_FREOPEN_S)) ||                       \
    defined(MINGW_HAS_SECURE_API)
#else

/**
 * @brief
 *    The \c freopen_s function first, attempts to close the file associated
 *    with the stream, ignoring any errors. Then, if filename is not null,
 *    attempts to open the file specified by filename using mode as if by
 *    fopen, and associates that file with the file stream pointed to by
 *    stream. If filename is a null pointer, then the function attempts to
 *    reopen the file that is already associated with stream (it is
 *    implementation defined which mode changes are allowed in this case).
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.2.2 The freopen_s function (p: 590)
 *    http://en.cppreference.com/w/c/io/freopen
 *
 * @param[out] newstreamptr pointer to a FILE stream that will be updated by
 * this function call
 * @param[in]  filename     file name to associate the file stream to
 * @param[in]  mode         as in fopen
 * @param[in]  stream       the file stream to modify
 *
 * @pre Neither streamptr, stream nor mode shall be a null pointer.
 *
 * If the file was reopened successfully, then the pointer to the FILE
 * pointed to by newstreamptr will be set to the pointer to the object
 * controlling the opened file. Otherwise, the pointer to the FILE pointed
 * to by newstreamptr will be set to a null pointer.
 *
 * @return The freopen_s function returns zero if it reopened the
 *         file. If not or there was a
 *         runtime-constraint violation, freopen_s returns a nonzero error
 * code, and sets newstreamptr to NULL. If there is a runtime-constraint
 * violation, freopen_s does not attempt to reopen the file. Sets the
 * newstreamptr on success.
 * @retval  EOK        on success
 * @retval  ESNULLP    when any argument is a NULL pointer
 * @retval  > 0        any other errno
 */

EXPORT errno_t freopen_s(FILE *restrict *restrict newstreamptr,
                         const char *restrict filename,
                         const char *restrict mode, FILE *restrict stream) {

    if (unlikely(newstreamptr == NULL)) {
        invoke_safe_str_constraint_handler("freopen_s: newstreamptr is null",
                                           NULL, ESNULLP);
        return ESNULLP;
    }

    if (unlikely(mode == NULL)) {
        invoke_safe_str_constraint_handler("freopen_s: mode is null", NULL,
                                           ESNULLP);
        return ESNULLP;
    }

    if (unlikely(stream == NULL)) {
        invoke_safe_str_constraint_handler("freopen_s: stream is null", NULL,
                                           ESNULLP);
        return ESNULLP;
    }

    errno = 0;
    *newstreamptr = freopen(filename, mode, stream);

    if (unlikely(*newstreamptr == NULL)) {
        char errstr[128] = "freopen_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, errno);
        return errno;
    }

    return EOK;
}

#endif /* MINGW_HAS_SECURE_API */
