/*------------------------------------------------------------------
 * strncpy_s.c
 *
 * November 2020, Reini Urban
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/* TODO not via the naive byte copy, but aligned long word copy
   via the (((X) - 0x0101010101010101) & ~(X) & 0x8080808080808080)
   trick */

/**
 * @def stpncpy_s(dest,dmax,src,slen,errp)
 * @brief
 *    The strncpy_s function copies max slen bytes of the string pointed
 *    to by src (excluding the terminating null character) into the array
 *    pointed to by dest. The strings may not overlap.
 * @details
 *    All elements following the terminating null character (if
 *    any) written by stpncpy_s in the array of dmax characters
 *    pointed to by dest take unspeciﬁed values when stpncpy_s returns.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by stpncpy_s in the
 *    array of dmax characters pointed to by dest are nulled when
 *    stpncpy_s returns.
 *    With modern compilers and constant arguments most errors
 *    will be caught at compile-time.
 *
 * @remark SPECIFIED IN
 *    * Since glibc 2.10:  _POSIX_C_SOURCE >= 200809L
 *    * Before glibc 2.10:  _GNU_SOURCE
 *
 * @param[out]  dest  pointer to string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the string that will be copied to dest
 * @param[in]   slen  the maximum number of bytes to copy from src
 * @param[out]  errp  EOK        success. the characters in src were
 *                               copied into dest and the result is null terminated.
 *                    ESNULLP    when dest or src is a NULL pointer
 *                    ESZEROL    when dmax = 0
 *                    ESLEMAX    when dmax or slen > RSIZE_MAX_STR
 *                    EOVERFLOW  when dmax/slen > size of dest/slen (optionally, when the compiler
 *                               knows the object_size statically)
 *                    ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 *                    ESUNTERM   when src is unterminated
 *                    ESOVRLP    when strings overlap
 *                    ESNOSPC    when src longer than dest
 *
 * @pre Neither dest, src nor err nor shall be a null pointer.
 * @pre dmax shall be size of dest
 * @pre dmax shall not be greater than RSIZE_MAX_STR or size of dest.
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than strnlen_s(src, dmax).
 * @pre If slen is either greater than or equal to dmax, then dmax should
 *      be more than strnlen_s(src,dmax) to avoid truncation.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  stpncpy() returns a pointer to the terminating null byte in dest, or,
            if dest is not null-terminated, dest+n.
 * @return  If there is a runtime-constraint violation, and if dest
 *          and dmax are valid, then stpncpy_s nulls dest.
 *
 * @see
 *    stpcpy_s(), strcpy_s(), strncpy_s()
 *
 */
#ifdef FOR_DOXYGEN
char *stpncpy_s(char *restrict dest, rsize_t dmax, const char *restrict src,
                rsize_t slen, errno_t *restrict errp)
#else
EXPORT char *_stpncpy_s_chk(char *restrict dest, rsize_t dmax,
                            const char *restrict src, rsize_t slen,
                            errno_t *restrict errp,
                            const size_t destbos, const size_t srcbos)
