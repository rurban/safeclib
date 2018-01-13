/*------------------------------------------------------------------
 * wcsncpy_s.c
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

#ifdef HAVE_WCHAR_H

/**
 * @brief
 *    The wcsncpy_s function copies the wide string pointed to by src
 *    (including the terminating null character) into the wide string
 *    pointed to by dest.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by wcsncpy_s in the
 *    array of dmax characters pointed to by dest are nulled when
 *    wcsncpy_s returns.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.2.1.2 The wcsncpy_s function (p: 639)
 *    http://en.cppreference.com/w/c/string/wide/wcsncpy
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to wide string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the wide string that will be copied to dest
 * @param[in]   slen  maximum number of wide characters to copy
 *
 * @pre Neither dest nor src shall be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_WSTR.
 * @pre dmax shall not equal zero.
 * @pre If slen is either greater than or equal to dmax, then dmax should
 *      be more than wcsnlen_s(src,dmax) to avoid truncation.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then wcsncpy_s nulls dest.
 * @retval  EOK        when successful operation, the wide characters in src were
 *                     copied into dest and the result is null terminated.
 * @retval  ESNULLP    when dest or src is a NULL pointer
 * @retval  ESZEROL    when dmax = 0. Before C11 also with slen = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR
 * @retval  ESOVRLP    when buffers overlap
 * @retval  ESNOSPC    when dest < src
 *
 * @see
 *    wcscpy_s(), strncpy_s(), wmemcpy_s(), wmemmove_s()
 */

EXPORT errno_t
wcsncpy_s (wchar_t * restrict dest, rsize_t dmax, const wchar_t * restrict src, rsize_t slen)
{
    rsize_t orig_dmax;
    wchar_t *orig_dest;
    const wchar_t *overlap_bumper;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("wcsncpy_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("wcsncpy_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcsncpy_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(src == NULL)) {
        handle_werror(dest, wcslen(dest), "wcsncpy_s: slen is null",
                     ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(slen == 0)) {
        /* Since C11 slen=0 is allowed */
#ifdef HAVE_C11
        return EOK;
#else
        handle_werror(dest, wcslen(dest), "wcsncpy_s: slen is zero",
                     ESZEROL);
        return RCNEGATE(ESZEROL);
#endif
    }

    if (unlikely(slen > RSIZE_MAX_STR)) {
        handle_werror(dest, wcslen(dest), "wcsncpy_s: slen exceeds max",
                     ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    /* hold base in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
              handle_werror(orig_dest, orig_dmax, "wcsncpy_s: "
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
                while (dmax) { *dest = L'\0'; dmax--; dest++; }
#else
                *dest = L'\0';
#endif
                return RCNEGATE(EOK);
            }

            *dest = *src;
            if (*dest == L'\0') {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear any data */
                while (dmax) { *dest = L'\0'; dmax--; dest++; }
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
                handle_werror(orig_dest, orig_dmax, "wcsncpy_s: "
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
                while (dmax) { *dest = L'\0'; dmax--; dest++; }
#else
                *dest = L'\0';
#endif
                return RCNEGATE(EOK);
            }

            *dest = *src;
            if (*dest == L'\0') {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear any data */
                while (dmax) { *dest = '\0'; dmax--; dest++; }
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
     * the entire src must have been copied, if not reset dest
     * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
     */
    handle_werror(orig_dest, orig_dmax, "wcsncpy_s: not "
                 "enough space for src",
                 ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
EXPORT_SYMBOL(wcsncpy_s)

#endif /* HAVE_WCHAR_H */
