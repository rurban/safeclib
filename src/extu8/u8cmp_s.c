/*------------------------------------------------------------------
 * u8cmp_s.c
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

/* Normalize src (NUL-terminated, up to inlen bytes) to NFC into a
   scratch buffer (stack if it fits, else heap), sized generously so
   normalization (which can grow the string) always has room. On
   success outp/outlenp describe the normalized, NUL-terminated
   result and the caller must free() it if it is not stackbuf. */
static errno_t u8cmp_normalize(const char8_t *src, rsize_t inlen,
                               char8_t *stackbuf, rsize_t stackcap,
                               char8_t **outp, rsize_t *outlenp) {
    rsize_t cap = (inlen + 1) * 4;
    char8_t *buf;
    errno_t rc;

    if (cap < 16)
        cap = 16;
    if (cap <= stackcap) {
        buf = stackbuf;
    } else {
        buf = (char8_t *)malloc(cap);
        if (!buf)
            return ENOMEM;
    }

    rc = u8norm_s(buf, cap, src, WCSNORM_NFC, outlenp);
    if (rc != EOK) {
        if (buf != stackbuf)
            free(buf);
        return rc;
    }
    *outp = buf;
    return EOK;
}

/**
 * @def u8cmp_s(dest,dmax,src,resultp)
 * @brief
 *    Compares utf-8 string src to utf-8 string dest, after
 *    normalizing both to NFC. This means two strings that are
 *    canonically equivalent but use different Unicode
 *    representations (e.g. a precomposed accented codepoint vs. the
 *    base letter followed by a combining mark) compare equal.
 *
 * @param[in]   dest       pointer to utf-8 string to compare against
 * @param[in]   dmax       restricted maximum byte-length of string dest
 * @param[in]   src        pointer to the utf-8 string to be compared to dest
 * @param[out]  resultp    pointer to int result, greater than 0,
 *                         equal to 0 or less than 0, if the string pointed
 *                         to by dest is greater than, equal to or less
 *                         than the string pointed to by src respectively,
 *                         after NFC normalization.
 *
 * @pre   Neither dest, src nor resultp shall be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @return  The error code of the result. On EOK, see resultp.
 * @retval  EOK        when comparison is complete and the result is returned
 *                     in resultp
 * @retval  ESUNTERM   when src is unterminated
 * @retval  ESNULLP    when dest, src or resultp is the NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ENOMEM     when a normalization scratch buffer could not be
 *                     allocated
 * @retval  *          any error returned by u8norm_s() while normalizing
 *                     dest or src (e.g. EILSEQ for malformed utf-8)
 *
 * @see
 *    u8icmp_s(), u8fccmp_s(), u8norm_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8cmp_s(const char8_t *dest, rsize_t dmax, const char8_t *src,
                int *resultp)
#else
EXPORT errno_t _u8cmp_s_chk(const char8_t *dest, rsize_t dmax,
                            const char8_t *src, int *resultp,
                            const size_t destbos, size_t srcbos)
#endif
{
    char8_t deststack[128], srcstack[128];
    char8_t *normdest = NULL, *normsrc = NULL;
    rsize_t destlen, srclen, normdestlen, normsrclen;
    const char8_t *p;
    errno_t rc;

    CHK_SRC_NULL("u8cmp_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("u8cmp_s")
    CHK_SRC_NULL("u8cmp_s", src)
    CHK_DMAX_ZERO("u8cmp_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8cmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8cmp_s", destbos)
    }

    /* measure dest's actual content within dmax */
    destlen = 0;
    p = dest;
    while (*p && destlen < dmax) {
        p++;
        destlen++;
    }

    /* measure src, enforcing the srcbos/RSIZE_MAX_STR termination bound
       like strcmp_s does */
    srclen = 0;
    p = src;
    while (*p) {
        p++;
        srclen++;
        if (unlikely(srclen >= srcbos)) {
            invoke_safe_str_constraint_handler("u8cmp_s"
                                               ": src unterminated",
                                               (void *)src, ESUNTERM);
            return RCNEGATE(ESUNTERM);
        }
    }

    rc = u8cmp_normalize(dest, destlen, deststack, sizeof(deststack), &normdest,
                         &normdestlen);
    if (rc != EOK)
        return RCNEGATE(rc);

    rc = u8cmp_normalize(src, srclen, srcstack, sizeof(srcstack), &normsrc,
                         &normsrclen);
    if (rc != EOK) {
        if (normdest != deststack)
            free(normdest);
        return RCNEGATE(rc);
    }

    *resultp = strcmp((const char *)normdest, (const char *)normsrc);

    if (normdest != deststack)
        free(normdest);
    if (normsrc != srcstack)
        free(normsrc);

    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_u8cmp_s_chk);
#endif /* __KERNEL__ */
