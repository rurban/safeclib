/*------------------------------------------------------------------
musl as a whole is licensed under the following standard MIT license:

----------------------------------------------------------------------
Copyright © 2005-2014 Rich Felker, et al.
Copyright © 2018,2020 Reini Urban

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------
*/

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#include <ctype.h>
#include <wctype.h>
#include <assert.h>
#endif

#define CASEMAP(u1, u2, l)                                                     \
    { (u1), (l) - (u1), (u2) - (u1) + 1 }
#define CASELACE(u1, u2) CASEMAP((u1), (u2), (u1) + 1)

/* Unicode 13.0 */

/* must be sorted */
static const struct {
    unsigned short upper;
    signed char lower;
    unsigned char len;
} casemaps[] = {
    CASEMAP(0x00c0, 0xd6, 0xe0),
    CASEMAP(0x00d8, 0xde, 0xf8),

    CASELACE(0x0100, 0x12e),
    CASELACE(0x0132, 0x136),
    CASELACE(0x0139, 0x147),
    CASELACE(0x014a, 0x176),
    CASELACE(0x0179, 0x17d),

    CASELACE(0x01a0, 0x1a4),
    CASELACE(0x01b3, 0x1b5),
    CASELACE(0x01cd, 0x1db),
    CASELACE(0x01de, 0x1ee),
    CASELACE(0x01f8, 0x21e),
    CASELACE(0x0222, 0x232),
    CASELACE(0x0246, 0x24e),

    CASELACE(0x0370, 0x372),
    CASEMAP(0x0388, 0x38a, 0x3ad),
    CASEMAP(0x0393, 0x39f, 0x3b3),
    CASEMAP(0x03a7, 0x3ab, 0x3c7),
    CASELACE(0x03d8, 0x3ee),
    /*CASEMAP (0x03da,0x3ee,0x3db),*/
    /*CASEMAP (0x03f4,0x3fa,0x3b8),*/
    CASEMAP(0x0400, 0x40f, 0x450),
    CASEMAP(0x0410, 0x42f, 0x430),

    CASELACE(0x0460, 0x480),
    CASELACE(0x048a, 0x4be),
    CASELACE(0x04c1, 0x4cd),
    CASELACE(0x04d0, 0x50e),

    CASELACE(0x0510, 0x512),
    CASELACE(0x0514, 0x526),
    CASEMAP(0x0531, 0x556, 0x561),
    /*CASEMAP (0x13a0,0x13ef,0xab70),*/ /* CHEROKEE reverse overflow */
    CASEMAP(0x13f0, 0x13f5, 0x13f8),    /* CHEROKEE reverse */
    CASEMAP(0x13f8, 0x13fd, 0x13f0),    /* CHEROKEE */
    /*CASEMAP(0x1c90,0x1cbf,0x10d0),*/  /* GEORGIAN MTAVRULI overflow
                                           (Unicode 11) */

    CASELACE(0x1e00, 0x1e94),
    CASELACE(0x1ea0, 0x1efe),

    CASEMAP(0x1f08, 0x1f0f, 0x1f00),
    CASEMAP(0x1f18, 0x1f1d, 0x1f10),
    CASEMAP(0x1f28, 0x1f2f, 0x1f20),
    CASEMAP(0x1f38, 0x1f3f, 0x1f30),
    CASEMAP(0x1f48, 0x1f4d, 0x1f40),

    CASEMAP(0x1f68, 0x1f6f, 0x1f60),
    CASEMAP(0x1f88, 0x1f8f, 0x1f80),
    CASEMAP(0x1f98, 0x1f9f, 0x1f90),
    CASEMAP(0x1fa8, 0x1faf, 0x1fa0),
    CASEMAP(0x1fb8, 0x1fb9, 0x1fb0),
    CASEMAP(0x1fba, 0x1fbb, 0x1f70),
    CASEMAP(0x1fc8, 0x1fcb, 0x1f72),
    CASEMAP(0x1fd8, 0x1fd9, 0x1fd0),
    CASEMAP(0x1fda, 0x1fdb, 0x1f76),
    CASEMAP(0x1fe8, 0x1fe9, 0x1fe0),
    CASEMAP(0x1fea, 0x1feb, 0x1f7a),
    CASEMAP(0x1ff8, 0x1ff9, 0x1f78),
    CASEMAP(0x1ffa, 0x1ffb, 0x1f7c),

    CASEMAP(0x2160, 0x216f, 0x2170),
    CASEMAP(0x24b6, 0x24cf, 0x24d0),
    CASEMAP(0x2c00, 0x2c2e, 0x2c30),
    CASELACE(0x2c67, 0x2c6b),
    CASELACE(0x2c80, 0x2ce2),
    CASELACE(0x2ceb, 0x2ced),

    CASELACE(0xa640, 0xa66c),
    CASELACE(0xa680, 0xa696),
    CASELACE(0xa698, 0xa69a),

    CASELACE(0xa722, 0xa72e),
    CASELACE(0xa732, 0xa76e),
    CASELACE(0xa779, 0xa77b),
    CASELACE(0xa77e, 0xa786),

    CASELACE(0xa790, 0xa792),
    CASELACE(0xa796, 0xa79e),
    CASELACE(0xa7a0, 0xa7a8),
    CASELACE(0xa7b4, 0xa7be), /* Unicode 8,12 */
    CASELACE(0xa7c2, 0xa7c3), /* Unicode 12 */
    CASELACE(0xa7c7, 0xa7c8), /* Unicode 13 */
    CASELACE(0xa7c9, 0xa7ca), /* Unicode 13 */
    CASELACE(0xa7f5, 0xa7f6), /* Unicode 13 */

    CASEMAP(0xff21, 0xff3a, 0xff41),
    {0, 0, 0}};

