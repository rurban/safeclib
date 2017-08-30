/*------------------------------------------------------------------
 * strpbrk_s.c
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
 *    Returns a pointer, first, to the first ocurrence of any character
 *    in src which is contained in dest.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest   pointer to string to compare against
 * @param  dmax   restricted maximum length of string dest
 * @param  src    pointer to the string
 * @param  slen   restricted length of string src
 * @param  first  returned pointer to first occurence
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  Neither dmax nor slen shall not be 0.
 * @pre  Neither dmax nor slen shall not be greater than RSIZE_MAX_STR.
 *
 * @return  pointer to the first ocurrence of any character contained in src
 * @retval  EOK         when successful operation
 * @retval  ESNULLP     when dest/src/first is NULL pointer
 * @retval  ESZEROL     when dmax/slen = 0
 * @retval  ESLEMAX     when dmax/slen > RSIZE_MAX_STR
 *
 * @see
 *    strfirstchar_s(), strlastchar_s(), strfirstdiff_s(),
 *    strfirstsame_s(), strlastdiff_s(), strlastsame_s()
 *
 */
errno_t
strpbrk_s (char *dest, rsize_t dmax,
           char *src,  rsize_t slen, char **first)
{
    char *ps;
    rsize_t len;

    if (first == NULL) {
        invoke_safe_str_constraint_handler("strpbrk_s: count is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }
    *first = NULL;

    if (dest == NULL) {
        invoke_safe_str_constraint_handler("strpbrk_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (src == NULL) {
        invoke_safe_str_constraint_handler("strpbrk_s: src is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (dmax == 0 ) {
        invoke_safe_str_constraint_handler("strpbrk_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (dmax > RSIZE_MAX_STR) {
        invoke_safe_str_constraint_handler("strpbrk_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (slen == 0 ) {
        invoke_safe_str_constraint_handler("strpbrk_s: slen is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (slen > RSIZE_MAX_STR) {
        invoke_safe_str_constraint_handler("strpbrk_s: slen exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    /*
     * look for a matching char in the substring src
     */
    while (*dest && dmax) {

        ps = src;
        len = slen;
        while (*ps) {

            /* check for a match with the substring */
            if (*dest == *ps) {
                *first = dest;
                return RCNEGATE(EOK);
            }
            ps++;
            len--;
        }
        dest++;
        dmax--;
    }

    return RCNEGATE(ESNOTFND);
}
EXPORT_SYMBOL(strpbrk_s)
