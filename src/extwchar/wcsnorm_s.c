/*------------------------------------------------------------------
 * wcsnorm_s.c
 *
 * September 2017, Reini Urban
 *
 * Copyright (c) 2017 by Reini Urban
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

#include "safeclib_private.h"
#include <wctype.h>
#include <assert.h>

/* generated via cperl Unicode-Normalize/mkheader -uni */
#include "unwifcan.h" /* NFD */
#ifdef HAVE_NFC
#include "unwiexc.h"
#include "unwicmb.h"
#include "unwicmp.h"
#endif

/* generated via cperl regen/regcharclass-safec.pl (via CharClass::Matcher) */
/* 1963 single mark characters (Combining, Overlay, ...) */
#include "mark.h"
/* Korean/Hangul has special (easy) normalization rules */
#include "hangul.h"
/* If it starts with a composed or decomposed codepoint. */
#include "composed.h"

#define _UNICODE_MAX 0x10ffff

int _decomp_s(wchar_t *restrict dest, rsize_t dmax, const wint_t cp);

/**
 * @brief
 *    Converts the wide string to the canonical NFD normalization (later NFC),
 *    as defined in the latest Unicode standard, latest 10.0.  The conversion
 *    stops at the first null or after dmax characters.
 *    Currently only works with 4-byte wchar's. So not on cygwin, windows,
 *    and not on solaris, aix with 32bit.
 * 
 * @details
 *    Decomposed characters are checked for the right-hand-side of the
 *    Decomposition_Mapping Unicode property, which means the codepoint will be
 *    normalized if the sequence is decomposed (NFD or NFKD).
 *    This is equivalent to all 1963 combining mark characters,
 *    plus some remaining 869 non-mark and non-hangul normalizables.
 *    Hangul has some special normalization logic.
 *
 * @param[out]  dest  wide string to hold the result
 * @param[in]   dmax  maximum length of string
 * @param[out]  src   wide string
 *
 * @pre  dest and src shall not be null pointers.
 * @pre  dmax shall not equal zero.
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR.
 *
 * @retval  EOK         on successful operation
 * @retval  ESNULLP     when dest or src is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_WSTR
 *
 * @see
 *    wcsfc_s()
 */

/* Assumes 4-byte wchar, and cp > 0xff */
EXPORT int
isw_maybe_composed(const wint_t cp) {
    /* TODO: 340 => 300 is not found, is composed.h wrong? No, it's not */
    return (Hangul_IsS(cp) || is_COMPOSED_cp_high(cp))
        ? 1 : 0;
}

/* Assumes 4-byte wchar, and cp > 0xff */
EXPORT int
isw_maybe_decomposed(const wint_t cp) {
    return (is_MARK_cp_high(cp) || Hangul_IsS(cp) || is_DECOMPOSED_REST_cp_high(cp))
        ? 1 : 0;
}

/* Check for the left-hand-side of the Decomposition_Mapping property,
 * which means the codepoint can be normalized and decomposed.
 * Assumes 4-byte wchar.
 */
EXPORT int
iswcomposed(const wchar_t *src, rsize_t smax) {
    (void)smax;
    if (*src > 0xff && isw_maybe_composed(*src)) {
        wchar_t dest[5]; /* with non-canonical must be 19 */
        int c = _decomp_s(dest, 5, *src);
        return c > 1 ? 1 : 0;
    } else 
        return 0;
}

/* Check for the right-hand-side of the Decomposition_Mapping property,
 * which means the codepoint can be normalized and composed.
 * Assumes 4-byte wchar. TODO.
 */
EXPORT int
iswdecomposed(const wchar_t *src, rsize_t smax) {
    (void)smax;
    if (*src > 0xff && isw_maybe_decomposed(*src)) {
        wchar_t dest[5]; /* with non-canonical must be 19 */
        int c = _decomp_s((wchar_t *)dest, 5, *src);
        return c > 1 ? 1 : 0;
    } else 
        return 0;
}

/* Note that we can generate two versions of the tables.  The old format as
 * used in Unicode::Normalize, and the new smaller NORMALIZE_IND_TBL cperl
 * variant, as used here and in cperl core since 5.27.2 (planned).
 */
