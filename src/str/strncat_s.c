/*------------------------------------------------------------------
 * strncat_s.c
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
 * January 2018, Reini Urban
 * October 2019, Reini Urban
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

#if defined(TEST_MSVCRT) && defined(HAVE_STRNCAT_S)
#else

/**
 * @def strncat_s(dest,dmax,src,slen)
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
 *    With modern compilers and constant arguments most errors
 *    will be caught at compile-time.
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
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  With --enable-warn-dmax dmax should be the exact object size of dest.
 * @pre  dmax and slen should not be greater than RSIZE_MAX_STR.
 * @pre  dmax and slen should not be greater than the object sizes of dest and
 * src.
 * @pre  dmax shall not equal zero.
 * @pre  dmax shall be greater than strnlen_s(src,dmax).
 * @pre  Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @note The Windows MSVCRT sec_api EINVAL and ERANGE works ok,
 *       ESLEMAX dmax/slen > MAX not, ESOVRLP partially. When dest > src
 * Windows appends the result, when dest < src ERANGE or EINVAL is returned.
 *
 * @returns  If there is a runtime-constraint violation, and if dest and dmax
 *           are valid, then strncat_s clears dest.
 * @retval  EOK        successful operation, when slen == 0 or all the
 * characters are copied from src and dest is null terminated. As special case,
 * analog to msvcrt: when slen == 0 and dmax is big enough for dest, also
 * return EOK, but clear the rest of dest.
 * @retval  ESNULLP    when dest/src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESUNTERM   when dest not terminated
 * @retval  ESOVRLP    when src overlaps with dest
 *
 * @details
 *    If dmax != sizeof(dest): With --enable-warn-dmax ESLEWRNG will be passed
 * to the constraint handler. With --enable-error-dmax this error will be
 * fatal, but dest will not be cleared. With clang-5 and/or diagnose_if and
 * __builtin_object_size() support wrong dmax values will be caught at
 * compile-time.
 *
 * @see
 *    strcat_s(), strncpy_s(), wcsncat_s()
 *    http://www.informit.com/articles/article.aspx?p=2036582&seqNum=5
 */
#ifdef FOR_DOXYGEN
errno_t strncat_s(char *restrict dest, rsize_t dmax,
                  const char *restrict src, rsize_t slen)
#else
EXPORT errno_t _strncat_s_chk(char *restrict dest, rsize_t dmax,
                              const char *restrict src, rsize_t slen,
                              size_t destbos, const size_t srcbos)
#endif
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    if (unlikely(slen == 0 && !dest &&
                 !dmax)) { /* silent ok as in the msvcrt */
        return EOK;
    }
    CHK_DEST_NULL("strncat_s")
    CHK_DMAX_ZERO("strncat_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strncat_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, slen);
    } else {
        CHK_DEST_OVR_CLEAR("strncat_s", destbos)
    }
    CHK_SRC_NULL_CLEAR("strncat_s", src)
    CHK_SRC_OVR_CLEAR("strncat_s", src, slen, RSIZE_MAX_STR)
    CHK_SLEN_MAX_CLEAR("strncat_s", slen, RSIZE_MAX_STR)
    else if (unlikely(slen == 0)) {
        /* Special case, analog to msvcrt: when dest is big enough
           return EOK, but clear dest. */
        errno_t error = (strnlen_s(dest, dmax) < dmax) ? EOK : ESZEROL;
        handle_error(dest, dmax, "strncat_s: slen is 0", error);
        return RCNEGATE(error);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen);
    } else if (unlikely(slen > srcbos)) {
        return handle_str_bos_overload("strncat_s: slen exceeds src",
                                       dest, destbos);
    }

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        /* Find the end of dest */
        while (*dest != '\0') {

            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strncat_s: "
                             "overlapping objects",
                             ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error(orig_dest, orig_dmax,
                             "strncat_s: "
                             "dest unterminated",
                             ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strncat_s: "
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
            if (unlikely(*dest == '\0')) {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear data */
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

        /* Find the end of dest */
        while (*dest != '\0') {

            /*
             * NOTE: no need to check for overlap here since src comes first
             * in memory and we're not incrementing src here.
             */
            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error(orig_dest, orig_dmax,
                             "strncat_s: "
                             "dest unterminated",
                             ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strncat_s: "
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
            if (unlikely(*dest == '\0')) {
#ifdef SAFECLIB_STR_NULL_SLACK
                /* null slack to clear any data */
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
     * the entire src was not copied, so the string will be nulled.
     */
    handle_error(orig_dest, orig_dmax,
                 "strncat_s: not enough "
                 "space for src",
                 ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strncat_s_chk);
#endif /* __KERNEL__ */

#endif /* TEST_MSVCRT */
