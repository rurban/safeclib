/*------------------------------------------------------------------
 * strprefix_s.c
 *
 * November 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
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
 * @brief
 *    Determines if the prefix pointed to by src is at the
 *    beginning of string pointed to by dest.  The prefix
 *    must be a complete match in dest.  Useful for command
 *    or user input parsing.  The scanning stops at the first
 *    null in dest or src, or after dmax characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest  pointer to string to compare against
 * @param  dmax  restricted maximum length of dest
 * @param  src   pointer to the prefix
 *
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  Neither dmax nor slen shall not be 0.
 * @pre  Neither dmax nor slen shall not be greater than RSIZE_MAX_STR.
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dest/src/substring is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  ESNOTFND   when prefix not found in dest
 *
 * @see
 *    strspn_s(), strcspn_s(), strpbrk_s(), strstr_s()
 *
 */
EXPORT errno_t
strprefix_s (const char *dest, rsize_t dmax, const char *src)
{
    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strprefix_s: dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("strprefix_s: src is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strprefix_s: dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strprefix_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    if (unlikely(*src == '\0')) {
        return (ESNOTFND);
    }

    /* TODO: if src and dmax are at least a word long, compare wordwise */
    while (*src && dmax) {

        if (*dest != *src) {
            return (ESNOTFND);
        }

        dmax--;
        dest++;
        src++;
    }

    return (EOK);
}
