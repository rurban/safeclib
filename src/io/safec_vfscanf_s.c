/*------------------------------------------------------------------
 * safec_vfscanf_s.c
 *
 * February 2022, Reini Urban
 * September 2026, Reini Urban
 *
 * Copyright © 2005-2014 Rich Felker, et al.
 * Copyright (c) 2022,2026 by Reini Urban
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
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#include "io/safec_file.h"
#include "io/safec_scan.h"
#endif

/* from musl: */

// #include <locale.h>
#define SIZE_hh -2
#define SIZE_h -1
#define SIZE_def 0
#define SIZE_l 1
#define SIZE_L 2
#define SIZE_ll 3

// CHECKME next 2
#define FLOCK(sf)
#define FUNLOCK(sf)

/* Cursor binding so safec_intscan()/safec_floatscan() can drive the
 * same shgetc()/shunget()/shlim() machinery this scanner itself uses. */
static int safec_ascii_cur_get(void *ctx) { return shgetc((_SAFEC_FILE *)ctx); }
static void safec_ascii_cur_unget(void *ctx) { shunget((_SAFEC_FILE *)ctx); }
static void safec_ascii_cur_setlim(void *ctx, long lim) {
    shlim((_SAFEC_FILE *)ctx, lim);
}

static void safec_store_int(void *dest, int size, unsigned long long i) {
    if (!dest)
        return;
    switch (size) {
    case SIZE_hh:
        *(char *)dest = i;
        break;
    case SIZE_h:
        *(short *)dest = i;
        break;
    case SIZE_def:
        *(int *)dest = i;
        break;
    case SIZE_l:
        *(long *)dest = i;
        break;
    case SIZE_ll:
        *(long long *)dest = i;
        break;
    default:
        break;
    }
}

static void *safec_arg_n(va_list ap, unsigned int n) {
    void *p;
    unsigned int i;
    va_list ap2;
    va_copy(ap2, ap);
    for (i = n; i > 1; i--)
        va_arg(ap2, void *);
    p = va_arg(ap2, void *);
    va_end(ap2);
    return p;
}

