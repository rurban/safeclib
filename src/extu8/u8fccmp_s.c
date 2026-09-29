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
    wchar_t folded[4];
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

        folded = towfc_s(iter->folded, 4, cp);
        if (folded < 0) {
            iter->folded[0] = (wchar_t)cp;
            iter->length = 1;
        } else {
            iter->length = (unsigned int)folded;
        }
        iter->position = 0;
    }

    *codepoint = (uint32_t)iter->folded[iter->position++];
    return EOK;
}

#ifdef FOR_DOXYGEN
errno_t u8fccmp_s(const char8_t *dest, rsize_t dmax, const char8_t *src,
                  int *resultp)
#else
EXPORT errno_t _u8fccmp_s_chk(const char8_t *dest, rsize_t dmax,
                              const char8_t *src, int *resultp,
                              const size_t destbos)
#endif
{
    u8fc_iter_t left = { dest, dmax, { 0, 0, 0, 0 }, 0, 0 };
    u8fc_iter_t right = { src, RSIZE_MAX_STR, { 0, 0, 0, 0 }, 0, 0 };
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

    for (;;) {
        uint32_t lcp, rcp;
        errno_t lrc = u8fc_next(&left, &lcp);
        errno_t rrc = u8fc_next(&right, &rcp);

        if (lrc == ESNOTFND || rrc == ESNOTFND) {
            *resultp = lrc == rrc ? 0 : lrc == ESNOTFND ? -1 : 1;
            return RCNEGATE(EOK);
        }
        if (unlikely(lrc != EOK))
            return RCNEGATE(lrc);
        if (unlikely(rrc != EOK))
            return RCNEGATE(rrc);
        if (lcp < rcp) {
            *resultp = -1;
            return RCNEGATE(EOK);
        }
        if (lcp > rcp) {
            *resultp = 1;
            return RCNEGATE(EOK);
        }
    }
}
