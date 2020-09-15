/*------------------------------------------------------------------
 * u8norm_s.c
 *
 * September 2020, Reini Urban (WIP)
 *
 * Copyright (c) 2017,2020 by Reini Urban
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
#include <assert.h>
#endif

#include "u8_private.h"

/* generated via cperl dist/Unicode-Normalize/mkheader -ind -std */
#define TRUE 1
#define FALSE 0
#define STDCHAR unsigned char
#include "un8ifcan.h" /* for NFD Canonical Decomposition */
#include "un8ifcmb.h" /* for reorder Canonical_Combining_Class_Values */
#include "un8ifcmp.h" /* for NFC Canonical Composition lists */
#ifdef HAVE_NORM_COMPAT
#include "un8ifcpt.h" /* for NFKD/NFKC Compat. Decomposition. */
#endif
#ifndef SAFECLIB_DISABLE_WCHAR
bool isExclusion (uint32_t uv);
#else
#include "../extwchar/unwifexc.h"
#endif
/* Korean/Hangul has special (easy) normalization rules */
#include "../extwchar/hangul.h"

#define _UNICODE_MAX 0x10ffff

/* size of array for combining characters */
/* enough as an initial value? */
#define CC_SEQ_SIZE 10
#define CC_SEQ_STEP 5


#if defined(HAVE_NORM_COMPAT) || UN8IF_canon_exc_size > 0

#ifndef HAVE_NORM_COMPAT
/* They may have different structs (16 or 32 bit cp), but not together.
   Either unw16ifcan+cpt (32) or unwifcpt (16) */
#define UN8IF_compat_exc_t UN8IF_canon_exc_t
#endif

static int _bsearch_exc(const void *ptr1, const void *ptr2) {
    UN8IF_compat_exc_t *e1 = (UN8IF_compat_exc_t *)ptr1;
    UN8IF_compat_exc_t *e2 = (UN8IF_compat_exc_t *)ptr2;
    return e1->cp > e2->cp ? 1 : e1->cp == e2->cp ? 0 : -1;
}

#endif

/* Note that we can generate two versions of the tables.  The old format as
 * used in Unicode::Normalize, and the new 3x smaller NORMALIZE_IND_TBL cperl
 * variant, as used here and in cperl core since 5.27.2.
 */
static int _u8decomp_canonical_s(char8_t *dest, rsize_t dmax, const uint32_t cp) {
#ifndef NORMALIZE_IND_TBL
    /* the old big format */
    if (unlikely(dmax < 5)) {
        return -ESNOSPC;
    } else {
        const char ***plane = UN8F_canon[cp >> 16];
        if (!plane) {
            return 0;
        } else {
            const char **row = plane[(cp >> 8) & 0xff];
            if (row) {
                const char *s = row[cp & 0xff];
                if (s) {
                    const int c = strlen(row[cp & 0xff]);
                    memcpy_s(dest, dmax, row[cp & 0xff], c);
                    return c;
                } else {
                    *dest = 0;
                    return 0;
                }
            } else {
                *dest = 0;
                return 0;
            }
        }
    }
#else
    /* the new format generated with cperl Unicode-Normalize/mkheader -uni -ind -std
     */
    const UN8IF_canon_PLANE_T **plane, *row;
    if (unlikely(dmax < 5)) {
        *dest = 0;
        return -ESNOSPC;
    }
    plane = UN8IF_canon[cp >> 16];
    if (!plane) { /* Only the first 3 of 16 are filled */
        return 0;
    }
    row = plane[(cp >> 8) & 0xff];
    if (row) { /* the first row is pretty filled, the rest very sparse */
        const UN8IF_canon_PLANE_T vi = row[cp & 0xff];
        if (!vi)
            return 0;
#if UN8IF_canon_exc_size > 0
        /* overlong: search in extra list */
        else if (unlikely(vi == (uint16_t)-1)) {
            UN8IF_canon_exc_t *e;
            assert(UN8IF_canon_exc_size);
            e = (UN8IF_canon_exc_t *)bsearch(
                &cp, &UN8IF_canon_exc, UN8IF_canon_exc_size,
                sizeof(UN8IF_canon_exc[0]), _bsearch_exc);
            if (e) {
                size_t l = strlen(e->v);
                memcpy(dest, e->v, l + 1); /* incl \0 */
                return (int)l;
            }
            return 0;
        }
#endif
        else {
            /* value => length-index and offset */
            const int l = UN8IF_canon_LEN(vi);
            const int i = UN8IF_canon_IDX(vi);
            const char *tbl = (const char *)UN8IF_canon_tbl[l - 1];
            const int len = l;
#if defined(DEBUG)
            printf("U+%04X vi=0x%x (>>12, &fff) => TBL(%d)|%d\n", cp, vi, l, i);
#endif
            assert(l > 0 && l <= 4);
            /* 13.0: tbl sizes: (917,763,227,36) */
            /* l: 1-4 */
            assert((l == 1 && i < 917) || (l == 2 && i < 763) ||
                   (l == 3 && i < 227) || (l == 4 && i < 36) || 0);
            assert(dmax > 4);
            memcpy(dest, &tbl[i * len], len); /* 33% perf */
            dest[len] = L'\0';
            return len;
        }
    } else {
        return 0;
    }
#endif
}

