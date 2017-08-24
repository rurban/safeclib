/*------------------------------------------------------------------
 * strfirstsame_s.c
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
#include "safe_str_constraint.h"
#include "safe_str_lib.h"


/**
 * @brief
 *    Returns the index of the first character that is the
 *    same between dest and src.  The scanning stops at the
 *    fisrt null in dest or src, or after dmax characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest   pointer to string to compare against
 * @param[in]   dmax   restricted maximum length of string dest
 * @param[in]   src    pointer to the string to be compared to dest
 * @param[out]  index  pointer to returned index
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  indicator shall not be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR.
 *
 * @return  index to first same char, when the return code is OK
 * @retval  EOK         when index to first same char is returned
 * @retval  ESNULLP     when dst/src/index is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR
 * @retval  ESNOTFND    when not found
 *
 * @see
 *    strfirstchar_s(), strfirstdiff_s(), strlastchar_s(),
 *    strlastdiff_s(), strlastsame_s()
 *
 */
errno_t
strfirstsame_s (const char *dest, rsize_t dmax,
                const char *src,  rsize_t *index)
{
    const char *rp = 0;

    if (index == NULL) {
        invoke_safe_str_constraint_handler("strfirstsame_s: index is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }
    *index = 0;

    if (dest == NULL) {
        invoke_safe_str_constraint_handler("strfirstsame_s: dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (src == NULL) {
        invoke_safe_str_constraint_handler("strfirstsame_s: src is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (dmax == 0 ) {
        invoke_safe_str_constraint_handler("strfirstsame_s: dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (dmax > RSIZE_MAX_STR) {
        invoke_safe_str_constraint_handler("strfirstsame_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    /* hold reference point */
    rp = dest;

    /*
     * find the offset
     */
    while (*dest && *src && dmax) {

        if (*dest == *src) {
            *index = (uint32_t)(dest - rp);
            return (EOK);
        }

        dest++;
        src++;
        dmax--;
    }

    return (ESNOTFND);
}
