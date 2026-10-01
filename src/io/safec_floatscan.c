/*------------------------------------------------------------------
 * safec_floatscan.c
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

/* Unlike safec_intscan.c, this is *not* a line-for-line port of
 * musl's internal/floatscan.c: that file hand-rolls a 128-word
 * bignum decimal-to-binary conversion to get correctly-rounded
 * results without depending on libc. We do depend on libc here
 * (strtold() is C99) so instead we only re-implement the *grammar*
 * recognition (which characters make up one floating-point token,
 * matching what scanf()'s %a/%e/%f/%g must accept: optional sign,
 * inf/infinity, nan(...), decimal, or 0x-hex-float) character by
 * character through the cursor, buffer the matched token, and let
 * strtold() do the actual, correctly-rounded, numeric conversion. */

#ifdef FOR_DOXYGEN
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#include "io/safec_scan.h"
#include <math.h>
#include <stdlib.h>
#endif

#define SAFEC_FLOATSCAN_BUFSZ 560

static int is_ascii_space(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
           c == '\r';
}

static void bufput(char *buf, size_t *n, size_t cap, int c) {
    if (*n < cap - 1)
        buf[(*n)++] = (char)c;
}

/* Scan a decimal-exponent suffix ([eE][+-]?digits) onto the buffer.
 * On entry `c` is the character immediately following the mantissa
 * (already fetched, not yet consumed into the buffer). If it starts
 * a valid exponent, appends it and returns the next not-yet-consumed
 * char (pushed back by the caller as usual); if not, pushes back
 * everything it spuriously consumed and returns the original `c'. */
static int scan_exponent(safec_scan_cursor *cur, char *buf, size_t *n,
                         size_t cap, int c) {
    int pending = 1; /* the 'e'/'E' itself, already fetched by the caller */
    int ec = cur->get(cur->ctx);
    int esign = 0;
    pending++;
    if (ec == '+' || ec == '-') {
        esign = ec;
        ec = cur->get(cur->ctx);
        pending++;
    }
    if (ec >= '0' && ec <= '9') {
        bufput(buf, n, cap, c);
        if (esign)
            bufput(buf, n, cap, esign);
        while (ec >= '0' && ec <= '9') {
            bufput(buf, n, cap, ec);
            ec = cur->get(cur->ctx);
        }
        return ec;
    }
    /* invalid exponent: undo every char fetched in this function */
    while (pending-- > 0)
        cur->unget(cur->ctx);
    return c;
}

/* Decimal mantissa + optional exponent, "c" is the first digit or '.'
 * already fetched. Returns the parsed value; errno is set to EINVAL
 * and 0 is returned if no digits were found at all. */
static long double scan_decimal(safec_scan_cursor *cur, int c, int neg,
                                char *buf, size_t *n, size_t cap) {
    int gotdig = 0, gotrad = 0;

    for (; (c >= '0' && c <= '9') || c == '.'; c = cur->get(cur->ctx)) {
        if (c == '.') {
            if (gotrad)
                break;
            gotrad = 1;
        } else {
            gotdig = 1;
        }
        bufput(buf, n, cap, c);
    }
    if (!gotdig) {
        if (c >= 0)
            cur->unget(cur->ctx);
        if (gotrad) {
            if (*n)
                (*n)--;
            cur->unget(cur->ctx);
        }
        errno = EINVAL;
        return 0;
    }
    if ((c | 32) == 'e') {
        c = scan_exponent(cur, buf, n, cap, c);
    }
    if (c >= 0)
        cur->unget(cur->ctx);
    buf[*n] = 0;
    return strtold(buf, NULL) * (neg ? -1 : 1);
}

/* 0x-prefixed hex float, "c" is the first char after "0x"/"0X". A
 * missing binary exponent (p/P suffix) is tolerated, matching the
 * permissive grammar scanf's %a historically accepts; strtold()
 * requires one, so synthesize "p0". */
static long double scan_hexfloat(safec_scan_cursor *cur, int c, int neg,
                                 char *buf, size_t *n, size_t cap) {
    int gotdig = 0, gotrad = 0, gotexp = 0;

    bufput(buf, n, cap, '0');
    bufput(buf, n, cap, 'x');
    for (; (c >= '0' && c <= '9') || ((c | 32) >= 'a' && (c | 32) <= 'f') ||
           c == '.';
         c = cur->get(cur->ctx)) {
        if (c == '.') {
            if (gotrad)
                break;
            gotrad = 1;
        } else {
            gotdig = 1;
        }
        bufput(buf, n, cap, c);
    }
    if (!gotdig) {
        if (c >= 0)
            cur->unget(cur->ctx);
        if (gotrad) {
            if (*n)
                (*n)--;
            cur->unget(cur->ctx);
        }
        errno = EINVAL;
        return 0;
    }
    if ((c | 32) == 'p') {
        int after = scan_exponent(cur, buf, n, cap, c);
        if (after != c) {
            gotexp = 1;
            c = after;
        }
    }
    if (!gotexp) {
        bufput(buf, n, cap, 'p');
        bufput(buf, n, cap, '0');
    }
    if (c >= 0)
        cur->unget(cur->ctx);
    buf[*n] = 0;
    return strtold(buf, NULL) * (neg ? -1 : 1);
}

long double safec_floatscan(safec_scan_cursor *cur, int prec, int pok) {
    char buf[SAFEC_FLOATSCAN_BUFSZ];
    size_t n = 0;
    int c, sign = 0, neg;
    int i;

    (void)pok;
    if (prec < 0 || prec > 2)
        return 0;

    do {
        c = cur->get(cur->ctx);
    } while (is_ascii_space(c));

    if (c == '+' || c == '-') {
        sign = (c == '-');
        c = cur->get(cur->ctx);
    }
    neg = sign;

    /* inf / infinity, case-insensitive */
    for (i = 0; i < 8 && (c | 32) == "infinity"[i]; i++)
        if (i < 7)
            c = cur->get(cur->ctx);
    if (i == 3 || i == 8) {
        if (i != 8)
            cur->unget(cur->ctx);
        return neg ? -(long double)INFINITY : (long double)INFINITY;
    }

    /* nan / nan(...), case-insensitive */
    if (!i)
        for (i = 0; i < 3 && (c | 32) == "nan"[i]; i++)
            if (i < 2)
                c = cur->get(cur->ctx);
    if (i == 3) {
        if (cur->get(cur->ctx) != '(') {
            cur->unget(cur->ctx);
            return (long double)NAN;
        }
        for (;;) {
            c = cur->get(cur->ctx);
            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z') || c == '_')
                continue;
            if (c == ')')
                return (long double)NAN;
            cur->unget(cur->ctx);
            errno = EINVAL;
            cur->setlim(cur->ctx, 0);
            return 0;
        }
    }
    if (i) {
        cur->unget(cur->ctx);
        errno = EINVAL;
        cur->setlim(cur->ctx, 0);
        return 0;
    }

    if (c == '0') {
        c = cur->get(cur->ctx);
        if ((c | 32) == 'x') {
            c = cur->get(cur->ctx);
            return scan_hexfloat(cur, c, neg, buf, &n, sizeof buf);
        }
        cur->unget(cur->ctx);
        c = '0';
    }
    return scan_decimal(cur, c, neg, buf, &n, sizeof buf);
}
