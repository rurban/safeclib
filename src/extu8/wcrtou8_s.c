/*------------------------------------------------------------------
 * wcrtou8_s.c
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
 * @def wcrtou8_s(retvalp,dest,dmax,wc,ps)
 * @brief
 *    Does not permit the \c ps parameter (the pointer to the conversion
 *    state) to be a null pointer.
 *    The restartable \c wcrtou8_s function encodes a single wide
 *    character \c wc, unconditionally as UTF-8 (independent of the
 *    current locale), storing 1-4 UTF-8 bytes in \c dest, followed by
 *    a terminating NUL. \c ps is accepted for API compatibility but
 *    unused, as UTF-8 encoding needs no shift state.
 *
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating NUL character (if any) written in the array of dmax
 *    characters pointed to by dest are nulled.
 *
 * @param[out]  retvalp  pointer to a \c size_t object where the number of
 *                       UTF-8 bytes written (excluding the NUL) is stored
 * @param[out]  dest     pointer to bytes where the result will be stored
 * @param[in]   dmax     restricted maximum length of \c dest
 * @param[in]   wc       the wide character (Unicode codepoint) to convert
 * @param[in]   ps       unused conversion state, accepted for API
 *                       compatibility
 *
 * @pre retvalp and ps shall not be a null pointer.
 * @pre dest shall not be a null pointer.
 * @pre dmax shall not equal zero.
 * @pre dmax shall not be greater than \c RSIZE_MAX_STR and size of dest.
 * @pre wc shall not exceed 0x10ffff.
 *
 * @return  Returns zero on success and non-zero on failure, in which
 *          case \c dest[0] is set to '\0' and \c *retvalp is set to
 *          (size_t)-1.
 * @retval  EOK        on successful conversion.
 * @retval  ESNULLP    when retvalp or ps are a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOSPC    when dmax is smaller than the required bytes + NUL
 * @retval  EILSEQ     when wc is an encoded surrogate half or exceeds
 *                     U+10FFFF
 * @see
 *    wctou8_s(), wcrtomb_s()
 */
#ifdef FOR_DOXYGEN
errno_t wcrtou8_s(size_t *restrict retvalp, char8_t *restrict dest,
                  rsize_t dmax, wchar_t wc, mbstate_t *restrict ps)
#else
EXPORT errno_t _wcrtou8_s_chk(size_t *restrict retvalp, char8_t *restrict dest,
                              rsize_t dmax, wchar_t wc, mbstate_t *restrict ps,
                              const size_t destbos)
#endif
{
    char8_t buf[4];
    int blen;

    CHK_SRC_NULL("wcrtou8_s", retvalp)
    *retvalp = (size_t)-1;
    CHK_SRC_NULL("wcrtou8_s", ps)
    CHK_DEST_NULL("wcrtou8_s")
    CHK_DMAX_ZERO("wcrtou8_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("wcrtou8_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("wcrtou8_s", destbos)
    }

    blen = enc_utf8(buf, (uint32_t)wc);
    if (unlikely(!blen)) {
        handle_error((char *)dest, dmax, "wcrtou8_s: illegal wide character",
                     EILSEQ);
        return RCNEGATE(EILSEQ);
    }
    if (unlikely((rsize_t)blen >= dmax)) {
        handle_error((char *)dest, dmax, "wcrtou8_s: not enough space",
                     ESNOSPC);
        return RCNEGATE(ESNOSPC);
    }

    memcpy(dest, buf, (size_t)blen);
    *retvalp = (size_t)blen;
#ifdef SAFECLIB_STR_NULL_SLACK
    memset(&dest[blen], 0, dmax - (rsize_t)blen);
#else
    dest[blen] = '\0';
#endif

    return RCNEGATE(EOK);
}

#endif /* SAFECLIB_DISABLE_WCHAR */
