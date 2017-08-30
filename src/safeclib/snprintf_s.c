/* August 2017, Reini Urban
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

/* Need restrict */
#include "config.h"
#include "safe_str_lib.h"
#include "safe_str_constraint.h"
#include <stdarg.h>

/* TODO: error when fmt contains %n, or encoding errors occur.
 */

/** 
 * @brief
 *    The snprintf_s function composes a string with same test that 
 *    would be printed if format was used on printf. Instead of being 
 *    printed, the content is stored in dest.
 *    More than dmax - 1 characters might be written, so this variant is unsafe!
 *    Always use sprintf_s instead.
 *    The resulting character string will be terminated with a null character,
 *    unless dmax is zero. If dmax is zero, nothing is written and
 *    dest may be a null pointer, however the return value (number
 *    of bytes that would be written) is still calculated and
 *    returned.
 *    Unlike the safe variant sprintf_s, snprintf_s does not
 *    guarantees that the buffer will be null-terminated unless
 *    the buffer size is zero.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.6 The snprintf_s function (p: 594-595)
 *    http://en.cppreference.com/w/c/string/byte/sprintf
 *
 * @param[out]  dest  pointer to string that will be written into.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   fmt   format-control string.
 * @param[in]   ...   optional arguments
 *
 * @pre Neither dest nor fmt shall be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR.
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than strnlen_s(dest, dmax).
 * @pre fmt  shall not contain the conversion specifier %n.
 * @pre None of the arguments corresponding to %s is a null pointer.
 * @pre No encoding error shall occur.
 *
 * @return Number of characters not including the terminating null
 *         character (which is always written as long as buffer is not
 *         a null pointer and dmax is not zero and not greater than
 *         RSIZE_MAX_STR), which would have been written to dest if
 *         dmax was ignored, or a negative value if a runtime
 *         constraints violation or an encoding error occurred.
 *
 * @retval  ESNULLP when dest/fmt is NULL pointer
 * @retval  ESZEROL when dmax = 0
 * @retval  ESLEMAX when dmax > RSIZE_MAX_STR
 *
 * @see
 *    sprintf_s(), vsnprintf_s()
 *
 */

int snprintf_s(char * restrict dest, rsize_t dmax, const char * restrict fmt, ...)
{
    va_list ap;
    int ret = -1;
    if (dmax > RSIZE_MAX_STR) {
        invoke_safe_str_constraint_handler("snprintf_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (dest == NULL) {
        invoke_safe_str_constraint_handler("snprintf_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (fmt == NULL) {
        invoke_safe_str_constraint_handler("snprintf_s: fmt is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (dmax == 0) {
        invoke_safe_str_constraint_handler("snprintf_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    va_start(ap, fmt);

    ret = vsnprintf(dest, (size_t)dmax, fmt, ap);

    /*if (ret >= (int)dmax) {
        invoke_safe_str_constraint_handler("snprintf_s: len exceeds dmax",
                   NULL, ESNOSPC);
        *dest = 0;
        ret = RCNEGATE(ESNOSPC);
    }*/

    va_end(ap);

    return ret;
}
