/*------------------------------------------------------------------
 * u8glen_s.c
 *
 * September 2020, Reini Urban (WIP)
 *
 * Copyright (c) 2020 by Reini Urban
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
#endif

#include "extu8/u8_private.h"
#include "extu8/u8gbreaks.h"

static int _bsearch_gbreak(const void *ptr1, const void *ptr2) {
  const struct _u8_gbreak_t *e1 = (const struct _u8_gbreak_t *)ptr1;
  const struct _u8_gbreak_t *e2 = (const struct _u8_gbreak_t *)ptr2;
  return e1->gbreak > e2->gbreak ? 1 : e1->gbreak == e2->gbreak ? 0 : -1;
}

static _u8_gbreaks_t _u8_gbreak(const uint32_t cp) {
  struct _u8_gbreak_t* e = (struct _u8_gbreak_t *)bsearch
    (&cp, &_u8_gbreaks, sizeof(_u8_gbreaks),
     sizeof(_u8_gbreaks[0]), _bsearch_gbreak);
  return e ? e->gbreak : _U8_GBREAK_NONE;
}

/**
 * @def u8glen_s(str,smax)
 * @brief
 *    The u8glen_s function computes the number of "graphemes" of the utf-8 string pointed
 *    to by str, stopping at smax bytes of str. This is equivalent to the number of extended
 *    grapheme clusters, the visual length of characters of the string. See
 *    the Unicode Text Segmentation Annex TR29.
 *
 * @param  str   pointer to utf-8 string
 * @param  smax  maximum byte-length of the given utf-8 string, incl. the final \0.
 *
 * @pre  str shall not be a null pointer.
 * @pre  smax shall not equal zero.
 * @pre  smax shall not be greater than RSIZE_MAX_STR and the size of str
 *       (inc. final null).
 *
 * @remark SPECIFIED IN
 *   * Unicode Text Segmentation Annex TR29
 *     http://www.unicode.org/reports/tr29/tr29-29.html
 *
 * @return The function returns the visual utf-8 string length, excluding the
 * terminating null character.  If \c str is NULL, then \c u8glen_s returns
 * 0. Otherwise, the \c u8glen_s function returns the number of extended grapheme
 * clusters that precede the terminating null character. (Not legacy).
 * If there is no null character in the first \c smax characters of str then \c u8glen_s
 * returns \c smax. At most the first \c smax characters of str are accessed
 * by \c u8glen_s.
 *
 * @see
 *    u8nlen_s()
 */
#ifdef FOR_DOXYGEN
rsize_t u8glen_s(const char8_t *str, rsize_t smax)
#else
EXPORT rsize_t _u8glen_s_chk(const char8_t *str, rsize_t smax, size_t strbos)
#endif
{
    //const char8_t *z;
    //rsize_t orig_smax = smax;

    if (unlikely(str == NULL)) {
        return RCNEGATE(0);
    }
    if (unlikely(smax == 0)) {
        invoke_safe_str_constraint_handler("u8glen_s: smax is 0", (void *)str,
                                           ESZEROL);
        return RCNEGATE(0);
    }
    if (unlikely(smax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("u8glen_s: smax exceeds max",
                                           (void *)str, ESLEMAX);
        return RCNEGATE(0);
    }
#if defined(HAVE_WARN_DMAX) || defined(HAVE_ERROR_DMAX) ||                     \
    defined(HAVE___BND_CHK_PTR_BOUNDS)
    if (strbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(str, smax);
#if defined(HAVE_WARN_DMAX) || defined(HAVE_ERROR_DMAX)
    } else {
        if (unlikely(smax != strbos)) {
            handle_str_bos_chk_warn("u8glen_s", (char *)str, smax, strbos);
            RETURN_ESLEWRNG;
        }
#endif
    }
#endif

    if (strbos != BOS_UNKNOWN && smax > strbos) {
      smax = strbos; // use the real length
    }
    {
      // TODO get neighboring char boundary classes (Grapheme_Cluster_Break) and compare them.
      char8_t *p = (char8_t *)str;
      _u8_gbreaks_t b1, b2;
      rsize_t c = 0;
      uint32_t cp1 = dec_utf8 (&p);
      if (!cp1)
        return 0;
      smax -= (p - str);
      c++; // GB1: start-of-text ÷ Any
      b1 = _u8_gbreak (cp1);
      /* Don't touch past smax */
      for (; smax > 0 && *p != 0; b1 = b2) {
        uint32_t cp2;
        const char8_t *z = p;
        if (!*p)
          return c;
        /* FIXME Normalize to NFD on the fly */
        cp2 = dec_utf8 (&p);
        smax -= (p - z);
        b2 = _u8_gbreak (cp2);
        // TODO: pre-compile counting valid state changes into bitmatrix?
        // "Do not break between a CR and LF. Otherwise, break before and after controls."
        if (b1 == _U8_GBREAK_CR && b2 == _U8_GBREAK_LF) // GB3: CR × LF
          continue;
        else if (b1 >= _U8_GBREAK_CONTROL && b1 <= _U8_GBREAK_CR) // GB4: CONTROL|CR|LF ÷
          {
            c++; continue;
          }
        else if (b2 >= _U8_GBREAK_CONTROL && b2 <= _U8_GBREAK_CR) // GB5: ÷ CONTROL|CR|LF
          {
            c++; continue;
          }
        // "Do not break Hangul syllable sequences."
        else if (b1 == _U8_GBREAK_L &&                           // GB6: L × L|V|LV|LVT
                 (b2 == _U8_GBREAK_L || b2 == _U8_GBREAK_V || b2 == _U8_GBREAK_LV ||
                  b2 == _U8_GBREAK_LVT))
          continue;
        else if ((b1 == _U8_GBREAK_LV || b1 == _U8_GBREAK_V) &&  // GB7: LV|V × V|T
                 (b2 == _U8_GBREAK_V || b2 == _U8_GBREAK_T))
          continue;
        else if ((b1 == _U8_GBREAK_LVT || b1 == _U8_GBREAK_T) && // GB8: LV|V × V|T
                 (b2 == _U8_GBREAK_V || b2 == _U8_GBREAK_T))
          continue;
        // "Do not break before extending characters or ZWJ."
        else if (b2 == _U8_GBREAK_EXTEND || b2 == _U8_GBREAK_ZWJ) // GB9: × Extend|ZWJ
          continue;
        // "Do not break before SpacingMarks, or after Prepend characters."
        else if (b2 == _U8_GBREAK_SPACINGMARK) // GB9a: × SpacingMark
          continue;
        else if (b1 == _U8_GBREAK_PREPEND)     // GB9b: Prepend ×
          continue;
#if 0
        // "Do not break within emoji flag sequences. That is, do not break between
        // regional indicator (RI) symbols if there is an odd number of RI characters
        // before the break point."
        else if (b1 == _U8_GBREAK_RI && b2 == _U8_GBREAK_RI) {
          // TODO GB12: ^(RI RI)* RI × RI
          // TODO GB13: [^RI] (RI RI)* RI × RI
          continue;
        }
#endif
        c++; // GB999: "Otherwise, break everywhere"
      }
      // GB2: Any ÷ end-of-text
      //if (c && (!smax || !*p))
      //  c++;
      return c;
    }
}
