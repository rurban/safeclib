/*------------------------------------------------------------------
 * safec_vfwscanf_s.c
 *
 * February 2022, Reini Urban
 *
 * Copyright © 2005-2014 Rich Felker, et al.
 * Copyright (c) 2022 by Reini Urban
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
#include <wctype.h>
#include "io/safec_file.h"
#include "io/safec_scan.h"
#endif

/* from musl: */

//#include <locale.h>
#define SIZE_hh -2
#define SIZE_h -1
#define SIZE_def 0
#define SIZE_l 1
#define SIZE_L 2
#define SIZE_ll 3

// CHECKME next 2
#define FLOCK(sf)
#define FUNLOCK(sf)

static void safec_store_int(void *dest, int size, unsigned long long i) {
    if (!dest)
        return;
    switch (size) {
    case SIZE_hh:
        *(char *)dest = (char)i;
        break;
    case SIZE_h:
        *(short *)dest = (short)i;
        break;
    case SIZE_def:
        *(int *)dest = (int)i;
        break;
    case SIZE_l:
        *(long *)dest = (long)i;
        break;
    case SIZE_ll:
        *(long long *)dest = (long long)i;
        break;
    default:
        break;
    }
}

/* Wide character cursor: for a real stream (sf->f set) delegate to
 * the platform's wide stdio (fgetwc/ungetwc); for an in-memory
 * wchar_t* source (sf->f == NULL) walk sf->cookie directly -- no
 * multibyte round-trip needed since the source is already decoded. */
static int safec_wgetc(_SAFEC_FILE *sf) {
    if (sf->f) {
        wint_t wc = fgetwc(sf->f);
        return wc == WEOF ? -1 : (int)wc;
    } else {
        const wchar_t *p = (const wchar_t *)sf->cookie;
        if (!p || !*p)
            return -1;
        sf->cookie = (void *)(p + 1);
        return (int)(unsigned int)*p;
    }
}
static void safec_wungetc(int c, _SAFEC_FILE *sf) {
    if (c < 0)
        return;
    if (sf->f) {
        ungetwc((wchar_t)c, sf->f);
    } else {
        sf->cookie = (void *)((const wchar_t *)sf->cookie - 1);
    }
}
#define shgetwc(sf) safec_wgetc(sf)
#define shungetwc(c, sf) safec_wungetc((c), (sf))

/* Cursor binding used only for the in-memory-string numeric fallback
 * (sf->f == NULL): real streams keep delegating to fscanf() below,
 * matching musl's own vfwscanf(). Tracks its own consumed-char count
 * since there is no shcnt()-equivalent on the wide side. */
