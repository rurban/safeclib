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
#include <stdio.h>
#endif

/* e.g. sdcc has only atof(), float and no NAN */
#if defined(HAVE_STRTOLD)
#define safec_strtold(buf) strtold((buf), NULL)
#elif defined(HAVE_STRTOD)
#define safec_strtold(buf) ((long double)strtod((buf), NULL))
#else
#define safec_strtold(buf) ((long double)atof(buf))
#endif
#ifndef INFINITY
#define INFINITY HUGE_VALF
#endif
#ifndef NAN
#define NAN (INFINITY - INFINITY)
#endif

#if defined(SIZEOF_SIZE_T) && SIZEOF_SIZE_T <= 2
/* 8/16-bit targets, e.g. sdcc: max 255 byte stack frames, floats only */
#define SAFEC_FLOATSCAN_BUFSZ 64
#else
#define SAFEC_FLOATSCAN_BUFSZ 560
#endif

#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
static int is_ascii_space(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
           c == '\r';
}

/* Collected mantissa digits, without the radix point and without leading
 * zeros. Digits that do not fit are dropped, but accounted for in `exp`
 * (the exponent adjustment in the token's base: 10 for decimal, 2 for hex),
 * so that long inputs keep their magnitude, like musl. */
typedef struct {
    char *buf;
    size_t n, cap;
    long exp;
    int sticky; /* a nonzero digit was dropped */
} mant_t;

static void mant_put(mant_t *m, int c, int gotrad, int shift) {
    if (m->n == 0 && c == '0') { /* leading zero */
        if (gotrad)
            m->exp -= shift;
        return;
    }
    if (m->n < m->cap) {
        m->buf[m->n++] = (char)c;
        if (gotrad)
            m->exp -= shift;
    } else {
        if (c != '0')
            m->sticky = 1;
        if (!gotrad)
            m->exp += shift;
    }
}

/* Scan a [eE]/[pP] exponent suffix ([+-]?digits). On entry `c` is the
 * exponent letter, already fetched. If a valid exponent follows, stores it
 * in *e, sets *valid and returns the next, not yet consumed char. If not,
 * pushes back exactly the chars fetched here (after the letter), clears
 * *valid and returns `c`, which the caller pushes back like any other
 * unconsumed char. */
static int scan_exponent(safec_scan_cursor *cur, int c, long *e, int *valid) {
    int pending = 1; /* chars fetched after the letter */
    int ec = cur->get(cur->ctx);
    int eneg = 0;
    long v = 0;
    if (ec == '+' || ec == '-') {
        eneg = (ec == '-');
        ec = cur->get(cur->ctx);
        pending++;
    }
    if (ec >= '0' && ec <= '9') {
        for (; ec >= '0' && ec <= '9'; ec = cur->get(cur->ctx))
            if (v < 100000000L)
                v = v * 10 + (ec - '0');
        *e = eneg ? -v : v;
        *valid = 1;
        return ec;
    }
    while (pending-- > 0)
        cur->unget(cur->ctx);
    *valid = 0;
    return c;
}

/* No digits: push back all `prefix` already consumed chars (sign, 0x),
 * the radix point and the lookahead char, and reset the consumed count so
 * the caller sees a matching failure. */
static long double scan_nodigits(safec_scan_cursor *cur, int c, int gotrad,
                                 int prefix) {
    if (c >= 0)
        cur->unget(cur->ctx);
    if (gotrad)
        cur->unget(cur->ctx);
    while (prefix-- > 0)
        cur->unget(cur->ctx);
    errno = EINVAL;
    cur->setlim(cur->ctx, 0);
    return 0;
}

/* Common decimal (shift 1, exponent letter 'e') and hex (shift 4, 'p')
 * scanner. "c" is the first char after the sign or the "0x" prefix;
 * `prefix` counts the already consumed prefix chars. The token is
 * reassembled as [0x]digits + exponent and converted with strtold(). */
static long double scan_mantissa(safec_scan_cursor *cur, int c, int neg,
                                 int hex, int prefix, char *buf, size_t cap) {
    int gotdig = 0, gotrad = 0, valid;
    long e = 0;
    mant_t m;
    char *tok;

    /* buf = "0x" + digits + exponent ("p-123456789") + NUL */
    m.buf = buf + 2;
    m.n = 0;
    m.cap = cap - 2 - 16;
    m.exp = 0;
    m.sticky = 0;

    for (; (c >= '0' && c <= '9') ||
           (hex && (c | 32) >= 'a' && (c | 32) <= 'f') || c == '.';
         c = cur->get(cur->ctx)) {
        if (c == '.') {
            if (gotrad)
                break;
            gotrad = 1;
        } else {
            gotdig = 1;
            mant_put(&m, c, gotrad, hex ? 4 : 1);
        }
    }
    if (!gotdig)
        return scan_nodigits(cur, c, gotrad, prefix);
    if ((c | 32) == (hex ? 'p' : 'e')) {
        c = scan_exponent(cur, c, &e, &valid);
        if (!valid)
            e = 0;
    }
    if (c >= 0)
        cur->unget(cur->ctx);

    if (!m.n)
        m.buf[m.n++] = '0';
    else if (m.sticky && m.buf[m.n - 1] == '0')
        m.buf[m.n - 1] = '1'; /* a dropped nonzero tail: break ties right */
    e += m.exp;
    if (e > 99999999L)
        e = 99999999L;
    else if (e < -99999999L)
        e = -99999999L;
    snprintf(m.buf + m.n, 16, hex ? "p%ld" : "e%ld", e);
    tok = buf + 2;
    if (hex) {
        buf[0] = '0';
        buf[1] = 'x';
        tok = buf;
    }
    return safec_strtold(tok) * (neg ? -1 : 1);
}

long double safec_floatscan(safec_scan_cursor *cur, int prec, int pok) {
    char buf[SAFEC_FLOATSCAN_BUFSZ];
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
            return scan_mantissa(cur, c, neg, 1, sign + 2, buf, sizeof buf);
        }
        cur->unget(cur->ctx);
        c = '0';
    }
    return scan_mantissa(cur, c, neg, 0, sign, buf, sizeof buf);
}
#endif // PRINTF_DISABLE_SUPPORT_FLOAT
