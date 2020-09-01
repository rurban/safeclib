/*------------------------------------------------------------------
 * strcat_s.c
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

/* With mingw shared with sec_api and -DTEST_MSVCRT skip it */
#if defined(TEST_MSVCRT) && defined(_WIN32) && !defined(DISABLE_DLLIMPORT) &&  \
    defined(HAVE_STRCAT_S)
#else

/**
 * @def strcat_s(dest,dmax,src)
 * @brief
 *    The \b strcat_s function appends a copy of the string pointed
 *    to by src (including the terminating null character) to the
 *    end of the string pointed to by dest. The initial character
 *    from src overwrites the null character at the end of dest.
 * @details
 *    All elements following the terminating null character (if
 *    any) written by strcat_s in the array of dmax characters
 *    pointed to by dest take unspeciﬁed values when strcat_s
 *    returns.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest of dest is
 *    cleared with NULL bytes.
 *    With modern compilers and constant arguments most errors
 *    will be caught at compile-time.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.2.1 The strcat_s function (p: 617-618)
 *    http://en.cppreference.com/w/c/string/byte/strcat
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest      pointer to string that will be extended by src
 *                        if dmax allows. The string is null terminated.
 *                        If the resulting concatenated string is less
 *                        than dmax, the remaining slack space is nulled.
 * @param[in]   dmax      restricted maximum length of the resulting dest,
 *                        including the null
 * @param[in]   src       pointer to the string that will be concatenaed
 *                        to string dest
 *
 * @pre  Neither dest nor src shall be a null pointer
 * @pre  dmax shall be size of dest
 * @pre  dmax shall not equal zero
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 * @pre  dmax shall be greater than strnlen_s(src,m).
 * @pre  Copying shall not take place between objects that overlap
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @note The Windows MSVCRT sec_api EINVAL and ERANGE works ok,
 *       ESLEMAX dmax/slen>MAX not, ESOVRLP returns EINVAL or ERANGE,
 *       unlike \c strncat_s.
 *
 * @return  If there is a runtime-constraint violation, then if dest is
 *          not a null pointer and dmax is greater than zero and not
 *          greater than RSIZE_MAX_STR, then strcat_s nulls dest.
 * @retval  EOK        when successful operation, all the characters from src
 *                     were appended to dest and the result in dest is null
 * terminated.
 * @retval  ESNULLP    when dest or src is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESUNTERM   when dest not terminated in the first dmax bytes
 * @retval  ESOVRLP    when src overlaps with dest
 *
 * @see
 *    strncat_s(), strcpy_s(), strncpy_s()
 *
 */

#ifdef FOR_DOXYGEN
errno_t strcat_s(char *restrict dest, rsize_t dmax,
                 const char *restrict src)
#else
EXPORT errno_t _strcat_s_chk(char *restrict dest, rsize_t dmax,
                             const char *restrict src, size_t destbos)
#endif
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    CHK_DEST_NULL("strcat_s")
    CHK_DMAX_ZERO("strcat_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strcat_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR_CLEAR("strcat_s", destbos)
    }
    CHK_SRC_NULL_CLEAR("strcat_s", src)

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (dest < src) {
        overlap_bumper = src;

        /* Find the end of dest */
        while (*dest != '\0') {

            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strcat_s: "
                             "overlapping objects",
                             ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }

            dest++;
            dmax--;
            if (unlikely(dmax == 0)) {
                handle_error(orig_dest, orig_dmax,
                             "strcat_s: "
                             "dest unterminated",
                             ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strcat_s: "
                             "overlapping objects",
                             ESOVRLP);
                return RCNEGATE(ESOVRLP);
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
                             "strcat_s: "
                             "dest unterminated",
                             ESUNTERM);
                return RCNEGATE(ESUNTERM);
            }
        }

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strcat_s: "
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
                        *dest = '\0';
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

    /* the entire src was not copied, so null the string */
    handle_error(orig_dest, orig_dmax,
                 "strcat_s: not enough "
                 "space for src",
                 ESNOSPC);

    return RCNEGATE(ESNOSPC);
}

#ifdef __KERNEL__
EXPORT_SYMBOL(_strcat_s_chk);
#endif /* __KERNEL__ */

#endif
