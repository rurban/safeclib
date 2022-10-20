/*------------------------------------------------------------------
 * safec_vfscanf_s.c
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

size_t safec_string_read(_SAFEC_FILE *f, unsigned char *buf, size_t len) {
    char *src = f->cookie;
    size_t k = len + 256;
    char *end = memchr(src, 0, k);
    if (end)
        k = end - src;
    if (k < len)
        len = k;
    memcpy(buf, src, len);
    f->rpos = (void *)(src + len);
    f->rend = (void *)(src + k);
    f->cookie = src + k;
    return len;
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

// from MUSL. TODO: safeties
int safec_vfscanf_s(_SAFEC_FILE *sf, const char *funcname, const char *fmt,
                    va_list ap) {
    int width;
    int size;
    int alloc = 0;
    int base;
    const unsigned char *p;
    int c, t;
    char *s = NULL;
    wchar_t *wcs = NULL;
    mbstate_t st;
    void *dest = NULL;
    int invert;
    int matches = 0;
    unsigned long long x;
    long double y;
    off_t pos = 0;
    unsigned char scanset[257];
    size_t i, k;
    wchar_t wc;

    FLOCK(sf);
    if (!sf->rpos)
        __toread(sf);
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
            pos += shcnt(sf);
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
            pos += shcnt(sf);
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

        /* C or S */
        if ((t & 0x2f) == 3) {
            t |= 32;
            size = SIZE_l;
        }

        switch (t) {
        case 'c':
            if (width < 1)
                width = 1;
        case '[':
            break;
        case 'n': {
            char tmp[128];
            snprintf(tmp, sizeof(tmp), "%s: illegal %%n", funcname);
            invoke_safe_str_constraint_handler(tmp, NULL, EINVAL);
            errno = EINVAL;
            return EOF;
        }
            //safec_store_int(dest, size, pos);
            /* do not increment match count, etc! */
            //continue;
        default:
            shlim(sf, 0);
            while (isspace(shgetc(sf)))
                ;
            shunget(sf);
            pos += shcnt(sf);
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
            wcs = 0;
            s = 0;
            i = 0;
            k = t == 'c' ? width + 1U : 31;
            if (size == SIZE_l) {
                if (alloc) {
                    wcs = malloc(k * sizeof(wchar_t));
                    if (!wcs)
                        goto alloc_fail;
                } else {
                    wcs = dest;
                }
                st = (mbstate_t){0};
                while (scanset[(c = shgetc(sf)) + 1]) {
                    switch (mbrtowc(&wc, &(char){c}, 1, &st)) {
                    case -1:
                        goto input_fail;
                    case -2:
                        continue;
                    default:
                        break;
                    }
                    if (wcs)
                        wcs[i++] = wc;
                    if (alloc && i == k) {
                        wchar_t *tmp;
                        k += k + 1;
                        tmp = realloc(wcs, k * sizeof(wchar_t));
                        if (!tmp)
                            goto alloc_fail;
                        wcs = tmp;
                    }
                }
                if (!mbsinit(&st))
                    goto input_fail;
            } else if (alloc) {
                s = malloc(k);
                if (!s)
                    goto alloc_fail;
                while (scanset[(c = shgetc(sf)) + 1]) {
                    s[i++] = c;
                    if (i == k) {
                        char *tmp;
                        k += k + 1;
                        tmp = realloc(s, k);
                        if (!tmp)
                            goto alloc_fail;
                        s = tmp;
                    }
                }
            } else if ((s = dest)) {
                while (scanset[(c = shgetc(sf)) + 1])
                    s[i++] = c;
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
            x = __intscan(sf, base, 0, ULLONG_MAX);
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
            y = __floatscan(sf, size, 0);
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
        default:
            goto fmt_fail;
        }

        pos += shcnt(sf);
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
