/*------------------------------------------------------------------
 * strnterminate_s.c
 *
 * February 2011, Bo Berry
 *
 * Copyright (c) 2011 by Cisco Systems, Inc
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
 * @def strnterminate_s(str,smax)
 * @brief
 *    The strnterminate_s function will terminate the string if a
 *    null is not encountered before dmax characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest  pointer to string
 * @param  dmax  maximum length of string
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall not equal zero.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @return
 *    The function returns a terminated string.  If a null is not
 *    encountered prior to dmax characters, the dmax character is
 *    set to null terminating the string. The string length is
 *    also returned.
 *
 * @see
 *    strnlen_s()
 *
 */
#ifdef FOR_DOXYGEN
rsize_t strnterminate_s(char *dest, rsize_t dmax)
#else
EXPORT rsize_t _strnterminate_s_chk(char *dest, rsize_t dmax,
                                    const size_t destbos)
#endif
{
    rsize_t count;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strnterminate_s: dest is null",
                                           (void *)dest, ESNULLP);
        return (0);
    }
    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strnterminate_s: dmax is 0",
                                           (void *)dest, ESZEROL);
        return (0);
    }
    if (destbos == BOS_UNKNOWN) {
        if (unlikely(dmax > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler(
                "strnterminate_s: dmax exceeds max", (void *)dest, ESLEMAX);
            return (0);
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        if (unlikely(dmax > destbos)) {
            invoke_safe_str_constraint_handler(
                "strnterminate_s: dmax exceeds dest", (void *)dest, EOVERFLOW);
            return (0);
        }
    }

    count = 0;
    while (dmax > 1) {
        if (*dest) {
            count++;
            dmax--;
            dest++;
        } else {
            break;
        }
    }
    *dest = '\0';

    return (count);
}
