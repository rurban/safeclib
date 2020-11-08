/*------------------------------------------------------------------
 * strncpy_s.c
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
 * January 2018, Reini Urban
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
 * Copyright (c) 2017-2019 Reini Urban
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

#if defined(TEST_MSVCRT) && defined(HAVE_STRNCPY_S)
#else

/**
 * @def strncpy_s(dest,dmax,src,slen)
 * @brief
 *    The \b strncpy_s function copies not more than slen successive characters
 *    (characters that follow a null character are not copied) from the
 *    array pointed to by src to the array pointed to by dest. If no null
 *    character was copied from src, then dest[n] is set to a null character,
 *    thus guaranteeing null termination.
 * @details
 *    All elements following the terminating null character (if
 *    any) written by strncpy_s in the array of dmax characters
 *    pointed to by dest take unspeciﬁed values when strncpy_s returns.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with
 *    NULL bytes.
 *    With modern compilers and constant arguments most errors
 *    will be caught at compile-time.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.1.4 The strncpy_s function (p: 616-617)
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
 * @pre  dmax shall not be zero.
 * @pre  With --enable-warn-dmax dmax should be the exact object size of dest.
 * @pre  dmax and slen should not be greater than RSIZE_MAX_STR.
 * @pre  dmax and slen should not be greater than the object sizes of dest and
 * src.
 * @pre  dmax shall not equal zero
 * @pre  If slen is either greater than or equal to dmax, then dmax should
 *       be more than strnlen_s(src,dmax) to avoid truncation.
 * @pre  Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, and if dest and
 *          dmax are valid, then strncpy_s nulls dest.
 * @retval  EOK        successful operation, when slen == 0 or the characters
 *                     in src were copied to dest and the result is null terminated.
 * @retval  ESNULLP    when dest/src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 *                     compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESOVRLP    when strings overlap
 * @retval  ESNOSPC    when src longer than dest
 *
 * @details
 *    If dmax != sizeof(dest): With --enable-warn-dmax ESLEWRNG will be passed
 * to the constraint handler. With --enable-error-dmax this error will be
 * fatal, but dest will not be cleared.
 *
 * @see
 *    strcat_s(), strncat_s(), strcpy_s(), wcsncpy_s()
 *    http://www.informit.com/articles/article.aspx?p=2036582&seqNum=5
 *
 */
#ifdef FOR_DOXYGEN
errno_t strncpy_s(char *restrict dest, rsize_t dmax,
                  const char *restrict src, rsize_t slen)
#else
EXPORT errno_t _strncpy_s_chk(char *restrict dest, rsize_t dmax,
                              const char *restrict src, rsize_t slen,
                              const size_t destbos, const size_t srcbos)
#endif
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    if (unlikely(slen == 0 && dest && dmax)) {
        *dest = '\0';
        return EOK;
    }
    CHK_DEST_NULL("strncpy_s")
    CHK_DMAX_ZERO("strncpy_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strncpy_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, slen);
    } else {
        CHK_DEST_OVR_CLEAR("strncpy_s", destbos)
    }
    CHK_SRC_NULL_CLEAR("strncpy_s", src)
    CHK_SRC_OVR_CLEAR("strncpy_s", src, slen, RSIZE_MAX_STR)
    CHK_SLEN_MAX_CLEAR("strncpy_s", slen, RSIZE_MAX_STR)
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen);
    } else if (unlikely(slen > srcbos)) {
        return handle_str_bos_overload("strncpy_s: slen exceeds src",
                                       dest, destbos);
    }

    /* hold base in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strncpy_s: "
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

            *dest = *src;
            if (*dest == '\0') {
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
                             "strncpy_s: "
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

            *dest = *src;
            if (*dest == '\0') {
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

            dmax--;
            slen--;
            dest++;
            src++;
        }
    }

    /*
     * the entire src was not copied, so zero the string
     */
    handle_error(orig_dest, orig_dmax,
                 "strncpy_s: not enough "
                 "space for src",
                 ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strncpy_s_chk);
#endif /* __KERNEL__ */

#endif /* TEST_MSVCRT */
