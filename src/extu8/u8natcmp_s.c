/*------------------------------------------------------------------
 * u8natcmp_s.c -- Natural order comparison of two utf-8 strings
 *
 * September 2026, Reini Urban
 *
 * Port of Martin Pool's strnatcmp.c (2000, 2004) to the safeclib
 * utf-8 API. Digits are compared as numbers; the longest run of
 * digits wins over alpha chars.
 *
 * Copyright (C) 2000, 2004 by Martin Pool <mbp sourcefrog net>
 * Copyright (c) 2026 by Reini Urban
 *------------------------------------------------------------------
 */

#ifdef FOR_DOXYGEN
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#include <ctype.h>
#endif

/**
 * @def u8natcmp_s(dest,dmax,src,resultp)
 * @def u8natfccmp_s(dest,dmax,src,resultp)
 * @brief
 *    Natural order comparison of utf-8 strings. The longest run of
 *    numbers wins over alpha chars. u8natfccmp_s() additionally
 *    folds ASCII case before comparing.
 *
 * @param[in]   dest       pointer to utf-8 string to compare against
 * @param[in]   dmax       restricted maximum byte-length of string dest
 * @param[in]   src        pointer to the utf-8 string to be compared to dest
 * @param[in]   fold_case  fold ASCII case before comparing (1/0)
 * @param[out]  resultp    pointer to int result, greater than 0,
 *                         equal to 0 or less than 0, if the string pointed
 *                         to by dest is greater than, equal to or less
 *                         than the string pointed to by src respectively.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   resultp shall not be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR or size of dest
 *
 * @return  The error code of the result. On EOK, see resultp.
 * @retval  EOK        when comparison is complete and the result is returned
 *                     in resultp
 * @retval  ESNULLP    when dest, src or resultp is the NULL pointer
 * @retval  ESUNTERM   when src is unterminated
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 *
 * @see
 *    u8cmp_s(), strnatcmp_s()
 */

/* TODO: bounds check */
static int u8_compare_right(char const *a, char const *b) {
    int bias = 0;

    /* The longest run of digits wins.  That aside, the greatest
       value wins, but we can't know that it will until we've scanned
       both numbers to know that they have the same magnitude, so we
       remember it in BIAS. */
    for (;; a++, b++) {
        if (!isdigit((int)*a) && !isdigit((int)*b))
            return bias;
        if (!isdigit((int)*a))
            return -1;
        if (!isdigit((int)*b))
            return +1;
        if (*a < *b) {
            if (!bias)
                bias = -1;
        } else if (*a > *b) {
            if (!bias)
                bias = +1;
        } else if (!*a && !*b)
            return bias;
    }

    return 0;
}

/* TODO: bounds check */
static int u8_compare_left(char const *a, char const *b) {
    /* Compare two left-aligned numbers: the first to have a
       different value wins. */
    for (;; a++, b++) {
        if (!isdigit((int)*a) && !isdigit((int)*b))
            return 0;
        if (!isdigit((int)*a))
            return -1;
        if (!isdigit((int)*b))
            return +1;
        if (*a < *b)
            return -1;
        if (*a > *b)
            return +1;
    }
    return 0;
}

#ifdef FOR_DOXYGEN
errno_t u8natcmp_s(const char8_t *dest, rsize_t dmax, const char8_t *src,
                   const int fold_case, int *resultp)
#else
EXPORT errno_t _u8natcmp_s_chk(const char8_t *dest, rsize_t dmax,
                               const char8_t *src, const int fold_case,
                               int *resultp, const size_t destbos,
                               const size_t srcbos)
#endif
{
    size_t ai, bi;
    char ca, cb;
    int fractional;

    CHK_SRC_NULL("u8natcmp_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("u8natcmp_s")
    CHK_SRC_NULL("u8natcmp_s", src)
    CHK_DMAX_ZERO("u8natcmp_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8natcmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8natcmp_s", destbos)
    }

    ai = bi = 0;
    while (ai < dmax) {
        ca = (char)dest[ai];
        cb = (char)src[bi];

        /* skip over leading spaces or zeros */
        while (isspace((int)(unsigned char)ca))
            ca = (char)dest[++ai];

        while (isspace((int)(unsigned char)cb))
            cb = (char)src[++bi];

        /* process run of digits */
        if (isdigit((int)(unsigned char)ca) &&
            isdigit((int)(unsigned char)cb)) {
            fractional = (ca == '0' || cb == '0');

            if (fractional) {
                if ((*resultp = u8_compare_left((const char *)dest + ai,
                                                (const char *)src + bi)) != 0) {
                    return RCNEGATE(EOK);
                }
            } else {
                if ((*resultp = u8_compare_right((const char *)dest + ai,
                                                 (const char *)src + bi)) != 0)
                    return RCNEGATE(EOK);
            }
        }

        if (!ca && !cb) {
            /* The strings compare the same.  Perhaps the caller
               will want to call u8cmp_s to break the tie. */
            *resultp = 0;
            return RCNEGATE(EOK);
        }

        if (fold_case) {
            ca = (char)toupper((int)(unsigned char)ca);
            cb = (char)toupper((int)(unsigned char)cb);
        }

        if (ca < cb) {
            *resultp = -1;
            return RCNEGATE(EOK);
        }

        if (ca > cb) {
            *resultp = 1;
            return RCNEGATE(EOK);
        }
        ++ai;
        ++bi;

        /* sentinel srcbos -1 = ULONG_MAX */
        if (unlikely(bi >= srcbos)) {
            invoke_safe_str_constraint_handler("u8natcmp_s"
                                               ": src unterminated",
                                               (void *)src, ESUNTERM);
            return RCNEGATE(ESUNTERM);
        }
    }
    return RCNEGATE(EOK);
}

#ifdef __KERNEL__
EXPORT_SYMBOL(_u8natcmp_s_chk);
#endif /* __KERNEL__ */
