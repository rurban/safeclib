/*------------------------------------------------------------------
 * sprintf_s.c
 *
 * November 2014, Charlie Lenahan
 * April 2017, Reini Urban
 *
 * Copyright (c) 2014 by Charlie Lenahan
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

/** 
 * @brief
 *    The sprintf_s function composes a string with same test that 
 *    would be printed if format was used on printf. Instead of being 
 *    printed, the content is stored in dest.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.6 The sprintf_s function (p: 595-596)
 *    http://en.cppreference.com/w/c/string/byte/sprintf
 *
 * @param[out] dest  storage location for output buffer.
 * @param[in]  dmax  maximum number of characters to store in buffer.
 * @param[in]  fmt   format-control string.
 * @param[in]  ...   optional arguments
 *
 * @pre Neither dest nor fmt shall be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR.
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than strnlen_s(dest, dmax).
 * @pre fmt  shall not contain the conversion specifier %n.
 * @pre None of the arguments corresponding to %s is a null pointer. (not yet)
 * @pre No encoding error shall occur.
 *
 * @return  On success the total number of characters written is returned. 
 * @return  On failure a negative number is returned.
 * @return  If the buffer dest is too small for the formatted text,
 *          including the terminating null, then the buffer is set to an
 *          empty string by placing a null character at dest[0], and the
 *          invalid parameter handler is invoked. Unlike _snprintf,
 *          sprintf_s guarantees that the buffer will be null-terminated
 *          unless the buffer size is zero.
 * @retval  -ESNULLP when dest/fmt is NULL pointer
 * @retval  -ESZEROL when dmax = 0
 * @retval  -ESLEMAX when dmax > RSIZE_MAX_STR
 * @retval  -ESNOSPC when return value exceeds dmax
 * @retval  -EINVAL  when fmt contains %n
 *
 */

#include "safeclib_private.h"

int sprintf_s(char * restrict dest, rsize_t dmax, const char * restrict fmt, ...)
{
    va_list ap;
    int ret = -1;
    const char *p;

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("sprintf_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("sprintf_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("sprintf_s: fmt is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("sprintf_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely((p = strnstr(fmt, "%n", RSIZE_MAX_STR)))) {
        /* at the beginning or if inside, not %%n */
        if ((p-fmt == 0) || *(p-1) != '%') {
            invoke_safe_str_constraint_handler("sprintf_s: illegal %n",
                                               NULL, EINVAL);
            return RCNEGATE(EINVAL);
        }
    }

    /* TODO: in order to check for NULL fmt args, need va_copy.
    va_copy(ap2, ap);
    void *ptr = va_arg(*ap2, void*);
    if (ptr == NULL)
        return RCNEGATE(ESNULLP);
    va_end(ap2);
    */

    va_start(ap, fmt);
    ret = vsnprintf(dest, (size_t)dmax, fmt, ap);

    if (unlikely(ret >= (int)dmax)) {
        invoke_safe_str_constraint_handler("sprintf_s: len exceeds dmax",
                   NULL, ESNOSPC);
        *dest = 0;
        ret = RCNEGATE(ESNOSPC);
    }

    va_end(ap);

    return ret;
}
