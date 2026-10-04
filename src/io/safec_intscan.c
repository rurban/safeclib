/*------------------------------------------------------------------
 * safec_intscan.c
 *
 * Copyright © 2005-2014 Rich Felker, et al. (musl libc, MIT licensed)
 * Copyright (c) 2026 by Reini Urban
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

/* Ported from musl's src/internal/intscan.c, generalized to operate
 * through a safec_scan_cursor instead of directly on a musl FILE, so
 * it can back both the byte-oriented and the wide-character scanf
 * engines. */

#ifdef FOR_DOXYGEN
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#include "io/safec_scan.h"
#endif

/* Lookup table for digit values. c==-1 (EOF) maps to table[0]==255,
 * and any value >=36 is treated as an invalid digit. */
static const unsigned char table[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   255,
    255, 255, 255, 255, 255, 255, 10,  11,  12,  13,  14,  15,  16,  17,  18,
    19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,
    34,  35,  255, 255, 255, 255, 255, 255, 10,  11,  12,  13,  14,  15,  16,
    17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
    32,  33,  34,  35,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255,
};

/* table[c+1], valid (and safe to index) for -1 <= c <= 255. Wide
 * cursors can hand us arbitrary code points above 255; treat those as
 * an invalid digit rather than reading out of bounds. */
static unsigned val_of(int c) {
    if (c < -1 || c > 255)
        return 255;
    return table[c + 1];
}

unsigned long long safec_intscan(safec_scan_cursor *cur, unsigned base, int pok,
                                 unsigned long long lim) {
    int c, neg = 0;
    unsigned x;
    unsigned long long y;

    (void)pok;

    if (base > 36 || base == 1) {
        errno = EINVAL;
        return 0;
    }
    while (isspace((c = cur->get(cur->ctx))))
        ;
    if (c == '+' || c == '-') {
        neg = -(c == '-');
        c = cur->get(cur->ctx);
    }
    if ((base == 0 || base == 16) && c == '0') {
        c = cur->get(cur->ctx);
        if ((c | 32) == 'x') {
            c = cur->get(cur->ctx);
            if (val_of(c) >= 16) {
                cur->unget(cur->ctx);
                cur->setlim(cur->ctx, 0);
                return 0;
            }
            base = 16;
        } else if (base == 0) {
            base = 8;
        }
    } else {
        if (base == 0)
            base = 10;
        if (val_of(c) >= base) {
            cur->unget(cur->ctx);
            cur->setlim(cur->ctx, 0);
            errno = EINVAL;
            return 0;
        }
    }
    if (base == 10) {
        for (x = 0; (unsigned)(c - '0') < 10U && x <= UINT_MAX / 10 - 1;
             c = cur->get(cur->ctx))
            x = x * 10 + (c - '0');
        for (y = x; (unsigned)(c - '0') < 10U && y <= ULLONG_MAX / 10 &&
                    10 * y <= ULLONG_MAX - (unsigned)(c - '0');
             c = cur->get(cur->ctx))
            y = y * 10 + (c - '0');
        if ((unsigned)(c - '0') >= 10U)
            goto done;
    } else if (!(base & (base - 1))) {
        int bs = "\0\1\2\4\7\3\6\5"[(0x17 * base) >> 5 & 7];
        for (x = 0; val_of(c) < base && x <= UINT_MAX / 32;
             c = cur->get(cur->ctx))
            x = x << bs | val_of(c);
        for (y = x; val_of(c) < base && y <= ULLONG_MAX >> bs;
             c = cur->get(cur->ctx))
            y = y << bs | val_of(c);
    } else {
        for (x = 0; val_of(c) < base && x <= UINT_MAX / 36 - 1;
             c = cur->get(cur->ctx))
            x = x * base + val_of(c);
        for (y = x; val_of(c) < base && y <= ULLONG_MAX / base &&
                    base * y <= ULLONG_MAX - val_of(c);
             c = cur->get(cur->ctx))
            y = y * base + val_of(c);
    }
    if (val_of(c) < base) {
        for (; val_of(c) < base; c = cur->get(cur->ctx))
            ;
        errno = ERANGE;
        y = lim;
        if (lim & 1)
            neg = 0;
    }
done:
    cur->unget(cur->ctx);
    if (y >= lim) {
        if (!(lim & 1) && !neg) {
            errno = ERANGE;
            return lim - 1;
        } else if (y > lim) {
            errno = ERANGE;
            return lim;
        }
    }
    return (y ^ (unsigned long long)neg) - (unsigned long long)neg;
}
