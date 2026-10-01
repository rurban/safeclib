/*------------------------------------------------------------------
 * u8fccmp_s.c
 *
 * September 2026, Reini Urban
 *
 * Copyright (c) 2026 by Reini Urban
 * All rights reserved.
 *------------------------------------------------------------------
 */

#ifdef FOR_DOXYGEN
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#endif
#include "u8_private.h"

typedef struct {
    const char8_t *str;
    rsize_t remaining;
    uint32_t folded[4];
    unsigned int length;
    unsigned int position;
} u8fc_iter_t;

/* Return one full-folded codepoint. ESNOTFND marks the terminating NUL. */
static errno_t u8fc_next(u8fc_iter_t *iter, uint32_t *codepoint) {
    while (iter->position == iter->length) {
        const char8_t *next;
        uint32_t cp;
        int folded;
        int bytes;

        if (!iter->remaining || !*iter->str)
            return ESNOTFND;

        bytes = u8_seqlen(iter->str, iter->remaining);
        if (!bytes) {
            invoke_safe_str_constraint_handler("u8fccmp_s: invalid utf-8",
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

/* Normalize src to NFC into a scratch buffer (stack if it fits, else
   heap), sized generously so normalization always has room. */
static errno_t u8fccmp_normalize(const char8_t *src, rsize_t inlen,
                                 char8_t *stackbuf, rsize_t stackcap,
                                 char8_t **outp) {
    rsize_t cap = (inlen + 1) * 4;
    char8_t *buf;
    rsize_t outlen;
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

    rc = u8norm_s(buf, cap, src, WCSNORM_NFC, &outlen);
    if (rc != EOK) {
        if (buf != stackbuf)
            free(buf);
        return rc;
    }
    *outp = buf;
    return EOK;
}

/**
 * @def u8fccmp_s(dest,dmax,src,resultp)
 * @brief
 *    Compares utf-8 string src to utf-8 string dest fold-cased, after
 *    normalizing both to NFC. Full Unicode case-folding is applied
 *    (via \c towfc_s(), the same CaseFolding.txt data used by
 *    \c u8u8_s()) codepoint by codepoint, so this also matches
 *    canonically equivalent strings that use different, case-varying
 *    Unicode representations.
 *
 * @param[in]   dest       pointer to utf-8 string to compare against
 * @param[in]   dmax       restricted maximum byte-length of string dest
 * @param[in]   src        pointer to the utf-8 string to be compared to dest
 * @param[out]  resultp    pointer to int result, greater than 0,
 *                         equal to 0 or less than 0, if the fold-cased,
 *                         normalized dest is greater than, equal to or
 *                         less than the fold-cased, normalized src.
 *
 * @pre   Neither dest, src nor resultp shall be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @retval  EOK        when comparison is complete and the result is returned
 *                     in resultp
 * @retval  ESNULLP    when dest/src/resultp is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  EILSEQ     when dest/src contains illegal utf-8
 * @retval  ENOMEM     when a normalization scratch buffer could not be
 *                     allocated
 * @retval  *          any error returned by u8norm_s() while normalizing
 *
 * @see
 *    u8cmp_s(), u8u8_s(), u8norm_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8fccmp_s(const char8_t *dest, rsize_t dmax, const char8_t *src,
                  int *resultp)
#else
EXPORT errno_t _u8fccmp_s_chk(const char8_t *dest, rsize_t dmax,
                              const char8_t *src, int *resultp,
                              const size_t destbos)
#endif
{
    char8_t deststack[128], srcstack[128];
    char8_t *normdest = NULL, *normsrc = NULL;
    rsize_t destlen;
    const char8_t *p;
    errno_t rc;
    u8fc_iter_t left, right;

    CHK_SRC_NULL("u8fccmp_s", resultp)
    *resultp = 0;
    CHK_DEST_NULL("u8fccmp_s")
    CHK_SRC_NULL("u8fccmp_s", src)
    CHK_DMAX_ZERO("u8fccmp_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8fccmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8fccmp_s", destbos)
    }

    destlen = 0;
    p = dest;
    while (*p && destlen < dmax) {
        p++;
        destlen++;
    }

    rc = u8fccmp_normalize(dest, destlen, deststack, sizeof(deststack),
                           &normdest);
    if (rc != EOK)
        return RCNEGATE(rc);

    rc = u8fccmp_normalize(src, strlen((const char *)src), srcstack,
                           sizeof(srcstack), &normsrc);
    if (rc != EOK) {
        if (normdest != deststack)
            free(normdest);
        return RCNEGATE(rc);
    }

    left.str = normdest;
    left.remaining = RSIZE_MAX_STR;
    left.length = left.position = 0;
    right.str = normsrc;
    right.remaining = RSIZE_MAX_STR;
    right.length = right.position = 0;

    for (;;) {
        uint32_t lcp, rcp;
        errno_t lrc = u8fc_next(&left, &lcp);
        errno_t rrc = u8fc_next(&right, &rcp);

        if (lrc == ESNOTFND || rrc == ESNOTFND) {
            *resultp = lrc == rrc ? 0 : lrc == ESNOTFND ? -1 : 1;
            rc = EOK;
            break;
        }
        if (unlikely(lrc != EOK)) {
            rc = lrc;
            break;
        }
        if (unlikely(rrc != EOK)) {
            rc = rrc;
            break;
        }
        if (lcp < rcp) {
            *resultp = -1;
            rc = EOK;
            break;
        }
        if (lcp > rcp) {
            *resultp = 1;
            rc = EOK;
            break;
        }
    }

    if (normdest != deststack)
        free(normdest);
    if (normsrc != srcstack)
        free(normsrc);

    return RCNEGATE(rc);
}
