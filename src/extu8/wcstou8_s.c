/*------------------------------------------------------------------
 * wcstou8_s.c
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
 * @def wcstou8_s(retvalp,dest,dmax,src,len)
 * @brief
 *    The \c wcstou8_s function encodes a null-terminated wide character
 *    string pointed to by \c src into UTF-8, unconditionally (independent
 *    of the current locale). On platforms with a 2-byte \c wchar_t
 *    (Windows, cygwin) UTF-16 surrogate pairs in \c src are combined
 *    into a single codepoint before encoding. If \c dest is not null,
 *    at most \c len UTF-8 bytes are written to the successive elements
 *    of \c dest.
 *
 *    The conversion stops if:
 *
 *    - The wide NUL character L'\\0' was converted and stored.
 *
 *    - A \c wchar_t was found that does not represent a valid Unicode
 *      codepoint (encoded surrogate half or beyond U+10FFFF).
 *
 *    - the next UTF-8 sequence to be stored would exceed \c len.
 *      This condition is not checked if \c dest==NULL.
 *
 *    With SAFECLIB_STR_NULL_SLACK defined the rest is cleared with 0.
 *
 * @param[out]  retvalp pointer to a \c size_t object where the number of
 *                      UTF-8 bytes written (excluding the final NUL) is
 *                      stored
 * @param[out]  dest    NULL or pointer to UTF-8 byte array for the result
 * @param[in]   dmax    restricted maximum length of \c dest
 * @param[in]   src     null-terminated wide character string that will
 *                      be converted to \c dest
 * @param[in]   len     maximal number of UTF-8 bytes to be written to
 *                      \c dest (exclusive the final NUL)
 *
 * @pre retvalp and src shall not be a null pointer.
 * @pre dmax shall not equal zero.
 * @pre dmax shall not be greater than \c RSIZE_MAX_STR.
 * @pre Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then \c wcstou8_s nulls dest.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retvalp or src are a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESOVRLP    when src and dest overlap
 * @retval  ESNOSPC    when the entire src does not fit within len bytes
 * @retval  EILSEQ     when src contains an illegal wide character
 *                     (encoded surrogate half or beyond U+10FFFF)
 * @see
 *    u8towcs_s(), wcstombs_s()
 */
#ifdef FOR_DOXYGEN
errno_t wcstou8_s(size_t *restrict retvalp, char8_t *restrict dest,
                  rsize_t dmax, const wchar_t *restrict src, rsize_t len)
#else
EXPORT errno_t _wcstou8_s_chk(size_t *restrict retvalp, char8_t *restrict dest,
                              rsize_t dmax, const wchar_t *restrict src,
                              rsize_t len, const size_t destbos)
#endif
{
    char8_t *orig_dest;
    rsize_t orig_dmax;
    size_t written = 0;

    CHK_SRC_NULL("wcstou8_s", retvalp)
    *retvalp = 0;
    CHK_DEST_NULL("wcstou8_s")
    CHK_DMAX_ZERO("wcstou8_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("wcstou8_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DESTW_OVR("wcstou8_s", dmax, destbos)
    }
    if (unlikely(src == NULL)) {
        handle_error((char *)dest, dmax, "wcstou8_s: src is null", ESNULLP);
        return RCNEGATE(ESNULLP);
    }
    if (unlikely((const wchar_t *)dest == src)) {
        handle_error((char *)dest, dmax, "wcstou8_s: dest overlapping objects",
                     ESOVRLP);
        return RCNEGATE(ESOVRLP);
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
            handle_error((char *)orig_dest, orig_dmax,
                         "wcstou8_s: illegal surrogate pair", EILSEQ);
            return RCNEGATE(EILSEQ);
        }

        src += (SIZEOF_WCHAR_T == 2 && cp > 0xffff) ? 2 : 1;

        blen = enc_utf8(buf, cp);
        if (unlikely(!blen)) {
            handle_error((char *)orig_dest, orig_dmax,
                         "wcstou8_s: illegal wide character", EILSEQ);
            return RCNEGATE(EILSEQ);
        }
        if (unlikely((rsize_t)blen >= dmax)) { /* keep room for final NUL */
            handle_error((char *)orig_dest, orig_dmax,
                         "wcstou8_s: not enough space for src", ESNOSPC);
            return RCNEGATE(ESNOSPC);
        }
        memcpy(dest, buf, (size_t)blen);
        dest += blen;
        dmax -= (rsize_t)blen;
        written += (size_t)blen;
    }

    *retvalp = written;
#ifdef SAFECLIB_STR_NULL_SLACK
    memset(dest, 0, dmax);
#else
    *dest = '\0';
#endif

    return RCNEGATE(EOK);
}

#endif /* SAFECLIB_DISABLE_WCHAR */
