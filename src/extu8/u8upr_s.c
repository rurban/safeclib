/*------------------------------------------------------------------
 * u8upr_s.c
 *
 * September 2026, Reini Urban
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
#include <wctype.h>
#endif
#include "u8_private.h"

#ifndef FOR_DOXYGEN
/* The dest/dmax constraints are already checked by _u8upr_s_chk or proven at
   compile-time. GH #48 */
EXPORT errno_t _u8upr_s_uchk(char8_t *dest, rsize_t dmax) {
    char8_t stackbuf[128];
    char8_t *buf;
    char8_t *out;
    const char8_t *src;
    rsize_t remaining;
    rsize_t outcap;
    rsize_t used;

    /* worst case: every byte is a single-byte codepoint whose uppercase
       mapping encodes to 4 bytes */
    outcap = dmax * 4;
    if (outcap <= sizeof(stackbuf)) {
        buf = stackbuf;
    } else {
        buf = (char8_t *)malloc(outcap);
        if (!buf) {
            invoke_safe_str_constraint_handler("u8upr_s: out of memory",
                                               (void *)dest, ENOMEM);
            return RCNEGATE(ENOMEM);
        }
    }

    src = dest;
    remaining = dmax;
    out = buf;
    used = 0;
    while (remaining && *src) {
        int bytes = u8_seqlen(src, remaining);
        uint32_t cp, upper;
        int n;

        if (!bytes) {
            if (buf != stackbuf)
                free(buf);
            invoke_safe_str_constraint_handler("u8upr_s: illegal utf-8",
                                               (void *)dest, EILSEQ);
            return RCNEGATE(EILSEQ);
        }
        cp = dec_utf8((char8_t **)&src);
        remaining -= (rsize_t)bytes;
        upper = (uint32_t)towupper((wint_t)cp);
        n = enc_utf8(out, upper);
        out += n;
        used += (rsize_t)n;
    }
    *out = '\0';

    if (used + 1 > dmax) {
        if (buf != stackbuf)
            free(buf);
        invoke_safe_str_constraint_handler("u8upr_s: dmax too small for result",
                                           (void *)dest, ESNOSPC);
        return RCNEGATE(ESNOSPC);
    }

    memcpy(dest, buf, used + 1);
#ifdef SAFECLIB_STR_NULL_SLACK
    memset(dest + used + 1, 0, dmax - used - 1);
#endif
    if (buf != stackbuf)
        free(buf);

    return (EOK);
}
#endif

/**
 * @def u8upr_s(dest,dmax)
 * @brief
 *    Scans the utf-8 string converting each codepoint to simple
 *    uppercase via \c towupper(), leaving all other codepoints
 *    unchanged. The scanning stops at the first NUL or after dmax
 *    bytes. The conversion is determined by the LC_CTYPE category
 *    setting of the locale; with the default "C" locale only ASCII
 *    is affected. It only performs simple case conversion, not full
 *    multi-char expansion (e.g. German sharp s stays unchanged), and
 *    does not obey special-casing context rules. Since a codepoint's
 *    uppercase encoding may take a different number of utf-8 bytes
 *    than its original encoding, the resulting string may be shorter
 *    or longer than the input, but always fits within dmax.
 *
 * @param[in,out]  dest  utf-8 string
 * @param[in]      dmax  maximum byte-length of dest
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 *
 * @retval  EOK         on successful operation
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESNOSPC     when the uppercased result needs more space than dmax
 * @retval  EILSEQ      when dest contains an illegal utf-8 sequence
 *
 * @see
 *    u8lwr_s(), u8fc_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8upr_s(char8_t *dest, rsize_t dmax)
#else
EXPORT errno_t _u8upr_s_chk(char8_t *dest, rsize_t dmax,
                            const size_t destbos)
#endif
{
    CHK_DEST_NULL("u8upr_s")
    CHK_DMAX_ZERO("u8upr_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8upr_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8upr_s", destbos)
    }

    return _u8upr_s_uchk(dest, dmax);
}
