/*------------------------------------------------------------------
 * u8icat_s.c
 *
 * September 2020, Reini Urban
 *
 * Copyright (c) 2020 by Reini Urban
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
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def u8icat_s(dest,dmax,src)
 * @brief
 *    The u8icat_s function appends a copy of the utf-8 string src
 *    (including the terminating null character) to
 *    the end of the utf-8 normalized identifier-like dest. The initial
 *    utf-8 character from src overwrites the null
 *    character at the end of dest.  Identifier-like strings follow stricter
 *    rules for mixed scripts, src will be normalized.
 * @details
 *    All elements following the terminating null utf-8 character (if
 *    any) written by u8icat_s in the array of dmax characters
 *    pointed to by dest take unspeciﬁed values when u8icat_s
 *    returns.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with
 *    0.
 *
 * @param[out]  dest      pointer to a normalized utf-8 string that will be
 *                        extended by a normalized copy of src if dmax allows.
 *                        The utf-8 string is normalized, follows the strict
 *                        security guidelines for identifiers (esp. no bidi changes,
 *                        no mixed scripts) and will be null terminated.
 *                        If the resulting concatenated utf-8 string is less
 *                        than dmax, the remaining slack space is nulled.
 * @param[in]   dmax      restricted maximum byte-length of the resulting
 *                        dest buffer, including the null.
 * @param[in]   src       pointer to the utf-8 string that will be normalized and 
 *                        concatenated to identifier dest.
 *
 * @pre  Neither dest nor src shall be a null pointer
 * @pre  dest must be already normalized and contain no disallowed mixed scripts
 *       characters or bidi changes.
 * @pre  src must not contain mixed scripts characters.
 * @pre  The union of src and dest must not contain mixed scripts characters.
 * @pre  dmax shall not equal zero
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR and size of dest
 * @pre  dmax shall be greater than u8nlen_s(src,m).
 * @pre  Copying shall not take place between objects that overlap
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, then if dest is
 *          not a null pointer and dmax is greater than zero and not
 *          greater than RSIZE_MAX_STR, then u8icat_s nulls dest.
 * @retval  EOK        when successful operation, all the normalized utf-8 characters
 *                     from src were appended to dest and the result in dest is null
 *                     terminated.
 * @retval  ESNULLP    when dest or src is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESUNTERM   when dest not terminated in the first dmax utf-8
 *                     bytes
 * @retval  ESOVRLP    when src overlaps with dest
 * @retval  ESU8I      when disallowed mixed scripts or bidi changes were detected.
 *
 * @see
 *    u8icat_s(), wcscat_s(), strcpy_s(), strncpy_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8icat_s(char8i_t *restrict dest, rsize_t dmax, const char8_t *restrict src)
#else
EXPORT errno_t _u8icat_s_chk(char8i_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src,
                             const size_t destbos)
#endif
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char8_t *overlap_bumper;

    CHK_DEST_NULL("u8icat_s")
    CHK_DMAX_ZERO("u8icat_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8icat_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DESTW_OVR("u8icat_s", dmax, destbos)
    }
    CHK_SRC_NULL_CLEAR("u8icat_s", src)

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = (char*)dest;

    if (dest < src) {
        overlap_bumper = src;

        /* Find the end of dest */
        while (*dest != L'\0') {

            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                              "u8icat_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error(orig_dest, orig_dmax,
                              "u8icat_s: "
                              "dest unterminated",
                              ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                              "u8icat_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            *dest = *src;
            if (unlikely(*dest == L'\0')) {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear any data */
                if (dmax > 0x20)
                    memset(dest, 0, dmax);
                else {
                    while (dmax) {
                        *dest = L'\0';
                        dmax--;
                        dest++;
                    }
                }
#endif
                return RCNEGATE(EOK);
            }

            dmax--;
            dest++;
            src++;
        }
    } else {
        overlap_bumper = dest;

        /* Find the end of dest */
        while (*dest != L'\0') {

            /*
             * NOTE: no need to check for overlap here since src comes first
             * in memory and we're not incrementing src here.
             */
            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error(orig_dest, orig_dmax,
                              "u8icat_s: "
                              "dest unterminated",
                              ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                              "u8icat_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            *dest = *src;
            if (*dest == L'\0') {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear any data */
                if (dmax > 0x20)
                    memset(dest, 0, dmax);
                else {
                    while (dmax) {
                        *dest = L'\0';
                        dmax--;
                        dest++;
                    }
                }
#endif
                return RCNEGATE(EOK);
            }

            dmax--;
            dest++;
            src++;
        }
    }

    /*
     * the entire src was not copied, so null the string
     */
    handle_error(orig_dest, orig_dmax,
                  "u8icat_s: not enough "
                  "space for src",
                  ESNOSPC);

    return RCNEGATE(ESNOSPC);
}
