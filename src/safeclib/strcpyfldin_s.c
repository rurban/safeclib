/*------------------------------------------------------------------
 * strcpyfldin_s.c
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
 *    The strcpyfldin_s function copies at most slen characters from the
 *    null terminated string pointed to by src into the fixed character
 *    array pointed to by dest. The copy operation stops on the  null
 *    character if encountered and then continues to fill the field
 *    with nulls up to dmax characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the null terminated string that will be copied
 *                    into the character array pointed to by dest
 * @param[in]   slen  maximum length of src
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dmax shall not equal zero.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR.
 * @pre  slen shall not equal zero.
 * @pre  slen shall not exceed dmax
 * @pre  Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then strcpyfldin_s nulls dest
 * @retval  EOK        when successful operation
 * @retval  ESNULLP    when dest/src is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  ESOVRLP    when strings overlap
 * @retval  ESNOSPC    when dmax < slen
 *
 * @see
 *    strcpyfld_s(), strcpyfldout_s(),
 *
 */
errno_t
strcpyfldin_s (char *dest, rsize_t dmax, const char *src, rsize_t slen)
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    if (dest == NULL) {
        invoke_safe_str_constraint_handler("strcpyfldin_s: dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (dmax == 0) {
        invoke_safe_str_constraint_handler("strcpyfldin_s: dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (dmax > RSIZE_MAX_STR) {
        invoke_safe_str_constraint_handler("strcpyfldin_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    if (src == NULL) {
        /* null string to clear data */
        while (dmax) {  *dest = '\0'; dmax--; dest++; }

        invoke_safe_str_constraint_handler("strcpyfldin_s: src is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (slen == 0) {
        /* null string to clear data */
        while (dmax) {  *dest = '\0'; dmax--; dest++; }

        invoke_safe_str_constraint_handler("strcpyfldin_s: slen is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (slen > dmax) {
        /* null string to clear data */
        while (dmax) {  *dest = '\0'; dmax--; dest++; }

        invoke_safe_str_constraint_handler("strcpyfldin_s: slen exceeds max",
                   NULL, ESNOSPC);
        return (ESNOSPC);
    }


    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        while (dmax > 0 && *src) {

            if (dest == overlap_bumper) {
                dmax = orig_dmax;
                dest = orig_dest;

                /* null string to eliminate partial copy */
                while (dmax) { *dest = '\0'; dmax--; dest++; }

                invoke_safe_str_constraint_handler(
                          "strcpyfldin_s: overlapping objects",
                           NULL, ESOVRLP);
                return (ESOVRLP);
            }

            dmax--;
            *dest++ = *src++;
        }

    } else {
        overlap_bumper = dest;

        while (dmax > 0 && *src) {

            if (src == overlap_bumper) {
                dmax = orig_dmax;
                dest = orig_dest;

                /* null string to eliminate partial copy */
                while (dmax) { *dest = '\0'; dmax--; dest++; }

                invoke_safe_str_constraint_handler(
                          "strcpyfldin_s: overlapping objects",
                           NULL, ESOVRLP);
                return (ESOVRLP);
            }

            dmax--;
            *dest++ = *src++;
        }
    }

    /*
     * finish filling in the field with nulls if there is slack space
     */
    while (dmax) { *dest = '\0'; dmax--; dest++; }

    return (EOK);
}