#ifdef HAVE_NORM_COMPAT

static int _u8decomp_compat_s(char8_t *dest, rsize_t dmax, uint32_t cp) {
#ifndef NORMALIZE_IND_TBL
    /* the old big format */
    if (unlikely(dmax < 19)) {
        *dest = 0;
        return -ESNOSPC;
    } else {
        const char ***plane = UN8F_compat[cp >> 16];
        if (!plane) {
            return 0;
        } else {
            const char **row = plane[(cp >> 8) & 0xff];
            if (row) {
                const char *s = row[cp & 0xff];
                if (s) {
                    const int c = strlen(row[cp & 0xff]);
                    memcpy_s(dest, dmax, row[cp & 0xff], c);
                    dest[c] = L'\0';
                    return c;
                } else {
                    *dest = 0;
                    return 0;
                }
            } else {
                *dest = 0;
                return 0;
            }
        }
    }
#else
    /* the new format generated with cperl Unicode-Normalize/mkheader -uni -ind -std
     */
    const UN8IF_compat_PLANE_T **plane, *row;
    plane = UN8IF_compat[cp >> 16];
    if (!plane) { /* Only the first 3 of 16 are filled */
        return 0;
    }
    row = plane[(cp >> 8) & 0xff];
    if (row) { /* the first row is pretty filled, the rest very sparse */
        const UN8IF_compat_PLANE_T vi = row[cp & 0xff];
        if (!vi)
            return 0;
#if UN8IF_compat_exc_size > 0
        else if (unlikely(vi ==
                          (uint16_t)-1)) { /* overlong: search in extra list */
            UN8IF_compat_exc_t *e;
            assert(UN8IF_compat_exc_size);
#if 1
            e = (UN8IF_compat_exc_t *)bsearch(
                &cp, &UN8IF_compat_exc, UN8IF_compat_exc_size,
                sizeof(UN8IF_compat_exc[0]), _bsearch_exc);
            if (e) {
                size_t l = strlen(e->v);
                memcpy(dest, e->v, l + 1); /* incl \0 */
                return (int)l;
            }
#else
            for (e = (UN8IF_compat_exc_t *)UN8IF_compat_exc; e->cp; e++) {
                if (cp == e->cp) {
                    size_t l = strlen(e->v);
                    if (unlikely(dmax < l)) {
                        *dest = 0;
                        return -(ESNOSPC);
                    }
                    /*assert(l < dmax);*/
                    memcpy(dest, e->v, l + 1); /* incl \0 */
                    return (int)l;
                } else if (cp < e->cp)
                    break;
            }
#endif
            return 0;
#endif
        } else {
            /* value => length and index */
            const int l = UN8IF_compat_LEN(vi);
            const int i = UN8IF_compat_IDX(vi);
            const char *tbl = (const char *)UN8IF_compat_tbl[l - 1];
#if 0 && defined(DEBUG)
            printf("U+%04X vi=0x%x (>>12, &&fff) => TBL(%d)|%d\n", cp, vi, l, i);
#endif
            if (unlikely(dmax < (rsize_t)l)) {
                *dest = 0;
                return -(ESNOSPC);
            }
            memcpy(dest, &tbl[i * l], l);
            dest[l] = L'\0';
            return l;
        }
    } else {
        return 0;
    }
#endif
}
#endif

