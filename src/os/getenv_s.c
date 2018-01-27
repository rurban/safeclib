/*------------------------------------------------------------------
 * getenv_s.c
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
 *    The \c getenv_s function searches for an environmental variable with
 *    name name in the host-specified environment list and returns a pointer
 *    to the string that is associated with the matched environment
 *    variable. The set of environmental variables and methods of altering it
 *    are implementation-defined.  The value of the environment variable is
 *    written to the user-provided buffer value (unless null) and the number
 *    of bytes written is stored in the user-provided location *len (unless
 *    null).
 *    If the environment variable is not set in the environment, zero
 *    is written to *len (unless null) and '\0' is written to value[0] (unless
 *    null).
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the
 *    array of dmax characters pointed to by dest are nulled.
 *    If \c secure_getenv() is available, it is used.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.6.2.1 The getenv_s function (p: 607-607)
 *    http://en.cppreference.com/w/c/program/getenv
 *
 * @param[out] len    pointer to a size_t where getenv_s will store the length
 *                    of the found environment variable. Might be NULL.
 * @param[out] dest   pointer to a string where getenv_s will store the contents
 *                    of the found environment variable.
 * @param[in]  dmax   maximum number of characters that getenv_s is allowed
 *                    to write to dest (size of the buffer).
 * @param[in]  name   null-terminated character string identifying the name of
 *                    the environmental variable to look for.
 *
 * @pre name and dest shall not be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR.
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than the strlen of the returned env value.
 *
 * @return zero if the environment variable was found, non-zero if it was not
 *         found of if a runtime constrant violation occurred. On any error,
 *         writes zero to *len (unless len is a null pointer).
 *
 * @retval  EOK        on success
 * @retval  -ESNULLP   when dest or name are a NULL pointer
 * @retval  -ESZEROL   when dmax = 0
 * @retval  -ESLEMAX   when dmax > RSIZE_MAX_STR
 * @retval  -ESNOSPC   when dmax is too small for the value
 * @retval  -1         when not found
 */

EXPORT errno_t
getenv_s(size_t *restrict len, char *restrict dest, rsize_t dmax,
         const char *restrict name)
{
    const char* buf;
    int len1;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("getenv_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("getenv_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("getenv_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(name == NULL)) {
        handle_error(dest, dmax, "getenv_s: name is null",
                     ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    errno = 0;
#ifdef HAVE_SECURE_GETENV
    buf = secure_getenv(name);
#else
    buf = getenv(name);
#endif

    if (unlikely(buf == NULL)) {
        char errstr[128] = "getenv_s: ";
        if (len)
            *len = 0;
        strcat(errstr, strerror(errno));
        handle_error(dest, dmax, errstr,
                     -1);
        return -1;
    }

    len1 = strlen(buf);
    if (unlikely((rsize_t)len1 >= dmax)) {
        if (len)
            *len = 0;
        handle_error(dest, dmax, "getenv_s: dmax is too small",
                     ESNOSPC);
        return RCNEGATE(ESNOSPC);
    } else {
        if (len)
            *len = len1;
        strcpy_s(dest, dmax, buf);
    }

    return EOK;
}
#ifdef __KERNEL__
EXPORT_SYMBOL(getenv_s);
#endif /* __KERNEL__ */