/* from MUSL.
   with added safeties:
   - each string argument must have 2 args, buffer and size.
   - %n is forbidden
*/
int _safec_vfscanf_impl(_SAFEC_FILE *sf, const char *funcname, const char *fmt,
                        va_list ap, int chk_destsize) {
    int width;
    int size;
    int alloc = 0;
    int base;
    const unsigned char *p;
    int c, t;
    char *s = NULL;
#ifndef SAFECLIB_DISABLE_WCHAR
    wchar_t *wcs = NULL;
    mbstate_t st;
#endif
    void *dest = NULL;
    int invert;
    int matches = 0;
    unsigned long long x;
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
    long double y;
#endif
#ifdef __SDCC
    /* 257 bytes overflows the tiny hardware/simulator stack on 8-bit
       targets; safec_vfscanf_s/vsscanf_s/sscanf_s are not reentrant
       there anyway (no threads). */
    static unsigned char scanset[257];
#else
    unsigned char scanset[257];
#endif
    size_t i, k;
    rsize_t destsize, cap;
#ifndef SAFECLIB_DISABLE_WCHAR
    wchar_t wc;
#endif
    safec_scan_cursor cur;

    /* field-wise: sdcc miscompiles function-pointer struct initializers
       with --stack-auto */
    cur.ctx = sf;
    cur.get = safec_ascii_cur_get;
    cur.unget = safec_ascii_cur_unget;
    cur.setlim = safec_ascii_cur_setlim;

    FLOCK(sf);
    if (!sf->rpos)
        safec_toread(sf);
    if (!sf->rpos)
        goto input_fail;

    for (p = (const unsigned char *)fmt; *p; p++) {

        alloc = 0;

        if (isspace(*p)) {
            while (isspace(p[1]))
                p++;
            shlim(sf, 0);
            while (isspace(shgetc(sf)))
                ;
            shunget(sf);
            continue;
        }
        if (*p != '%' || p[1] == '%') {
            shlim(sf, 0);
            if (*p == '%') {
                p++;
                while (isspace((c = shgetc(sf))))
                    ;
            } else {
                c = shgetc(sf);
            }
            if (c != *p) {
                shunget(sf);
                if (c < 0)
                    goto input_fail;
                goto match_fail;
            }
            continue;
        }

        p++;
        if (*p == '*') {
            dest = 0;
            p++;
        } else if (isdigit(*p) && p[1] == '$') {
            dest = safec_arg_n(ap, *p - '0');
            p += 2;
        } else {
            dest = va_arg(ap, void *);
        }

        for (width = 0; isdigit(*p); p++) {
            width = 10 * width + *p - '0';
        }

        if (*p == 'm') {
#ifndef SAFECLIB_DISABLE_WCHAR
            wcs = 0;
#endif
            s = 0;
            alloc = !!dest;
            p++;
        } else {
            alloc = 0;
        }

        size = SIZE_def;
        switch (*p++) {
        case 'h':
            if (*p == 'h')
                p++, size = SIZE_hh;
            else
                size = SIZE_h;
            break;
        case 'l':
            if (*p == 'l') {
                p++;
#ifdef PRINTF_DISABLE_SUPPORT_LONG_LONG
                goto fmt_fail;
#else
                size = SIZE_ll;
#endif
            } else
                size = SIZE_l;
            break;
        case 'j':
#ifdef PRINTF_DISABLE_SUPPORT_LONG_LONG
            goto fmt_fail;
#else
            size = SIZE_ll;
#endif
            break;
        case 'z':
            size = SIZE_l;
            break;
        case 't':
#ifdef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
            goto fmt_fail;
#else
#ifdef _WIN32
            size = SIZE_ll;
#else
            size = SIZE_l;
#endif
#endif
            break;
        case 'L':
#ifdef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
            goto fmt_fail;
#else
            size = SIZE_L;
#endif
            break;
        case 'd':
        case 'i':
        case 'o':
        case 'u':
        case 'x':
        case 'a':
        case 'e':
        case 'f':
        case 'g':
        case 'A':
        case 'E':
        case 'F':
        case 'G':
        case 'X':
        case 's':
        case 'c':
        case '[':
        case 'S':
        case 'C':
        case 'p':
            p--;
            break;
        case 'n': {
            char tmp[128];
            snprintf(tmp, sizeof(tmp), "%s: illegal %%n", funcname);
            invoke_safe_str_constraint_handler(tmp, NULL, EINVAL);
            errno = EINVAL;
            return EOF;
        }
        default:
            goto fmt_fail;
        }

        t = *p;

        /* C or S */
        if ((t & 0x2f) == 3) {
            t |= 32;
            size = SIZE_l;
        }
        destsize = (rsize_t)-1;
        if (chk_destsize && dest && !alloc &&
            (t == 'c' || t == 's' || t == '[')) {
            destsize = va_arg(ap, rsize_t);
            if (unlikely(destsize == 0 || destsize > RSIZE_MAX_STR)) {
                char etmp[96];
                snprintf(etmp, sizeof etmp, "%s: invalid destination size",
                         funcname);
                invoke_safe_str_constraint_handler(etmp, NULL, ESZEROL);
                errno = ESZEROL;
                return EOF;
            }
        }

        switch (t) {
        case 'c':
            if (width < 1)
                width = 1;
        case '[':
            break;
        case 'n': { // unsafe
            char tmp[64];
            snprintf(tmp, sizeof(tmp), "%s: illegal %%n", funcname);
            invoke_safe_str_constraint_handler(tmp, NULL, EINVAL);
            errno = EINVAL;
            return EOF;
        }
        default:
            shlim(sf, 0);
            while (isspace(shgetc(sf)))
                ;
            shunget(sf);
        }

        shlim(sf, width);
        if (shgetc(sf) < 0)
            goto input_fail;
        shunget(sf);

        switch (t) {
        case 's':
        case 'c':
        case '[':
            if (t == 'c' || t == 's') {
                memset(scanset, -1, sizeof scanset);
                scanset[0] = 0;
                if (t == 's') {
                    scanset[1 + '\t'] = 0;
                    scanset[1 + '\n'] = 0;
                    scanset[1 + '\v'] = 0;
                    scanset[1 + '\f'] = 0;
                    scanset[1 + '\r'] = 0;
                    scanset[1 + ' '] = 0;
                }
            } else {
                if (*++p == '^')
                    p++, invert = 1;
                else
                    invert = 0;
                memset(scanset, invert, sizeof scanset);
                scanset[0] = 0;
                if (*p == '-')
                    p++, scanset[1 + '-'] = 1 - invert;
                else if (*p == ']')
                    p++, scanset[1 + ']'] = 1 - invert;
                for (; *p != ']'; p++) {
                    if (!*p)
                        goto fmt_fail;
                    if (*p == '-' && p[1] && p[1] != ']')
                        for (c = p++ [-1]; c < *p; c++)
                            scanset[1 + c] = 1 - invert;
                    scanset[1 + *p] = 1 - invert;
                }
            }
#ifndef SAFECLIB_DISABLE_WCHAR
            wcs = 0;
#endif
            s = 0;
            i = 0;
            k = t == 'c' ? width + 1U : 31;
            cap = (destsize == (rsize_t)-1)
                      ? (rsize_t)-1
                      : (t == 'c' ? destsize : destsize - 1);
            if (size == SIZE_l) {
#ifdef SAFECLIB_DISABLE_WCHAR
                /* no wide-char support on this freestanding target, e.g.
                   avr-libc: reject %ls/%lc/%l[ as an unsupported format,
                   like the other unreachable specifiers below. */
                goto fmt_fail;
#else
                if (alloc) {
                    wcs = (wchar_t *)malloc(k * sizeof(wchar_t));
                    if (!wcs)
                        goto alloc_fail;
                } else {
                    wcs = (wchar_t *)dest;
                }
                memset(&st, 0, sizeof(st));
                while (scanset[(c = shgetc(sf)) + 1]) {
                    char c_mb = (char)c;
                    size_t mbr = mbrtowc(&wc, &c_mb, 1, &st);
                    if (mbr == (size_t)-1) {
                        goto input_fail;
                    } else if (mbr == (size_t)-2) {
                        continue;
                    }
                    if (wcs) {
                        if (chk_destsize && i >= cap)
                            goto overflow_fail;
                        wcs[i++] = wc;
                    }
                    if (alloc && i == k) {
                        wchar_t *tmp;
                        k += k + 1;
                        tmp = (wchar_t *)realloc(wcs, k * sizeof(wchar_t));
                        if (!tmp)
                            goto alloc_fail;
                        wcs = tmp;
                    }
                }
                if (!mbsinit(&st))
                    goto input_fail;
#endif
            } else if (alloc) {
                s = (char *)malloc(k);
                if (!s)
                    goto alloc_fail;
                while (scanset[(c = shgetc(sf)) + 1]) {
                    s[i++] = c;
                    if (i == k) {
                        char *tmp;
                        k += k + 1;
                        tmp = (char *)realloc(s, k);
                        if (!tmp)
                            goto alloc_fail;
                        s = tmp;
                    }
                }
            } else if ((s = (char *)dest)) {
                while (scanset[(c = shgetc(sf)) + 1]) {
                    if (chk_destsize && i >= cap)
                        goto overflow_fail;
                    s[i++] = c;
                }
            } else {
                while (scanset[(c = shgetc(sf)) + 1])
                    ;
            }
            shunget(sf);
            if (!shcnt(sf))
                goto match_fail;
            if (t == 'c' && shcnt(sf) != width)
                goto match_fail;
            if (alloc) {
#ifndef SAFECLIB_DISABLE_WCHAR
                if (size == SIZE_l)
                    *(wchar_t **)dest = wcs;
                else
#endif
                    *(char **)dest = s;
            }
            if (t != 'c') {
#ifndef SAFECLIB_DISABLE_WCHAR
                if (wcs)
                    wcs[i] = 0;
#endif
                if (s)
                    s[i] = 0;
            }
            break;
        case 'p':
        case 'X':
        case 'x':
            base = 16;
            goto int_common;
        case 'o':
            base = 8;
            goto int_common;
        case 'd':
        case 'u':
            base = 10;
            goto int_common;
        case 'i':
            base = 0;
        int_common:
            x = safec_intscan(&cur, (unsigned)base, 0, ULLONG_MAX);
            if (!shcnt(sf))
                goto match_fail;
            if (t == 'p' && dest)
                *(void **)dest = (void *)(uintptr_t)x;
            else
                safec_store_int(dest, size, x);
            break;
        case 'a':
        case 'A':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
#ifdef PRINTF_DISABLE_SUPPORT_FLOAT
            /* e.g. the freestanding ENABLE_MINIMAL build */
            goto fmt_fail;
#else
#ifdef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
            if (t != 'f' && t != 'F')
                goto fmt_fail;
#endif
            y = safec_floatscan(&cur, size, 0);
            if (!shcnt(sf))
                goto match_fail;
            if (dest)
                switch (size) {
                case SIZE_def:
                    *(float *)dest = y;
                    break;
                case SIZE_l:
                    *(double *)dest = y;
                    break;
                case SIZE_L:
                    *(long double *)dest = y;
                    break;
                default:
                    goto fmt_fail;
                }
            break;
#endif
        default:
            goto fmt_fail;
        }

        if (dest)
            matches++;
    }
    if (0) {
    fmt_fail:
    alloc_fail:
    input_fail:
        if (!matches)
            matches--;
    match_fail:
        if (alloc) {
            free(s);
#ifndef SAFECLIB_DISABLE_WCHAR
            free(wcs);
#endif
        }
    }
    FUNLOCK(sf);
    return matches;

overflow_fail:
    if (s)
        s[i] = 0;
#ifndef SAFECLIB_DISABLE_WCHAR
    if (wcs)
        wcs[i] = 0;
#endif
    FUNLOCK(sf);
    {
        char etmp[96];
        snprintf(etmp, sizeof etmp, "%s: destination buffer too small",
                 funcname);
        invoke_safe_str_constraint_handler(etmp, NULL, ESNOSPC);
    }
    errno = ESNOSPC;
    return EOF;
}
