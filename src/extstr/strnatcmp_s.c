/*------------------------------------------------------------------
 * strnatcmp.c -- Natural order comparison of two strings
 *
 * Changed February 2018, Reini Urban
 *
 * Copyright (C) 2000, 2004 by Martin Pool <mbp sourcefrog net>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *------------------------------------------------------------------
 */

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def strnatcmp_s(dest,dmax,src,resultp)
 * @brief
 *    Natural order comparison of strings. The longest run of numbers wins
 *    over alpha chars.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest       pointer to string to compare against
 * @param[in]   dmax       restricted maximum length of string dest
 * @param[in]   src        pointer to the string to be compared to dest
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
 * @return  *resultp, when the return code is OK:
 *            >0 when dest greater than src
 *             0 when strings are the same
 *            <0 when dest less than src
 *
 * @retval  EOK        when comparison is complete
 * @retval  ESUNTERM   when dest or src is unterminated, or dmax is too small.
 * @retval  ESNULLP    when dest/src/resultp is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 *
 * @see
 *    strcmp_s()
 */

/* TODO: bounds check */
static int compare_right(char const *a, char const *b) {
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
static int compare_left(char const *a, char const *b) {
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
errno_t strnatcmp_s(const char *dest, rsize_t dmax, const char *src,
                    const int fold_case, int *resultp)
#else
EXPORT errno_t _strnatcmp_s_chk(const char *dest, rsize_t dmax, const char *src,
                                const int fold_case, int *resultp,
                                const size_t destbos, const size_t srcbos)
#endif
{
    size_t ai, bi;
    char ca, cb;
    int fractional;

    CHK_SRC_NULL("strnatcmp_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("strnatcmp_s")
    CHK_SRC_NULL("strnatcmp_s", src)
    CHK_DMAX_ZERO("strnatcmp_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strnatcmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strnatcmp_s", destbos)
    }

    ai = bi = 0;
    while (ai < dmax) {
        ca = dest[ai];
        cb = src[bi];

        /* skip over leading spaces or zeros */
        while (isspace((int)ca))
            ca = dest[++ai];

        while (isspace((int)cb))
            cb = src[++bi];

        /* process run of digits */
        if (isdigit((int)ca) && isdigit((int)cb)) {
            fractional = (ca == '0' || cb == '0');

            if (fractional) {
                if ((*resultp = compare_left(dest + ai, src + bi)) != 0) {
                    return RCNEGATE(EOK);
                }
            } else {
                if ((*resultp = compare_right(dest + ai, src + bi)) != 0)
                    return RCNEGATE(EOK);
            }
        }

        if (!ca && !cb) {
            /* The strings compare the same.  Perhaps the caller
               will want to call strcmp_s to break the tie. */
            *resultp = 0;
            return RCNEGATE(EOK);
        }

        if (fold_case) {
            ca = toupper((int)ca);
            cb = toupper((int)cb);
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
            invoke_safe_str_constraint_handler("strnatcmp_s"
                                               ": src unterminated",
                                               (void *)src, ESUNTERM);
            return RCNEGATE(ESUNTERM);
        }
    }
    return RCNEGATE(EOK);
}

#ifdef __KERNEL__
EXPORT_SYMBOL(_strnatcmp_s_chk);
#endif /* __KERNEL__ */
