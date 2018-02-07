/*------------------------------------------------------------------
 * strerror_s.c
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#include "strerror_s.h"
#endif

/**
 * @brief
 *    The \c strerror_s function returns a pointer to the textual description
 *    of the system error code errnum, identical to the description that would
 *    be printed by perror(). In addition to \c strerror() it adds the size of
 *    the destination array in order to prevent buffer overflow, and it
 *    truncates overlong error messages with "...".
 *
 * @details
 *    No more than bufsz-1 bytes are written, the buffer is always
 *    null-terminated. If the message had to be truncated to fit the buffer
 *    and bufsz is greater than 3, then only bufsz-4 bytes are written, and
 *    the characters "..." are appended before the null terminator.  The
 *    behavior is undefined if writing to dest occurs past the end of the
 *    array, which can happen when the size of the buffer pointed to by dest is
 *    less than the number of characters in the error message which in turn is
 *    less than dmax.
 *
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.4.2 The strerror_s function (p: 622)
 *    http://en.cppreference.com/w/c/byte/strerror
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest    pointer to a user-provided buffer.
 * @param[in]   dmax    restricted maximum length of dest
 * @param[in]   errnum  integer value referring to an error code
 *
 * @pre dest shall not be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR.
 * @pre dmax shall not equal zero.
 *
 * @return  Zero if the entire message was successfully stored in dest,
 *          non-zero otherwise.
 * @retval  EOK        on success
 * @retval  ESNULLP    when dest is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  ESLEMIN    when the result would be longer than 4 and dmax < 4
 *
 * @see
 *    strerrorlen_s()
 */

EXPORT errno_t
strerror_s(char *dest, rsize_t dmax, errno_t errnum)
{
    size_t len;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strerror_s: dest is null",
                   NULL, ESNULLP);
        return ESNULLP;
    }
    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strerror_s: dmax is 0",
                   NULL, ESZEROL);
        return ESZEROL;
    }
    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strerror_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return ESLEMAX;
    }

    len = strerrorlen_s(errnum);
    if (likely(len < dmax)) {
        if (errnum >= ESNULLP && errnum <= ESLAST) {
            const char *tmpbuf = errmsgs_s[errnum - ESNULLP];
            strcpy_s(dest, dmax, tmpbuf);
        } else {
            const char *tmpbuf = strerror(errnum);
            strcpy_s(dest, dmax, tmpbuf);
        }
    } else if (dmax > 3) { /* truncate */
        const char *tmpbuf = (errnum >= ESNULLP && errnum <= ESLAST)
            ? errmsgs_s[errnum - ESNULLP]
            : strerror(errnum);
        strncpy_s(dest, dmax, tmpbuf, dmax-4);
        strcat_s(dest, dmax, "...");
    } else {
        invoke_safe_str_constraint_handler("strerror_s: dmax is too small",
                   NULL, ESLEMIN);
        return ESLEMIN;
    }

    return EOK;
}

/**
 * @brief
 *    The \c strerrorlen_s function returns the untruncated length of the
 *    textual description of the system error code \c errnum, identical to the
 *    description that would be printed by \c perror().
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.4.2 The strerrorlen_s function (p: 622)
 *    http://en.cppreference.com/w/c/byte/strerror
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   errnum  integer value referring to an error code
 *
 * @return The length of the error message or 0
 *
 * @see
 *    strerror_s()
 */

EXPORT size_t
strerrorlen_s(errno_t errnum)
{
    if (errnum >= ESNULLP && errnum <= ESLAST) {
        return len_errmsgs_s[errnum-ESNULLP]-1;
    } else {
        const char *buf = strerror(errnum);
        return buf ? strlen(buf) : 0;
    }
}
