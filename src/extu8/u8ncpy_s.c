/*------------------------------------------------------------------
 * u8ncpy_s.c
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
 * @def u8ncpy_s(dest,dmax,src,slen)
 * @brief
 *    The \b u8ncpy_s function copies the utf-8 string pointed to by src
 *    (including the terminating null character) into the utf-8 string
 *    pointed to by dest.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by u8ncpy_s in the
 *    array of dmax characters pointed to by dest are nulled when
 *    u8ncpy_s returns.
 *
 * @param[out]  dest  pointer to utf-8 string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the utf-8 string that will be copied to dest
 * @param[in]   slen  maximum number of utf-8 characters to copy
 *
 * @pre Neither dest nor src shall be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR.
 * @pre dmax shall not equal zero.
 * @pre If slen is either greater than or equal to dmax, then dmax should
 *      be more than u8nlen_s(src,dmax) to avoid truncation.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation, then if dest and
 *          dmax are valid, then u8ncpy_s nulls dest.
 * @retval  EOK        successful operation, when slen == 0 or the utf-8
 * characters in src were copied into dest and the result is null terminated.
 * @retval  ESNULLP    when dest/src is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESOVRLP    when buffers overlap
 * @retval  ESNOSPC    when src > dest
 *
 * @see
 *    u8cpy_s(), strncpy_s(), wmemcpy_s(), wmemmove_s()
 */

EXPORT errno_t _u8ncpy_s_chk(char *restrict dest, rsize_t dmax,
                             const char *restrict src, rsize_t slen,
                             const size_t destbos, const size_t srcbos) {
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    if (unlikely(slen == 0 && dest && dmax)) {
        *dest = L'\0';
        return EOK;
    }
    CHK_DEST_NULL("u8ncpy_s")
    CHK_DMAX_ZERO("u8ncpy_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8ncpy_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR_CLEAR("u8ncpy_s", destbos)
    }
    CHK_SRC_NULL_CLEAR("u8ncpy_s", src)
    if (unlikely(slen > RSIZE_MAX_STR)) {
            handle_error(dest, u8nlen_s(dest, dmax),
                         "u8ncpy_s: slen exceeds max",
                         ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen);
    } else {
        if (unlikely(slen > srcbos)) {
            handle_error(dest, u8nlen_s(dest, dmax),
                          "u8ncpy_s: slen exceeds src", EOVERFLOW);
            return RCNEGATE(EOVERFLOW);
        }
    }

    /* hold base in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "u8ncpy_s: "
                             "overlapping objects",
                             ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            if (unlikely(slen == 0)) {
                /* Copying truncated to slen chars.  Note that the TR says to
                 * copy slen chars plus the null char.  We null the slack.
                 */
#ifdef SAFECLIB_STR_NULL_SLACK
                if (dmax > 0x20)
                    memset(dest, 0, dmax);
                else {
                    while (dmax) {
                        *dest = L'\0';
                        dmax--;
                        dest++;
                    }
                }
#else
                *dest = L'\0';
#endif
                return RCNEGATE(EOK);
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
            slen--;
            dest++;
            src++;
        }
    } else {
        overlap_bumper = dest;

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "u8ncpy_s: "
                             "overlapping objects",
                             ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            if (unlikely(slen == 0)) {
                /* Copying truncated to slen chars.  Note that the TR says to
                 * copy slen chars plus the null char.  We null the slack.
                 */
#ifdef SAFECLIB_STR_NULL_SLACK
                if (dmax > 0x20)
                    memset(dest, 0, dmax);
                else {
                    while (dmax) {
                        *dest = L'\0';
                        dmax--;
                        dest++;
                    }
                }
#else
                *dest = L'\0';
#endif
                return RCNEGATE(EOK);
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
            slen--;
            dest++;
            src++;
        }
    }

    /*
     * the entire src must have been copied, if not reset dest
     * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
     */
    handle_error(orig_dest, orig_dmax,
                  "u8ncpy_s: not "
                  "enough space for src",
                  ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