#endif
{
    rsize_t orig_dmax = dmax;
    char* orig_dest = dest;
    const char *overlap_bumper;

    if (unlikely(errp == NULL)) {
        invoke_safe_str_constraint_handler("stpncpy_s: errp is null",
                                           (void *)dest, ESNULLP);
        return NULL;
    }
    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("stpncpy_s: dest is null",
                                           (void *)dest, ESNULLP);
        *errp = RCNEGATE(ESNULLP);
        return NULL;
    }
    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("stpncpy_s: dmax is 0", (void *)dest,
                                           ESNULLP);
        *errp = RCNEGATE(ESZEROL);
        return NULL;
    }
    if (destbos == BOS_UNKNOWN) {
        if (unlikely(dmax > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("stpncpy_s: dmax exceeds max",
                                               (void *)dest, ESLEMAX);
            *errp = RCNEGATE(ESLEMAX);
            return NULL;
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        if (unlikely(dmax > destbos)) {
            if (dmax > RSIZE_MAX_STR) {
                handle_error(dest, destbos, "stpncpy_s: dmax exceeds max",
                             ESLEMAX);
                *errp = RCNEGATE(ESLEMAX);
                return NULL;
            } else {
                *errp = handle_str_bos_overload("stpncpy_s: dmax exceeds dest",
                                               (char *)dest, destbos);
                return NULL;
            }
        }
    }
    if (unlikely(src == NULL)) {
        handle_error(dest, _BOS_KNOWN(dest) ? BOS(dest) : dmax,
                     "strpcpy_s: src is null", ESNULLP);
        *errp = RCNEGATE(ESNULLP);
        return NULL;
    }
    //CHK_SRC_OVR_CLEAR("stpncpy_s", src, slen, RSIZE_MAX_STR)
    if (_BOS_KNOWN(src)) {
        if (unlikely(_BOS_OVR_N(src, slen))) {
            *errp = handle_str_bos_overload("stpncpy_s", (char *)dest,
                                           _BOS_KNOWN(dest) ? BOS(dest) : dmax);
            return NULL;
        }
        BND_CHK_PTR_BOUNDS(src, slen);
    }
    // CHK_SLEN_MAX_CLEAR("stpncpy_s", slen, RSIZE_MAX_STR)
    if (unlikely(slen > RSIZE_MAX_STR)) {
        handle_error(dest, _BOS_KNOWN(dest) ? BOS(dest) : strnlen_s(dest, dmax),
                     "stpncpy_s: " _XSTR(slen) " exceeds max", ESLEMAX);
        *errp = RCNEGATE(ESLEMAX);
        return NULL;
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen);
    } else if (unlikely(slen > srcbos)) {
        *errp = handle_str_bos_overload("stpncpy_s: slen exceeds src", dest,
                                       destbos);
        return NULL;
    }

    if (unlikely(dest == src)) {
        /* walk to the terminating null character */
        while (dmax > 0) {
            if (*dest == '\0') {
                goto eok;
            }
            dmax--;
            dest++;
        }
        goto enospc;
    }

    if (dest < src) {
        overlap_bumper = src;

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "stpncpy_s: "
                             "overlapping objects",
                             ESOVRLP);
                *errp = RCNEGATE(ESOVRLP);
                return NULL;
            }
            if (unlikely(slen == 0)) {
                /*
                 * Copying truncated to slen chars.  Note that the TR says to
                 * copy slen chars plus the null char.  We null the slack.
                 */
                goto eok;
            }

            *dest = *src;
            if (*dest == '\0')
                goto eok;
            dmax--;
            slen++;
            dest++;
            src++;
            if (unlikely(slen >= srcbos)) {
                invoke_safe_str_constraint_handler("stpncpy_s: src unterminated",
                                                   (void *)src, ESUNTERM);
                *errp = RCNEGATE(ESUNTERM);
                return NULL;
            }
        }
    } else {
        overlap_bumper = dest;

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "stpncpy_s: "
                             "overlapping objects",
                             ESOVRLP);
                *errp = RCNEGATE(ESOVRLP);
                return NULL;
            }

            if (unlikely(slen == 0)) {
                /*
                 * Copying truncated to slen chars.  Note that the TR says to
                 * copy slen chars plus the null char.  We null the slack.
                 */
                goto eok;
            }

            *dest = *src;
            if (*dest == '\0') {
              eok:
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
                *errp = RCNEGATE(EOK);
                return dest;
            }

            dmax--;
            slen++;
            dest++;
            src++;
            if (unlikely(slen >= srcbos)) {
                invoke_safe_str_constraint_handler("stpncpy_s: src unterminated",
                                                   (void *)src, ESUNTERM);
                *errp = RCNEGATE(ESUNTERM);
                return NULL;
            }
        }
    }

  enospc:
    /*
     * the entire src must have been copied, if not reset dest
     * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
     */
    handle_error(orig_dest, orig_dmax,
                 "stpncpy_s: not enough space for src",
                 ESNOSPC);
    *errp = RCNEGATE(ESNOSPC);
    return NULL;
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_stpncpy_s_chk);
#endif /* __KERNEL__ */

