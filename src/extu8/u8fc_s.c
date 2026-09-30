/*------------------------------------------------------------------
 * u8fc_s.c
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
#endif
#include "u8_private.h"

/**
 * @def u8fc_s(dest,dmax,src,lenp)
 * @brief
 *    Converts the utf-8 string to fully fold-cased lowercase, per the
 *    Unicode CaseFolding.txt table (the same data used by u8u8_s()
 *    for case-insensitive matching).
 * @details
 *    Full case-folding performs the 105 multi-codepoint expansions
 *    specified by CaseFolding.txt (e.g. U+00DF LATIN SMALL LETTER
 *    SHARP S folds to "ss"), via \c towfc_s(). All other codepoints
 *    are simple-folded via \c towlower(). Because a folded codepoint
 *    may encode to a different number of utf-8 bytes than the
 *    original, and multi-codepoint expansions grow the string, dest
 *    should be sized generously (dmax >= 3x the byte-length of src is
 *    always sufficient).
 *
 *    Unlike \c wcsfc_s(), this implementation does not additionally
 *    NFD-decompose composed base+accent codepoints, and does not
 *    apply the locale-conditional special-casing rules (Lithuanian
 *    dotted I/J, Turkish/Azeri dotless i, Greek final sigma). Use
 *    \c u8norm_s() first if NFD decomposition is also required.
 *
 * @param[out]  dest  utf-8 string to hold the result
 * @param[in]   dmax  maximum result buffer size
 * @param[in]   src   utf-8 string
 * @param[out]  lenp  pointer to byte-length of the result, may be NULL
 *
 * @pre  dest and src shall not be null pointers.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 *
 * @retval  EOK         on successful operation
 * @retval  ESNULLP     when dest or src is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOSPC     when dmax is too small for the result. *lenp is
 *                      still written, to know how much space is needed.
 * @retval  EILSEQ      when src contains an illegal utf-8 sequence
 *
 * @see
 *    u8u8_s(), u8lwr_s(), u8upr_s(), wcsfc_s(), u8norm_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8fc_s(char8_t *restrict dest, rsize_t dmax,
              const char8_t *restrict src, rsize_t *restrict lenp)
#else
EXPORT errno_t _u8fc_s_chk(char8_t *restrict dest, rsize_t dmax,
                           const char8_t *restrict src, rsize_t *restrict lenp,
                           const size_t destbos)
#endif
{
    const char8_t *s = src;
    char8_t *out = dest;
    rsize_t remaining;
    rsize_t used = 0;

    if (lenp)
        *lenp = 0;
    CHK_DEST_NULL("u8fc_s")
    CHK_DMAX_ZERO("u8fc_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8fc_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8fc_s", destbos)
    }
    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("u8fc_s: src is null", dest,
                                           ESNULLP);
        *dest = '\0';
        return RCNEGATE(ESNULLP);
    }

    /* remaining measures only the bound on how far we may scan src; src
       itself is NUL-terminated and not otherwise length-limited */
    remaining = RSIZE_MAX_STR;
    while (*s) {
        int bytes = u8_seqlen(s, remaining);
        uint32_t cp;
        wchar_t tmp[4];
        int n, i;

        if (!bytes) {
            invoke_safe_str_constraint_handler("u8fc_s: illegal utf-8", dest,
                                               EILSEQ);
            *dest = '\0';
            return RCNEGATE(EILSEQ);
        }
        cp = dec_utf8((char8_t **)&s);
        remaining -= (rsize_t)bytes;

        n = towfc_s(tmp, 4, cp);
        if (n < 0)
            n = 1; /* tmp[0] still holds towlower(cp) */

        for (i = 0; i < n; i++) {
            char8_t encbuf[4];
            int elen = enc_utf8(encbuf, (uint32_t)tmp[i]);
            if (used + (rsize_t)elen < dmax) {
                memcpy(out, encbuf, (size_t)elen);
                out += elen;
            }
            used += (rsize_t)elen;
        }
    }

    if (lenp)
        *lenp = used;

    if (unlikely(used >= dmax)) {
        invoke_safe_str_constraint_handler("u8fc_s: dmax too small", dest,
                                           ESNOSPC);
        *dest = '\0';
        return RCNEGATE(ESNOSPC);
    }
    *out = '\0';
#ifdef SAFECLIB_STR_NULL_SLACK
    memset(out + 1, 0, dmax - used - 1);
#endif

    return (EOK);
}
