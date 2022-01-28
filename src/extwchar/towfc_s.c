/*------------------------------------------------------------------
 * towfc_s.c
 *
 * September 2017, Reini Urban
 * January 2022, Reini Urban
 *
 * Copyright (c) 2017,2022 by Reini Urban
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
#include <ctype.h>
#include <wctype.h>
#endif

/**
 * @brief
 *    \b iswfc() checks the uppercase character for a mapping to foldcase,
 *    returning the number of new wide character codepoints needed.
 *    The usual \c iswupper(wc) case returns 1, and the special 104 full
 *    folding cases as specified in Unicode 10.0 \c CaseFolding.txt return either
 *    2 or 3. This implements Unicode 14.0
 *
 * @param[in]   wc  unicode character codepoint
 *
 * @retval  0   when there is no corresponding lowercase mapping.
 * @retval  1   when there is a corresponding common or simple foldcase
 *              mapping.
 * @retval  2   a full case-folding expands to 2 characters
 * @retval  3   a full case-folding expands to 3 characters
 *
 * @see
 *    towfc_s(), wcsfc_s(), towupper()
 */

/* Unicode 10.0 has 297 code points for which the fold case mapping is not to
 * itself, or to tolower(), 194 of it return length 1, the rest 102 map to 2 or
 * 3 and are handled below.
 */

