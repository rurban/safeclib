/*------------------------------------------------------------------
 * mbstowcs_s.c
 *
 * August 2017, Reini Urban
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

#if (defined(TEST_MSVCRT) && defined(HAVE_MBSTOWCS_S)) ||                      \
    !defined(HAVE_WCHAR_H)
#else

/* newlib, cygwin64 has no STDC_HEADERS!
   more importantly it has but does not declare mbstowcs.
 */
#ifdef HAVE_CYGWIN64
#define mbstowcs(dest, src, len)                                               \
    mbsrtowcs((dest), (const char **restrict) & (src), (len), &st)
#endif

/**
 * @def mbstowcs_s(retvalp,dest,dmax,src,len)
 * @brief
 *    The \c mbstowcs_s function converts a null-terminated multibyte
 *    character sequence from the current LC_CTYPE locale to wchar, which
 *    begins in the conversion state described by \c *ps, from the
 *    array whose first element is pointed to by \c *src to its wide
 *    character representation. If dest is not null, converted
 *    characters are stored in the successive elements of the
 *    \c wchar_t array pointed to by \c dest. No more than \c len wide
 *    characters are written to the destination array. Each multibyte
 *    character is converted as if by a call to \c mbrtowc.
 *    \c mbstowcs_s clobbers the destination array from the terminating
 *    null and until \c dmax.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with
 *    0.
 *
 *    The conversion stops if:
 *
 *    - The multibyte null character was converted and stored.
 *
 *    - An invalid multibyte character (according to the current C
 *      locale) was encountered.
 *
 *    - the next wide character to be stored would exceed \c len.
 *      This condition is not checked if \c dest==NULL.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.6.5.1 The mbstowcs_s function (p: 611-612)
 *    http://en.cppreference.com/w/c/string/multibyte/mbstowcs
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  retvalp pointer to a \c size_t object where the result will be
 * stored
 * @param[out]  dest    NULL or pointer to wide character array for the result
 * @param[in]   dmax    restricted maximum length of \c dest
 * @param[in]   src     string that will be converted to \c dest
 * @param[in]   len     maximal number of wide characters to be written to \c
 * dest (exclusive the final L'\0' when needed)
 *
 * @pre retvalp and src shall not be a null pointer.
 * @pre dmax and len shall not be greater than \c RSIZE_MAX_WSTR,
 *      unless dest is null.
 * @pre dmax shall not equal zero, unless dest is null.
 * @pre dmax shall be greater than <tt>wcsnlen_s(src, dmax)</tt>.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then \c mbstowcs_s nulls dest.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retval or src are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR, unless dest is NULL
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax and
 * dest != NULL
 * @retval  ESOVRLP    when src and dest overlap
 * @retval  ESNOSPC    when there is no null character in the first dmax
 *                     multibyte characters in the src array and len is
 *                     greater than dmax (unless dest is null)
 * @see
 *    mbsrtowc_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t mbstowcs_s(size_t *restrict retvalp, wchar_t *restrict dest,
                   rsize_t dmax, const char *restrict src,
                   rsize_t len)
#else
EXPORT errno_t _mbstowcs_s_chk(size_t *restrict retvalp, wchar_t *restrict dest,
                               rsize_t dmax, const char *restrict src,
                               rsize_t len, const size_t destbos)
#endif
{
    wchar_t *orig_dest;
    errno_t rc;
#ifdef HAVE_CYGWIN64
    mbstate_t st;
#endif

    CHK_SRC_NULL("mbstowcs_s", retvalp)
    *retvalp = 0;
    CHK_SRCW_NULL_CLEAR("mbstowcs_s", src)
    if (dest) {
        /* string literals also have the ending \0 */
        size_t destsz = dmax * sizeof(wchar_t);
        CHK_DMAX_ZERO("mbstowcs_s")
        if (destbos == BOS_UNKNOWN) {
            if (unlikely(dmax > RSIZE_MAX_WSTR || len > RSIZE_MAX_WSTR)) {
                invoke_safe_str_constraint_handler("mbstowcs"
                                                   ": dmax/len exceeds max",
                                                   (void *)dest, ESLEMAX);
                return RCNEGATE(ESLEMAX);
            }
            BND_CHK_PTR_BOUNDS(dest, destsz);
        } else {
            if (unlikely(destsz > destbos || len * sizeof(wchar_t) > destbos)) {
                if (unlikely(dmax > RSIZE_MAX_WSTR || len > RSIZE_MAX_WSTR)) {
                    handle_error((char *)(void *)dest, destbos,
                                 "mbstowcs"
                                 ": dmax/len exceeds max",
                                 ESLEMAX);
                    return RCNEGATE(ESLEMAX);
                } else {
                    handle_error((char *)(void *)dest, destbos,
                                 "mbstowcs"
                                 ": dmax/len exceeds destsz",
                                 EOVERFLOW);
                    return RCNEGATE(EOVERFLOW);
                }
            }
#ifdef HAVE_WARN_DMAX
            if (unlikely(destsz != destbos)) {
                handle_str_bos_chk_warn("mbstowcs", (char *)dest, dmax,
                                        destbos / sizeof(wchar_t));
                RETURN_ESLEWRNG;
            }
#endif
        }
    }
    if (unlikely((char *)dest == src)) {
        return RCNEGATE(ESOVRLP);
    }

    /* hold base of dest in case src was not copied */
    orig_dest = dest;
    errno = 0;

    *retvalp = mbstowcs(dest, src, len);

    if (likely(*retvalp < dmax)) {
        if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
            memset(&dest[*retvalp], 0, (dmax - *retvalp) * sizeof(wchar_t));
#else
            dest[*retvalp] = L'\0';
#endif
        }
        rc = EOK;
    } else {
        if (dest) {
            size_t tmp = 0;
            errno = 0;
            if (*retvalp > RSIZE_MAX_WSTR) { /* else ESNOSPC */
                tmp = mbstowcs(NULL, src, len);
            }
            /* with NULL either 0 or -1 is returned */
            rc = (tmp == 0) ? ESNOSPC : errno;
            /* the entire src must have been copied, if not reset dest
             * to null the string. (only with SAFECLIB_STR_NULL_SLACK) */
            handle_werror(orig_dest, dmax,
                          !tmp ? "mbstowcs_s: not enough space for src"
                               : "mbstowcs_s: illegal sequence",
                          rc);
        } else {
            rc = ((size_t)*retvalp == 0) ? EOK : errno;
        }
    }

    return RCNEGATE(rc);
}

#endif /* HAVE_WCHAR_H && !TEST_MSVCRT ... */
