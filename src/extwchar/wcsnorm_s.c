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
#include "unifcan.h"
#include "unifexc.h"
#ifdef HAVE_NFC
#include "unifcmb.h"
#include "unifcmp.h"
#endif

/* generated via cperl regen/regcharclass-safec.pl (via CharClass::Matcher) */
/* 1963 single mark characters (Combining, Overlay, ...) */
#include "mark.h"
/* Korean/Hangul has special (easy) normalization rules */
#include "hangul.h"
/* The remaining 869 non-mark and non-hangul normalizables, multiple chars!
   All NFD's of valid identifiers IDStart/IDContinue, which do have a different NFC.
   diacrits, dialytika, tonos... */
#include "dec_rest.h"

#define _UNICODE_MAX 0x10ffff

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

/* Check for the right-hand-side of the Decomposition_Mapping property,
 * which means the codepoint can be normalized, if the sequence is
 * decomposed (NFD or NFKD).
 * Assumes 4-byte wchar.
 */
EXPORT int
iswdecomposed(const wchar_t *src, rsize_t smax) {
    if (*src > 0xff && (is_MARK_cp_high(*src) || Hangul_IsS(*src)))
        return 1;

    /*TODO return is_DECOMPOSED_REST_safe(src, dmax, 0)) ? 1 : 0;*/
    (void)smax;
    return 0;
}

/* Note that we can generate two versions of the tables.  The old format as
 * used in Unicode::Normalize, and the new smaller NORMALIZE_IND_TBL cperl
 * variant, as used here and in cperl core since 5.27.2 (planned).
 */
static int
_decomp_canonical(wchar_t *dest, wint_t cp)
{
#ifndef NORMALIZE_IND_TBL
    wchar_t ***plane, **row;
    if (unlikely(_UNICODE_MAX < cp)) {
        *dest = 0;
	return 0;
    }
    plane = UNIF_canon[cp >> 16];
    if (! plane) {
	return 0;
    }
    row = plane[(cp >> 8) & 0xff];
    if (row) {
        int c = wcslen(row[cp & 0xff]);
        wmemcpy(dest, row[cp & 0xff], c);
        return c;
    }
    else {
        *dest = 0;
        return 0;
    }
#else
    /* the new format generated with Unicode-Normalize/mkheader -uni -ind */
    const UNIF_canon_PLANE_T **plane, *row;
    if (unlikely(_UNICODE_MAX < cp)) {
        *dest = 0;
	return 0;
    }
    plane = UNIF_canon[cp >> 16];
    if (!plane) { /* Only the first 3 of 16 are filled */
	return 0;
    }
    row = plane[(cp >> 8) & 0xff];
    if (row) { /* the first row is pretty filled, the rest very sparse */
        const UNIF_canon_PLANE_T vi = row[cp & 0xff]; /* 3|TBL(2) == 3 | 0x8000>>1 */
        if (!vi)
            return 0;
        else {
            const int l = TBL(vi); /* length of the value */
            const int i = vi & UNIF_canon_MASK;
            /*
            const wchar_t* w = (const wchar_t*)UNIF_canon_tbl[l-1][i];
            printf("index 0x%x => l=0x%x i=0x%x\n", vi, l, i);
            memmove(dest, w, sizeof(wchar_t)*l);
            dest[l] = 0;
            return l;
            */
        }
    }
    else {
        return 0;
    }
#endif
}

static int
_decomp_hangul(wchar_t *dest, wint_t cp)
{
    wint_t sindex =  cp - Hangul_SBase;
    wint_t lindex =  sindex / Hangul_NCount;
    wint_t vindex = (sindex % Hangul_NCount) / Hangul_TCount;
    wint_t tindex =  sindex % Hangul_TCount;
    
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


/* codepoint decomposition of one NFD sequence,
   with possible mult. src chars, if decomposed. */

EXPORT int
wcdecomp_s(wchar_t *restrict dest, rsize_t dmax, const wchar_t *restrict src)
{
    wint_t cp = *src;
    assert(dmax > 3);
    /* The costly is_HANGUL_cp_high(cp) checks also all composing chars.
       Hangul_IsS only for the valid start points. Which we can do here. */
    if (Hangul_IsS(cp)) {
        return _decomp_hangul(dest, cp);
    }
    else {
        return _decomp_canonical(dest, cp);
    }
}

#ifdef HAVE_NFC

/* canonical ordering of combining characters (c.c.). */
typedef struct {
    uint8_t cc;	/* combining class */
    wint_t cp;	/* codepoint */
    size_t pos; /* position */
} UNIF_cc;

static int _compare_cc(const void *a, const void *b)
{
    int ret_cc;
    ret_cc = ((UNIF_cc*) a)->cc - ((UNIF_cc*) b)->cc;
    if (ret_cc)
	return ret_cc;

    return ( ((UNIF_cc*) a)->pos > ((UNIF_cc*) b)->pos )
	 - ( ((UNIF_cc*) a)->pos < ((UNIF_cc*) b)->pos );
}

static wint_t _composite_cp(wint_t cp, wint_t cp2)
{
    UNIF_complist ***plane, **row, *cell, *i;

    if (!cp2 || (_UNICODE_MAX < cp) || (_UNICODE_MAX < cp2))
	return 0;

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
    plane = UNIF_compos[cp >> 16];
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

static int _getCombinClass(wchar_t* dest, wint_t cp)
{
    wchar_t **plane, *row;
    if (_UNICODE_MAX < cp)
	return 0;
    plane = UNIF_combin[cp >> 16];
    if (! plane)
	return 0;
    row = plane[(cp >> 8) & 0xff];
    if (row) {
        int c = wcslen(row[cp & 0xff]);
        wmemcpy(dest, row[cp & 0xff], c);
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
        if (iswdecomposed(src, dmax)) {
            int c = wcdecomp_s(dest, dmax, src);
            src += c;
            dmax -=  c;
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
        if (iswdecomposed(src, dmax)) {
            int c = wcdecomp_s(dest, dmax, src);
            src += c;
            dmax -=  c;
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
