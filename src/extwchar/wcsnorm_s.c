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

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#include <wctype.h>
#include <assert.h>
#endif

bool isExclusion (wint_t uv);
bool isSingleton (wint_t uv);
bool isNonStDecomp (wint_t uv);
bool isComp2nd (wint_t uv);

#if SIZEOF_WCHAR_T > 2
/* generated via cperl Unicode-Normalize/mkheader -uni -ind */
# include "unwifcan.h" /* for NFD Canonical Decomposition */
# include "unwifcmb.h" /* for reorder Canonical_Combining_Class_Values */
# include "unwifcmp.h" /* for NFC Canonical Composition lists */
# include "unwifexc.h" /* for NFC Composite exclusions */
# ifdef HAVE_NORM_COMPAT
#  include "unwifcpt.h" /* for NFKD/NFKC Compat. Decomposition. */
# endif
#else /* with UTF-16 surrogate pairs */
# include "unw16ifcan.h" /* for NFD Canonical Decomposition */
# include "unw16ifcmb.h" /* for reorder Canonical_Combining_Class_Values */
# include "unw16ifcmp.h" /* for NFC Canonical Composition lists */
# include "unw16ifexc.h" /* for NFC Composite exclusions */
# ifdef HAVE_NORM_COMPAT
#  include "unw16ifcpt.h" /* for NFKD/NFKC Compat. Decomposition. */
# endif
#endif

/* Korean/Hangul has special (easy) normalization rules */
#include "hangul.h"

#define _UNICODE_MAX 0x10ffff

/* size of array for combining characters */
/* enough as an initial value? */
#define CC_SEQ_SIZE  10
#define CC_SEQ_STEP  5

/* else a passthru macro in *_private.h */
#if SIZEOF_WCHAR_T == 2
/* convert surrogate pair to unicode codepoint */
EXPORT wint_t _dec_w16(wchar_t *src) {
    wint_t s1 = src[0];
    /*if (unlikely(s1 >= 0xd800 && s1 < 0xdc00)) {*/
    if (unlikely((s1 & 0xfc00) == 0xd800)) {
        wint_t s2 = src[1];
        if (likely((s2 & 0xfc00) == 0xdc00)) {
#if 0
            s1 = ((s1 & 0x3ff) << 10) + (s2 & 0x3ff);
            if (s1 < 0x10000)
                s1 += 0x10000;
            return s1;
#else
            return (s1 << 10) + s2 - 0x35FDC00;
#endif
        } else {
            invoke_safe_str_constraint_handler("wcsnorm_s: "
                "illegal surrogate pair",
                NULL, EILSEQ);
            return 0;
        }
    } else {
        return *src;
    }
}
#endif

/* Note that we can generate two versions of the tables.  The old format as
 * used in Unicode::Normalize, and the new 3x smaller NORMALIZE_IND_TBL cperl
 * variant, as used here and in cperl core since 5.27.2.
 */
static int
_decomp_canonical_s(wchar_t *dest, rsize_t dmax, wint_t cp)
{
#ifndef NORMALIZE_IND_TBL
    /* the old big format */
    if (unlikely(dmax < 5)) {
	return -ESNOSPC;
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
    /* the new format generated with cperl Unicode-Normalize/mkheader -uni -ind */
    const UNWIF_canon_PLANE_T **plane, *row;
    if (unlikely(dmax < 5)) {
        *dest = 0;
	return -ESNOSPC;
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
            /* value => length-index and offset */
            const int l = UNWIF_canon_LEN(vi);
            const int i = UNWIF_canon_IDX(vi);
            const wchar_t* tbl = (const wchar_t*)UNWIF_canon_tbl[l-1];
#if SIZEOF_WCHAR_T > 2
            const int len = l;
#else
            /* unw16ifcan.h needs TBL(5) for len 6 */
            const int len = (l == 5) ? 6 : l;
#endif
#if 0 && defined(DEBUG)
            printf("U+%04X vi=0x%x (>>12, &fff) => TBL(%d)|%d\n", cp, vi, l, i);
#endif
#if SIZEOF_WCHAR_T > 2
            assert(l>0 && l<=4);
            /* (917,762,227,36) */
            assert((l==1 && i<917) || (l==2 && i<762) || (l==3 && i<227) ||
                   (l==4 && i<36) || 0);
            assert(dmax > 4);
#endif
            memcpy(dest, &tbl[i*len], len*sizeof(wchar_t)); /* 33% perf */
            dest[len] = L'\0';
            return len;
        }
    }
    else {
        return 0;
    }
#endif
}