static int _u8decomp_hangul_s(char8_t *dest, rsize_t dmax, const uint32_t cp) {
    const uint32_t sindex = cp - Hangul_SBase;
    const uint32_t lindex = sindex / Hangul_NCount;
    const uint32_t vindex = (sindex % Hangul_NCount) / Hangul_TCount;
    const uint32_t tindex = sindex % Hangul_TCount;

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

EXPORT int _u8decomp_s(char8_t *restrict dest, rsize_t dmax, const uint32_t cp,
                       const bool iscompat)
{
    /*assert(dmax > 4);*/

    /* The costly is_HANGUL_cp_high(cp) checks also all composing chars.
       Hangul_IsS only for the valid start points. Which we can do here. */
    if (Hangul_IsS(cp)) {
        return _u8decomp_hangul_s(dest, dmax, cp);
    } else {
#ifdef HAVE_NORM_COMPAT
        return iscompat ? _u8decomp_compat_s(dest, dmax, cp)
                        : _u8decomp_canonical_s(dest, dmax, cp);
#else
        (void)iscompat;
        return _u8decomp_canonical_s(dest, dmax, cp);
#endif
    }
}

/* canonical ordering of combining characters (c.c.). */
typedef struct {
    uint8_t cc;  /* combining class */
    uint32_t cp; /* codepoint */
    size_t pos;  /* position */
} UN8IF_cc;

/* rc = u8norm_reorder_s(tmp, len+1, dest); */
static int _compare_cc(const void *a, const void *b) {
    int ret_cc;
    ret_cc = ((UN8IF_cc *)a)->cc - ((UN8IF_cc *)b)->cc;
    if (ret_cc)
        return ret_cc;

    return (((UN8IF_cc *)a)->pos > ((UN8IF_cc *)b)->pos) -
           (((UN8IF_cc *)a)->pos < ((UN8IF_cc *)b)->pos);
}

static uint32_t _composite_cp(uint32_t cp, uint32_t cp2) {
    const UN8IF_complist_s ***plane, **row, *cell;

    if (unlikely(!cp2)) {
        invoke_safe_str_constraint_handler("u8norm_compose_s: "
                                           "cp is 0",
                                           NULL, ESZEROL);
        return 0;
    }

    if (unlikely((_UNICODE_MAX < cp) || (_UNICODE_MAX < cp2))) {
        return -(ESLEMAX);
    }

    if (Hangul_IsL(cp) && Hangul_IsV(cp2)) {
        uint32_t lindex = cp - Hangul_LBase;
        uint32_t vindex = cp2 - Hangul_VBase;
        return (Hangul_SBase +
                (lindex * Hangul_VCount + vindex) * Hangul_TCount);
    }
    if (Hangul_IsLV(cp) && Hangul_IsT(cp2)) {
        uint32_t tindex = cp2 - Hangul_TBase;
        return (cp + tindex);
    }
    plane = UN8IF_compos[cp >> 16];
    if (!plane) { /* only 3 of 16 are defined */
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
    if (likely(cp < UN8IF_COMPLIST_FIRST_LONG)) {
        UN8IF_complist_s *i;
        for (i = (UN8IF_complist_s *)cell; i->nextchar; i++) {
            if ((uint16_t)cp2 == i->nextchar) {
                return (uint32_t)(i->composite);
            } else if ((uint16_t)cp2 < i->nextchar) { /* nextchar is sorted */
                break;
            }
        }
    } else {
        UN8IF_complist *i;
        GCC_DIAG_IGNORE(-Wcast-align)
        for (i = (UN8IF_complist *)cell; i->nextchar; i++) {
            GCC_DIAG_RESTORE
            if (cp2 == i->nextchar) {
                return i->composite;
            } else if (cp2 < i->nextchar) { /* nextchar is sorted */
                break;
            }
        }
    }
    return 0;
}

static uint8_t _combin_class(uint32_t cp) {
    const uint8_t **plane, *row;
    plane = UN8IF_combin[cp >> 16];
    if (!plane)
        return 0;
    row = plane[(cp >> 8) & 0xff];
    if (row)
        return row[cp & 0xff];
    else
        return 0;
}

/**
 * @def u8norm_decompose_s(dest,dmax,src,lenp,iscompat)
 * @brief
 *    Converts the utf-8 string to the canonical NFD normalization,
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
 *    This function is used by \c u8norm_s() to do the argument checking,
 *    overlap checking and to do the first of three passes for NFC.
 *
 * @param[out]  dest      utf-8 string to hold the result
 * @param[in]   dmax      maximum result buffer size
 * @param[in]   src       utf-8 string
 * @param[out]  lenp      pointer to length of the result, may be NULL
 * @param[in]   iscompat  do NFKD, and not NFD (with \c --enable-norm-compat)
 *
 * @pre  dest and src shall not be null pointers.
 * @pre  dmax shall not equal zero and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then u8norm_s nulls dest.
 * @retval  EOK         on success
 * @retval  ESNULLP    when dest or src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMIN    when dmax < 5 or 19 with a compat mode
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESOVRLP    when buffers overlap
 * @retval  ESNOSPC    when dmax too small for the result buffer
 * @retval  EOF        on some normalization error
 *
 * @see
 *    u8fc_s(), u8norm_s(), u8norm_compose_s(), u8norm_reorder_s(),
 *    ICU, gnulib/libunistring, utf8proc
 */

/* create an unordered decomposed utf-8 string */
#ifdef FOR_DOXYGEN
errno_t u8norm_decompose_s(char8_t *restrict dest, rsize_t dmax,
                           const char8_t *restrict src,
                           rsize_t *restrict lenp,
                           const bool iscompat)
#else
EXPORT errno_t _u8norm_decompose_s_chk(char8_t *restrict dest, rsize_t dmax,
                                       const char8_t *restrict src,
                                       rsize_t *restrict lenp,
                                       const bool iscompat,
                                       const size_t destbos)
#endif
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char8_t *overlap_bumper;
    uint32_t cp;
    int c;

    if (lenp)
        *lenp = 0;
    CHK_DEST_NULL("u8norm_s")
    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "src is null",
                                           dest, ESNULLP);
        *dest = 0;
        return RCNEGATE(ESNULLP);
    }
    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "dmax is 0",
                                           dest, ESZEROL);
        *dest = 0;
        return RCNEGATE(ESZEROL);
    }
    if (unlikely(dmax < 5)) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "dmax < 5",
                                           dest, ESLEMIN);
        *dest = 0;
        return RCNEGATE(ESLEMIN);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "dmax exceeds max",
                                           dest, ESLEMAX);
        *dest = 0;
        return RCNEGATE(ESLEMAX);
    }
    if (destbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR_CLEAR("u8fc_s", destbos)
    }

    if (unlikely(dest == src)) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "overlapping objects",
                                           dest, ESOVRLP);
        return RCNEGATE(ESOVRLP);
    }
