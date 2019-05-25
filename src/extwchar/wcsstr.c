/*------------------------------------------------------------------
 * wcsstr.c
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

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @brief
 *    The wcsstr() function locates the first occurrence of the
 *    wide substring pointed to by src which would be located in the
 *    wide string pointed to by dest.
 *
 * @remark EXTENSION TO
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest       wide string to be searched for the substring
 * @param[in]   src        pointer to the wide sub string
 *
 * @pre     Neither dest nor src shall be a null pointer.
 *
 * @retval  pointer to the found substring, or NULL
 *
 * @see
 *     wcsstr_s(), strstr_s(), memcmp32_s()
 *
 */
wchar_t *wcsstr(wchar_t *restrict dest, const wchar_t *restrict src) {
    int i;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("wcsstr: dest is null", NULL,
                                           ESNULLP);
        return NULL;
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("wcsstr: src is null", NULL,
                                           ESNULLP);
        return NULL;
    }

    /*
     * src points to a string with zero length, or
     * src equals dest, return dest.
     */
    if (unlikely(*src == '\0' || dest == src)) {
        return dest;
    }

    while (*dest) {
        i = 0;
        while (src[i]) {
            /* not a match, not a substring */
            if (dest[i] != src[i]) {
                break;
            }
            /* move to the next char */
            i++;
            if (src[i] == '\0') {
                return dest;
            }
        }
        dest++;
    }
    return NULL;
}
