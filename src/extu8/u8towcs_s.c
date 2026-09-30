/*------------------------------------------------------------------
 * u8towcs_s.c
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

#ifndef SAFECLIB_DISABLE_WCHAR

/**
 * @def u8towcs_s(retvalp,dest,dmax,src,len)
 * @brief
 *    The \c u8towcs_s function decodes a null-terminated UTF-8 string
 *    pointed to by \c src, unconditionally as UTF-8 (independent of the
 *    current locale), into its wide character representation. If \c dest
 *    is not null, at most \c len wide characters are stored in the
 *    successive elements of the \c wchar_t array pointed to by \c dest.
 *    On platforms with a 2-byte \c wchar_t (Windows, cygwin) codepoints
 *    above U+FFFF are stored as UTF-16 surrogate pairs.
 *
 *    The conversion stops if:
 *
 *    - The UTF-8 NUL character was converted and stored.
 *
 *    - An illegal or truncated UTF-8 sequence was encountered.
 *
 *    - the next wide character to be stored would exceed \c len.
 *      This condition is not checked if \c dest==NULL.
 *
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with 0.
 *
 * @param[out]  retvalp pointer to a \c size_t object where the number of
 *                      wide characters written (excluding the final
 *                      L'\0') is stored
 * @param[out]  dest    NULL or pointer to wide character array for the
 *                      result
 * @param[in]   dmax    restricted maximum length of \c dest
 * @param[in]   src     null-terminated UTF-8 string that will be
 *                      converted to \c dest
 * @param[in]   len     maximal number of wide characters to be written
 *                      to \c dest (exclusive the final L'\0')
 *
 * @pre retvalp and src shall not be a null pointer.
 * @pre dmax and len shall not be greater than \c RSIZE_MAX_WSTR,
 *      unless dest is null.
 * @pre dmax shall not equal zero, unless dest is null.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then \c u8towcs_s nulls dest.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retvalp or src are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR, unless dest is NULL
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax and
 *                     dest != NULL
 * @retval  ESOVRLP    when src and dest overlap
 * @retval  ESNOSPC    when the entire src does not fit within len wide
 *                     characters (unless dest is null)
 * @retval  EILSEQ     when src contains an illegal or truncated UTF-8
 *                     sequence
 * @see
 *    wcstou8_s(), mbstowcs_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8towcs_s(size_t *restrict retvalp, wchar_t *restrict dest,
                  rsize_t dmax, const char8_t *restrict src, rsize_t len)
#else
EXPORT errno_t _u8towcs_s_chk(size_t *restrict retvalp, wchar_t *restrict dest,
                              rsize_t dmax, const char8_t *restrict src,
                              rsize_t len, const size_t destbos)
#endif
{
    wchar_t *orig_dest;
    rsize_t orig_dmax;
    size_t written = 0;

    CHK_SRC_NULL("u8towcs_s", retvalp)
    *retvalp = 0;
    CHK_SRCW_NULL_CLEAR("u8towcs_s", src)

    if (dest) {
        const size_t destsz = dmax * sizeof(wchar_t);
        CHK_DMAX_ZERO("u8towcs_s")
        if (destbos == BOS_UNKNOWN) {
            if (unlikely(dmax > RSIZE_MAX_WSTR || len > RSIZE_MAX_WSTR)) {
                invoke_safe_str_constraint_handler(
                    "u8towcs_s: dmax/len exceeds max", (void *)dest,
                    ESLEMAX);
                return RCNEGATE(ESLEMAX);
            }
            BND_CHK_PTR_BOUNDS(dest, destsz);
        } else {
            CHK_DESTW_OVR_CLEAR("u8towcs_s", destsz, destbos)
        }
        if (unlikely((const char8_t *)dest == src)) {
            handle_werror(dest, dmax, "u8towcs_s: dest overlapping objects",
                          ESOVRLP);
            return RCNEGATE(ESOVRLP);
        }
    }

    orig_dest = dest;
    orig_dmax = dmax;

    while (*src) {
        const int slen = u8_seqlen(src, RSIZE_MAX_STR);
        uint32_t cp;

        if (unlikely(!slen)) {
            if (dest) {
                handle_werror(orig_dest, orig_dmax,
                              "u8towcs_s: illegal UTF-8 sequence", EILSEQ);
            }
            return RCNEGATE(EILSEQ);
        }

        if (dest && written >= (size_t)len) {
            handle_werror(orig_dest, orig_dmax,
                          "u8towcs_s: not enough space for src", ESNOSPC);
            return RCNEGATE(ESNOSPC);
        }

        cp = dec_utf8((char8_t **)&src);

        if (dest) {
            /* on a 2-byte wchar_t, codepoints >= 0x10000 need a surrogate
               pair, i.e. 2 wchar_t slots instead of 1 */
            const rsize_t need =
                (SIZEOF_WCHAR_T <= 2 && cp >= 0x10000) ? 2 : 1;
            if (unlikely(dmax <= need)) { /* keep room for final NUL */
                handle_werror(orig_dest, orig_dmax,
                              "u8towcs_s: not enough space for src",
                              ESNOSPC);
                return RCNEGATE(ESNOSPC);
            }
            _ENC_W16(dest, dmax, cp);
        }
        written++;
    }

    *retvalp = written;

    if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
        memset(dest, 0, dmax * sizeof(wchar_t));
#else
        *dest = L'\0';
#endif
    }

    return RCNEGATE(EOK);
}

#endif /* SAFECLIB_DISABLE_WCHAR */
