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
#endif

/* from musl: */

//#include <locale.h>
#define SIZE_hh -2
#define SIZE_h -1
#define SIZE_def 0
#define SIZE_l 1
#define SIZE_L 2
#define SIZE_ll 3

// CHECKME next 3
#define FLOCK(sf)
#define FUNLOCK(sf)

static void __toread(_SAFEC_FILE *sf) {
    int n = fread(sf->buf, 1, 1, sf->f);
    if (n != 1)
        return;
}
static unsigned long long __intscan(_SAFEC_FILE *sf, int base, int zero,
                                    unsigned long max) {
    (void)*sf;
    return 0;
}
static long double __floatscan(_SAFEC_FILE *sf, int size, int zero) {
    (void)*sf;
    return 0.0L;
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

size_t safec_wstring_read(_SAFEC_FILE *f, unsigned char *buf, size_t len) {
    const wchar_t *src = f->cookie;
    size_t k;

    if (!src)
        return 0;

    k = wcsrtombs((void *)f->buf, &src, f->buf_size, 0);
    if (k == (size_t)-1) {
        f->rpos = f->rend = 0;
        return 0;
    }

    f->rpos = f->buf;
    f->rend = f->buf + k;
    f->cookie = (void *)src;

    if (!len || !k)
        return 0;

    *buf = *f->rpos++;
    return 1;
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
    int j;
    const wchar_t *p = set;
    if (*p == '-') {
        if (c == '-')
            return 1;
        p++;
    } else if (*p == ']') {
        if (c == ']')
            return 1;
        p++;
    }
    for (; *p && *p != ']'; p++) {
        if (*p == '-' && p[1] && p[1] != ']')
            for (j = p++ [-1]; j < *p; j++)
                if (c == j)
                    return 1;
        if (c == *p)
            return 1;
    }
    return 0;
}

int safec_vfwscanf_s(_SAFEC_FILE *sf, const char *funcname, const wchar_t *fmt,
                     va_list ap) {
    int width;
    int size;
    int alloc = 0;
    // int base;
    const wchar_t *p;
    int c, t;
    char *s = NULL;
    wchar_t *wcs = NULL;
    // mbstate_t st;
    void *dest = NULL;
    int invert;
    int matches = 0;
    // unsigned long long x;
    // long double y;
    off_t pos = 0, cnt;
    static const char size_pfx[][3] = {"hh", "h", "", "l", "L", "ll"};
    char tmp[3 * sizeof(int) + 10];
    const wchar_t *set;
    size_t i, k;
    // wchar_t wc;
    int gotmatch;

    FLOCK(sf);
    fwide(sf->f, 1);

    for (p = fmt; *p; p++) {

        alloc = 0;

        if (iswspace(*p)) {
            while (iswspace(p[1]))
                p++;
            while (iswspace((c = shgetwc(sf))))
                pos++;
            shungetwc(c, sf);
            continue;
        }
        if (*p != '%' || p[1] == '%') {
            if (*p == '%') {
                p++;
                while (iswspace((c = shgetwc(sf))))
                    pos++;
            } else {
                c = shgetwc(sf);
            }
            if (c != *p) {
                shungetwc(c, sf);
                if (c < 0)
                    goto input_fail;
                goto match_fail;
            }
            pos++;
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

        /* Transform S,C -> ls,lc */
        if ((t & 0x2f) == 3) {
            t |= 32;
            size = SIZE_l;
        }

        if (t != 'n') {
            if (t != '[' && (t | 32) != 'c')
                while (iswspace((c = shgetwc(sf))))
                    pos++;
            else
                c = shgetwc(sf);
            if (c < 0)
                goto input_fail;
            shungetwc(c, sf);
        }

        switch (t) {
        case 'n':
#if 0
            safec_store_int(dest, size, pos);
            /* do not increment match count, etc! */
            continue;
#else
            {
                char tmp[128];
                snprintf(tmp, sizeof(tmp), "%s: illegal %%n", funcname);
                invoke_safe_str_constraint_handler(tmp, NULL, EINVAL);
                errno = EINVAL;
                return EOF;
            }
#endif
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

            s = (size == SIZE_def) ? dest : 0;
            wcs = (size == SIZE_l) ? dest : 0;

            gotmatch = 0;

            if (width < 1)
                width = -1;

            i = 0;
            if (alloc) {
                k = t == 'c' ? width + 1U : 31;
                if (size == SIZE_l) {
                    wcs = malloc(k * sizeof(wchar_t));
                    if (!wcs)
                        goto alloc_fail;
                } else {
                    s = malloc(k);
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
                    wcs[i++] = c;
                    if (alloc && i == k) {
                        wchar_t *tmp;
                        k += k + 1;
                        tmp = realloc(wcs, k * sizeof(wchar_t));
                        if (!tmp)
                            goto alloc_fail;
                        wcs = tmp;
                    }
                } else if (size != SIZE_l) {
                    int l = wctomb(s ? s + i : tmp, c);
                    if (l < 0)
                        goto input_fail;
                    i += l;
                    if (alloc && i > k - 4) {
                        char *tmp;
                        k += k + 1;
                        tmp = realloc(s, k);
                        if (!tmp)
                            goto alloc_fail;
                        s = tmp;
                    }
                }
                pos++;
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
            snprintf(tmp, sizeof tmp, "%.*s%.0d%s%c%%lln", 1 + !dest, "%*",
                     width, size_pfx[size + 2], t);
            cnt = 0;
            // FIXME
            if (fscanf(sf->f, tmp, dest ? dest : &cnt, &cnt) == -1)
                goto input_fail;
            else if (!cnt)
                goto match_fail;
            pos += cnt;
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
}
