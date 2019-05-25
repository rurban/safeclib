/*------------------------------------------------------------------
 * tmpfile_s.c
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

/**
 * @brief
 *    The \c tmpfile_s function creates a temporary binary file that is
 *    different from any other existing file and that will
 *    automatically be removed when it is closed or at program
 *    termination. If the program terminates abnormally, whether an
 *    open temporary file is removed is implementation-defined. The
 *    file is opened for update with "wb+" mode with the meaning that
 *    mode has in the \c fopen_s function (including the mode’s effect on
 *    exclusive access and file permissions).
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.1.1 The tmpfile_s function (p: 586-587)
 *    http://en.cppreference.com/w/c/io/tmpfile
 *
 * @param streamptr pointer to a pointer that will be updated by this
 *                  function call
 *
 * @pre streamptr shall not be a null pointer.
 * @pre No more than TMP_MAX_S files may be opened
 *
 * If the file was created successfully, then the pointer to FILE
 * pointed to by streamptr will be set to the pointer to the object
 * controlling the opened file. Otherwise, the pointer to FILE pointed
 * to by streamptr will be set to a null pointer.
 *
 * @return The tmpfile_s function returns zero if it created the
 *         file. If it did not create the file or there was a
 *         runtime-constraint violation, tmpfile_s returns a nonzero value.
 *         If there is a runtime-constraint violation, tmpfile_s does not
 * attempt to create a file. Sets the streamptr on success.
 *
 * @retval  EOK        on success
 * @retval  ESNULLP    when streamptr is a NULL pointer
 * @retval  ESLEMAX    when more than TMP_MAX_S files were opened.
 * @retval  errno()    when tmpfile() failed, typically ENOENT or EACCES
 */

EXPORT errno_t tmpfile_s(FILE *restrict *restrict streamptr) {
    static int count = 0;

    if (unlikely(streamptr == NULL)) {
        invoke_safe_str_constraint_handler("tmpfile_s: streamptr is null", NULL,
                                           ESNULLP);
        return ESNULLP;
    }

    ++count;
    if (unlikely(count > TMP_MAX_S)) {
        *streamptr = NULL;
        invoke_safe_str_constraint_handler("tmpfile_s: exceeds TMP_MAX_S", NULL,
                                           ESLEMAX);
        return ESLEMAX;
    }

    errno = 0;
    *streamptr = tmpfile();

    if (unlikely(*streamptr == NULL)) {
        char errstr[128] = "tmpfile_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, errno);
        return errno;
    }

    return EOK;
}
