/*------------------------------------------------------------------
 * strcpy_s.c
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
    defined(HAVE_STRCPY_S)
#else

/* not via the naive byte copy, but aligned long word copy
   via the (((X) - 0x0101010101010101) & ~(X) & 0x8080808080808080)
   check */

/**
 * @def strcpy_s(dest,dmax,src)
 * @brief
 *    The strcpy_s function copies the string pointed to by src
 *    (including the terminating null character) into the array
 *    pointed to by dest.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written by strcpy_s in the
 *    array of dmax characters pointed to by dest are nulled when
 *    strcpy_s returns.
 *    With modern compilers and constant arguments most errors
 *    will be caught at compile-time.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.1.3 The strcpy_s function (p: 615-616)
 *    http://en.cppreference.com/w/c/string/byte/strcpy
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the string that will be copied to dest
 *
 * @pre Neither dest nor src shall be a null pointer.
 * @pre dmax shall be size of dest
 * @pre dmax shall not be greater than RSIZE_MAX_STR or size of dest.
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than strnlen_s(src, dmax).
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @note The Windows MSVCRT sec_api EINVAL and ERANGE works ok,
 *       ESLEMAX dmax > MAX not, ESOVRLP partially. When dest > src Windows
 *       copies the result, when dest < src ERANGE is returned.
 *
 * @return  If there is a runtime-constraint violation, and if dest
 *          and dmax are valid, then strcpy_s nulls dest.
 * @retval  EOK        when successful operation, the characters in src were
 *                     copied into dest and the result is null terminated.
 * @retval  ESNULLP    when dest or src is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESOVRLP    when strings overlap
 * @retval  ESNOSPC    when dest < src
 *
 * @see
 *    strcat_s(), strncat_s(), strncpy_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strcpy_s(char *restrict dest, rsize_t dmax, const char *restrict src)
#else
EXPORT errno_t _strcpy_s_chk(char *restrict dest, rsize_t dmax,
                             const char *restrict src, const size_t destbos)
#endif
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    CHK_DEST_NULL("strcpy_s")
    CHK_DMAX_ZERO("strcpy_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strcpy_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR_CLEAR("strcpy_s", destbos)
    }
    CHK_SRC_NULL_CLEAR("strcpy_s", src)

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
                             "strcpy_s: "
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
    } else {
        overlap_bumper = dest;

        while (dmax > 0) {
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strcpy_s: "
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

    /*
     * the entire src must have been copied, if not reset dest
     * to null the string. (only with SAFECLIB_STR_NULL_SLACK)
     */
    handle_error(orig_dest, orig_dmax,
                 "strcpy_s: not "
                 "enough space for src",
                 ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strcpy_s_chk);
#endif /* __KERNEL__ */

#endif