/* created via test_towfc_s() */
static const struct {
    unsigned short upper;
    unsigned short lower1;
    unsigned short lower2;
} tbl2[] = {
    // clang-format off
    {0x00DF, 0x0073, 0x0073}, /* LATIN SMALL LETTER SHARP S => ss */
    {0x0130, 0x0069, 0x0307}, /* LATIN CAPITAL LETTER I WITH DOT ABOVE */
    {0x0149, 0x02BC, 0x006E}, /* LATIN SMALL LETTER N PRECEDED BY APOSTROPHE */
    {0x01F0, 0x006A, 0x030C}, /* LATIN SMALL LETTER J WITH CARON */
    {0x0587, 0x0565, 0x0582}, /* ARMENIAN SMALL LIGATURE ECH YIWN */
    {0x1E96, 0x0068, 0x0331}, /* LATIN SMALL LETTER H WITH LINE BELOW */
    {0x1E97, 0x0074, 0x0308}, /* LATIN SMALL LETTER T WITH DIAERESIS */
    {0x1E98, 0x0077, 0x030A}, /* LATIN SMALL LETTER W WITH RING ABOVE */
    {0x1E99, 0x0079, 0x030A}, /* LATIN SMALL LETTER Y WITH RING ABOVE */
    {0x1E9A, 0x0061, 0x02BE}, /* LATIN SMALL LETTER A WITH RIGHT HALF RING */
    {0x1E9E, 0x0073, 0x0073}, /* LATIN CAPITAL LETTER SHARP S => ss */
    {0x1F50, 0x03C5, 0x0313}, /* GREEK SMALL LETTER UPSILON WITH PSILI */
    /* the following folding mappings need to be normalized to NFD */
    {0x1F80, 0x1F00, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH PSILI AND YPOGEGRAMMENI */
    {0x1F81, 0x1F01, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH DASIA AND YPOGEGRAMMENI */
    {0x1F82, 0x1F02, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH PSILI AND VARIA
                                 AND YPOGEGRAMMENI */
    {0x1F83, 0x1F03, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH DASIA AND VARIA
                                 AND YPOGEGRAMMENI */
    {0x1F84, 0x1F04, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH PSILI AND OXIA
                                 AND YPOGEGRAMMENI */
    {0x1F85, 0x1F05, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH DASIA AND OXIA
                                 AND YPOGEGRAMMENI */
    {0x1F86, 0x1F06, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH PSILI AND
                                 PERISPOMENI AND YPOGEGRAMMENI */
    {0x1F87, 0x1F07, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH DASIA AND
                                 PERISPOMENI AND YPOGEGRAMMENI */
    {0x1F88, 0x1F00, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI */
    {0x1F89, 0x1F01, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH DASIA AND PROSGEGRAMMENI */
    {0x1F8A, 0x1F02, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH PSILI AND
                                 VARIA AND PROSGEGRAMMENI */
    {0x1F8B, 0x1F03, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH DASIA AND
                                 VARIA AND PROSGEGRAMMENI */
    {0x1F8C, 0x1F04, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH PSILI AND OXIA
                                 AND PROSGEGRAMMENI */
    {0x1F8D, 0x1F05, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH DASIA AND OXIA
                                 AND PROSGEGRAMMENI */
    {0x1F8E, 0x1F06, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH PSILI AND
                                 PERISPOMENI AND PROSGEGRAMMENI */
    {0x1F8F, 0x1F07, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH DASIA AND
                                 PERISPOMENI AND PROSGEGRAMMENI */
    {0x1F90, 0x1F20, 0x03B9}, /* GREEK SMALL LETTER ETA WITH PSILI AND YPOGEGRAMMENI */
    {0x1F91, 0x1F21, 0x03B9}, /* GREEK SMALL LETTER ETA WITH DASIA AND YPOGEGRAMMENI */
    {0x1F92, 0x1F22, 0x03B9}, /* GREEK SMALL LETTER ETA WITH PSILI AND VARIA
                                 AND YPOGEGRAMMENI */
    {0x1F93, 0x1F23, 0x03B9}, /* GREEK SMALL LETTER ETA WITH DASIA AND VARIA
                                 AND YPOGEGRAMMENI */
    {0x1F94, 0x1F24, 0x03B9}, /* GREEK SMALL LETTER ETA WITH PSILI AND OXIA AND
                                 YPOGEGRAMMENI */
    {0x1F95, 0x1F25, 0x03B9}, /* GREEK SMALL LETTER ETA WITH DASIA AND OXIA AND
                                 YPOGEGRAMMENI */
    {0x1F96, 0x1F26, 0x03B9}, /* GREEK SMALL LETTER ETA WITH PSILI AND
                                 PERISPOMENI AND YPOGEGRAMMENI */
    {0x1F97, 0x1F27, 0x03B9}, /* GREEK SMALL LETTER ETA WITH DASIA AND
                                 PERISPOMENI AND YPOGEGRAMMENI */
    {0x1F98, 0x1F20, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH PSILI AND PROSGEGRAMMENI */
    {0x1F99, 0x1F21, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH DASIA AND PROSGEGRAMMENI */
    {0x1F9A, 0x1F22, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH PSILI AND VARIA
                                 AND PROSGEGRAMMENI */
    {0x1F9B, 0x1F23, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH DASIA AND VARIA
                                 AND PROSGEGRAMMENI */
    {0x1F9C, 0x1F24, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH PSILI AND OXIA
                                 AND PROSGEGRAMMENI */
    {0x1F9D, 0x1F25, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH DASIA AND OXIA
                                 AND PROSGEGRAMMENI */
    {0x1F9E, 0x1F26, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH PSILI AND
                                 PERISPOMENI AND PROSGEGRAMMENI */
    {0x1F9F, 0x1F27, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH DASIA AND
                                 PERISPOMENI AND PROSGEGRAMMENI */
    {0x1FA0, 0x1F60, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH PSILI AND YPOGEGRAMMENI */
    {0x1FA1, 0x1F61, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH DASIA AND YPOGEGRAMMENI */
    {0x1FA2, 0x1F62, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH PSILI AND VARIA
                                 AND YPOGEGRAMMENI */
    {0x1FA3, 0x1F63, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH DASIA AND VARIA
                                 AND YPOGEGRAMMENI */
    {0x1FA4, 0x1F64, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH PSILI AND OXIA
                                 AND YPOGEGRAMMENI */
    {0x1FA5, 0x1F65, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH DASIA AND OXIA
                                 AND YPOGEGRAMMENI */
    {0x1FA6, 0x1F66, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH PSILI AND
                                 PERISPOMENI AND YPOGEGRAMMENI */
    {0x1FA7, 0x1F67, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH DASIA AND
                                 PERISPOMENI AND YPOGEGRAMMENI */
    {0x1FA8, 0x1F60, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH PSILI AND PROSGEGRAMMENI */
    {0x1FA9, 0x1F61, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH DASIA AND PROSGEGRAMMENI */
    {0x1FAA, 0x1F62, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH PSILI AND
                                 VARIA AND PROSGEGRAMMENI */
    {0x1FAB, 0x1F63, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH DASIA AND
                                 VARIA AND PROSGEGRAMMENI */
    {0x1FAC, 0x1F64, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH PSILI AND OXIA
                                 AND PROSGEGRAMMENI */
    {0x1FAD, 0x1F65, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH DASIA AND OXIA
                                 AND PROSGEGRAMMENI */
    {0x1FAE, 0x1F66, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH PSILI AND
                                 PERISPOMENI AND PROSGEGRAMMENI */
    {0x1FAF, 0x1F67, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH DASIA AND
                                 PERISPOMENI AND PROSGEGRAMMENI */
    {0x1FB2, 0x1F70, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH VARIA AND YPOGEGRAMMENI */
    {0x1FB3, 0x03B1, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH YPOGEGRAMMENI */
    {0x1FB4, 0x03AC, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH OXIA AND YPOGEGRAMMENI */
    {0x1FB6, 0x03B1, 0x0342}, /* GREEK SMALL LETTER ALPHA WITH PERISPOMENI */
    {0x1FBC, 0x03B1, 0x03B9}, /* GREEK CAPITAL LETTER ALPHA WITH PROSGEGRAMMENI */
    {0x1FC2, 0x1F74, 0x03B9}, /* GREEK SMALL LETTER ETA WITH VARIA AND YPOGEGRAMMENI */
    {0x1FC3, 0x03B7, 0x03B9}, /* GREEK SMALL LETTER ETA WITH YPOGEGRAMMENI */
    {0x1FC4, 0x03AE, 0x03B9}, /* GREEK SMALL LETTER ETA WITH OXIA AND YPOGEGRAMMENI */
    {0x1FC6, 0x03B7, 0x0342}, /* GREEK SMALL LETTER ETA WITH PERISPOMENI */
    {0x1FCC, 0x03B7, 0x03B9}, /* GREEK CAPITAL LETTER ETA WITH PROSGEGRAMMENI */
    {0x1FD6, 0x03B9, 0x0342}, /* GREEK SMALL LETTER IOTA WITH PERISPOMENI */
    {0x1FE4, 0x03C1, 0x0313}, /* GREEK SMALL LETTER RHO WITH PSILI */
    {0x1FE6, 0x03C5, 0x0342}, /* GREEK SMALL LETTER UPSILON WITH PERISPOMENI */
    {0x1FF2, 0x1F7C, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH VARIA AND YPOGEGRAMMENI */
    {0x1FF3, 0x03C9, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH YPOGEGRAMMENI */
    {0x1FF4, 0x03CE, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH OXIA AND YPOGEGRAMMENI */
    /* eof nfd */
    {0x1FF6, 0x03C9, 0x0342}, /* GREEK SMALL LETTER OMEGA WITH PERISPOMENI */
    {0x1FFC, 0x03C9, 0x03B9}, /* GREEK CAPITAL LETTER OMEGA WITH PROSGEGRAMMENI */
    {0xFB00, 0x0066, 0x0066}, /* LATIN SMALL LIGATURE FF */
    {0xFB01, 0x0066, 0x0069}, /* LATIN SMALL LIGATURE FI */
    {0xFB02, 0x0066, 0x006C}, /* LATIN SMALL LIGATURE FL */
    {0xFB05, 0x0073, 0x0074}, /* LATIN SMALL LIGATURE LONG S T */
    {0xFB06, 0x0073, 0x0074}, /* LATIN SMALL LIGATURE ST */
    {0xFB13, 0x0574, 0x0576}, /* ARMENIAN SMALL LIGATURE MEN NOW */
    {0xFB14, 0x0574, 0x0565}, /* ARMENIAN SMALL LIGATURE MEN ECH */
    {0xFB15, 0x0574, 0x056B}, /* ARMENIAN SMALL LIGATURE MEN INI */
    {0xFB16, 0x057E, 0x0576}, /* ARMENIAN SMALL LIGATURE VEW NOW */
    {0xFB17, 0x0574, 0x056D}, /* ARMENIAN SMALL LIGATURE MEN XEH */
    {0, 0, 0}
    // clang-format on
};

static const struct {
    unsigned short upper;
    unsigned short lower1;
    unsigned short lower2;
    unsigned short lower3;
} tbl3[] = {
    // clang-format off
    {0x0390, 0x03B9, 0x0308, 0x0301}, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS */
    {0x03B0, 0x03C5, 0x0308, 0x0301}, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS */
    {0x1F52, 0x03C5, 0x0313, 0x0300}, /* GREEK SMALL LETTER UPSILON WITH PSILI AND VARIA */
    {0x1F54, 0x03C5, 0x0313, 0x0301}, /* GREEK SMALL LETTER UPSILON WITH PSILI AND OXIA */
    {0x1F56, 0x03C5, 0x0313, 0x0342}, /* GREEK SMALL LETTER UPSILON WITH PSILI AND PERISPOMENI */
    {0x1FB7, 0x03B1, 0x0342, 0x03B9}, /* GREEK SMALL LETTER ALPHA WITH PERISPOMENI AND YPOGEGRAMMENI */
    {0x1FC7, 0x03B7, 0x0342, 0x03B9}, /* GREEK SMALL LETTER ETA WITH PERISPOMENI AND YPOGEGRAMMENI */
    {0x1FD2, 0x03B9, 0x0308, 0x0300}, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND VARIA */
    {0x1FD3, 0x03B9, 0x0308, 0x0301}, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND OXIA */
    {0x1FD7, 0x03B9, 0x0308, 0x0342}, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND PERISPOMENI */
    {0x1FE2, 0x03C5, 0x0308, 0x0300}, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND VARIA */
    {0x1FE3, 0x03C5, 0x0308, 0x0301}, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND OXIA */
    {0x1FE7, 0x03C5, 0x0308, 0x0342}, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND PERISPOMENI */
    {0x1FF7, 0x03C9, 0x0342, 0x03B9}, /* GREEK SMALL LETTER OMEGA WITH PERISPOMENI AND YPOGEGRAMMENI */
    {0xFB03, 0x0066, 0x0066, 0x0069}, /* LATIN SMALL LIGATURE FFI */
    {0xFB04, 0x0066, 0x0066, 0x006C}, /* LATIN SMALL LIGATURE FFL */
    {0, 0, 0, 0}
    // clang-format on
};

/* Return the number of wide lower-case characters needed to full fold-case
   the given uppercase character. Returns 0, 1, 2 or 3.
   0 if the charcater stays the same, 1 if one character changes,
   2 or 3 if the character will be replaced with 2 or 3.

   Note that accents expand to more characters than 1 via NFD decomposition.
   iswfc only matches towfc_s, but not wcsfc_s which does NFD decomposition
   also.
*/
int iswfc(const uint32_t wc) {
    /* the slow variant would walk the 2 loops */
    if (likely((wc < 0xdf) || (wc > 0x0587 && wc < 0x1e96) ||
               (wc > 0x1FFC && wc < 0xFB00) || (wc > 0xFB17)))
        goto single;
    if (wc < 0x1e96) {
        if (wc == 0xdf || wc == 0x130 || wc == 0x149 || wc == 0x1f0 ||
            wc == 0x587)
            return 2;
        else if (wc == 0x390 || wc == 0x3b0)
            return 3;
        else
            goto single;
    }
    if (wc <= 0x1e9a || wc == 0x1e9e || wc == 0x1f50)
        return 2;
    if (wc < 0x1f50)
        goto single;
    if (wc == 0x1f52 || wc == 0x1f54 || wc == 0x1f56)
        return 3;
    if (wc < 0x1f80)
        goto single;
    /* Note these ranges 1f80 - 1faf deviate in fc from lc */
    if (wc <= 0x1faf || (wc >= 0x1fb2 && wc < 0x1fb6)) {
        if (wc == 0x1fb5)
            goto single;
        else
            return 2;
    }
    /* XXX 1f80- expand via nfd => 3-4
    if (wc <= 0x1faf) {
        if ((wc & 0x7) == 0 || (wc & 0x7) == 1)
            return 2;
        else
            return 3;
    }
    if (wc >= 0x1fb2 && wc < 0x1fb6)
        return 2;
    */
    if (wc == 0x1fb7 || wc == 0x1fc7 || wc == 0x1fd2 || wc == 0x1fd3 ||
        wc == 0x1fd7 || wc == 0x1fe2 || wc == 0x1fe3 || wc == 0x1fe7 ||
        wc == 0x1ff7)
        return 3;
    if (wc == 0x1fb5)
        goto single;
    else if (0)
        return 2;
    if (wc == 0x1fc5)
        goto single;
    if (wc == 0x1fb6 || wc == 0x1fbc || (wc >= 0x1fc2 && wc <= 0x1fc6) ||
        wc == 0x1fcc || wc == 0x1fd6 || wc == 0x1fe4 || wc == 0x1fe6 ||
        (wc >= 0x1ff2 && wc <= 0x1ff4) || wc == 0x1ff6 || wc == 0x1ffc)
        return 2;
    if (wc < 0xfb00 || wc > 0xfb17 || (wc > 0xfb06 && wc < 0xfb13))
        goto single;
    if (wc == 0xfb03 || wc == 0xfb04)
        return 3;
    return 2;

single:
    return iswupper(wc) ? 1 : 0;
}

/* The 194 single fc chars where fc is different to lc.
   Must only be called when we know for sure that the length is 1!
   Returns 1 if fc is different to lc, else -ESNOTFND.
   May return 2 on sizeof(wchar_t)==2 if >0xffff, i.e. converted to surrogate
   pair

   perl5.27.3 -E'no warnings; for (0..0x10ffff){
     my ($lc,$fc) = (lc(pack"W",$_), fc(pack"W",$_));
     printf "U+%04X: fc: %X, lc: %X\n", $_, unpack("W",$fc), unpack("W",$lc)
       if $lc ne $fc and length($fc)==1;
   }'
 */
int _towfc_single(wchar_t *restrict dest, const uint32_t src) {
    /* fc exceptions: not towlower */
    dest[1] = L'\0';
    if (src < 0xb5)
        goto single;
    if (src >= 0xb5 && src <= 0x3f5) {
        switch (src) {
        case 0xb5:
            dest[0] = 0x3bc;
            return EOK; /* lc=b5 */
        case 0x17f:
            dest[0] = 0x73;
            return EOK; /* lc=17f */
        case 0x345:
            dest[0] = 0x3b9;
            return EOK; /* lc=345 special casing fc */
        case 0x3c2:
            dest[0] = 0x3c3;
            return EOK; /* lc=3c2 */
        case 0x3d0:
            dest[0] = 0x3b2;
            return EOK; /* lc=3d0 */
        case 0x3d1:
            dest[0] = 0x3b8;
            return EOK; /* lc=3d1 */
        case 0x3d5:
            dest[0] = 0x3c6;
            return EOK; /* lc=3d5 */
        case 0x3d6:
            dest[0] = 0x3c0;
            return EOK; /* lc=3d6 */
        case 0x3f0:
            dest[0] = 0x3ba;
            return EOK; /* lc=3f0 */
        case 0x3f1:
            dest[0] = 0x3c1;
            return EOK; /* lc=3f1 */
        case 0x3f5:
            dest[0] = 0x3b5;
            return EOK; /* lc=3f5 */
        default:
            break;
        }
        goto single;
    } else if (unlikely(src >= 0x13a0 && src <= 0x13f5)) {
        dest[0] = src;
        return EOK;
    } else if (unlikely(src >= 0x13f8 && src <= 0x13fd)) {
        dest[0] = src - 8;
        return EOK;
    } else if (unlikely(src <= 0x1c88)) {
        if (src < 0x1c80)
            goto single;
        switch (src) {
        case 0x1c80:
            dest[0] = 0x432;
            return EOK;
        case 0x1c81:
            dest[0] = 0x434;
            return EOK;
        case 0x1c82:
            dest[0] = 0x43e;
            return EOK;
        case 0x1c83:
            dest[0] = 0x441;
            return EOK;
        case 0x1c84:
            dest[0] = 0x442;
            return EOK;
        case 0x1c85:
            dest[0] = 0x442;
            return EOK;
        case 0x1c86:
            dest[0] = 0x44a;
            return EOK;
        case 0x1c87:
            dest[0] = 0x463;
            return EOK;
        case 0x1c88:
            dest[0] = 0xa64b;
            return EOK;
        default:
            goto single;
        }
    } else if (unlikely(src <= 0x1fbe)) {
        if (src < 0x1e9b)
            goto single;
        if (src == 0x1e9b) {
            dest[0] = 0x1e61;
            return EOK;
        }
        if (src == 0x1fbe) {
            dest[0] = 0x3b9;
            return EOK;
        }
        goto single;
    } else if (unlikely(src >= 0xab70 && src <= 0xabbf)) {
        dest[0] = src - (0xab70 - 0x13a0);
        return EOK;
    }
    /*
    else if (unlikely( (src >= 0xff21 && src <= 0xff3a) ||
                       (src >= 0x10400 && src <= 0x104d3) ||
                       (src >= 0x10c80 && src <= 0x10cb2) ||
                       (src >= 0x118a0 && src <= 0x118bf) ||
                       (src >= 0x118a0 && src <= 0x118bf) )) {
        dest[0] = src + 0x20;
        return EOK;
    }
    else if (unlikely( src >= 0x1e900 && src <= 0x1e921 )) {
        dest[0] = src + 0x22;
        return EOK;
    }
    */

single:
#if SIZEOF_WCHAR_T > 2
    dest[0] = src < 128 ? (wchar_t)tolower(src) : (wchar_t)_towcase(src, 1);
    return (uint32_t)dest[0] == src ? -(ESNOTFND) : 1;
#else
{
    uint32_t cp = src < 128 ? (uint32_t)tolower(src) : _towcase(src, 1);
    if (unlikely(cp > 0xffff)) {
        dest[0] = 0xd800 + ((cp >> 10) & 0x3ff);
        dest[1] = 0xdc00 + (cp & 0x3ff);
        dest[2] = 0;
        return cp == src ? -(ESNOTFND) : 2;
    } else {
        dest[0] = cp;
        return cp == src ? -(ESNOTFND) : 1;
    }
}
#endif
}

/**
 * @def towfc_s(dest,dmax,src)
 * @brief
 *    \c towfc_s() converts a wide character to fully fold-cased (lowercased
 *    with possible expansions), according to the Unicode 10.0 CaseFolding
 *    table. Even in most the unsuccessul cases, just not with with ESNULLP
 *    and ESZEROL dest is being written to.
 *
 *  @details
 *    As of Unicode 10.0 there are no possible results as surrogate pairs with
 *    \c sizeof(wchar_t)==2, all results are below U+10000.
 *
 * @param[out]  dest  wide string buffer to store result
 * @param[in]   dmax  maximum size of dest, should be 4. (3 + NULL)
 * @param[in]   src   wide character to convert to lowercase
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall be bigger than 3.
 * @pre  dmax shall not be greater than RSIZE_MAX_WSTR and size of dest.
 *
 * @retval  >=0          on successful operation, returns the number
 *                       of converted wide characters: 0-3
 * @retval  -ESNULLP     when dest is NULL pointer
 * @retval  -ESZEROL     when dmax = 0
 * @retval  -ESLEMIN     when dmax < 4
 * @retval  -ESLEMAX     when dmax > RSIZE_MAX_WSTR
 * @retval  -EOVERFLOW   when dmax > size of dest (optionally, when the
 * compiler knows the object_size statically)
 * @retval  -ESLEWRNG    when dmax != size of dest and --enable-error-dmax
 * @retval  -ESNOTFND    when no mapping for src was found, iswfc is wrong.
 *
 * @see
 *    iswfc(), wcsfc_s(), towlower()
 */

/* Writes the fold-cased wide string to dest from the given uppercase wide
   character. dmax should be 4 (3 wchar's + \0).
   Returns the number of replaced wide characters, or -ESNOTFND if not
   replaced.
*/
#ifdef FOR_DOXYGEN
int towfc_s(wchar_t *restrict dest, rsize_t dmax, const uint32_t src)
#else
EXPORT int _towfc_s_chk(wchar_t *restrict dest, rsize_t dmax, const uint32_t src,
                        const size_t destbos)
#endif
{
    int i;
    const size_t destsz = dmax * sizeof(wchar_t);

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("towfc_s: "
                                           "dest is null",
                                           NULL, ESNULLP);
        return -(ESNULLP);
    }
    if (unlikely(dmax < 4)) {
        invoke_safe_str_constraint_handler("towfc_s: "
                                           "dmax is < 4",
                                           (void *)dest, ESLEMIN);
        return -(ESLEMIN);
    }
    dest[0] = L'\0';
    if (unlikely(dmax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("towfc_s: "
                                           "dmax exceeds max",
                                           (void *)dest, ESLEMAX);
        return -(ESLEMAX);
    }
    if (destbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(dest, destsz);
    } else {
        if (unlikely(destsz > destbos)) {
            invoke_safe_str_constraint_handler("towfc_s: dmax exceeds dest",
                                               (void *)dest, EOVERFLOW);
            return -(EOVERFLOW);
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(destsz != destbos)) {
            handle_str_bos_chk_warn("towfc_s", (char *)dest, dmax,
                                    destbos / sizeof(wchar_t));
#ifdef HAVE_ERROR_DMAX
            return (-ESLEWRNG);
#endif
        }
#endif
    }

    if (src < 128) {
        dest[1] = L'\0';
        dest[0] = tolower(src);
        return (uint32_t)dest[0] == src ? -(ESNOTFND) : 1;
    }

    for (i = 0; tbl2[i].upper; i++) {
        if (tbl2[i].upper == src) {
            dest[0] = tbl2[i].lower1;
            dest[1] = tbl2[i].lower2;
            dest[2] = L'\0';
            return 2;
        }
        if (tbl2[i].upper > src)
            break;
    }
    for (i = 0; tbl3[i].upper; i++) {
        if (tbl3[i].upper == src) {
            dest[0] = tbl3[i].lower1;
            dest[1] = tbl3[i].lower2;
            dest[2] = tbl3[i].lower3;
            dest[3] = L'\0';
            return 3;
        }
        if (tbl3[i].upper > src)
            break;
    }

    /* fc exceptions: not towlower, for c == 1 */
    return _towfc_single(dest, src);
}
