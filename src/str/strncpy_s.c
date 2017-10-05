/*------------------------------------------------------------------
 * strncpy_s.c
 *
 * October 2008, Bo Berry
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
 *    The strncpy_s function copies not more than slen successive characters
 *    (characters that follow a null character are not copied) from the
 *    array pointed to by src to the array pointed to by dest. If no null
 *    character was copied from src, then dest[n] is set to a null character.
 * @details
 *    All elements following the terminating null character (if
 *    any) written by strncpy_s in the array of dmax characters
 *    pointed to by dest take unspeciﬁed values when strncpy_s returns.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with
 *    NULL bytes.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    7.21.2.4 The strncpy function (p: 326-327)
 *    http://en.cppreference.com/w/c/string/byte/strncpy
 *    * ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the string that will be copied to dest
 * @param[in]   slen  the maximum number of characters to copy from src
 *
 * @pre  Neither dmax nor slen shall be equal to zero.
 * @pre  Neither dmax nor slen shall be equal zero.
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_STR.
 * @pre  If slen is either greater than or equal to dmax, then dmax should
 *       be more than strnlen_s(src,dmax) to avoid truncation.
 * @pre  Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax greater than RSIZE_MAX_STR,
 *          then strncpy_s nulls dest.
 * @retval  EOK        successful operation, the characters in src were copied
 *                     to dest and the result is null terminated.
 * @retval  ESNULLP    when dest/src is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  ESOVRLP    when strings overlap
 * @retval  ESNOSPC    when dest < src
 *
 * @see
 *    strcat_s(), strncat_s(), strcpy_s(), wcsncpy_s()
 *
 */
EXPORT errno_t
strncpy_s (char * restrict dest, rsize_t dmax, const char * restrict src, rsize_t slen)
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strncpy_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strncpy_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strncpy_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    /* hold base in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (unlikely(src == NULL)) {
        handle_error(orig_dest, orig_dmax, "strncpy_s: "
                     "src is null",
                     ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(slen == 0)) {
        handle_error(orig_dest, orig_dmax, "strncpy_s: "
                     "slen is zero",
                     ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(slen > RSIZE_MAX_STR)) {
        handle_error(orig_dest, orig_dmax, "strncpy_s: "
                     "slen exceeds max",
                     ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }


    if (dest < src) {
       overlap_bumper = src;

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax, "strncpy_s: "
                        "overlapping objects",
                        ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

	    if (unlikely(slen == 0)) {
                /*
                 * Copying truncated to slen chars.  Note that the TR says to
                 * copy slen chars plus the null char.  We null the slack.
                 */
#ifdef SAFECLIB_STR_NULL_SLACK
                memset(dest, 0, dmax);
                /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#else
                *dest = '\0';
#endif
                return RCNEGATE(EOK);
            }

            *dest = *src;
            if (*dest == '\0') {
#ifdef SAFECLIB_STR_NULL_SLACK
                memset(dest, 0, dmax);
                /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#endif
                return RCNEGATE(EOK);
            }

            dmax--;
            slen--;
            dest++;
            src++;
        }

    } else {
        overlap_bumper = dest;

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax, "strncpy_s: "
                        "overlapping objects",
                        ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

	    if (unlikely(slen == 0)) {
                /*
                 * Copying truncated to slen chars.  Note that the TR says to
                 * copy slen chars plus the null char.  We null the slack.
                 */
#ifdef SAFECLIB_STR_NULL_SLACK
                memset(dest, 0, dmax);
                /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#else
                *dest = '\0';
#endif
                return RCNEGATE(EOK);
            }

            *dest = *src;
            if (*dest == '\0') {
#ifdef SAFECLIB_STR_NULL_SLACK
                memset(dest, 0, dmax);
                /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#endif
                return RCNEGATE(EOK);
            }

            dmax--;
            slen--;
            dest++;
            src++;
        }
    }

    /*
     * the entire src was not copied, so zero the string
     */
    handle_error(orig_dest, orig_dmax, "strncpy_s: not enough "
                 "space for src",
                 ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
EXPORT_SYMBOL(strncpy_s)
