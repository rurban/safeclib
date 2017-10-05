/*------------------------------------------------------------------
 * strfirstchar_s.c
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
 *    This function returns a pointer to the first occurrence
 *    of character c in dest. The scanning stops at the first null
 *    or after dmax characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest   pointer to string to compare against
 * @param[in]   dmax   restricted maximum length of string
 * @param[in]   c      character to locate
 * @param[out]  first  returned pointer to first occurrence of c
 *
 * @pre  dest shall not be a null pointer.
 * @pre  first shall not be a null pointer.
 * @pre  dmax shall not be 0
 * @pre  dmax shall not be greater than RSIZE_MAX_STR
 *
 * @return  pointer to first occurence of c, NULL if not found
 * @retval  EOK         when pointer to first occurrence is returned
 * @retval  ESNULLP     when dst/first is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR
 *
 * @see
 *    strlastchar_s(), strfirstdiff_s(), strfirstsame_s(),
 *    strlastdiff_s(), strlastsame_s(),
 *
 */
EXPORT errno_t
strfirstchar_s (char *dest, rsize_t dmax, char c, char **first)
{

    if (unlikely(first == NULL)) {
        invoke_safe_str_constraint_handler("strfirstchar_s: index is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }
    *first = NULL;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strfirstchar_s: dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(dmax == 0 )) {
        invoke_safe_str_constraint_handler("strfirstchar_s: dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strfirstchar_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    while (*dest && dmax) {

        if (*dest == c) {
            *first = dest;
            return (EOK);
        }
        dest++;
        dmax--;
    }

    return (ESNOTFND);
}