static int
_decomp_canonical_s(wchar_t *dest, rsize_t dmax, wint_t cp)
{
#ifndef NORMALIZE_IND_TBL
    if (unlikely(_UNICODE_MAX < cp)) {
        invoke_safe_str_constraint_handler("_decomp_canonical_s: "
                   "cp is too high",
                   NULL, ESLEMAX);
        *dest = 0;
	return 0;
    } else {
        const wchar_t ***plane = UNWIF_canon[cp >> 16];
        if (! plane) {
            return 0;
        } else {
            const wchar_t **row = plane[(cp >> 8) & 0xff];
            if (row) {
                const wchar_t *s = row[cp & 0xff];
                if (s) {
                    const int c = wcslen(row[cp & 0xff]);
                    wmemcpy_s(dest, dmax, row[cp & 0xff], c);
                    return c;
                } else {
                    *dest = 0;
                    return 0;
                }
            }
            else {
                *dest = 0;
                return 0;
            }
        }
    }
#else
    /* the new format generated with Unicode-Normalize/mkheader -uni -ind */
    const UNWIF_canon_PLANE_T **plane, *row;
    if (unlikely(_UNICODE_MAX < cp)) {
        invoke_safe_str_constraint_handler("_decomp_canonical_s: "
                   "cp is too high",
                   NULL, ESLEMAX);
        *dest = 0;
	return 0;
    }
    if (unlikely(dmax < 5)) {
        invoke_safe_str_constraint_handler("_decomp_canonical_s: "
                   "dmax is < 5",
                   NULL, ESLEMIN);
        *dest = 0;
	return 0;
    }
    plane = UNWIF_canon[cp >> 16];
    if (!plane) { /* Only the first 3 of 16 are filled */
	return 0;
    }
    row = plane[(cp >> 8) & 0xff];
    if (row) { /* the first row is pretty filled, the rest very sparse */
        const UNWIF_canon_PLANE_T vi = row[cp & 0xff];
        if (!vi)
            return 0;
        else {
            /* value => length and index */
            const int l = UNWIF_canon_LEN(vi);
            const int i = UNWIF_canon_IDX(vi);
            const wchar_t* tbl = (const wchar_t*)UNWIF_canon_tbl[l-1];
#if 0 && defined(DEBUG)
            printf("U+%04X vi=0x%x (>>12, &&fff) => %d|TBL(%d)\n", cp, vi, i, l);
#endif
            assert(l>0 && l<=4);
            /* (917,762,227,36) */
            assert((l==1 && i<917) || (l==2 && i<762) || (l==3 && i<227) ||
                   (l==4 && i<36) || 0);
            assert(dmax > 4);
            memcpy(dest, &tbl[i*l], l*sizeof(wchar_t));
            dest[l] = L'\0';
            return l;
        }
    }
    else {
        return 0;
    }
#endif
}

static int
_decomp_hangul_s(wchar_t *dest, rsize_t dmax, wint_t cp)
{
    wint_t sindex =  cp - Hangul_SBase;
    wint_t lindex =  sindex / Hangul_NCount;
    wint_t vindex = (sindex % Hangul_NCount) / Hangul_TCount;
    wint_t tindex =  sindex % Hangul_TCount;
    
    if (unlikely(dmax < 4)) {
        invoke_safe_str_constraint_handler("wcsfc_s: "
                   "dmax is < 4",
                   NULL, ESNOSPC);
        return (ESNOSPC);
    }

    dest[0] = lindex + Hangul_LBase;
    dest[1] = vindex + Hangul_VBase;
    if (tindex) {
	dest[2] = tindex + Hangul_TBase;
        dest[3] = 0;
        return 3;
    } else {
        dest[2] = 0;
        return 2;
    }
}


/* codepoint canonical decomposition.
   dmax should be > 4,
   19 with the single arabic outlier U+FDFA for compat accepted,
   but we only do canon here.
*/

EXPORT int
_decomp_s(wchar_t *restrict dest, rsize_t dmax, const wint_t cp)
{
    assert(dmax > 4);
    /* The costly is_HANGUL_cp_high(cp) checks also all composing chars.
       Hangul_IsS only for the valid start points. Which we can do here. */
    if (Hangul_IsS(cp)) {
        return _decomp_hangul_s(dest, dmax, cp);
    }
    else {
        return _decomp_canonical_s(dest, dmax, cp);
    }
}

#ifdef HAVE_NFC

/* canonical ordering of combining characters (c.c.). */
typedef struct {
    uint8_t cc;	/* combining class */
    wint_t cp;	/* codepoint */
    size_t pos; /* position */
} UNWIF_cc;

static int _compare_cc(const void *a, const void *b)
{
    int ret_cc;
    ret_cc = ((UNWIF_cc*) a)->cc - ((UNWIF_cc*) b)->cc;
    if (ret_cc)
	return ret_cc;

    return ( ((UNWIF_cc*) a)->pos > ((UNWIF_cc*) b)->pos )
	 - ( ((UNWIF_cc*) a)->pos < ((UNWIF_cc*) b)->pos );
}

