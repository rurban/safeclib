/*------------------------------------------------------------------
 * vsscanf_s.c
 *
 * September 2017, Reini Urban
 * February 2022, Reini Urban
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
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#endif

/* TODO:
 %s null pointer check
 the two-arg versions of \c %c, \c %s, and \c %[.
*/

/**
 * @brief
 *    The \c vsscanf_s function reads a formatted string, and writes
 *    to a list of arguments.
 *    Reaching the end of the string is equivalent to reaching the
 *    end-of-file condition for \c fscanf.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.14 The vsscanf_s function (p: 602)
 *    http://en.cppreference.com/w/c/io/vfscanf
 *
 * @param[in]   buffer pointer to a null-terminated string to read from
 * @param[in]   fmt    format-control string.
 * @param[out]  ap     arguments to write to
 *
 * @pre Neither \c buffer nor \c fmt shall be a null pointer.
 * @pre \c fmt shall not contain the conversion specifier \c %n
 * @pre None of the arguments corresponding to \c %s is a null pointer. (not
 * yet)
 * @pre No encoding error shall occur.
 * @pre \c %c, \c %s, and \c %[ conversion specifiers each expect two
 *      arguments (the usual pointer and a value of type \c rsize_t
 *      indicating the size of the receiving array, which may be 1
 *      when reading with a \c %c into a single character) and
 *      except that the following errors are detected at runtime and
 *      call the currently installed constraint handler function. (not yet)
 *
 * @warning The current implementation just does some basic argument
 *      checks and then calls the native \c vsscanf() libc
 *      function. Thus the \c %s null pointer check and the two-arg
 *      versions of \c %c, \c %s, and \c %[ are not yet implemented.
 *
 * @return Number of receiving arguments successfully assigned, or \c EOF
 *         if read failure occurs before the first receiving argument
 *         was assigned or if there is a runtime constraint
 *         violation.
 *
 * @retval  > 0  on success, the number of arguments assigned
 * @retval  EOF  on error
 *
 * @see
 *    vvsscanf_s(), swscanf_s(), vsnprintf_s()
 *
 */

EXPORT int vsscanf_s(const char *restrict buffer, const char *restrict fmt,
                     va_list ap)
{
#if defined(HAVE_STRSTR)
    char *p;
#endif
    int ret;
    in_fct_wrap_type wrap;

    if (unlikely(buffer == NULL)) {
        invoke_safe_str_constraint_handler("vsscanf_s: buffer is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vsscanf_s: fmt is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

#if defined(HAVE_STRSTR)
    if (unlikely((p = strstr((char *)fmt, "%n")))) {
        if ((p - fmt == 0) || *(p - 1) != '%') {
            invoke_safe_str_constraint_handler("vsscanf_s: illegal %n", NULL,
                                               EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#elif defined(HAVE_STRCHR)
    if (unlikely((p = strchr(fmt, flen, 'n')))) {
        /* at the beginning or if inside, not %%n */
        if (((p - fmt >= 1) && *(p - 1) == '%') &&
            ((p - fmt == 1) || *(p - 2) != '%')) {
            invoke_safe_str_constraint_handler("vsscanf_s: illegal %n", NULL,
                                               EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#endif

    errno = 0;
    wrap.arg = (void*)buffer;
    ret = safec_vscanf_s(safec_in_buffer, "vsscanf_s", &wrap, fmt, ap);

    if (unlikely(ret < 0)) { /* always -1 EOF */
        char errstr[128] = "vsscanf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, errno);
    }

    return ret;
}

// =============================================================================
/* from musl:
----------------------------------------------------------------------
Copyright © 2005-2014 Rich Felker, et al.

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

#include <locale.h>
#define SIZE_hh -2
#define SIZE_h  -1
#define SIZE_def 0
#define SIZE_l   1
#define SIZE_L   2
#define SIZE_ll  3

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

// from MUSL. TODO: generalize, wide, ...
int safec_vscanf_s(in_fct_type in, const char *funcname,
                   const in_fct_wrap_type *wrap, const char *fmt, va_list ap)
{
    int width;
    int size;
    int alloc = 0;
    int base;
    const unsigned char *p;
    int c, t;
    char *s;
    wchar_t *wcs;
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

#define shlim(a,b)
#define shgetc(a) 'a'
#define shunget(a)
#define shcnt(a) 1
    
    // FLOCK(f);
    // if (!f->rpos) __toread(f);
    // if (!f->rpos) goto input_fail;

    for (p = (const unsigned char *)fmt; *p; p++) {

        alloc = 0;

        if (isspace(*p)) {
            while (isspace(p[1]))
                p++;
            shlim(f, 0);
            while (isspace(shgetc(f)))
                ;
            shunget(f);
            pos += shcnt(f);
            continue;
        }
        if (*p != '%' || p[1] == '%') {
            shlim(f, 0);
            if (*p == '%') {
                p++;
                while (isspace((c = shgetc(f))))
                    ;
            } else {
                c = shgetc(f);
            }
            if (c != *p) {
                shunget(f);
                if (c < 0)
                    goto input_fail;
                goto match_fail;
            }
            pos += shcnt(f);
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
        case 'n':
            p--;
            break;
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
        case 'n':
            safec_store_int(dest, size, pos);
            /* do not increment match count, etc! */
            continue;
        default:
            shlim(f, 0);
            while (isspace(shgetc(f)))
                ;
            shunget(f);
            pos += shcnt(f);
        }

        //shlim(f, width);
        //if (shgetc(f) < 0)
        //    goto input_fail;
        //shunget(f);

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
                while (scanset[(c = shgetc(f)) + 1]) {
                    switch (mbrtowc(&wc, &(char){c}, 1, &st)) {
                    case -1:
                        goto input_fail;
                    case -2:
                        continue;
                    default:
                    }
                    if (wcs)
                        wcs[i++] = wc;
                    if (alloc && i == k) {
                        k += k + 1;
                        wchar_t *tmp = realloc(wcs, k * sizeof(wchar_t));
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
                while (scanset[(c = shgetc(f)) + 1]) {
                    s[i++] = c;
                    if (i == k) {
                        k += k + 1;
                        char *tmp = realloc(s, k);
                        if (!tmp)
                            goto alloc_fail;
                        s = tmp;
                    }
                }
            } else if ((s = dest)) {
                while (scanset[(c = shgetc(f)) + 1])
                    s[i++] = c;
            } else {
                while (scanset[(c = shgetc(f)) + 1])
                    ;
            }
            shunget(f);
            if (!shcnt(f))
                goto match_fail;
            if (t == 'c' && shcnt(f) != width)
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
            x = __intscan(f, base, 0, ULLONG_MAX);
            if (!shcnt(f))
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
            y = __floatscan(f, size, 0);
            if (!shcnt(f))
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

        pos += shcnt(f);
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
    // FUNLOCK(f);
    return matches;
}
