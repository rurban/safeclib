/*------------------------------------------------------------------
 * wcsnatcmp.c -- Natural order comparison of two strings
 *
 * Changed February 2018, Reini Urban
 *
 * Copyright (C) 2000, 2004 by Martin Pool <mbp sourcefrog net>
 * Copyright (C) 2018 by Reini Urban
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
#include <wctype.h>
#endif

#ifndef HAVE_ISWDIGIT
#define iswdigit(ch) isdigit((int)(ch))
#endif
#ifndef HAVE_ISWSPACE
#define iswspace(ch) isspace((int)(ch))
#endif

/**
 * @def wcsnatcmp_s(dest,dmax,src,smax,resultp)
 * @def wcsnaticmp_s(dest,dmax,src,smax,resultp)
 * @brief
 *    Natural order comparison of strings. The longest run of numbers wins
 *    over alpha chars.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest       pointer to wide string to compare against
 * @param[in]   dmax       restricted maximum length of wide string dest
 * @param[in]   src        pointer to the wide string to be compared to dest
 * @param[in]   smax       restricted maximum length of wide string src
 * @param[out]  resultp    pointer to int result, greater than 0,
 *                         equal to 0 or less than 0, if the string pointed
 *                         to by dest is greater than, equal to or less
 *                         than the string pointed to by src respectively.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   resultp shall not be a null pointer.
 * @pre   dmax/smax shall not be 0
 * @pre   dmax/smax shall not be greater than RSIZE_MAX_WSTR and size of
 * dest/src
 *
 * @return  resultp (when the return code is OK)
 *            >0 when dest greater than src
 *             0 when strings are the same
 *            <0 when dest less than src
 *
 * @retval  EOK        when comparison is complete
 * @retval  ESUNTERM   when dest or src is unterminated, or dmax is too small.
 * @retval  ESNULLP    when dest/src/resultp is NULL pointer
 * @retval  ESZEROL    when dmax/smax = 0
 * @retval  ESLEMAX    when dmax/smax > RSIZE_MAX_WSTR
 * @retval  EOVERFLOW  when dmax/smax > size of dest/src (optionally, when the
 *                     compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 *
 * @see
 *    strnatcmp_s() wcscmp_s()
 */

/* TODO: bounds check */
static int compare_right(wchar_t const *a, wchar_t const *b) {
    int bias = 0;

    /* The longest run of digits wins.  That aside, the greatest
       value wins, but we can't know that it will until we've scanned
       both numbers to know that they have the same magnitude, so we
       remember it in BIAS. */
    for (;; a++, b++) {
        if (!iswdigit((wint_t)*a) && !iswdigit((wint_t)*b))
            return bias;
        if (!iswdigit((wint_t)*a))
            return -1;
        if (!iswdigit((wint_t)*b))
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
static int compare_left(wchar_t const *a, wchar_t const *b) {
    /* Compare two left-aligned numbers: the first to have a
       different value wins. */
    for (;; a++, b++) {
        if (!iswdigit((wint_t)*a) && !iswdigit((wint_t)*b))
            return 0;
        if (!iswdigit((wint_t)*a))
            return -1;
        if (!iswdigit((wint_t)*b))
            return +1;
        if (*a < *b)
            return -1;
        if (*a > *b)
            return +1;
    }
    return 0;
}

#ifdef FOR_DOXYGEN
errno_t wcsnaticmp_s(const wchar_t *dest, rsize_t dmax,
                    const wchar_t *src, rsize_t smax,
                     const int fold_case, int *resultp){}
errno_t wcsnatcmp_s(const wchar_t *dest, rsize_t dmax,
                    const wchar_t *src, rsize_t smax,
                    const int fold_case, int *resultp)
#else
EXPORT errno_t _wcsnatcmp_s_chk(const wchar_t *dest, rsize_t dmax,
                                const wchar_t *src, rsize_t smax,
                                const int fold_case, int *resultp,
                                const size_t destbos, const size_t srcbos)
#endif
{
    wchar_t *d1 = NULL, *d2 = NULL;
    size_t ai, bi;
    const size_t destsz = dmax * sizeof(wchar_t);
    const size_t srcsz = smax * sizeof(wchar_t);

    CHK_SRC_NULL("wcsnatcmp_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("wcsnatcmp_s")
    CHK_SRC_NULL("wcsnatcmp_s", src)
    CHK_DMAX_ZERO("wcsnatcmp_s")
    if (unlikely(dmax == 0 || smax == 0)) {
        invoke_safe_str_constraint_handler("wcsnatcmp_s: dmax/smax is 0",
                                           (void *)dest, ESZEROL);
        return RCNEGATE(ESZEROL);
    }
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("wcsnatcmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, destsz);
    } else {
        CHK_DESTW_OVR("wcsnatcmp_s", destsz, destbos)
    }
    if (unlikely(smax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcsnatcmp_s"
                                           ": smax exceeds max",
                                           (void *)src, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, srcsz);
    } else {
        if (unlikely(srcsz > srcbos)) {
            invoke_safe_str_constraint_handler("wcsnatcmp_s"
                                               ": smax exceeds src",
                                               (void *)src, EOVERFLOW);
            return RCNEGATE(EOVERFLOW);
        }
    }

    if (fold_case) {
        rsize_t l1, l2;
        errno_t rc;

        d1 = (wchar_t *)malloc(2 * destsz);
        rc = wcsfc_s(d1, dmax * 2, (wchar_t * restrict) dest, &l1);
        if (rc != EOK) {
            free(d1);
            return rc;
        }

        d2 = (wchar_t *)malloc(2 * srcsz);
        rc = wcsfc_s(d2, smax * 2, (wchar_t * restrict) src, &l2);
        if (rc != EOK) {
            free(d1);
            free(d2);
            return rc;
        }
        dest = d1;
        src = d2;
        dmax = l1;
        smax = l2;
    }

    ai = bi = 0;
    while (ai < dmax) {
        int fractional;
        wchar_t ca = dest[ai];
        wchar_t cb = src[bi];

        /* skip over leading spaces or zeros */
        while (iswspace((wint_t)ca))
            ca = dest[++ai];

        while (iswspace((wint_t)cb))
            cb = src[++bi];

        /* process run of digits */
        if (iswdigit((wint_t)ca) && iswdigit((wint_t)cb)) {
            fractional = (ca == L'0' || cb == L'0');

            if (fractional) {
                if ((*resultp = compare_left(dest + ai, src + bi)) != 0) {
                    goto eok;
                }
            } else if ((*resultp = compare_right(dest + ai, src + bi)) != 0) {
                goto eok;
            }
        }

        if (!ca && !cb) {
            /* The strings compare the same.  Perhaps the caller
               will want to call wcscmp_s to break the tie. */
            *resultp = 0;
            goto eok;
        }

        if (ca < cb) {
            *resultp = -1;
            goto eok;
        }

        if (ca > cb) {
            *resultp = 1;
            goto eok;
        }
        ++ai;
        ++bi;

        if (unlikely(bi >= smax)) {
            invoke_safe_str_constraint_handler("wcsnatcmp_s"
                                               ": src unterminated",
                                               (void *)src, ESUNTERM);
            if (fold_case) {
                free(d1);
                free(d2);
            }
            return RCNEGATE(ESUNTERM);
        }
    }
eok:
    if (fold_case) {
        free(d1);
        free(d2);
    }
    return RCNEGATE(EOK);
}

#ifdef __KERNEL__
EXPORT_SYMBOL(_wcsnatcmp_s_chk);
#endif /* __KERNEL__ */
