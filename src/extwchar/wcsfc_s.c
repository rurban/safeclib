/*------------------------------------------------------------------
 * wcsfc_s.c
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
#include <locale.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h> /* DEBUG */
#endif
#include <assert.h>
#endif

/* with lithuanian only grave, acute, tilde above, and ogonek
   See http://unicode.org/reports/tr21/tr21-5.html#SpecialCasing */
static inline int _is_lt_accented(uint32_t wc) {
    return (wc == 0x300 || /* grave */
            wc == 0x301 || /* acute */
            wc == 0x303 || /* tilde */
            wc == 0x328)   /* ogonek */
               ? 1
               : 0;
}

/**
 * @def wcsfc_s(dest,dmax,src,lenp)
 * @brief
 *    Converts the wide string via full case-folding NFD normalized to
 * lowercase. The conversion stops at the first null or after dmax characters.
 * The conversion is determined by the LC_CTYPE category setting of the locale.
 * Other characters are not affected. fold-case performs full case folding,
 * i.e. if iswfc() of a character > 1, the length of dest might be greater than
 * the length of src (max 4 per char), the conversion is done via \c towfc_s()
 * and Unicode 10.0, the Unicode special-casing rules are obeyed, and composed
 * characters are normalized to NFD via \c wcsnorm_decompose_s() and \c
 * wcsnorm_reorder_s(). If not, the conversion is per character done via normal
 * \c towlower().  Note that decomposition creates larger strings, typically
 * 2-3 chars more.
 *
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the
 *    array of dmax characters pointed to by dest are nulled.
 *
 * @details
 *   SpecialCasing checks for conditional boundary context at the begin or end
 *   of certain characters (final greek sigma), and locale sensitive rules
 *   for the Lithuanian and the Turkish/Azeri I-dot.
 *
 *   Composed characters are checked for the left-hand-side of the
 *   Decomposition_Mapping Unicode property, which means the codepoint will be
 *   normalized to NFD if any codepoint is composed. Technically only FCD as
 * all FC expansions are already properly ordered, and all mangled marks will
 * not be reordered, as the have the same Combining Class.
 *
 * @param[out]  dest  wide string to hold the result (~130% larger than src)
 * @param[in]   dmax  maximum result buffer size
 * @param[in]   src   wide string
 * @param[out]  lenp  pointer to length of the result, may be NULL
 *
 * @pre  dest and src shall not be null pointers.
 * @pre  dmax shall not be smaller than 5 and big enough for dest.
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR and size of dest.
 *
 * @retval  EOK         on successful operation
 * @retval  ESNULLP     when dest or src is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_WSTR or a cp > 0x10ffff
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOSPC     when dmax is too small for the decomposition.
 *                      *lenp is still written, to know how much space is
 * needed.
 * @retval  ESNOTFND    Internal error as returned by towfc_s() for multi-char
 * foldings. happens only when the internal implementations of iswfc() and
 *                      towfc_s() are mismatched.
 *
 * @see
 *    iswfc(), towfc_s(), towupper(), wcslwr_s(), wcsupr_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t wcsfc_s(wchar_t *restrict dest, rsize_t dmax,
                const wchar_t *restrict src, rsize_t *restrict lenp)
#else
EXPORT errno_t _wcsfc_s_chk(wchar_t *restrict dest, rsize_t dmax,
                            const wchar_t *restrict src, rsize_t *restrict lenp,
                            const size_t destbos)
#endif
{
    wchar_t *orig_dest;
    rsize_t orig_dmax;
    int is_lithuanian = 0;
    int is_tr_az = 0;

    if (lenp)
        *lenp = 0;
    CHK_DEST_NULL("wcsfc_s")
    CHK_DMAX_ZERO("wcsfc_s")
    CHK_DMAX_MAX("wcsfc_s", RSIZE_MAX_WSTR)
    if (destbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(dest, dmax * sizeof(wchar_t));
    } else {
        const size_t destsz = dmax * sizeof(wchar_t);
        CHK_DESTW_OVR_CLEAR("wcsfc_s", destsz, destbos)
    }
    if (unlikely(src == NULL)) {
        handle_werror(dest, dmax,
                      "wcsfc_s: "
                      "src is null",
                      ESNULLP);
        return (ESNULLP);
    }

    if (unlikely((*src >= 0x49 && *src <= 0x130) &&
                 (*src == 0x49 || *src == 0x4A || *src == 0x130 ||
                  *src == 0xcc || *src == 0xcd || *src == 0x128 ||
                  *src == 0x12e))) {
        const char *lc = setlocale(LC_CTYPE, NULL);
        if (lc && *lc) {
            if ((*lc == 't' && *(lc + 1) == 'r') ||
                (*lc == 'a' && *(lc + 1) == 'z'))
                is_tr_az = 1;
            else if (*lc == 'l' && *(lc + 1) == 't')
                is_lithuanian = 1;
        }
    }

    orig_dest = dest;
    orig_dmax = dmax;

    while (*src && dmax > 0) {
        wchar_t tmp[4];
        uint32_t cp = _dec_w16((wchar_t *)src);
        int c = iswfc(cp);
#if SIZEOF_WCHAR_T == 2
        if (cp > 0xffff)
            src++;
#endif
        if (unlikely(c > 1)) {
            /* can this be further decomposed? */
            errno_t rc = towfc_s(tmp, 4, cp);
            if (rc < 0)
                return rc;
            /* I-Dot for Turkish and Azeri */
            if (unlikely(is_tr_az && *src == 0x130)) {
                *dest++ = 0x69;
                dmax--; /* skip the \x307 dot */
            } else if (unlikely(*src >= 0x1f80 && *src <= 0x1ff4)) {
                /* only some greek PROSGEGRAMMENI + YPOGEGRAMMENI need to be
                 * decomposed further */
                /* decompose mult. chars */
                wchar_t tmpd[8];
                int i;
                for (i = 0; i < c; i++) {
                    int d;
                    uint32_t cp1 = _dec_w16(&tmp[i]);
#if SIZEOF_WCHAR_T > 2
                    if (unlikely(_UNICODE_MAX < cp1)) {
                        handle_werror(orig_dest, orig_dmax,
                                      "wcsfc_s: "
                                      "cp is too high",
                                      ESLEMAX);
                        return ESLEMAX;
                    }
#endif
                    d = _decomp_s(tmpd, 8, cp1, false);
                    if (d) { /* decomp. max 4 */
                        memcpy(dest, tmpd, d * sizeof(wchar_t));
                        dest += d;
                        dmax -= d;
                    } else {
                        _ENC_W16(dest, dmax, cp1);
                    }
                }
            } else {
                memcpy(dest, tmp, c * sizeof(wchar_t));
                dest += c;
                dmax -= c;
            }
            src++;
        } else { /* c = 0 or 1. 1 might still be special case */
            if (unlikely(*src == 0x3a3)) {
                if (iswspace(*(src + 1))) /* final sigma? */
                    *dest++ = 0x3c2;
                else
                    *dest++ = 0x3c3;
                src++;
                dmax--;
            } else if (unlikely(is_lithuanian)) {
                /* I-Dot/J-Dot for Lithuanian, I-Dot for Turkish and Azeri.
                   http://unicode.org/reports/tr21/tr21-5.html#SpecialCasing */
                switch (*src) {
                case 0xcc:
                    *dest++ = 0x69;
                    *dest++ = 0x307;
                    *dest++ = 0x300;
                    dmax -= 3;
                    break;
                case 0xcd:
                    *dest++ = 0x69;
                    *dest++ = 0x307;
                    *dest++ = 0x301;
                    dmax -= 3;
                    break;
                case 0x128:
                    *dest++ = 0x69;
                    *dest++ = 0x307;
                    *dest++ = 0x303;
                    dmax -= 3;
                    break;
                case 0x49: /* Add the dot with accented I's or J's */
                case 0x4A:
                    /* lt More_Above */
                    if (_is_lt_accented(*(src + 1))) {
                        /*towfc_s(tmp, 4, 0x0130);*/
                        *dest++ = (*src + 0x20);
                        *dest++ = 0x307;
                        dmax -= 2;
                        src++;
                        break;
                    }
                    /* FALLTHRU */
                case 0x12e:
                    if (_is_lt_accented(*(src + 1))) {
                        *dest++ = 0x12f;
                        *dest++ = 0x307;
                        dmax -= 2;
                        src++;
                        break;
                    }
                    /* FALLTHRU */
                default:
                    goto is_single;
                }
                src++;
            } else if (unlikely(is_tr_az)) {
                /* pair I-Dot with i for Turkish and Azeri.
                   http://unicode.org/reports/tr21/tr21-5.html#SpecialCasing */
                if (*src == 0x130) {
                    *dest++ = 0x307;
                    src++;
                    dmax--;
                }
                /* Remove dot_above in the sequence
                   I + dot_above, which will turn into i */
                else if (*src == L'I' && *(src + 1) == 0x307) {
                    *dest++ = L'i';
                    dmax--;
                    src++;
                    src++;
                }
                /* Unless an I is before a dot_above, it turns
                   into a dotless i. */
                else if (*src == 0x49 && *(src + 1) != 0x307 &&
                         *(src + 1) != 0x130) {
                    *dest++ = 0x131;
                    src++;
                    dmax--;
                } else {
                is_single:
                    (void)_towfc_single(dest, _dec_w16((wchar_t *)src));
                    src++;
                    /* even if not found dest[0] still contains towlower */
                    dest++;
                    dmax--;
                }
            } else {
                if (unlikely(dmax < 5))
                    goto too_small;
#if 1
                (void)_towfc_single(tmp, _dec_w16((wchar_t *)src));
                src++;
                if (tmp[0] >= 0xc0) {
                    cp = _dec_w16(tmp);
                    /* needed? I thought only 0x1f80-0x1ff4 needs to be
                     * decomposed */
                    c = _decomp_s(dest, dmax, cp, false);
                } else
                    c = 0;
                if (!c) {
                    *dest++ = tmp[0];
                    dmax--;
                } else {
                    /* eg accents 101 => 61 304 */
                    dest += c;
                    dmax -= c;
                }
#else
                c = _towfc_single(dest, _dec_w16((wchar_t *)src));
                if (c > 0) {
                    dest += c;
                    dmax -= c;
                }
                src++;
#endif
            }
        }
    }
    /* write the length even if too small,
       so the client knows how much is needed */
    if (lenp)
        *lenp = orig_dmax - dmax;
    if (unlikely(dmax <= 0)) {
    too_small:
        handle_werror(orig_dest, orig_dmax,
                      "wcsfc_s: "
                      "dmax too small",
                      ESNOSPC);
        return (ESNOSPC);
    }

#ifdef SAFECLIB_STR_NULL_SLACK
    memset(dest, 0, dmax * sizeof(wchar_t));
    /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#else
    *dest = 0;
#endif

    return (EOK);
}