#ifndef HAVE_NORM_COMPAT
    if (iscompat) {
        invoke_safe_str_constraint_handler(
            "u8norm_s: "
            "not configured with --enable-norm-compat",
            dest, EOF);
        *dest = 0;
        return RCNEGATE(EOF);
    }
#else
    if (unlikely(iscompat && dmax < 19)) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "dmax is < 19",
                                           dest, ESLEMIN);
        *dest = 0;
        return RCNEGATE(ESLEMIN);
    }
#endif

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = (char*)dest;

    if (dest < src) {
        overlap_bumper = src;

        while (dmax > 0) {
            cp = dec_utf8((char8_t**)&src);
            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                              "u8norm_decompose_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }
            /* A surrogate pair can only represent max _UNICODE_MAX */
            if (unlikely(_UNICODE_MAX < cp)) {
                handle_error(orig_dest, orig_dmax,
                              "u8norm_decompose_s: "
                              "cp is too high",
                              ESLEMAX);
                return RCNEGATE(ESLEMAX);
            }
            if (!cp)
                goto done;

            c = _u8decomp_s(dest, dmax, cp, iscompat);
            if (c > 0) {
                dest += c;
                dmax -= c;
                if (cp > 0xffff) {
                    src++;
                }
            } else if (c == 0) {
                *dest++ = *src;
                dmax--;
            } else {
                handle_error(orig_dest, orig_dmax,
                              "u8norm_decompose_s: "
                              "decomposition error",
                              -c);
                return -c;
            }
            src++;
        }
    } else {
        overlap_bumper = dest;

        while (dmax > 0) {
            cp = dec_utf8((char8_t**)&src);
            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                              "u8norm_decompose_s: "
                              "overlapping objects",
                              ESOVRLP);
                return RCNEGATE(ESOVRLP);
            }
            if (unlikely(_UNICODE_MAX < cp)) {
                handle_error(orig_dest, orig_dmax,
                              "u8norm_decompose_s: "
                              "cp is too high",
                              ESLEMAX);
                return RCNEGATE(ESLEMAX);
            }
            if (!cp)
                goto done;

            c = _u8decomp_s(dest, dmax, cp, iscompat);
            if (c > 0) {
                dest += c;
                dmax -= c;
                if (cp > 0xffff)
                    src++;
            } else if (c == 0) {
                if (cp > 0xffff) {
                    *dest++ = *src++;
                    dmax--;
                }
                *dest++ = *src;
                dmax--;
            } else {
                handle_error(orig_dest, orig_dmax,
                              "u8norm_decompose_s: "
                              "decomposition error",
                              -c);
                return RCNEGATE(-c);
            }
            src++;
        }
    }

    if (lenp)
        *lenp = orig_dmax - dmax;
    handle_error(orig_dest, orig_dmax,
                  "u8norm_decompose_s: "
                  "dmax too small",
                  ESNOSPC);
    return RCNEGATE(ESNOSPC);