static wint_t _composite_cp_s(wint_t cp, wint_t cp2)
{
    UNWIF_complist ***plane, **row, *cell, *i;

    if (unlikely(!cp2)) {
        invoke_safe_str_constraint_handler("_composite_cp_s: "
                   "cp is 0",
                   NULL, ESZEROL);
	return 0;
    }
    if (unlikely((_UNICODE_MAX < cp) || (_UNICODE_MAX < cp2))) {
        invoke_safe_str_constraint_handler("_composite_cpl_s: "
                   "cp is too high",
                   NULL, ESLEMAX);
	return 0;
    }

    if (Hangul_IsL(cp) && Hangul_IsV(cp2)) {
	wint_t lindex = cp  - Hangul_LBase;
	wint_t vindex = cp2 - Hangul_VBase;
	return(Hangul_SBase + (lindex * Hangul_VCount + vindex) *
	       Hangul_TCount);
    }
    if (Hangul_IsLV(cp) && Hangul_IsT(cp2)) {
	wint_t tindex = cp2 - Hangul_TBase;
	return(cp + tindex);
    }
    plane = UNWIF_compos[cp >> 16];
    if (!plane) { /* only 3 of 16 are defined */
	return 0;
    row = plane[(cp >> 8) & 0xff];
    if (!row) { /* the zero plane is pretty filled, the others sparse */
	return 0;
    cell = row[cp & 0xff];
    if (!cell) {
	return 0;
    for (i = cell; i->nextchar; i++) {
	if (cp2 == i->nextchar)
	    return i->composite;
    }
    return 0;
}

static int _getCombinClass_s(wchar_t* dest, rsize_t dmax, wint_t cp)
{
    wchar_t **plane, *row;
    if (unlikely(_UNICODE_MAX < cp)) {
        invoke_safe_str_constraint_handler("_getCombinClass_s: "
                   "cp is too high",
                   NULL, ESLEMAX);
	return 0;
    }
    plane = UNWIF_combin[cp >> 16];
    if (! plane)
	return 0;
    row = plane[(cp >> 8) & 0xff];
    if (row) {
        int c = wcslen(row[cp & 0xff]);
        wmemcpy_s(dest, dmax, row[cp & 0xff], c);
        return c;
    }
    else {
        *dest = 0;
        return 0;
    }
}

#endif

/* create a NFD wide string */

EXPORT errno_t
wcsnorm_decompose_s(wchar_t *restrict dest, rsize_t dmax, wchar_t *restrict src)
{
    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("wcsnorm_decompose_s: "
                   "dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("wcsnorm_decompose_s: "
                   "src is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("wcsnorm_decompose_s: "
                   "dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("wcsnorm_decompose_s: "
                   "dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    while (*src && dmax) {
        if (isw_maybe_composed(*src)) {
            int c = _decomp_s(dest, dmax, *src);
            src++;
            dmax += c;
            dmax -= c;
        }
        else {
            *dest++ = *src++;
            dmax--;
        }
    }
    if (unlikely(!dmax && *src)) {
        invoke_safe_str_constraint_handler("wcsnorm_decompose_s: "
                   "dmax too small",
                   NULL, ESNOSPC);
        return (ESNOSPC);
    }
    return (EOK);
}

/* create a canonical NFC wide string (not yet, just NFD) */

EXPORT errno_t
wcsnorm_s(wchar_t *restrict dest, rsize_t dmax, wchar_t *restrict src)
{
    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "src is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    while (*src && dmax) {
        if (isw_maybe_composed(*src)) {
            int c = _decomp_s(dest, dmax, *src);
            src++;
            dmax += c;
            dmax -= c;
        }
        else {
            *dest++ = *src++;
            dmax--;
        }
    }

    if (unlikely(!dmax && *src)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "dmax too small",
                   NULL, ESNOSPC);
        return (ESNOSPC);
    }

#ifdef HAVE_NFC
    /* TODO: need temp. scratch space here */
    rc = wcsnorm_reorder_s(dest, dmax, src);
    if (unlikely(!rc)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "reorder error",
                   NULL, EOF);
        return (EOF);
    }
    
    /* TODO: need temp. scratch space again */
    wcsnorm_compose_s(dest, dmax, src);
    if (unlikely(!rc)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "compose error",
                   NULL, EOF);
        return (EOF);
    }
#endif
    return (EOK);
}