#ifdef HAVE_NORM_COMPAT

static int
_bsearch_exc (const void *ptr1, const void *ptr2) {
    UNWIF_compat_exc_t *e1 = (UNWIF_compat_exc_t *)ptr1;
    UNWIF_compat_exc_t *e2 = (UNWIF_compat_exc_t *)ptr2;
    return e1->cp > e2->cp ? 1 : e1->cp == e2->cp ? 0 : -1;
}

static int
_decomp_compat_s(wchar_t *dest, rsize_t dmax, wint_t cp)
{
#ifndef NORMALIZE_IND_TBL
    /* the old big format */
    if (unlikely(dmax < 19)) {
        *dest = 0;
	return -ESNOSPC;
    }
    else {
        const wchar_t ***plane = UNWF_compat[cp >> 16];
        if (! plane) {
            return 0;
        } else {
            const wchar_t **row = plane[(cp >> 8) & 0xff];
            if (row) {
                const wchar_t *s = row[cp & 0xff];
                if (s) {
                    const int c = wcslen(row[cp & 0xff]);
                    wmemcpy_s(dest, dmax, row[cp & 0xff], c);
                    dest[c] = L'\0';
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
    /* the new format generated with cperl Unicode-Normalize/mkheader -uni -ind */
    const UNWIF_compat_PLANE_T **plane, *row;
    plane = UNWIF_compat[cp >> 16];
    if (!plane) { /* Only the first 3 of 16 are filled */
	return 0;
    }
    row = plane[(cp >> 8) & 0xff];
    if (row) { /* the first row is pretty filled, the rest very sparse */
        const UNWIF_compat_PLANE_T vi = row[cp & 0xff];
        if (!vi)
            return 0;
        else if (unlikely(vi == (uint16_t)-1)) { /* overlong */
            UNWIF_compat_exc_t *e;
            assert(UNWIF_compat_exc_size);
#if 1
            e = (UNWIF_compat_exc_t *)bsearch(&cp, &UNWIF_compat_exc,
                    UNWIF_compat_exc_size, sizeof(UNWIF_compat_exc[0]), _bsearch_exc);
            if (e) {
                size_t l = wcslen(e->v);
                memcpy(dest, e->v, (l+1)*sizeof(wchar_t)); /* incl \0 */
                return (int)l;
            }
#else
            for (e=(UNWIF_compat_exc_t*)UNWIF_compat_exc; e->cp; e++) {
                if (cp == e->cp) {
                    size_t l = wcslen(e->v);
                    if (unlikely(dmax < l)) {
                        *dest = 0;
                        return -(ESNOSPC);
                    }
                    /*assert(l < dmax);*/
                    memcpy(dest, e->v, (l+1)*sizeof(wchar_t)); /* incl \0 */
                    return (int)l;
                }
                else if (cp < e->cp)
                    break;
            }
#endif
            return 0;
        }
        else {
            /* value => length and index */
            const int l = UNWIF_compat_LEN(vi);
            const int i = UNWIF_compat_IDX(vi);
            const wchar_t* tbl = (const wchar_t*)UNWIF_compat_tbl[l-1];
#if 0 && defined(DEBUG)
            printf("U+%04X vi=0x%x (>>12, &&fff) => TBL(%d)|%d\n", cp, vi, l, i);
#endif
            if (unlikely(dmax < (rsize_t)l)) {
                *dest = 0;
                return -(ESNOSPC);
            }
            /* TODO: on sizeof(wchar_t)==2 pre-converted to surrogate pairs */
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
#endif

static int
_decomp_hangul_s(wchar_t *dest, rsize_t dmax, wint_t cp)
{
    wint_t sindex =  cp - Hangul_SBase;
    wint_t lindex =  sindex / Hangul_NCount;
    wint_t vindex = (sindex % Hangul_NCount) / Hangul_TCount;
    wint_t tindex =  sindex % Hangul_TCount;
    
    if (unlikely(dmax < 4)) {
        return -(ESNOSPC);
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
   returns a negative errno_t on error
*/

EXPORT int
_decomp_s(wchar_t *restrict dest, rsize_t dmax, const wint_t cp, const bool iscompat)
{
    /*assert(dmax > 4);*/

    /* The costly is_HANGUL_cp_high(cp) checks also all composing chars.
       Hangul_IsS only for the valid start points. Which we can do here. */
    if (Hangul_IsS(cp)) {
        return _decomp_hangul_s(dest, dmax, cp);
    }
    else {
#ifdef HAVE_NORM_COMPAT
        return iscompat ? _decomp_compat_s(dest, dmax, cp)
                        : _decomp_canonical_s(dest, dmax, cp);
#else
        (void)iscompat;
        return _decomp_canonical_s(dest, dmax, cp);
#endif
    }
}

/* canonical ordering of combining characters (c.c.). */
typedef struct {
    uint8_t  cc;  /* combining class */
    wint_t cp;  /* codepoint */
    size_t   pos; /* position */
} UNWIF_cc;

/* rc = wcsnorm_reorder_s(tmp, len+1, dest); */
static int _compare_cc(const void *a, const void *b)
{
    int ret_cc;
    ret_cc = ((UNWIF_cc*) a)->cc - ((UNWIF_cc*) b)->cc;
    if (ret_cc)
	return ret_cc;

    return ( ((UNWIF_cc*) a)->pos > ((UNWIF_cc*) b)->pos )
	 - ( ((UNWIF_cc*) a)->pos < ((UNWIF_cc*) b)->pos );
}

static wint_t _composite_cp(wint_t cp, wint_t cp2)
{
    const UNWIF_complist_s ***plane, **row, *cell;

    if (unlikely(!cp2)) {
        invoke_safe_str_constraint_handler("wcsnorm_compose_s: "
                   "cp is 0",
                   NULL, ESZEROL);
	      return 0;
    }

#if SIZEOF_WCHAR_T > 2
    if (unlikely((_UNICODE_MAX < cp) || (_UNICODE_MAX < cp2))) {
	      return -(ESLEMAX);
    }
#endif

    if (Hangul_IsL(cp) && Hangul_IsV(cp2)) {
	      wint_t lindex = cp  - Hangul_LBase;
	      wint_t vindex = cp2 - Hangul_VBase;
	      return(Hangul_SBase + (lindex * Hangul_VCount + vindex) *
	             Hangul_TCount);
    }
    if (Hangul_IsLV(cp) && Hangul_IsT(cp2)) {
	      wint_t tindex = cp2 - Hangul_TBase;
	      return (cp + tindex);
    }
    plane = UNWIF_compos[cp >> 16];
    if (!plane) {  /* only 3 of 16 are defined */
	      return 0;
    }
    row = plane[(cp >> 8) & 0xff];
    if (!row) { /* the zero plane is pretty filled, the others sparse */
	      return 0;
    }
    cell = row[cp & 0xff];
    if (!cell) {
        return 0;
    }
    /* no indirection here, but search in the composition lists */
    /* only 16 lists 011099-01d1bc need uint32, the rest can be short, uint16 */
    /* TODO: above which length is bsearch faster?
       But then we'd need to store the lengths also */
#if SIZEOF_WCHAR_T > 2
    if (likely(cp < UNWIF_COMPLIST_FIRST_LONG)) {
#endif
        UNWIF_complist_s *i;
        for (i = (UNWIF_complist_s *)cell; i->nextchar; i++) {
            if ((uint16_t)cp2 == i->nextchar) {
                return (wint_t)(i->composite);
            }
            else if ((uint16_t)cp2 < i->nextchar) { /* nextchar is sorted */
                break;
            }
        }
#if SIZEOF_WCHAR_T > 2
    } else {
        UNWIF_complist *i;
        GCC_DIAG_IGNORE(-Wcast-align)
        for (i = (UNWIF_complist *)cell; i->nextchar; i++) {
            GCC_DIAG_RESTORE
            if (cp2 == i->nextchar) {
                return i->composite;
            }
            else if (cp2 < i->nextchar) { /* nextchar is sorted */
                break;
            }
        }
    }
#endif
    return 0;
}

static uint8_t _combin_class(wint_t cp)
{
    const uint8_t **plane, *row;
    plane = UNWIF_combin[cp >> 16];
    if (!plane)
	return 0;
    row = plane[(cp >> 8) & 0xff];
    if (row)
        return row[cp & 0xff];
    else
        return 0;
}

/**
 * @brief
 *    Converts the wide string to the canonical NFD normalization,
 *    as defined in the latest Unicode standard, latest 10.0.  The conversion
 *    stops at the first null or after dmax characters.
 * 
 * @details
 *    Composed characters are checked for the left-hand-size of the
 *    Decomposition_Mapping Unicode property, which means the codepoint will
 *    be normalized if the sequence is composed.
 *    This is equivalent to all 1963 combining mark characters, plus some
 *    remaining 869 non-mark and non-hangul normalizables.  Hangul has some
 *    special normalization logic.
 *
 *    This function is used by \c wcsnorm_s() to do the argument checking,
 *    overlap checking and to do the first of three passes for NFC.
 *
 * @param[out]  dest      wide string to hold the result
 * @param[in]   dmax      maximum result buffer size
 * @param[in]   src       wide string
 * @param[out]  lenp      pointer to length of the result, may be NULL
 * @param[in]   iscompat  do NFKD, and not NFD (with \c --enable-norm-compat)
 *
 * @pre  dest and src shall not be null pointers.
 * @pre  dmax shall not equal zero and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then wcsnorm_s nulls dest.
 * @retval  EOK         on success
 * @retval  ESNULLP    when dest or src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMIN    when dmax < 5 or 19 with a compat mode
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR
 * @retval  ESOVRLP    when buffers overlap
 * @retval  ESNOSPC    when dmax too small for the result buffer
 * @retval  EOF        on some normalization error
 *
 * @see
 *    wcsfc_s(), wcsnorm_s(), wcsnorm_compose_s(), wcsnorm_reorder_s(),
 *    ICU, gnulib/libunistring, utf8proc
 */

/* create an unordered decomposed wide string */
EXPORT errno_t
wcsnorm_decompose_s(wchar_t *restrict dest, rsize_t dmax, wchar_t *restrict src,
                    rsize_t *restrict lenp, bool iscompat)
{
    rsize_t orig_dmax;
    wchar_t *orig_dest;
    const wchar_t *overlap_bumper;
    wint_t cp;
    int c;

    if (lenp)
        *lenp = 0;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "src is null",
                   NULL, ESNULLP);
        *dest = 0;
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "dmax is 0",
                   NULL, ESZEROL);
        *dest = 0;
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax < 5)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "dmax < 5",
                   NULL, ESLEMIN);
        *dest = 0;
        return RCNEGATE(ESLEMIN);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "dmax exceeds max",
                   NULL, ESLEMAX);
        *dest = 0;
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(dest == src)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                   "overlapping objects",
                   NULL, ESOVRLP);
        return RCNEGATE(ESOVRLP);
    }
#ifndef HAVE_NORM_COMPAT
    if (iscompat) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                      "not configured with --enable-norm-compat",
                      NULL, EOF);
        *dest = 0;
        return RCNEGATE(EOF);
    }
#else
    if (unlikely(iscompat && dmax < 19)) {
        invoke_safe_str_constraint_handler("wcsnorm_s: "
                      "dmax is < 19",
                      NULL, ESLEMIN);
        *dest = 0;
	return RCNEGATE(ESLEMIN);
    }
#endif

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;
    
    if (dest < src) {
        overlap_bumper = src;
    
        while (dmax > 0) {
            cp = _dec_w16(src);
            if (unlikely(dest == overlap_bumper)) {
                handle_werror(orig_dest, orig_dmax, "wcsnorm_decompose_s: "
                             "overlapping objects",
                             ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }
            /* A surrogate pair can only represent max _UNICODE_MAX */
#if SIZEOF_WCHAR_T > 2
            if (unlikely(_UNICODE_MAX < cp)) {
                handle_werror(orig_dest, orig_dmax, "wcsnorm_decompose_s: "
                              "cp is too high",
                              ESLEMAX);
                return ESLEMAX;
            }
#endif
            if (!cp)
                goto done;

            c = _decomp_s(dest, dmax, cp, iscompat);
            if (c > 0) {
                dest += c;
                dmax -= c;             
            } else if (c == 0) {
                *dest++ = *src;
                dmax--;
            } else {
                handle_werror(orig_dest, orig_dmax, "wcsnorm_decompose_s: "
                              "decomposition error",
                              -c);
                return -c;
            }
            src++;
        }
    } else {
        overlap_bumper = dest;

        while (dmax > 0) {
            cp = _dec_w16(src);
            if (unlikely(src == overlap_bumper)) {
                handle_werror(orig_dest, orig_dmax, "wcsnorm_decompose_s: "
                             "overlapping objects",
                             ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }
#if SIZEOF_WCHAR_T > 2
            if (unlikely(_UNICODE_MAX < cp)) {
                handle_werror(orig_dest, orig_dmax, "wcsnorm_decompose_s: "
                              "cp is too high",
                              ESLEMAX);
                return ESLEMAX;
            }
#endif
            if (!cp)
                goto done;

            c = _decomp_s(dest, dmax, cp, iscompat);
            if (c > 0) {
                dest += c;
                dmax -= c;
            } else if (c == 0) {
                *dest++ = *src;
                dmax--;
            } else {
                handle_werror(orig_dest, orig_dmax, "wcsnorm_decompose_s: "
                              "decomposition error",
                              -c);
                return RCNEGATE(-c);
            }
            src++;
        }
    }

    if (lenp)
        *lenp = orig_dmax - dmax;
    handle_werror(orig_dest, orig_dmax, "wcsnorm_decompose_s: "
                  "dmax too small",
                  ESNOSPC);
    return RCNEGATE(ESNOSPC);

  done:
    if (lenp)
        *lenp = orig_dmax - dmax;
#ifdef SAFECLIB_STR_NULL_SLACK
    memset(dest, 0, dmax*sizeof(wchar_t));
#else
    *dest = 0;
#endif
    return EOK;
}

/**
 * @brief
 *    Reorder all decomposed sequences in a wide string to NFD,
 *    as defined in the latest Unicode standard, latest 10.0. The conversion
 *    stops at the first null or after dmax characters.
 *
 * @param[out]  dest      wide string to hold the result
 * @param[in]   dmax      maximum result buffer size
 * @param[in]   p         wide string to be converted
 * @param[in]   len       length of p
 *
 * @pre  dest and p shall not be null pointers.
 * @pre  dmax shall not equal zero and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then \c wcsnorm_reorder_s nulls dest.
 * @retval  EOK        on success
 * @retval  ESNOSPC    when dmax too small for the result buffer
 * @retval  EOF        on some normalization error
 *
 * @see
 *    wcsnorm_s(), wcsnorm_decompose_s(), ICU, gnulib/libunistring, utf8proc
 */

/* reorder decomposed sequence to NFD */
EXPORT errno_t
wcsnorm_reorder_s(wchar_t *restrict dest, rsize_t dmax, wchar_t *restrict p,
                  rsize_t len)
{
    UNWIF_cc seq_ary[CC_SEQ_SIZE];
    UNWIF_cc* seq_ptr = (UNWIF_cc*)seq_ary; /* start with stack */
    UNWIF_cc* seq_ext = NULL;               /* heap when needed */
    size_t seq_max = CC_SEQ_SIZE;
    size_t cc_pos = 0;
    const wchar_t *e = p + len;
    wchar_t *orig_dest = dest;
    rsize_t orig_dmax = dmax;

    while (p < e) {
	uint8_t cur_cc;
        wint_t cp = _dec_w16(p);
        p++;

	cur_cc = _combin_class(cp);
	if (cur_cc != 0) {
	    if (seq_max < cc_pos + 1) {         /* extend if need */
		seq_max = cc_pos + CC_SEQ_STEP; /* new size */
		if (CC_SEQ_SIZE == cc_pos) {    /* seq_ary full */
                    seq_ext = (UNWIF_cc*)malloc(seq_max*sizeof(UNWIF_cc));
                    memcpy(seq_ext, seq_ary, cc_pos*sizeof(UNWIF_cc));
		}
		else {
                    seq_ext = (UNWIF_cc*)realloc(seq_ext, seq_max*sizeof(UNWIF_cc));
		}
		seq_ptr = seq_ext; /* use seq_ext from now */
	    }

	    seq_ptr[cc_pos].cc  = cur_cc;
	    seq_ptr[cc_pos].cp  = cp;
	    seq_ptr[cc_pos].pos = cc_pos;
	    ++cc_pos;

	    if (p < e)
		continue;
	}

	/* output */
	if (cc_pos) {
	    size_t i;

            if (unlikely(dmax - cc_pos <= 0)) {
                handle_werror(orig_dest, orig_dmax, "wcsnorm_reorder_s: "
                              "dmax too small",
                              ESNOSPC);
                return RCNEGATE(ESNOSPC);
            }

            if (cc_pos > 1) /* reorder if there are two Combining Classes */
		qsort((void*)seq_ptr, cc_pos, sizeof(UNWIF_cc), _compare_cc);

	    for (i = 0; i < cc_pos; i++) {
                _ENC_W16(dest, dmax, seq_ptr[i].cp);
	    }
	    cc_pos = 0;
	}

	if (cur_cc == 0) {
            _ENC_W16(dest, dmax, cp);
        }

        if (unlikely(!dmax)) {
            handle_werror(orig_dest, orig_dmax, "wcsnorm_reorder_s: "
                          "dmax too small",
                          ESNOSPC);
            return RCNEGATE(ESNOSPC);
        }
    }
    if (seq_ext)
	free(seq_ext);
    /* surrogate pairs can actually collapse */
#if defined(SAFECLIB_STR_NULL_SLACK) && SIZEOF_WCHAR_T == 2
    memset(dest, 0, dmax*sizeof(wchar_t));
#else
    *dest = 0;
#endif
    return EOK;
}

/**
 * @brief
 *    Combine all decomposed sequences in a wide string to NFC,
 *    as defined in the latest Unicode standard, latest 10.0. The conversion
 *    stops at the first null or after dmax characters.
 *
 * @param[out]  dest      wide string to hold the result
 * @param[in]   dmax      maximum result buffer size
 * @param[in]   p         wide string to be converted
 * @param[out]  lenp      pointer to length of p and the result length.
 * @param[in]   iscontig  if true, the result will only be a fast FCC
 *
 * @pre  dest, p and lenp shall not be null pointers.
 * @pre  dmax shall not equal zero and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then \c wcsnorm_reorder_s nulls dest.
 * @retval  EOK        on success
 * @retval  ESNOSPC    when dmax too small for the result buffer
 * @retval  EOF        on some normalization error
 *
 * @see
 *    wcsnorm_s(), wcsnorm_decompose_s(), ICU, gnulib/libunistring, utf8proc
 */

/* combine decomposed sequences to NFC. */
/* iscontig = false; composeContiguous? FCC if true */
EXPORT errno_t
wcsnorm_compose_s(wchar_t *restrict dest, rsize_t dmax, wchar_t *restrict p,
                  rsize_t *restrict lenp, bool iscontig)
{
    const wchar_t *e = p + *lenp;
    wint_t  cpS = 0;           /* starter code point */
    bool    valid_cpS = false; /* if false, cpS isn't initialized yet */
    uint8_t pre_cc = 0;

    wint_t seq_ary[CC_SEQ_SIZE];
    wint_t* seq_ptr = (wint_t*) seq_ary; /* either stack or heap */
    wint_t* seq_ext = NULL;    /* heap */
    size_t seq_max = CC_SEQ_SIZE;
    size_t cc_pos = 0;
    wchar_t *orig_dest = dest;
    rsize_t orig_dmax = dmax;

    while (p < e) {
	uint8_t cur_cc;
        wint_t cp = _dec_w16(p);
        p++;

	cur_cc = _combin_class(cp);

	if (!valid_cpS) {
	    if (cur_cc == 0) {
		cpS = cp; /* found the first Starter */
		valid_cpS = true;
		if (p < e)
		    continue;
	    }
	    else {
                _ENC_W16(dest, dmax, cp);
                if (unlikely(!dmax)) {
                    handle_werror(orig_dest, orig_dmax, "wcsnorm_compose_s: "
                                  "dmax too small",
                                  ESNOSPC);
                    return RCNEGATE(ESNOSPC);
                }
		continue;
	    }
	}
	else {
	    bool composed;

	    /* blocked */
	    if ((iscontig && cc_pos) || /* discontiguous combination (FCC) */
		 (cur_cc != 0 && pre_cc == cur_cc) || /* blocked by same CC */
		 (pre_cc > cur_cc)) /* blocked by higher CC: revised D2 */
		composed = false;

	    /* not blocked:
		 iscontig && cc_pos == 0      -- contiguous combination
		 cur_cc == 0 && pre_cc == 0     -- starter + starter
		 cur_cc != 0 && pre_cc < cur_cc  -- lower CC */
	    else {
		/* try composition */
		wint_t cpComp = _composite_cp(cpS, cp);

		if (cpComp && !isExclusion(cpComp))  {
		    cpS = cpComp;
		    composed = true;

		    /* pre_cc should not be changed to cur_cc */
		    /* e.g. 1E14 = 0045 0304 0300 where CC(0304) == CC(0300) */
		    if (p < e)
			continue;
		}
		else
		    composed = false;
	    }

	    if (!composed) {
		pre_cc = cur_cc;
		if (cur_cc != 0 || !(p < e)) {
		    if (seq_max < cc_pos + 1) { /* extend if need */
			seq_max = cc_pos + CC_SEQ_STEP; /* new size */
			if (CC_SEQ_SIZE == cc_pos) { /* seq_ary full */
                            seq_ext = (wint_t*)malloc(seq_max*sizeof(wint_t));
                            memcpy(seq_ext, seq_ary, cc_pos*sizeof(wint_t));
			}
			else {
                            seq_ext = (wint_t*)realloc(seq_ext, seq_max*sizeof(wint_t));
			}
			seq_ptr = seq_ext; /* use seq_ext from now */
		    }
		    seq_ptr[cc_pos] = cp;
		    ++cc_pos;
		}
		if (cur_cc != 0 && p < e)
		    continue;
	    }
	}

	/* output */
        _ENC_W16(dest, dmax, cpS); /* starter (composed or not) */
        if (unlikely(!dmax)) {
            handle_werror(orig_dest, orig_dmax, "wcsnorm_compose_s: "
                          "dmax too small",
                          ESNOSPC);
            return RCNEGATE(ESNOSPC);
        }

	if (cc_pos == 1) {
            _ENC_W16(dest, dmax, *seq_ptr);
	    cc_pos = 0;
	} else if (cc_pos > 1) {
            memcpy(dest, seq_ptr, cc_pos*sizeof(wchar_t));
            dest += cc_pos;
            dmax -= cc_pos;
	    cc_pos = 0;
        }

	cpS = cp;
    }
    if (seq_ext)
        free(seq_ext);

#ifdef SAFECLIB_STR_NULL_SLACK
    memset(dest, 0, dmax*sizeof(wchar_t));
#else
    *dest = 0;
#endif
    *lenp = orig_dmax - dmax;
    return EOK;
}

/**
 * @brief
 *    Converts the wide string to the canonical NFC or NFD normalization,
 *    as defined in the latest Unicode standard, latest 10.0.  The conversion
 *    stops at the first null or after dmax characters.
 * 
 * @details
 *    Decomposed characters are checked for the left-hand-size and then
 *    right-hand-side of the Decomposition_Mapping Unicode property, which
 *    means the codepoint will be normalized if the sequence is composed or
 *    decomposed (NFD or NFKD).  This is equivalent to all 1963 combining mark
 *    characters, plus some remaining 869 non-mark and non-hangul
 *    normalizables.  Hangul has some special normalization logic.
 *
 *    The compat tables for NFKC or NFKD are too large for a libc, and
 *    mostly unused. As default we only provide the smaller canonical
 *    conversions, but it can be enabled with \c --enable-norm-compat.
 *    The compat modes also don't roundtrip.
 *
 * @param[out]  dest  wide string to hold the result
 * @param[in]   dmax  maximum length of string
 * @param[in]   src   wide string
 * @param[in]   mode  convert to nfc or just nfd.
 *                    experimentally to fast modes FCD or FCC.
 *                    optionally to compat modes NFKD, NFKC with \c --enable-norm-compat
 *                    @see enum \c wcsnorm_mode.
 * @param[out]  lenp  pointer to length of the result, may be NULL
 *
 * @pre  dest and src shall not be null pointers.
 * @pre  dmax shall not equal zero and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_WSTR, then wcsnorm_s nulls dest.
 * @retval  EOK        on success
 * @retval  ESNULLP    when dest or src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMIN    when dmax < 5
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_WSTR
 * @retval  ESOVRLP    when buffers overlap
 * @retval  ESNOSPC    when dmax too small for the result buffer
 * @retval  EOF        any other normalization error
 *
 * @see
 *    wcsfc_s(), ICU, gnulib/libunistring, utf8proc
 */

/* Normalize to NFC, NFD, FCC, FCD (fastest, used in wcsfc_s), and optionally NFKD, NFKC */
EXPORT errno_t
wcsnorm_s(wchar_t *restrict dest, rsize_t dmax, wchar_t *restrict src,
          wcsnorm_mode_t mode, rsize_t *restrict lenp)
{
    wchar_t tmp_stack[128];
    wchar_t *tmp_ptr;
    wchar_t *tmp = NULL;
    rsize_t len;
    const bool iscompat = mode & WCSNORM_NFKD; /* WCSNORM_NFKD or WCSNORM_NFKC */

    errno_t rc = wcsnorm_decompose_s(dest, dmax, src, &len, iscompat);
    if (lenp)
        *lenp = len;
    if (unlikely(rc))
        return rc;
    if (unlikely(mode == WCSNORM_FCD))
        return EOK;

    /* temp. scratch space, on stack or heap */
    if (len+2 < 128)
        tmp_ptr = tmp_stack;
    else
        tmp_ptr = tmp = (wchar_t*)malloc((len+2) * sizeof(wchar_t));

    rc = wcsnorm_reorder_s(tmp_ptr, len+2, dest, len);
    if (unlikely(rc)) {
        if (tmp)
            free(tmp);
#ifdef SAFECLIB_STR_NULL_SLACK
        memset(dest, 0, dmax*sizeof(wchar_t));
#else
        *dest = L'\0';
#endif
        return (rc);
    }
    if (mode == WCSNORM_NFD || mode == WCSNORM_NFKD) {
        memcpy(dest, tmp_ptr, (len+1)*sizeof(wchar_t));
        if (tmp)
            free(tmp);
        return (EOK);
    }

    rc = wcsnorm_compose_s(dest, dmax, tmp_ptr, &len, mode == WCSNORM_FCC);
    if (tmp)
        free(tmp);
    if (unlikely(rc))
        return (rc);
    if (lenp)
        *lenp = len;

    return (EOK);
}
