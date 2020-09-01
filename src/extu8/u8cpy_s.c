/*------------------------------------------------------------------
 * u8cpy_s.c
 *
 * September 2020, Reini Urban
 *
 * Copyright (c) 2017,2020 by Reini Urban
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
 * @def u8cpy_s(dest,dmax,src)
 * @brief
 *    The \c u8cpy_s function copies the string pointed to by \c src
 *    (including the terminating null character) into the buffer
 *    pointed to by dest.
 *    With \c SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by \c u8cpy_s in the
 *    array of \c dmax characters pointed to by \c dest are nulled when
 *    \c u8cpy_s returns.
 *
 * @param[out]  dest  pointer to wide string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the wide string that will be copied to dest
 *
 * @pre Neither dest nor src shall be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than u8nlen_s(src, dmax).
 * @pre Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then u8cpy_s nulls dest.
 * @retval  EOK         when successful operation, the wide characters in src
 *                      were copied into dest and the result is null terminated.
 * @retval  -ESNULLP    when dest or src is a NULL pointer
 * @retval  -ESZEROL    when dmax = 0
 * @retval  -ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  -ESOVRLP    when buffers overlap
 * @retval  -ESNOSPC    when dest < src
 *
 * @see
 *    u8ncpy(), wmemcpy(), wmemmove(), strncpy_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8cpy_s(char *restrict dest, rsize_t dmax,
                const char *restrict src)
#else
EXPORT errno_t _u8cpy_s_chk(char *restrict dest, rsize_t dmax,
                            const char *restrict src,
                            const size_t destbos)
#endif
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    CHK_DEST_NULL("u8cpy_s")
    CHK_DMAX_ZERO("u8cpy_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8cpy_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR_CLEAR("u8cpy_s", destbos)
    }
    CHK_SRC_NULL_CLEAR("u8cpy_s", src)

    if (unlikely(dest == src)) {
        return RCNEGATE(EOK);
    }

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                              "u8cpy_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            *dest = *src;
            if (*dest == '\0') {
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

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                              "u8cpy_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            *dest = *src;
            if (*dest == '\0') {
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
     * the entire src must have been copied, if not reset dest
     * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
     */
    handle_error(orig_dest, orig_dmax,
                  "u8cpy_s: not "
                  "enough space for src",
                  ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
