/*------------------------------------------------------------------
 * u8ncat_s.c
 *
 * September 2026
 *
 * Copyright (c) 2026 by Reini Urban
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
#include "u8_private.h"
#endif

/**
 * @def u8ncat_s(dest,dmax,src,slen)
 * @brief
 *    The u8ncat_s function appends not more than slen bytes from the
 *    utf-8 string pointed to by src to the end of the utf-8 string pointed
 *    to by dest. The initial utf-8 character from src overwrites the null
 *    character at the end of dest. If no null character was found in the
 *    first slen bytes of src, a null character is appended to the result.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with 0.
 *
 * @param[out]  dest      pointer to utf-8 string that will be extended by
 *                        src if dmax allows.
 * @param[in]   dmax      restricted maximum byte-length of the resulting
 *                        dest, including the null
 * @param[in]   src       pointer to the utf-8 string that will be
 *                        concatenated to string dest
 * @param[in]   slen      maximum number of bytes to append from src
 *
 * @pre  Neither dest nor src shall be a null pointer, unless slen is 0.
 * @pre  dmax shall not equal zero.
 * @pre  dmax and slen shall not be greater than RSIZE_MAX_STR.
 * @pre  Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation, then if dest is
 *          not a null pointer and dmax is greater than zero and not
 *          greater than RSIZE_MAX_STR, then u8ncat_s nulls dest.
 * @retval  EOK        when successful operation, when slen == 0 or all the
 *                     utf-8 characters from src were appended to dest and
 *                     the result in dest is null terminated.
 * @retval  ESNULLP    when dest or src is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESUNTERM   when dest not terminated in the first dmax utf-8
 *                     bytes
 * @retval  ESOVRLP    when src overlaps with dest
 * @retval  ESNOSPC    when there is not enough room in dest
 * @retval  EILSEQ     when src contains an illegal or truncated UTF-8
 *                     sequence
 *
 * @see
 *    u8cat_s(), u8ncpy_s(), strncat_s(), wcsncat_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8ncat_s(char8_t *restrict dest, rsize_t dmax,
                 const char8_t *restrict src, rsize_t slen)
#else
EXPORT errno_t _u8ncat_s_chk(char8_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src, rsize_t slen,
                             const size_t destbos, const size_t srcbos)
#endif
{
    rsize_t orig_dmax;
    char8_t *orig_dest;
    const char8_t *overlap_bumper;
    int u8_need = 0; /* bytes remaining in the UTF-8 sequence in progress;
                         0 means the next byte starts a new sequence */

    if (unlikely(slen == 0 && !dest && !dmax)) { /* silent ok as in msvcrt */
        return EOK;
    }
    CHK_DEST_NULL("u8ncat_s")
    CHK_DMAX_ZERO("u8ncat_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8ncat_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DESTW_OVR("u8ncat_s", dmax, destbos)
    }
    CHK_SRC_NULL_CLEAR("u8ncat_s", src)
    if (unlikely(slen > RSIZE_MAX_STR)) {
        handle_error((char *)dest, dmax, "u8ncat_s: slen exceeds max",
                     ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen);
    } else if (unlikely(slen > srcbos)) {
        handle_error((char *)dest, dmax, "u8ncat_s: slen exceeds src",
                     EOVERFLOW);
        return RCNEGATE(EOVERFLOW);
    }

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        /* Find the end of dest */
        while (*dest != '\0') {
            if (unlikely(dest == overlap_bumper)) {
                handle_error((char *)orig_dest, orig_dmax,
                             "u8ncat_s: overlapping objects", ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }
            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error((char *)orig_dest, orig_dmax,
                             "u8ncat_s: dest unterminated", ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error((char *)orig_dest, orig_dmax,
                             "u8ncat_s: overlapping objects", ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            if (unlikely(slen == 0)) {
#ifdef SAFECLIB_STR_NULL_SLACK
                if (dmax > 0x20)
                    memset(dest, 0, dmax);
                else {
                    while (dmax) {
                        *dest = '\0';
                        dmax--;
                        dest++;
                    }
                }
#else
                *dest = '\0';
#endif
                return RCNEGATE(EOK);
            }

            if (u8_need == 0) {
                u8_need = u8_seqlen(src, RSIZE_MAX_STR);
                if (unlikely(!u8_need)) {
                    handle_error((char *)orig_dest, orig_dmax,
                                 "u8ncat_s: illegal UTF-8 sequence in src",
                                 EILSEQ);
                    return RCNEGATE(EILSEQ);
                }
                if (unlikely((rsize_t)u8_need > slen)) {
                    /* doesn't fit the remaining slen budget: truncate at
                     * this character boundary instead of splitting it */
#ifdef SAFECLIB_STR_NULL_SLACK
                    if (dmax > 0x20)
                        memset(dest, 0, dmax);
                    else {
                        while (dmax) {
                            *dest = '\0';
                            dmax--;
                            dest++;
                        }
                    }
#else
                    *dest = '\0';
#endif
                    return RCNEGATE(EOK);
                }
            }

            *dest = *src;
            if (unlikely(*dest == '\0')) {
#ifdef SAFECLIB_STR_NULL_SLACK
                if (dmax > 0x20)
                    memset(dest, 0, dmax);
                else {
                    while (dmax) {
                        *dest = '\0';
                        dmax--;
                        dest++;
                    }
                }
#endif
                return RCNEGATE(EOK);
            }

            u8_need--;
            dmax--;
            slen--;
            dest++;
            src++;
        }
    } else {
        overlap_bumper = dest;

        /* Find the end of dest */
        while (*dest != '\0') {
            /* no overlap check needed: src comes first in memory and we
             * are not incrementing src here */
            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error((char *)orig_dest, orig_dmax,
                             "u8ncat_s: dest unterminated", ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error((char *)orig_dest, orig_dmax,
                             "u8ncat_s: overlapping objects", ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            if (unlikely(slen == 0)) {
#ifdef SAFECLIB_STR_NULL_SLACK
                if (dmax > 0x20)
                    memset(dest, 0, dmax);
                else {
                    while (dmax) {
                        *dest = '\0';
                        dmax--;
                        dest++;
                    }
                }
#else
                *dest = '\0';
#endif
                return RCNEGATE(EOK);
            }

            if (u8_need == 0) {
                u8_need = u8_seqlen(src, RSIZE_MAX_STR);
                if (unlikely(!u8_need)) {
                    handle_error((char *)orig_dest, orig_dmax,
                                 "u8ncat_s: illegal UTF-8 sequence in src",
                                 EILSEQ);
                    return RCNEGATE(EILSEQ);
                }
                if (unlikely((rsize_t)u8_need > slen)) {
#ifdef SAFECLIB_STR_NULL_SLACK
                    if (dmax > 0x20)
                        memset(dest, 0, dmax);
                    else {
                        while (dmax) {
                            *dest = '\0';
                            dmax--;
                            dest++;
                        }
                    }
#else
                    *dest = '\0';
#endif
                    return RCNEGATE(EOK);
                }
            }

            *dest = *src;
            if (unlikely(*dest == '\0')) {
#ifdef SAFECLIB_STR_NULL_SLACK
                if (dmax > 0x20)
                    memset(dest, 0, dmax);
                else {
                    while (dmax) {
                        *dest = '\0';
                        dmax--;
                        dest++;
                    }
                }
#endif
                return RCNEGATE(EOK);
            }

            u8_need--;
            dmax--;
            slen--;
            dest++;
            src++;
        }
    }

    /*
     * the entire src was not copied, so null the string
     */
    handle_error((char *)orig_dest, orig_dmax,
                 "u8ncat_s: not enough space for src", ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
