/*------------------------------------------------------------------
 * u8rtowcs_s.c
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
 * @def u8rtowcs_s(retvalp,dest,dmax,srcp,len,ps)
 * @brief
 *    The restartable variant of \c u8towcs_s(). Converts a UTF-8
 *    string, unconditionally as UTF-8 (independent of the current
 *    locale), pointed to by \c *srcp into wide characters stored in
 *    \c dest.  At most \c len wide characters are converted, and
 *    \c *srcp is updated to point one past the last UTF-8 byte
 *    converted, so that a subsequent call can resume the conversion.
 *    \c ps is accepted for API compatibility but unused, as UTF-8
 *    decoding needs no shift state.
 *
 * @param[out]     retvalp pointer to a \c size_t object where the number
 *                         of wide characters written is stored
 * @param[out]     dest    NULL or pointer to wide character array for
 *                         the result
 * @param[in]      dmax    restricted maximum length of \c dest
 * @param[in,out]  srcp    pointer to the UTF-8 string pointer that will
 *                         be converted to \c dest, and advanced
 * @param[in]      len     maximal number of wide characters to be
 *                         written to \c dest
 * @param[in,out]  ps      unused conversion state, accepted for API
 *                         compatibility
 *
 * @pre retvalp, srcp, *srcp and ps shall not be a null pointer.
 * @pre dmax and len shall not be greater than \c RSIZE_MAX_WSTR,
 *      unless dest is null.
 * @pre dmax shall not equal zero, unless dest is null.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then \c u8rtowcs_s nulls dest.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retvalp, srcp, *srcp or ps are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR, unless dest is NULL
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESOVRLP    when *srcp and dest overlap
 * @retval  EILSEQ     when *srcp contains an illegal or truncated UTF-8
 *                     sequence
 * @see
 *    u8towcs_s(), wcstou8_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8rtowcs_s(size_t *restrict retvalp, wchar_t *restrict dest,
                   rsize_t dmax, const char8_t **restrict srcp, rsize_t len,
                   mbstate_t *restrict ps)
#else
EXPORT errno_t _u8rtowcs_s_chk(size_t *restrict retvalp, wchar_t *restrict dest,
                               rsize_t dmax, const char8_t **restrict srcp,
                               rsize_t len, mbstate_t *restrict ps,
                               const size_t destbos)
#endif
{
    wchar_t *orig_dest;
    rsize_t orig_dmax;
    const char8_t *src;
    size_t written = 0;

    CHK_SRC_NULL("u8rtowcs_s", retvalp)
    *retvalp = 0;
    CHK_SRC_NULL("u8rtowcs_s", ps)
    CHK_SRC_NULL("u8rtowcs_s", srcp)
    CHK_SRCW_NULL_CLEAR("u8rtowcs_s", *srcp)
    src = *srcp;

    if (dest) {
        const size_t destsz = dmax * sizeof(wchar_t);
        CHK_DMAX_ZERO("u8rtowcs_s")
        if (destbos == BOS_UNKNOWN) {
            if (unlikely(dmax > RSIZE_MAX_WSTR || len > RSIZE_MAX_WSTR)) {
                invoke_safe_str_constraint_handler(
                    "u8rtowcs_s: dmax/len exceeds max", (void *)dest, ESLEMAX);
                return RCNEGATE(ESLEMAX);
            }
            BND_CHK_PTR_BOUNDS(dest, destsz);
        } else {
            CHK_DESTW_OVR_CLEAR("u8rtowcs_s", destsz, destbos)
        }
        if (unlikely((const char8_t *)dest == src)) {
            handle_werror(dest, dmax, "u8rtowcs_s: dest overlapping objects",
                          ESOVRLP);
            return RCNEGATE(ESOVRLP);
        }
    }

    orig_dest = dest;
    orig_dmax = dmax;

    while (*src && written < (size_t)len) {
        const int slen = u8_seqlen(src, RSIZE_MAX_STR);
        char8_t *peek;
        uint32_t cp;
        rsize_t need;

        if (unlikely(!slen)) {
            if (dest) {
                handle_werror(orig_dest, orig_dmax,
                              "u8rtowcs_s: illegal UTF-8 sequence", EILSEQ);
            }
            *srcp = src;
            return RCNEGATE(EILSEQ);
        }

        peek = (char8_t *)src;
        cp = dec_utf8(&peek);
        need = (SIZEOF_WCHAR_T <= 2 && cp >= 0x10000) ? 2 : 1;
        if (dest && dmax <= need) /* not enough room, stop before consuming */
            break;

        src = (const char8_t *)peek;
        if (dest) {
            _ENC_W16(dest, dmax, cp);
        }
        written++;
    }

    *srcp = src;
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
