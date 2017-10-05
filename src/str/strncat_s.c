/*------------------------------------------------------------------
 * strncat_s.c
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
 * Copyright (c) 2017 Reini Urban
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
 *    The strncat_s function appends a copy of the string pointed
 *    to by src (including the terminating null character) to the
 *    end of the string pointed to by dest. The initial character
 *    from src overwrites the null character at the end of dest.
 * @details
 *    All elements following the terminating null character (if
 *    any) written by strncat_s in the array of dmax characters
 *    pointed to by dest take unspeciﬁed values when strncat_s returns.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with
 *    NULL bytes.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.2.2 The strncat_s function (p: 618-620)
 *    http://en.cppreference.com/w/c/string/byte/strncat
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string that will be extended by src
 *                    if dmax allows. The string is null terminated.
 *                    If the resulting concatenated string is less
 *                    than dmax, the remaining slack space is nulled.
 * @param[in]   dmax  restricted maximum length of the resulting dest,
 *                    including the null
 * @param[in]   src   pointer to the string that will be concatenaed
 *                    to string dest
 * @param[in]   slen  maximum characters to append
 *
 * @pre  Neither dest nor src shall be a null pointer
 * @pre  dmax shall not equal zero
 * @pre  dmax shall not be greater than RSIZE_MAX_STR
 * @pre  dmax shall be greater than strnlen_s(src,m).
 * @pre  Copying shall not takeplace between objects that overlap
 *  
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @returns  If there is a runtime-constraint violation, then if dest is
 *           not a null pointer and dmax is greater than zero and not
 *           greater than RSIZE_MAX_STR, then strncat_s sets dest[0] to the
 *           null character.
 * @retval  EOK        successful operation, all the characters from src
 *                     null terminated.
 * @retval  ESNULLP    when dest/src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  ESUNTERM   when dest not terminated
 * @retval  ESOVRLP    when src overlaps with dest
 *
 */
EXPORT errno_t
strncat_s (char * restrict dest, rsize_t dmax, const char * restrict src, rsize_t slen)
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strncat_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("strncat_s: src is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(slen > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strncat_s: slen exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strncat_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strncat_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        /* Find the end of dest */
        while (*dest != '\0') {

            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax, "strncat_s: "
                        "overlapping objects",
                        ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error(orig_dest, orig_dmax, "strncat_s: "
                       "dest unterminated",
                       ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax, "strncat_s: "
                       "overlapping objects",
                       ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            /*
             * Copying truncated before the source null is encountered
             */
            if (unlikely(slen == 0)) {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null remaining string */
                memset(dest, 0, dmax);
                /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#else
                *dest = '\0';
#endif
                return RCNEGATE(EOK);
            }

            *dest = *src;
            if (unlikely(*dest == '\0')) {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear data */
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

        /* Find the end of dest */
        while (*dest != '\0') {

            /*
             * NOTE: no need to check for overlap here since src comes first
             * in memory and we're not incrementing src here.
             */
            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error(orig_dest, orig_dmax, "strncat_s: "
                       "dest unterminated",
                       ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax, "strncat_s: "
                        "overlapping objects",
                        ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            /*
             * Copying truncated
             */
            if (unlikely(slen == 0)) {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null remaining string */
                memset(dest, 0, dmax);
                /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#else
                *dest = '\0';
#endif
                return RCNEGATE(EOK);
            }

            *dest = *src;
            if (unlikely(*dest == '\0')) {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear any data */
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
     * the entire src was not copied, so the string will be nulled.
     */
    handle_error(orig_dest, orig_dmax, "strncat_s: not enough "
                 "space for src",
                 ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
EXPORT_SYMBOL(strncat_s)
