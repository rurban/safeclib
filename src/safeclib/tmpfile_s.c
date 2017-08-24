#include "safe_lib.h"
#include <stdio.h>

/** 
 * @brief
 *    The tmpfile_s function creates a temporary binary file that is
 *    different from any other existing file and that will
 *    automatically be removed when it is closed or at program
 *    termination. If the program terminates abnormally, whether an
 *    open temporary file is removed is implementation-defined. The
 *    file is opened for update with "wb+" mode with the meaning that
 *    mode has in the fopen_s function (including the mode’s effect on
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
 *
 * If the file was created successfully, then the pointer to FILE
 * pointed to by streamptr will be set to the pointer to the object
 * controlling the opened file. Otherwise, the pointer to FILE pointed
 * to by streamptr will be set to a null pointer.
 *
 * @return The tmpfile_s function returns zero if it created the
 *         file. If it did not create the file or there was a
 *         runtime-constraint violation, tmpfile_s returns a nonzero value.
 *         If there is a runtime-constraint violation, tmpfile_s does not attempt
 *         to create a file.
 *         Returns the streamptr on success..
 */

errno_t tmpfile_s(FILE * restrict * restrict streamptr)
{
    return RCNEGATE(EOK);
}
EXPORT_SYMBOL(tmpfile_s)
