/*------------------------------------------------------------------
 * wcsrtou8_s.c
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
 * @def wcsrtou8_s(retvalp,dest,dmax,srcp,len,ps)
 * @brief
 *    The restartable variant of \c wcstou8_s(). Encodes a wide
 *    character string, unconditionally as UTF-8 (independent of the
 *    current locale), pointed to by \c *srcp into UTF-8 bytes stored
 *    in \c dest. At most \c len bytes are written, and \c *srcp is
 *    updated to point one past the last wide character converted, so
 *    that a subsequent call can resume the conversion. \c ps is
 *    accepted for API compatibility but unused.
 *
 * @param[out]     retvalp pointer to a \c size_t object where the number
 *                         of UTF-8 bytes written is stored
 * @param[out]     dest    NULL or pointer to UTF-8 byte array for the
 *                         result
 * @param[in]      dmax    restricted maximum length of \c dest
 * @param[in,out]  srcp    pointer to the wide string pointer that will
 *                         be converted to \c dest, and advanced
 * @param[in]      len     maximal number of UTF-8 bytes to be written
 *                         to \c dest
 * @param[in,out]  ps      unused conversion state, accepted for API
 *                         compatibility
 *
 * @pre retvalp, srcp, *srcp and ps shall not be a null pointer.
 * @pre dmax shall not equal zero, unless dest is null.
 * @pre dmax shall not be greater than \c RSIZE_MAX_STR, unless dest is
 *      null.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then \c wcsrtou8_s nulls dest.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retvalp, srcp, *srcp or ps are a NULL pointer
 * @retval  ESZEROL    when dmax = 0, unless dest is NULL
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, unless dest is NULL
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESOVRLP    when *srcp and dest overlap
 * @retval  EILSEQ     when *srcp contains an illegal wide character
 *                     (encoded surrogate half or beyond U+10FFFF)
 * @see
 *    wcstou8_s(), u8rtowcs_s()
 */
#ifdef FOR_DOXYGEN
errno_t wcsrtou8_s(size_t *restrict retvalp, char8_t *restrict dest,
                   rsize_t dmax, const wchar_t **restrict srcp, rsize_t len,
                   mbstate_t *restrict ps)
#else
EXPORT errno_t _wcsrtou8_s_chk(size_t *restrict retvalp, char8_t *restrict dest,
                               rsize_t dmax, const wchar_t **restrict srcp,
                               rsize_t len, mbstate_t *restrict ps,
                               const size_t destbos)
#endif
{
    char8_t *orig_dest;
    rsize_t orig_dmax;
    const wchar_t *src;
    size_t written = 0;

    CHK_SRC_NULL("wcsrtou8_s", retvalp)
    *retvalp = 0;
    CHK_SRC_NULL("wcsrtou8_s", ps)
    CHK_SRC_NULL("wcsrtou8_s", srcp)
    CHK_SRC_NULL("wcsrtou8_s", *srcp)
    src = *srcp;

    if (dest) {
        CHK_DMAX_ZERO("wcsrtou8_s")
        if (destbos == BOS_UNKNOWN) {
            CHK_DMAX_MAX("wcsrtou8_s", RSIZE_MAX_STR)
            BND_CHK_PTR_BOUNDS(dest, dmax);
        } else {
            CHK_DESTW_OVR("wcsrtou8_s", dmax, destbos)
        }
        if (unlikely((const wchar_t *)dest == src)) {
            handle_error((char *)dest, dmax,
                         "wcsrtou8_s: dest overlapping objects", ESOVRLP);
            return RCNEGATE(ESOVRLP);
        }
    }

    orig_dest = dest;
    orig_dmax = dmax;

    while (*src && written < (size_t)len) {
        uint32_t cp = _dec_w16((wchar_t *)src);
        char8_t buf[4];
        int blen;

        /* *src != 0 (loop guard), so cp == 0 can only mean _dec_w16
           rejected an unpaired/invalid surrogate half */
        if (unlikely(cp == 0)) {
            if (dest) {
                handle_error((char *)orig_dest, orig_dmax,
                             "wcsrtou8_s: illegal surrogate pair", EILSEQ);
            }
            *srcp = src;
            return RCNEGATE(EILSEQ);
        }

        blen = enc_utf8(buf, cp);
        if (unlikely(!blen)) {
            if (dest) {
                handle_error((char *)orig_dest, orig_dmax,
                             "wcsrtou8_s: illegal wide character", EILSEQ);
            }
            *srcp = src;
            return RCNEGATE(EILSEQ);
        }
        if (dest && (rsize_t)blen >= dmax) /* stop before consuming */
            break;

        src += (SIZEOF_WCHAR_T == 2 && cp > 0xffff) ? 2 : 1;

        if (dest) {
            memcpy(dest, buf, (size_t)blen);
            dest += blen;
            dmax -= (rsize_t)blen;
        }
        written += (size_t)blen;
    }

    *srcp = src;
    *retvalp = written;

    if (dest) {
#ifdef SAFECLIB_STR_NULL_SLACK
        memset(dest, 0, dmax);
#else
        *dest = '\0';
#endif
    }

    return RCNEGATE(EOK);
}

#endif /* SAFECLIB_DISABLE_WCHAR */
