/*------------------------------------------------------------------
 * strstr_s.c
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
 *    The strstr_s() function locates the first occurrence of the
 *    substring pointed to by src which would be located in the
 *    string pointed to by dest.
 *
 * @remark EXTENSION TO
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest       pointer to string to be searched for the substring
 * @param[in]   dmax       restricted maximum length of dest string
 * @param[in]   src        pointer to the sub string
 * @param[in]   slen       the maximum number of characters to use from src
 * @param[out]  substring  the returned substring pointer
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  Neither dmax nor slen shall not be 0.
 * @pre  Neither dmax nor slen shall not be greater than RSIZE_MAX_STR.
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dest/src/substring is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  ESNOTFND   when substring not found
 *
 * @see
 *     strprefix_s(), strspn_s(), strcspn_s(), strpbrk_s()
 *
 */
EXPORT errno_t
strstr_s (char *dest, rsize_t dmax,
          const char *src, rsize_t slen, char **substring)
{
    rsize_t len;
    rsize_t dlen;
    int i;

    if (unlikely(substring == NULL)) {
        invoke_safe_str_constraint_handler("strstr_s: substring is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }
    *substring = NULL;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strstr_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strstr_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strstr_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("strstr_s: src is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("strstr_s: slen is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(slen > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strstr_s: slen exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    /*
     * src points to a string with zero length, or
     * src equals dest, return dest
     */
    if (unlikely(*src == '\0' || dest == src)) {
        *substring = dest;
        return RCNEGATE(EOK);
    }

    while (*dest && dmax) {
        i = 0;
        len = slen;
        dlen = dmax;

        while (src[i] && dlen) {

            /* not a match, not a substring */
            if (dest[i] != src[i]) {
                break;
            }

            /* move to the next char */
            i++;
            len--;
            dlen--;

            if (src[i] == '\0' || !len) {
                *substring = dest;
                return RCNEGATE(EOK);
            }
        }
        dest++;
        dmax--;
    }

    /*
     * substring was not found, return NULL
     */
    *substring = NULL;
    return RCNEGATE(ESNOTFND);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(strstr_s);
#endif /* __KERNEL__ */