done:
    if (lenp)
        *lenp = orig_dmax - dmax;
#ifdef SAFECLIB_STR_NULL_SLACK
    memset(dest, 0, dmax);
#else
    *dest = 0;
#endif
    return EOK;
}

/**
 * @def u8norm_reorder_s(dest,dmax,src,len)
 * @brief
 *    Reorder all decomposed sequences in a utf-8 string to NFD,
 *    as defined in the latest Unicode standard, latest 10.0. The conversion
 *    stops at the first null or after dmax characters.
 *
 * @param[out]  dest      utf-8 string to hold the result
 * @param[in]   dmax      maximum result buffer size
 * @param[in]   src       utf-8 string to be converted
 * @param[in]   len       length of src
 *
 * @pre  dest and src shall not be null pointers.
 * @pre  dmax shall not equal zero and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then \c u8norm_reorder_s nulls
 * dest.
 * @retval  EOK        on success
 * @retval  ESNOSPC    when dmax too small for the result buffer
 * @retval  EOF        on some normalization error
 *
 * @see
 *    u8norm_s(), u8norm_decompose_s(), ICU, gnulib/libunistring, utf8proc
 */

/* reorder decomposed sequence to NFD */
#ifdef FOR_DOXYGEN
errno_t u8norm_reorder_s(char8_t *restrict dest, rsize_t dmax, const char8_t *restrict src,
                          const rsize_t len)
#else
EXPORT errno_t _u8norm_reorder_s_chk(char8_t *restrict dest, rsize_t dmax,
                                     const char8_t *restrict src, const rsize_t len,
                                     const size_t destbos)
