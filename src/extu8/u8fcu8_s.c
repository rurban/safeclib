/*------------------------------------------------------------------
 * u8fcu8_s.c
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
#endif
#include "u8_private.h"

/* Walks a utf-8 string one fully-folded codepoint at a time (the same
   towfc_s()/CaseFolding.txt full case-folding used by u8fccmp_s()).
   ESNOTFND from u8fcu8_next() marks the terminating NUL / end of the
   'remaining' byte budget, i.e. "pattern/haystack exhausted", not an
   error. */
typedef struct {
    const char8_t *str;
    rsize_t remaining;
    uint32_t folded[4];
    unsigned int length;
    unsigned int position;
} u8fcu8_iter_t;

static errno_t u8fcu8_next(u8fcu8_iter_t *iter, uint32_t *codepoint) {
    while (iter->position == iter->length) {
        const char8_t *next;
        uint32_t cp;
        int folded;
        int bytes;

        if (!iter->remaining || !*iter->str)
            return ESNOTFND;

        bytes = u8_seqlen(iter->str, iter->remaining);
        if (!bytes) {
            invoke_safe_str_constraint_handler("u8fcu8_s: invalid utf-8",
                                               (void *)iter->str, EILSEQ);
            return EILSEQ;
        }
        next = iter->str;
        cp = dec_utf8((char8_t **)&next);
        iter->str = next;
        iter->remaining -= (rsize_t)bytes;

        folded = _u8_towfc(iter->folded, cp);
        if (folded < 0) {
            iter->folded[0] = cp;
            iter->length = 1;
        } else {
            iter->length = (unsigned int)folded;
        }
        iter->position = 0;
    }

    *codepoint = iter->folded[iter->position++];
    return EOK;
}

/**
 * @def u8fcu8_s(dest,dmax,src,slen,substring)
 * @brief
 *    Locates the first occurrence of the utf-8 substring src in the
 *    utf-8 string dest, comparing codepoint by codepoint after full
 *    Unicode case-folding (the same \c towfc_s() CaseFolding.txt data
 *    used by \c u8fccmp_s() and \c u8u8_s()), so the match is
 *    case-insensitive.
 * @details
 *    This does NOT perform NFC/NFD canonicalization: a substring that
 *    is only canonically equivalent to, but not fold-casewise
 *    codepoint-identical with, a run of codepoints in dest (e.g. a
 *    precomposed accented letter in one operand against the
 *    decomposed base-letter-plus-combining-mark form in the other) is
 *    not matched. Normalize both operands with \c u8norm_s() first
 *    (\c WCSNORM_NFC) if that guarantee is required.
 *
 * @param[in]   dest       pointer to utf-8 string to be searched for the
 *                         substring
 * @param[in]   dmax       restricted maximum byte-length of dest string
 * @param[in]   src        pointer to the utf-8 sub string
 * @param[in]   slen       the maximum number of bytes to use from src
 * @param[out]  substring  the returned substring pointer, into dest
 *
 * @pre  Neither dest, src nor substring shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  slen shall not be 0.
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_STR and size of
 * dest/src.
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dest/src/substring is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  EILSEQ     when dest/src contains illegal utf-8
 * @retval  ESNOTFND   when substring not found
 *
 * @see
 *    u8u8_s(), u8fccmp_s(), u8ifcu8_s(), u8norm_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8fcu8_s(char8_t *dest, rsize_t dmax, const char8_t *src, rsize_t slen,
                 char8_t **substring)
#else
EXPORT errno_t _u8fcu8_s_chk(char8_t *dest, rsize_t dmax, const char8_t *src,
                             rsize_t slen, char8_t **substring,
                             const size_t destbos, const size_t srcbos)
#endif
{
    const char8_t *scan;
    rsize_t remaining;

    CHK_SRC_NULL("u8fcu8_s", substring)
    *substring = NULL;

    CHK_DEST_NULL("u8fcu8_s")
    CHK_SRC_NULL("u8fcu8_s", src)
    CHK_DMAX_ZERO("u8fcu8_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8fcu8_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8fcu8_s", destbos)
    }

    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("u8fcu8_s: slen is 0", (void *)src,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }
    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("u8fcu8_s: slen exceeds max",
                                               (void *)src, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
        BND_CHK_PTR_BOUNDS(src, slen);
    } else if (unlikely(slen > srcbos)) {
        invoke_safe_str_constraint_handler("u8fcu8_s: slen exceeds src",
                                           (void *)src, EOVERFLOW);
        return RCNEGATE(EOVERFLOW);
    }

    scan = dest;
    remaining = dmax;

    while (remaining && *scan) {
        u8fcu8_iter_t d_iter, s_iter;
        int matched = 1;
        int bytes;

        d_iter.str = scan;
        d_iter.remaining = remaining;
        d_iter.length = d_iter.position = 0;

        s_iter.str = src;
        s_iter.remaining = slen;
        s_iter.length = s_iter.position = 0;

        for (;;) {
            uint32_t scp, dcp;
            errno_t s_rc = u8fcu8_next(&s_iter, &scp);
            errno_t d_rc;

            if (s_rc == ESNOTFND) {
                /* pattern fully consumed: match at scan */
                break;
            }
            if (unlikely(s_rc != EOK))
                return RCNEGATE(s_rc);

            d_rc = u8fcu8_next(&d_iter, &dcp);
            if (d_rc == ESNOTFND) {
                matched = 0;
                break;
            }
            if (unlikely(d_rc != EOK))
                return RCNEGATE(d_rc);

            if (dcp != scp) {
                matched = 0;
                break;
            }
        }

        if (matched) {
            *substring = (char8_t *)scan;
            return RCNEGATE(EOK);
        }

        /* advance the search origin by exactly one codepoint */
        bytes = u8_seqlen(scan, remaining);
        if (!bytes) {
            invoke_safe_str_constraint_handler("u8fcu8_s: invalid utf-8",
                                               (void *)scan, EILSEQ);
            return RCNEGATE(EILSEQ);
        }
        scan += bytes;
        remaining -= (rsize_t)bytes;
    }

    *substring = NULL;
    return RCNEGATE(ESNOTFND);
}