/* must be sorted */
static const struct {
    unsigned int upper;
    int lower;
    unsigned short len;
} casemapsl[] = {
    CASEMAP(0x13a0, 0x13ef, 0xab70), /* CHEROKEE reverse */
    CASEMAP(0x1c90, 0x1cbf, 0x10d0), /* GEORGIAN MTAVRULI (Unicode 11) */
    CASEMAP(0xab70, 0xabbf, 0x13a0), /* CHEROKEE */
    CASEMAP(0x10400, 0x10427, 0x10428),
    CASEMAP(0x104b0, 0x104d3, 0x104d8), /* Unicode 9 */
    CASEMAP(0x10c80, 0x10cb2, 0x10cc0), /* Unicode 8 */
    CASEMAP(0x118a0, 0x118bf, 0x118c0), /* Unicode 7 */
    CASEMAP(0x16e40, 0x16e5f, 0x16e60), /* MEDEFAIDRIN reverse (Unicode 11) */
    CASEMAP(0x1e900, 0x1e921, 0x1e922), /* Unicode 9 */
    {0, 0, 0}};

/* must now be sorted */
static const unsigned short pairs[][2] = {
    /* upper - lower */
    {'I', 0x0131},
    {'S', 0x017f},
    {0x00b5, 0x03bc},
    {0x0130, 'i'},
    {0x0178, 0x00ff},
    {0x017f, 0x73},
    {0x0181, 0x0253},
    {0x0182, 0x0183},
    {0x0184, 0x0185},
    {0x0186, 0x0254},
    {0x0187, 0x0188},
    {0x0189, 0x0256},
    {0x018a, 0x0257},
    {0x018b, 0x018c},
    {0x018e, 0x01dd},
    {0x018f, 0x0259},
    {0x0190, 0x025b},
    {0x0191, 0x0192},
    {0x0193, 0x0260},
    {0x0194, 0x0263},
    {0x0196, 0x0269},
    {0x0197, 0x0268},
    {0x0198, 0x0199},
    {0x019c, 0x026f},
    {0x019d, 0x0272},
    {0x019f, 0x0275},
    /*CASELACE(0x01a0,0x01a4),*/
    {0x01a6, 0x0280},
    {0x01a7, 0x01a8},
    {0x01a9, 0x0283},
    {0x01ac, 0x01ad},
    {0x01ae, 0x0288},
    {0x01af, 0x01b0},
    {0x01b1, 0x028a},
    {0x01b2, 0x028b},
    {0x01b3, 0x01b4},
    {0x01b5, 0x01b6},
    {0x01b7, 0x0292},
    {0x01b8, 0x01b9},
    {0x01bc, 0x01bd},
    {0x01c4, 0x01c6},
    /*{ 0x01c4, 0x01c5 },*/
    {0x01c5, 0x01c6},
    {0x01c7, 0x01c9},
    /*{ 0x01c7, 0x01c8 },*/
    {0x01c8, 0x01c9},
    {0x01ca, 0x01cc},
    /*{ 0x01ca, 0x01cb },*/
    /*CASELACE(0x01cb,0x01db),*/
    {0x01cb, 0x01cc},

    {0x01f1, 0x01f3},
    /*{ 0x01f1, 0x01f2 },*/
    {0x01f2, 0x01f3},
    {0x01f4, 0x01f5},
    {0x01f6, 0x0195},
    {0x01f7, 0x01bf},
    /*CASELACE(0x01f8,0x021e),*/
    {0x0220, 0x019e},
    /*CASELACE(0x0222,0x0232),*/
    {0x023a, 0x2c65},
    {0x023b, 0x23c},
    {0x023d, 0x19a},
    {0x023e, 0x2c66},
    {0x0241, 0x242},
    {0x0243, 0x180},
    {0x0244, 0x289},
    {0x0245, 0x28c},

    {0x0345, 0x3b9},
    {0x0376, 0x377}, /* bogus greek 'symbol' */
    {0x037f, 0x3f3},
    {0x0386, 0x3ac},
    {0x038c, 0x03cc},
    {0x038e, 0x03cd},
    {0x038f, 0x03ce},
    {0x0391, 0x3b1},
    {0x0392, 0x3b2},
    {0x0392, 0x3d0}, /* reverse */
    /*CASEMAP (0x0393,0x39f,0x3b3),*/
    {0x0395, 0x3f5}, /* reverse */
    {0x0398, 0x3d1},
    {0x0399, 0x1fbe}, /* reverse */
    {0x039a, 0x3f0},  /* reverse */
    {0x03a0, 0x3c0},
    {0x03a0, 0x3d6}, /* reverse */
    {0x03a1, 0x3c1},
    {0x03a1, 0x3f1}, /* reverse */
    {0x03a3, 0x3c3},
    {0x03a3, 0x3c2}, /* reverse */
    {0x03a4, 0x3c4},
    {0x03a5, 0x3c5},
    {0x03a6, 0x3c6},
    {0x03a6, 0x3d5}, /* reverse */
    /*CASEMAP(0x0391,0x3a1,0x3b1),*/
    {0x03c2, 0x3c3},
    {0x03cf, 0x3d7},
    {0x03d0, 0x3b2},
    {0x03d1, 0x3b8},
    {0x03d5, 0x3c6},
    {0x03d6, 0x3c0},
    /*CASELACE(0x03d8,0x3ee),*/
    /*CASEMAP(0x03da,0x3ee,0x3db),*/
    {0x03f0, 0x03ba},
    {0x03f1, 0x03c1},
    {0x03f4, 0x03b8},
    {0x03f5, 0x03b5},
    {0x03f7, 0x03f8},
    {0x03f9, 0x03f2},
    {0x03fa, 0x03fb},
    {0x03fd, 0x037b},
    {0x03fe, 0x037c},
    {0x03ff, 0x037d},
    /*CASEMAP(0x0400,0x40f,0x450),
      CASEMAP(0x0410,0x42f,0x430),*/
    {0x412, 0x1c80}, /* reverse */
    {0x414, 0x1c81}, /* reverse */
    {0x41e, 0x1c82}, /* reverse */
    {0x421, 0x1c83}, /* reverse */
    {0x422, 0x1c84}, /* reverse */
    {0x422, 0x1c85}, /* reverse */
    {0x42a, 0x1c86}, /* reverse */
    {0x462, 0x463},
    {0x462, 0x1c87}, /* reverse */

    {0x04c0, 0x04cf},
    /*CASELACE(0x04c1,0x4cd),*/
    {0x0528, 0x0529},
    {0x052a, 0x052b},
    {0x052c, 0x052d},
    {0x052e, 0x052f},

    {0x10c7, 0x2d27},
    {0x10cd, 0x2d2d},

    {0x1c80, 0x0432}, /* Unicode 12 */
    {0x1c81, 0x0434}, /* Unicode 12 */
    {0x1c82, 0x043e}, /* Unicode 12 */
    {0x1c83, 0x0441}, /* Unicode 12 */
    {0x1c84, 0x0442}, /* Unicode 12 */
    {0x1c85, 0x0442}, /* Unicode 12 */
    {0x1c86, 0x044a}, /* Unicode 12 */
    {0x1c87, 0x0463}, /* Unicode 12 */
    {0x1c88, 0xa64b}, /* Unicode 12 */

    {0x1e60, 0x1e9b},
    {0x1e9b, 0x1e61},
    {0x1e9e, 0xdf},

    {0x1f59, 0x1f51},
    {0x1f5b, 0x1f53},
    {0x1f5d, 0x1f55},
    {0x1f5f, 0x1f57},
    {0x1fbc, 0x1fb3},
    {0x1fbe, 0x3b9},
    {0x1fcc, 0x1fc3},
    {0x1fec, 0x1fe5},
    {0x1ffc, 0x1ff3},

    {0x2126, 0x3c9},
    {0x212a, 'k'},
    {0x212b, 0xe5},
    {0x2132, 0x214e},
    {0x2183, 0x2184},
    {0x2c60, 0x2c61},
    {0x2c62, 0x26b},
    {0x2c63, 0x1d7d},
    {0x2c64, 0x27d},
    {0x2c6d, 0x251},
    {0x2c6e, 0x271},
    {0x2c6f, 0x250},
    {0x2c70, 0x252},
    {0x2c72, 0x2c73},
    {0x2c75, 0x2c76},
    {0x2c7e, 0x23f},
    {0x2c7f, 0x240},
    {0x2cf2, 0x2cf3},

    {0xa64a, 0xa64b},
    {0xa64a, 0x1c88}, /* reverse */

    {0xa77d, 0x1d79},
    {0xa78b, 0xa78c},
    {0xa78d, 0x265},
    {0xa7aa, 0x266},

    {0xa7ab, 0x25c},  /* Unicode 7.0 */
    {0xa7ac, 0x261},  /* Unicode 7.0 */
    {0xa7ad, 0x26c},  /* Unicode 7.0 */
    {0xa7ae, 0x26a},  /* Unicode 9.0 */
    {0xa7b0, 0x29e},  /* Unicode 7.0 */
    {0xa7b1, 0x287},  /* Unicode 7.0 */
    {0xa7b2, 0x29d},  /* Unicode 7.0 */
    {0xa7b3, 0xab53}, /* Unicode 8.0 */
    /*{ 0xa7b4, 0xa7b5 },  Unicode 8.0 LACE'd
      { 0xa7b6, 0xa7b7 },  Unicode 8.0
      { 0xa7b8, 0xa7b9 },  Unicode 11.0 */
    {0xa7c4, 0xa794}, /* Unicode 12.0 */
    {0xa7c5, 0x0282}, /* Unicode 12.0 */
    {0xa7c6, 0x1d8e}, /* Unicode 12.0 */

    {0, 0}};