typedef struct {
    _SAFEC_FILE *sf;
    long lim;
    long cnt;
    int last;
} safec_wcur_ctx;
static int safec_wcur_get(void *v) {
    safec_wcur_ctx *x = (safec_wcur_ctx *)v;
    int c;
    if (x->lim > 0 && x->cnt >= x->lim)
        return -1;
    c = safec_wgetc(x->sf);
    if (c >= 0) {
        x->cnt++;
        x->last = c;
    }
    return c;
}
static void safec_wcur_unget(void *v) {
    safec_wcur_ctx *x = (safec_wcur_ctx *)v;
    if (x->cnt > 0)
        x->cnt--;
    safec_wungetc(x->last, x->sf);
}
static void safec_wcur_setlim(void *v, long lim) {
    safec_wcur_ctx *x = (safec_wcur_ctx *)v;
    x->lim = lim;
    x->cnt = 0;
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

static int safec_in_wset(const wchar_t *set, int c) {
    wchar_t j;
    const wchar_t wc = (wchar_t) c;
    const wchar_t *p = set;
    if (*p == '-') {
        if (wc == '-')
            return 1;
        p++;
    } else if (*p == ']') {
        if (wc == ']')
            return 1;
        p++;
    }
    for (; *p && *p != ']'; p++) {
        if (*p == '-' && p[1] && p[1] != ']')
            for (j = p++ [-1]; j < *p; j++)
                if (wc == j)
                    return 1;
        if (wc == *p)
            return 1;
    }
    return 0;
}

int _safec_vfwscanf_impl(_SAFEC_FILE *sf, const char *funcname,
                         const wchar_t *fmt, va_list ap, int chk_destsize) {
    int width;
    int size;
    int alloc = 0;
    int base;
    const wchar_t *p;
    int c, t;
    char *s = NULL;
    wchar_t *wcs = NULL;
    void *dest = NULL;
    int invert;
    int matches = 0;
    unsigned long long x;
    long double y;
    off_t cnt;
    static const wchar_t *size_pfx[] = {L"hh", L"h", L"", L"l", L"L", L"ll"};
    char numfmt[3 * sizeof(int) + 10];
    wchar_t wnumfmt[3 * sizeof(int) + 10];
    const wchar_t *set;
    size_t i = 0, k = 0;
    rsize_t destsize = (rsize_t)-1, cap = (rsize_t)-1;
    int gotmatch;

    FLOCK(sf);
    if (sf->f)
        fwide(sf->f, 1);

    for (p = fmt; *p; p++) {

        alloc = 0;

        if (iswspace(*p)) {
            while (iswspace(p[1]))
                p++;
            while (iswspace((c = shgetwc(sf))))
                ;
            shungetwc(c, sf);
            continue;
        }
        if (*p != '%' || p[1] == '%') {
            if (*p == '%') {
                p++;
                while (iswspace((c = shgetwc(sf))))
                    ;
            } else {
                c = shgetwc(sf);
            }
            if (c != (int)*p) {
                shungetwc(c, sf);
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
        } else if (iswdigit(*p) && p[1] == '$') {
            dest = safec_arg_n(ap, *p - '0');
            p += 2;
        } else {
            dest = va_arg(ap, void *);
        }

        for (width = 0; iswdigit(*p); p++) {
            width = 10 * width + *p - '0';
        }

        if (*p == 'm') {
            wcs = 0;
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
            if (*p == 'l')
                p++, size = SIZE_ll;
            else
                size = SIZE_l;
            break;
        case 'j':
            size = SIZE_ll;
            break;
        case 'z':
        case 't':
            size = SIZE_l;
            break;
        case 'L':
            size = SIZE_L;
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
        //case 'n':
            p--;
            break;
        case 'n': {
            char errbuf[128];
            snprintf(errbuf, sizeof(errbuf), "%s: illegal %%n", funcname);
            invoke_safe_str_constraint_handler(errbuf, NULL, EINVAL);
            errno = EINVAL;
            return EOF;
        }
        default:
            goto fmt_fail;
        }

        t = *p;

        /* Transform S,C -> ls,lc */
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
                snprintf(etmp, sizeof etmp,
                         "%s: invalid destination size", funcname);
                invoke_safe_str_constraint_handler(etmp, NULL, ESZEROL);
                errno = ESZEROL;
                return EOF;
            }
        }

        if (t != 'n') {
            if (t != '[' && (t | 32) != 'c')
                while (iswspace((c = shgetwc(sf))))
                    ;
            else
                c = shgetwc(sf);
            if (c < 0)
                goto input_fail;
            shungetwc(c, sf);
        }

        switch (t) {
        case 'n': { // unsafe
            char errbuf[64];
            snprintf(errbuf, sizeof(errbuf), "%s: illegal %%n", funcname);
            invoke_safe_str_constraint_handler(errbuf, NULL, EINVAL);
            errno = EINVAL;
            return EOF;
        }
        case 's':
        case 'c':
        case '[':
            if (t == 'c') {
                if (width < 1)
                    width = 1;
                invert = 1;
                set = L"";
            } else if (t == 's') {
                static const wchar_t spaces[] = {
                    ' ',    '\t',   '\n',   '\r',   11,     12,
                    0x0085, 0x2000, 0x2001, 0x2002, 0x2003, 0x2004,
                    0x2005, 0x2006, 0x2008, 0x2009, 0x200a, 0x2028,
                    0x2029, 0x205f, 0x3000, 0};
                invert = 1;
                set = spaces;
            } else {
                if (*++p == '^')
                    p++, invert = 1;
                else
                    invert = 0;
                set = p;
                if (*p == ']')
                    p++;
                while (*p != ']') {
                    if (!*p)
                        goto fmt_fail;
                    p++;
                }
            }

            s = (size == SIZE_def) ? (char *)dest : NULL;
            wcs = (size == SIZE_l) ? (wchar_t *)dest : NULL;

            gotmatch = 0;

            if (width < 1)
                width = -1;

            i = 0;
            cap = (destsize == (rsize_t)-1)
                      ? (rsize_t)-1
                      : (t == 'c' ? destsize : destsize - 1);
            if (alloc) {
                k = t == 'c' ? width + 1U : 31;
                if (size == SIZE_l) {
                    wcs = (wchar_t *)malloc(k * sizeof(wchar_t));
                    if (!wcs)
                        goto alloc_fail;
                } else {
                    s = (char *)malloc(k);
                    if (!s)
                        goto alloc_fail;
                }
            }
            while (width) {
                if ((c = shgetwc(sf)) < 0)
                    break;
                if (safec_in_wset(set, c) == invert)
                    break;
                if (wcs) {
                    if (chk_destsize && i >= cap)
                        goto overflow_fail;
                    wcs[i++] = c;
                    if (alloc && i == k) {
                        wchar_t *newwcs;
                        k += k + 1;
                        newwcs = (wchar_t *)realloc(wcs, k * sizeof(wchar_t));
                        if (!newwcs)
                            goto alloc_fail;
                        wcs = newwcs;
                    }
                } else if (size != SIZE_l) {
                    char mbbuf[MB_LEN_MAX];
                    int l = wctomb(s ? mbbuf : numfmt, c);
                    if (l < 0)
                        goto input_fail;
                    if (s) {
                        if (chk_destsize && i + (size_t)l > cap)
                            goto overflow_fail;
                        memcpy(s + i, mbbuf, (size_t)l);
                    }
                    i += l;
                    if (alloc && i > k - 4) {
                        char *news;
                        k += k + 1;
                        news = (char *)realloc(s, k);
                        if (!news)
                            goto alloc_fail;
                        s = news;
                    }
                }
                width -= (width > 0);
                gotmatch = 1;
            }
            if (width) {
                shungetwc(c, sf);
                if (t == 'c' || !gotmatch)
                    goto match_fail;
            }

            if (alloc) {
                if (size == SIZE_l)
                    *(wchar_t **)dest = wcs;
                else
                    *(char **)dest = s;
            }
            if (t != 'c') {
                if (wcs)
                    wcs[i] = 0;
                if (s)
                    s[i] = 0;
            }
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
        case 'p':
            if (width < 1)
                width = 0;
            if (sf->f) {
                /* Real stream: delegate to the platform's fwscanf().
                 * Must build a *wide* format string and call the wide
                 * entry point: once fwide() has oriented the stream
                 * wide (done above), glibc forbids mixing in a byte
                 * fscanf() call on it (musl's internal stdio has no
                 * such restriction, which is why musl's own vfwscanf()
                 * gets away with delegating to plain fscanf() here). */
                swprintf(wnumfmt, sizeof(wnumfmt) / sizeof(wchar_t),
                        L"%.*s%.0d%ls%lc%%lln", 1 + !dest, L"%*", width,
                        size_pfx[size + 2], (wint_t)t);
                cnt = 0;
                if (fwscanf(sf->f, wnumfmt, dest ? dest : &cnt, &cnt) == -1)
                    goto input_fail;
                else if (!cnt)
                    goto match_fail;
            } else {
                /* In-memory wchar_t* source: no real FILE* to hand to
                 * fscanf(), so drive the shared numeric engine through
                 * a wide cursor instead. */
                safec_wcur_ctx wctx = {sf, 0, 0, -1};
                safec_scan_cursor cur = {&wctx, safec_wcur_get,
                                         safec_wcur_unget, safec_wcur_setlim};
                safec_wcur_setlim(&wctx, width);
                switch (t) {
                case 'p':
                case 'X':
                case 'x':
                    base = 16;
                    goto wint_common;
                case 'o':
                    base = 8;
                    goto wint_common;
                case 'd':
                case 'u':
                    base = 10;
                    goto wint_common;
                case 'i':
                    base = 0;
                wint_common:
                    x = safec_intscan(&cur, (unsigned)base, 0, ULLONG_MAX);
                    if (!wctx.cnt)
                        goto match_fail;
                    if (t == 'p' && dest)
                        *(void **)dest = (void *)(uintptr_t)x;
                    else
                        safec_store_int(dest, size, x);
                    break;
                default: /* a,e,f,g,A,E,F,G */
                    y = safec_floatscan(&cur, size, 0);
                    if (!wctx.cnt)
                        goto match_fail;
                    if (dest)
                        switch (size) {
                        case SIZE_def:
                            *(float *)dest = (float)y;
                            break;
                        case SIZE_l:
                            *(double *)dest = (double)y;
                            break;
                        case SIZE_L:
                            *(long double *)dest = y;
                            break;
                        default:
                            goto fmt_fail;
                        }
                    break;
                }
            }
            break;
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
            free(wcs);
        }
    }
    FUNLOCK(sf);
    return matches;

overflow_fail:
    if (s)
        s[i] = 0;
    if (wcs)
        wcs[i] = 0;
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
