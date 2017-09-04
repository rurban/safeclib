/*------------------------------------------------------------------
 * strcasecmp_s.c
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
 *    Case insensitive string comparison by converting
 *    to uppercase prior to the compare.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest       pointer to string to compare against
 * @param[in]   dmax       restricted maximum length of string dest
 * @param[in]   src        pointer to the string to be compared to dest
 * @param[out]  indicator  pointer to result indicator, greater than 0,
 *                         equal to 0 or less than 0, if the string pointed
 *                         to by dest is greater than, equal to or less
 *                         than the string pointed to by src respectively.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   indicator shall not be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR
 *
 * @return  indicator (when the return code is OK)
 * @retval  >0 when dest greater than src
 * @retval  0 when strings the same
 * @retval  <0 when dest less than src
 * @retval  EOK          when comparison is complete
 * @retval  ESNULLP      when dest/src/indicator is NULL pointer
 * @retval  ESZEROL      when dmax = 0
 * @retval  ESLEMAX      when dmax > RSIZE_MAX_STR
 *
 * @see
 *    strcmp_s()
 *
 */
EXPORT errno_t
strcasecmp_s (const char *dest, rsize_t dmax,
              const char *src, int *indicator)
{
    const unsigned char *udest = (const unsigned char *) dest;
    const unsigned char *usrc = (const unsigned char *) src;

    if (unlikely(indicator == NULL)) {
        invoke_safe_str_constraint_handler("strcasecmp_s: indicator is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }
    *indicator = 0;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strcasecmp_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("strcasecmp_s: src is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strcasecmp_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strcasecmp_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    while (*udest && *usrc && dmax) {

        if (toupper(*udest) != toupper(*usrc)) {
            break;
        }

        udest++;
        usrc++;
        dmax--;
    }

    *indicator = (toupper(*udest) - toupper(*usrc));
    return RCNEGATE(EOK);
}
EXPORT_SYMBOL(strcasecmp_s)