EXPORT uint32_t _towcase(uint32_t wc, int lower) {
    int i;
    int lmul = 2 * lower - 1;
    int lmask = lower - 1;
    /* no letters with case in these large ranges */
    if (/*(!iswalpha(wc) is broken on some platforms) */
        wc < 0x41 || (unsigned)wc - 0x0600 <= 0x0fff - 0x0600 ||
        (unsigned)wc - 0x2e00 <= 0xa63f - 0x2e00 ||
        (unsigned)wc - 0xa800 <= 0xab69 - 0xa800 ||
        (unsigned)wc - 0xabc0 <= 0xfeff - 0xabc0)
        return wc;
    /* special case because the diff between upper/lower is too big */
    if (lower && (unsigned)wc - 0x10a0 < 0x2e) {
        if (wc > 0x10c5 && wc != 0x10c7 && wc != 0x10cd) {
            return wc;
        } else {
            return wc + 0x2d00 - 0x10a0;
        }
    } else if (!lower && (unsigned)wc - 0x2d00 < 0x26) {
        if (wc > 0x2d25 && wc != 0x2d27 && wc != 0x2d2d)
            return wc;
        else
            return wc + 0x10a0 - 0x2d00;
    }
    for (i = 0; casemaps[i].len; i++) {
        int base = casemaps[i].upper + (lmask & casemaps[i].lower);
        assert(i > 0 ? casemaps[i].upper >= casemaps[i - 1].upper : 1);
        if ((unsigned)wc - base < casemaps[i].len) {
            if (casemaps[i].lower == 1)
                return wc + lower - ((wc - casemaps[i].upper) & 1);
            return wc + lmul * casemaps[i].lower;
        }
        if (lower && casemaps[i].upper > wc)
            break;
    }
    for (i = 0; pairs[i][1 - lower]; i++) {
        assert(i > 0 ? pairs[i][0] >= pairs[i - 1][0] : 1);
        if (pairs[i][1 - lower] == wc)
            return pairs[i][lower];
        if (lower && pairs[i][0] > wc)
            break;
    }
    for (i = 0; casemapsl[i].len; i++) {
        unsigned long base = casemapsl[i].upper + (lmask & casemapsl[i].lower);
        assert(i > 0 ? casemapsl[i].upper >= casemapsl[i - 1].upper : 1);
        if ((unsigned)wc - base < casemapsl[i].len) {
            if (casemapsl[i].lower == 1)
                return wc + lower - ((wc - casemapsl[i].upper) & 1);
            return wc + lmul * casemapsl[i].lower;
        }
        if (lower && casemaps[i].upper > wc)
            break;
    }
    return wc;
}

EXPORT uint32_t _towupper(uint32_t wc) {
    return wc < 128 ? (uint32_t)toupper(wc) : _towcase(wc, 0);
}
#ifndef HAVE_TOWUPPER
EXPORT uint32_t towupper(uint32_t wc) {
    return wc < 128 ? (uint32_t)toupper(wc) : _towcase(wc, 0);
}
#endif

#ifndef HAVE_TOWLOWER
EXPORT uint32_t towlower(uint32_t wc) {
    return (unsigned)wc < 128 ? (wint_t)tolower(wc) : _towcase(wc, 1);
}
#endif