#endif
{
    UN8IF_cc seq_ary[CC_SEQ_SIZE];
    UN8IF_cc *seq_ptr = (UN8IF_cc *)seq_ary; /* start with stack */
    UN8IF_cc *seq_ext = NULL;                /* heap when needed */
    size_t seq_max = CC_SEQ_SIZE;
    size_t cc_pos = 0;
    char *p = (char *)src;
    const char *e = p + len;
    char *orig_dest = (char*)dest;
    rsize_t orig_dmax = dmax;

    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8norm_reorder_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR_CLEAR("u8norm_reorder_s", destbos)
    }

    while (p < e) {
        uint8_t cur_cc;
        uint32_t cp = dec_utf8((char8_t**)&p);

        cur_cc = _combin_class(cp);
        if (cur_cc != 0) {
            if (seq_max < cc_pos + 1) {         /* extend if need */
                seq_max = cc_pos + CC_SEQ_STEP; /* new size */
                if (CC_SEQ_SIZE == cc_pos) {    /* seq_ary full */
                    seq_ext = (UN8IF_cc *)malloc(seq_max * sizeof(UN8IF_cc));
                    memcpy(seq_ext, seq_ary, cc_pos * sizeof(UN8IF_cc));
                } else {
                    seq_ext = (UN8IF_cc *)realloc(seq_ext,
                                                  seq_max * sizeof(UN8IF_cc));
                }
                seq_ptr = seq_ext; /* use seq_ext from now */
            }

            seq_ptr[cc_pos].cc = cur_cc;
            seq_ptr[cc_pos].cp = cp;
            seq_ptr[cc_pos].pos = cc_pos;
            ++cc_pos;

            if (p < e)
                continue;
        }

        /* output */
        if (cc_pos) {
            size_t i;

            if (unlikely(dmax - cc_pos <= 0)) {
                handle_error(orig_dest, orig_dmax,
                              "u8norm_reorder_s: "
                              "dmax too small",
                              ESNOSPC);
                return RCNEGATE(ESNOSPC);
            }

            if (cc_pos > 1) /* reorder if there are two Combining Classes */
                qsort((void *)seq_ptr, cc_pos, sizeof(UN8IF_cc), _compare_cc);

            for (i = 0; i < cc_pos; i++) {
              //enc_utf8(dest, dmax, seq_ptr[i].cp);
            }
            cc_pos = 0;
        }

        if (cur_cc == 0) {
          //_ENC_W16(dest, dmax, cp);
        }

        if (unlikely(!dmax)) {
            handle_error(orig_dest, orig_dmax,
                          "u8norm_reorder_s: "
                          "dmax too small",
                          ESNOSPC);
            return RCNEGATE(ESNOSPC);
        }
    }
    if (seq_ext)
        free(seq_ext);
        /* surrogate pairs can actually collapse */
#if defined(SAFECLIB_STR_NULL_SLACK)
    memset(dest, 0, dmax);
#else
    *dest = 0;
#endif
    return EOK;
}

/**
 * @def u8norm_compose_s(dest,dmax,src,lenp,iscontig)
 * @brief
 *    Combine all decomposed sequences in a utf-8 string to NFC,
 *    as defined in the latest Unicode standard, latest 13.0. The conversion
 *    stops at the first null or after dmax characters.
 *
 * @param[out]  dest      utf-8 string to hold the result
 * @param[in]   dmax      maximum result buffer size
 * @param[in]   src       utf-8 string to be converted
 * @param[out]  lenp      pointer to length of p and the result length.
 * @param[in]   iscontig  if true, the result will only be a fast FCC
 *
 * @pre  dest, src and lenp shall not be null pointers.
 * @pre  dmax shall not equal zero and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then \c u8norm_reorder_s nulls
 * dest.
 * @retval  EOK        on success
 * @retval  ESNOSPC    when dmax too small for the result buffer
 * @retval  EOF        on some normalization error
 *
 * @see
 *    u8norm_s(), u8norm_decompose_s(), ICU, gnulib/libunistring, utf8proc
 */

