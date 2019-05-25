/*------------------------------------------------------------------
 * fopen_s.c
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
#if (defined(TEST_MSVCRT) && defined(HAVE_FOPEN_S)) ||                         \
    defined(MINGW_HAS_SECURE_API)
#else

/**
 * @brief
 *    The \c fopen_s function opens a file indicated by filename and updates
 *    the pointer to the file stream associated with that file. mode is used to
 *    determine the file access mode.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.2.1 The fopen_s function (p: 588-590)
 *    http://en.cppreference.com/w/c/io/fopen
 *
 * @param[out] streamptr pointer to a FILE stream that will be updated by this
 *                       function call
 * @param[in]  filename
 * @param[in]  mode      as in fopen
 *
 * @pre Neither streamptr, filename nor mode shall be a null pointer.
 *
 * If the file was created successfully, then the pointer to the FILE
 * pointed to by streamptr will be set to the pointer to the object
 * controlling the opened file. Otherwise, the pointer to the FILE pointed
 * to by streamptr will be set to a null pointer.
 *
 * @return The fopen_s function returns zero if it created the
 *         file. If it did not create the file or there was a
 *         runtime-constraint violation, fopen_s returns a nonzero error code,
 *         sets streamptr to NULL.
 *         If there is a runtime-constraint violation, fopen_s does not attempt
 *         to create a file.
 *         Sets the streamptr on success.
 * @retval  EOK        on success
 * @retval  ESNULLP    when any argument is a NULL pointer
 * @retval  > 0        any other errno
 */

EXPORT errno_t fopen_s(FILE *restrict *restrict streamptr,
                       const char *restrict filename,
                       const char *restrict mode) {

    if (unlikely(streamptr == NULL)) {
        invoke_safe_str_constraint_handler("fopen_s: streamptr is null", NULL,
                                           ESNULLP);
        return ESNULLP;
    }

    if (unlikely(filename == NULL)) {
        invoke_safe_str_constraint_handler("fopen_s: filename is null", NULL,
                                           ESNULLP);
        return ESNULLP;
    }

    if (unlikely(mode == NULL)) {
        invoke_safe_str_constraint_handler("fopen_s: mode is null", NULL,
                                           ESNULLP);
        return ESNULLP;
    }

    errno = 0;
    *streamptr = fopen(filename, mode);

    if (unlikely(*streamptr == NULL)) {
        char errstr[128] = "fopen_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, errno);
        return errno;
    }

    return EOK;
}

#endif /* MINGW_HAS_SECURE_API */
