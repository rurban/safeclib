/*------------------------------------------------------------------
 * vfwscanf_s.c
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#include "io/safec_file.h"
#endif

/**
 * @brief
 *    The \c vfwscanf_s function reads a formatted wide string.
 *    Reaching the end of the string is equivalent to reaching the
 *    end-of-file condition for \c fwscanf.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.9.1.7 The vfwscanf_s function (p: 632-633)
 *    http://en.cppreference.com/w/c/io/vfwscanf
 *
 * @param[in]   stream pointer to a FILE stream to read from
 * @param[in]   fmt    format-control wide string.
 * @param[out]  ap     arguments to write to
 *
 * @pre Neither \c stream nor \c fmt shall be a null pointer.
 * @pre \c fmt shall not contain the conversion specifier \c %n
 * @pre None of the arguments corresponding to \c %s is a null pointer.
 * @pre No encoding error shall occur.
 * @pre \c %c, \c %s, and \c %[ conversion specifiers each expect two
 *      arguments (the usual pointer and a value of type \c rsize_t
 *      indicating the size of the receiving array, which may be 1
 *      when reading with a \c %lc into a single wide character) and
 *      except that the following errors are detected at runtime and
 *      call the currently installed constraint handler function.
 *
 * @return Number of receiving arguments successfully assigned, or \c EOF
 *         if read failure occurs before the first receiving argument
 *         was assigned or if there is a runtime constraint
 *         violation.
 *
 * @todo   When an argument is not assigned to, it should be zero'd
 *         (not yet).
 *
 * @retval  int >0  on success, the number of arguments assigned
 * @retval  EOF  on error
 *
 * @see
 *    vswscanf_s(), fwscanf_s(), vfwprintf_s()
 *
 */

EXPORT int vfwscanf_s(FILE *restrict stream, const wchar_t *restrict fmt,
                      va_list ap)
{
    wchar_t *p;
    int ret;
    unsigned char buf[256];
    _SAFEC_FILE sf = {
        .f = stream,
        .buf = buf,
        .buf_size = sizeof buf,
        .cookie = (void *)stream,
        .read = safec_wstring_read,
        .lock = -1
    };

    if (unlikely(stream == NULL)) {
        invoke_safe_str_constraint_handler("vfwscanf_s: stream is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }

    if (unlikely(fmt == NULL)) {
        invoke_safe_str_constraint_handler("vfwscanf_s: fmt is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return EOF;
    }
#ifdef HAVE_MINGW32
    if (unlikely(!*fmt)) {
        errno = 0;
        return EOF;
    }
#endif

#if defined(HAVE_WCSSTR) || !defined(SAFECLIB_DISABLE_EXTENSIONS)
    if (unlikely((p = wcsstr((wchar_t *)fmt, L"%n")))) {
        if ((p - fmt == 0) || *(p - 1) != L'%') {
            invoke_safe_str_constraint_handler("vfwscanf_s: illegal %n", NULL,
                                               EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#elif defined(HAVE_WCSCHR)
    if (unlikely((p = wcschr(fmt, flen, L'n')))) {
        /* at the beginning or if inside, not %%n */
        if (((p - fmt >= 1) && *(p - 1) == L'%') &&
            ((p - fmt == 1) || *(p - 2) != L'%')) {
            invoke_safe_str_constraint_handler("vfwscanf_s: illegal %n", NULL,
                                               EINVAL);
            errno = EINVAL;
            return EOF;
        }
    }
#else
#error need wcsstr or wcschr
#endif

    errno = 0;
    //ret = vfwscanf(stream, fmt, ap);
    ret = safec_vfwscanf_s(&sf, "vfwscanf_s", fmt, ap);

    if (unlikely(ret < 0)) { /* always -1 EOF */
        char errstr[128] = "vfwscanf_s: ";
        strcat(errstr, strerror(errno));
        invoke_safe_str_constraint_handler(errstr, NULL, errno);
        /* TODO: zero the out args */
    }

    return ret;
}

#if 0
// from MUSL. TODO: safeties
int safec_vfwscanf(FILE *restrict f, const wchar_t *restrict fmt, va_list ap) {
    int width;
    int size;
    int alloc;
    const wchar_t *p;
    int c, t;
    char *s;
    wchar_t *wcs;
    void *dest = NULL;
    int invert;
    int matches = 0;
    off_t pos = 0, cnt;
    static const char size_pfx[][3] = {"hh", "h", "", "l", "L", "ll"};
    char tmp[3 * sizeof(int) + 10];
    const wchar_t *set;
    size_t i, k;

    FLOCK(f);
    fwide(f, 1);

    for (p = fmt; *p; p++) {

        alloc = 0;

        if (iswspace(*p)) {
            while (iswspace(p[1]))
                p++;
            while (iswspace((c = getwc(f))))
                pos++;
            ungetwc(c, f);
            continue;
        }
        if (*p != '%' || p[1] == '%') {
            if (*p == '%') {
                p++;
                while (iswspace((c = getwc(f))))
                    pos++;
            } else {
                c = getwc(f);
            }
            if (c != *p) {
                ungetwc(c, f);
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
        case 'n':
            p--;
            break;
        default:
            goto fmt_fail;
        }

        t = *p;

        /* Transform S,C -> ls,lc */
        if ((t & 0x2f) == 3) {
            size = SIZE_l;
            t |= 32;
        }

        if (t != 'n') {
            if (t != '[' && (t | 32) != 'c')
                while (iswspace((c = getwc(f))))
                    pos++;
            else
                c = getwc(f);
            if (c < 0)
                goto input_fail;
            ungetwc(c, f);
        }

        switch (t) {
        case 'n':
            store_int(dest, size, pos);
            /* do not increment match count, etc! */
            continue;

        case 's':
        case 'c':
        case '[':
            if (t == 'c') {
                if (width < 1)
                    width = 1;
                invert = 1;
                set = L"";
            } else if (t == 's') {
                invert = 1;
                static const wchar_t spaces[] = {
                    ' ',    '\t',   '\n',   '\r',   11,     12,
                    0x0085, 0x2000, 0x2001, 0x2002, 0x2003, 0x2004,
                    0x2005, 0x2006, 0x2008, 0x2009, 0x200a, 0x2028,
                    0x2029, 0x205f, 0x3000, 0};
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

            int gotmatch = 0;

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
                if ((c = getwc(f)) < 0)
                    break;
                if (safec_in_set(set, c) == invert)
                    break;
                if (wcs) {
                    wcs[i++] = c;
                    if (alloc && i == k) {
                        k += k + 1;
                        wchar_t *tmp = realloc(wcs, k * sizeof(wchar_t));
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
                        k += k + 1;
                        char *tmp = realloc(s, k);
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
                ungetwc(c, f);
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
            if (fscanf(f, tmp, dest ? dest : &cnt, &cnt) == -1)
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
    FUNLOCK(f);
    return matches;
}
#endif
