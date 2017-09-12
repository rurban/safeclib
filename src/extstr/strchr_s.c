/*------------------------------------------------------------------
 * strchr_s.c
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

#include "safeclib_private.h"

/**
 * @brief
 *    Finds the first occurrence of ch (after conversion to char as if by
 *    (char)ch) in the null-terminated byte string pointed to by dest (each
 *    character interpreted as unsigned char). The terminating null character
 *    is considered to be a part of the string and can be found when searching
 *    for '\0'.
 *
 * @remark IMPLEMENTED IN
 *
 * @param[in]  dest    pointer to string to compare against
 * @param[in]  dmax    restricted maximum length of dest
 * @param[in]  ch     character to search for
 * @param[out] result  pointer to char* in dest
 *
 * @pre  Neither dest nor result shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR.
 * @pre  ch shall not be greater than 255
 *
 * @retval  EOK        when successfully character found.
 * @retval  ESNULLP    when dest/result is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  ESLEMAX    when ch > 255
 * @retval  ESNOTFND   when ch not found in dest
 *
 * @see
 *    memchr_s(), strspn_s(), strcspn_s(), strpbrk_s(), strstr_s()
 *
 */
EXPORT errno_t
strchr_s (const char *restrict dest, rsize_t dmax,
          const int ch, char **result)
{
    if (unlikely(result == NULL)) {
        invoke_safe_str_constraint_handler("strchr_s: result is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strchr_s: dest is null",
                   NULL, ESNULLP);
        *result = NULL;
        return (ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strchr_s: dmax is 0",
                   NULL, ESZEROL);
        *result = NULL;
        return (ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strchr_s: dmax exceeds max",
                   NULL, ESLEMAX);
        *result = NULL;
        return (ESLEMAX);
    }

    if (unlikely(ch > 255)) {
        invoke_safe_str_constraint_handler("strchr_s: ch exceeds max",
                   NULL, ESLEMAX);
        *result = NULL;
        return (ESLEMAX);
    }

    /* compares wordwise */
    *result = strchr((char *)dest, ch);

    if (!*result)
        return (ESNOTFND);
    else if ((long)(*result - dest) > (long)dmax) {
        *result = NULL;
        return (ESNOTFND);
    }
    return (EOK);
}
