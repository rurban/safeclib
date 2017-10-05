/*------------------------------------------------------------------
 * strcpyfld_s.c
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
 *    The \c strcpyfld_s function copies \c slen characters from the character
 *    array pointed to by \c src into the character array pointed to by \c dest.
 *    The copy operation does not stop on the null character as the
 *    function copies \c slen characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the character array that will be copied to dest
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
 *          is not a null pointer and destmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then strcpyfld_s nulls dest.
 * @retval  EOK        when successful operation
 * @retval  ESNULLP    when dest/src is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  ESOVRLP    when strings overlap
 * @retval  ESNOSPC    when dmax < slen
 *
 * @see
 *    strcpyfldin_s(), strcpyfldout_s()
 *
 */
EXPORT errno_t
strcpyfld_s (char *dest, rsize_t dmax, const char *src, rsize_t slen)
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strcpyfld_s: dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strcpyfld_s: dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strcpyfld_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    if (unlikely(src == NULL)) {
        /* null string to clear data */
        while (dmax) {  *dest = '\0'; dmax--; dest++; }

        invoke_safe_str_constraint_handler("strcpyfld_s: src is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(slen == 0)) {
        /* null string to clear data */
        while (dmax) {  *dest = '\0'; dmax--; dest++; }

        invoke_safe_str_constraint_handler("strcpyfld_s: slen is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(slen > dmax)) {
        /* null string to clear data */
        while (dmax) {  *dest = '\0'; dmax--; dest++; }

        invoke_safe_str_constraint_handler("strcpyfld_s: src exceeds max",
                   NULL, ESNOSPC);
        return (ESNOSPC);
    }


    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        while (slen > 0) {

            if (unlikely(dest == overlap_bumper)) {
                dmax = orig_dmax;
                dest = orig_dest;

                /* null string to eliminate partial copy */
                while (dmax) { *dest = '\0'; dmax--; dest++; }

                invoke_safe_str_constraint_handler(
                          "strcpyfld_s: overlapping objects",
                           NULL, ESOVRLP);
                return (ESOVRLP);
            }

            *dest++ = *src++;
            slen--;
            dmax--;
        }

    } else {
        overlap_bumper = dest;

        while (slen > 0) {

            if (unlikely(src == overlap_bumper)) {
                dmax = orig_dmax;
                dest = orig_dest;

                /* null string to eliminate partial copy */
                while (dmax) { *dest = '\0'; dmax--; dest++; }

                invoke_safe_str_constraint_handler(
                          "strcpyfld_s: overlapping objects",
                           NULL, ESOVRLP);
                return (ESOVRLP);
            }

            *dest++ = *src++;
            slen--;
            dmax--;
        }
    }

    /* null slack space in the field */
    while (dmax) { *dest = '\0'; dmax--; dest++; }
    return (EOK);
}