/* combine decomposed sequences to NFC. */
/* iscontig = false; composeContiguous? FCC if true */
#ifdef FOR_DOXYGEN
errno_t u8norm_compose_s(char8_t *restrict dest, rsize_t dmax,
                          const char8_t *restrict src,
                          rsize_t *restrict lenp,
                          const bool iscontig)
#else
EXPORT errno_t _u8norm_compose_s_chk(char8_t *restrict dest, rsize_t dmax,
                                      const char8_t *restrict src,
                                      rsize_t *restrict lenp,
                                      const bool iscontig,
                                      const size_t destbos)
#endif
{
    char *p = (char *)src;
    const char *e = p + *lenp;
    uint32_t cpS = 0;       /* starter code point */
    bool valid_cpS = false; /* if false, cpS isn't initialized yet */
    uint8_t pre_cc = 0;

    uint32_t seq_ary[CC_SEQ_SIZE];
    uint32_t *seq_ptr = (uint32_t *)seq_ary; /* either stack or heap */
    uint32_t *seq_ext = NULL;                /* heap */
    size_t seq_max = CC_SEQ_SIZE;
    size_t cc_pos = 0;
    char *orig_dest = (char*)dest;
    rsize_t orig_dmax = dmax;

    if (destbos == BOS_UNKNOWN) {
        if (unlikely(dmax > RSIZE_MAX_STR)) {
            *lenp = 0;
            invoke_safe_str_constraint_handler("u8norm_compose_s: "
                                               "dmax exceeds max",
                                               (void *)dest, ESLEMAX);
            return ESLEMAX;
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        const size_t destsz = dmax;
        if (unlikely(destsz > destbos)) {
            *lenp = 0;
            handle_error((char*)dest, destbos,
                          "u8norm_compose_s: dmax exceeds dest", EOVERFLOW);
            return EOVERFLOW;
        }
    }

    while (p < e) {
        uint8_t cur_cc;
        uint32_t cp = dec_utf8((char8_t**)&p);

        cur_cc = _combin_class(cp);

        if (!valid_cpS) {
            if (cur_cc == 0) {
                cpS = cp; /* found the first Starter */
                valid_cpS = true;
                if (p < e)
                    continue;
            } else {
                //_ENC_W16(dest, dmax, cp);
                if (unlikely(!dmax)) {
                    handle_error(orig_dest, orig_dmax,
                                  "u8norm_compose_s: "
                                  "dmax too small",
                                  ESNOSPC);
                    return RCNEGATE(ESNOSPC);
                }
                continue;
            }
        } else {
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
                uint32_t cpComp = _composite_cp(cpS, cp);

                if (cpComp && !isExclusion(cpComp)) {
                    cpS = cpComp;
                    composed = true;

                    /* pre_cc should not be changed to cur_cc */
                    /* e.g. 1E14 = 0045 0304 0300 where CC(0304) == CC(0300) */
                    if (p < e)
                        continue;
                } else
                    composed = false;
            }

            if (!composed) {
                pre_cc = cur_cc;
                if (cur_cc != 0 || !(p < e)) {
                    if (seq_max < cc_pos + 1) {         /* extend if need */
                        seq_max = cc_pos + CC_SEQ_STEP; /* new size */
                        if (CC_SEQ_SIZE == cc_pos) {    /* seq_ary full */
                            seq_ext =
                                (uint32_t *)malloc(seq_max * sizeof(uint32_t));
                            memcpy(seq_ext, seq_ary, cc_pos * sizeof(uint32_t));
                        } else {
                            seq_ext = (uint32_t *)realloc(
                                seq_ext, seq_max * sizeof(uint32_t));
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
        //_ENC_W16(dest, dmax, cpS); /* starter (composed or not) */
        if (unlikely(!dmax)) {
            handle_error(orig_dest, orig_dmax,
                          "u8norm_compose_s: "
                          "dmax too small",
                          ESNOSPC);
            return RCNEGATE(ESNOSPC);
        }

        if (cc_pos == 1) {
            //_ENC_W16(dest, dmax, *seq_ptr);
            cc_pos = 0;
        } else if (cc_pos > 1) {
            memcpy(dest, seq_ptr, cc_pos * sizeof(char));
            dest += cc_pos;
            dmax -= cc_pos;
            cc_pos = 0;
        }

        cpS = cp;
    }
    if (seq_ext)
        free(seq_ext);

#ifdef SAFECLIB_STR_NULL_SLACK
    memset(dest, 0, dmax);
#else
    *dest = 0;
#endif
    *lenp = orig_dmax - dmax;
    return EOK;
}

/**
 * @def u8norm_s(dest,dmax,src,mode,lenp)
 * @brief
 *    Converts the utf-8 string to the canonical NFC or NFD normalization,
 *    as defined in the latest Unicode standard, latest 13.0.  The conversion
 *    stops at the first null or after dmax characters.
 *
 * @details
 *    The default mode should always be NFD.
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
 * @param[out]  dest  utf-8 string to hold the result
 * @param[in]   dmax  maximum length of string
 * @param[in]   src   utf-8 string
 * @param[in]   mode  convert to nfc or just nfd.
 *                    experimentally to fast modes FCD or FCC.
 *                    optionally to compat modes NFKD, NFKC with \c
 *                    --enable-norm-compat
 *                    @see enum \c u8norm_mode.
 * @param[out]  lenp  pointer to length of the result, may be NULL
 *
 * @pre  dest and src shall not be null pointers.
 * @pre  dmax shall not equal zero and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, then if dest
 *          is not a null pointer and dmax is greater than zero and
 *          not greater than RSIZE_MAX_STR, then u8norm_s nulls dest.
 * @retval  EOK        on success
 * @retval  ESNULLP    when dest or src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMIN    when dmax < 5
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  ESOVRLP    when buffers overlap
 * @retval  ESNOSPC    when dmax too small for the result buffer
 * @retval  EOF        any other normalization error
 *
 * @see
 *    u8fc_s(), ICU, gnulib/libunistring, utf8proc
 */

/* Normalize to NFC, NFD, FCC, FCD (fastest, used in u8fc_s), and
 * optionally NFKD, NFKC */
#ifdef FOR_DOXYGEN
errno_t u8norm_s(char8_t *restrict dest, rsize_t dmax, const char8_t *restrict src,
                 const wcsnorm_mode_t mode, rsize_t *restrict lenp)
#else
EXPORT errno_t _u8norm_s_chk(char8_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src,
                             const wcsnorm_mode_t mode, rsize_t *restrict lenp,
                             const size_t destbos)
#endif
{
    char8_t tmp_stack[128];
    char8_t *tmp_ptr;
    char8_t *tmp = NULL;
    rsize_t len;
    const bool iscompat =
        mode & WCSNORM_NFKD; /* WCSNORM_NFKD or WCSNORM_NFKC */

    errno_t rc =
        _u8norm_decompose_s_chk(dest, dmax, src, &len, iscompat, destbos);
    if (lenp)
        *lenp = len;
    if (unlikely(rc))
        return rc;
    if (unlikely(mode == WCSNORM_FCD))
        return EOK;

    /* temp. scratch space, on stack or heap */
    if (len + 2 < 128)
        tmp_ptr = tmp_stack;
    else
        tmp_ptr = tmp = (char8_t *)malloc((len + 2));

    rc = _u8norm_reorder_s_chk(tmp_ptr, len + 2, dest, len, destbos);
    if (unlikely(rc)) {
        if (tmp)
            free(tmp);
#ifdef SAFECLIB_STR_NULL_SLACK
        memset(dest, 0, dmax);
#else
        *dest = L'\0';
#endif
        return (rc);
    }
    if (mode == WCSNORM_NFD || mode == WCSNORM_NFKD) {
        memcpy(dest, tmp_ptr, len + 1);
        if (tmp)
            free(tmp);
        return (EOK);
    }

    rc = _u8norm_compose_s_chk(dest, dmax, tmp_ptr, &len, mode == WCSNORM_FCC,
                                destbos);
    if (tmp)
        free(tmp);
    if (unlikely(rc))
        return (rc);
    if (lenp)
        *lenp = len;

    return (EOK);
}
