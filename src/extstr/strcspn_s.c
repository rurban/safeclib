/*------------------------------------------------------------------
 * strcspn_s.c
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

#include "safeclib_private.h"

/**
 * @brief
 *    This function computes the prefix length of the string pointed
 *    to by dest which consists entirely of characters that are
 *    excluded from the string pointed to by src. The scanning stops
 *    at the first null in dest or after dmax characters. The
 *    exclusion string is checked to the null or after slen
 *    characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest   pointer to string to determine the prefix
 * @param[in]   dmax   restricted maximum length of string dest
 * @param[in]   src    pointer to exclusion string
 * @param[in]   slen   restricted maximum length of string src
 * @param[out]  count  pointer to a count variable that will be updated
 *                     with the dest substring length
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  count shall not be a null pointer.
 * @pre  dmax shall not be 0
 * @pre  dmax shall not be greater than RSIZE_MAX_STR
 *
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest/src/count is NULL pointer
 * @retval  ESZEROL     when dmax/slen = 0 
 * @retval  ESLEMAX     when dmax/slen > RSIZE_MAX_STR
 * 
 * @see
 *    strspn_s(), strpbrk_s(), strstr_s()
 *
 */
EXPORT errno_t
strcspn_s (const char *dest, rsize_t dmax,
           const char *src,  rsize_t slen, rsize_t *count)
{
    const char *scan2;
    rsize_t smax;

    if (unlikely(count == NULL)) {
        invoke_safe_str_constraint_handler("strcspn_s: count is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }
    *count = 0;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strcspn_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("strcspn_s: src is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0 )) {
        invoke_safe_str_constraint_handler("strcspn_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strcspn_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(slen == 0 )) {
        invoke_safe_str_constraint_handler("strcspn_s: slen is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(slen > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strcspn_s: slen exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    while (*dest && dmax) {

        /*
         * Scanning for exclusions, so if there is a match,
         * we're done!
         */
        smax = slen;
        scan2 = src;
        while (*scan2 && smax) {

             if (*dest == *scan2) {
                 return RCNEGATE(EOK);
             }
             scan2++;
             smax--;
        }

        (*count)++;
        dest++;
        dmax--;
    }

    return RCNEGATE(EOK);
}
EXPORT_SYMBOL(strcspn_s)
