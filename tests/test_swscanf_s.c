/*------------------------------------------------------------------
 * test_swscanf_s
 * File 'wchar/swscanf_s.c'
 * Lines executed:100.00% of 24
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#ifdef HAVE_STDDEF_H
#include <stddef.h> // for ptrdiff_t
#endif

#ifdef HAVE_SWSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static wchar_t wstr1[LEN];
static wchar_t wstr2[LEN];
static char str3[LEN];

/*--------------------------------------------------*/
/* scanf core: conversions, modifiers, limits (the same engine drives
   fscanf_s, vsscanf_s, ...) */
#ifndef HAVE_SWSCANF_S
#define WCKRC(exp)                                                              \
    if (rc != (exp)) {                                                         \
        debug_printf("%s %u  Error rc=%d, expected %d\n", __FUNCTION__,        \
                     __LINE__, (int)rc, (int)(exp));                           \
        errs++;                                                                \
    }
#define WCKINT(v, exp)                                                          \
    if ((long long)(v) != (long long)(exp)) {                                  \
        debug_printf("%s %u  Error value=%lld, expected %lld\n",               \
                     __FUNCTION__, __LINE__, (long long)(v),                   \
                     (long long)(exp));                                        \
        errs++;                                                                \
    }
#define WCKERRNO(exp)                                                           \
    if (errno != (exp)) {                                                      \
        debug_printf("%s %u  Error errno=%d, expected %d\n", __FUNCTION__,     \
                     __LINE__, errno, (int)(exp));                             \
        errs++;                                                                \
    }
static volatile size_t wvsz;
static int test_wscan_core(void);
static int test_wscan_core(void) {
    int errs = 0;
    int rc;
    int a = 0, b = 0;
    unsigned int ua;
    short sh;
    signed char sc;
    long lv;
    unsigned long ulv;
    size_t zv;
    char buf[64];
    char buf2[64];
    void *pv;
    char *lc_cat;
    const char *lang = "";
    char *p1 = NULL;

    GCC_DIAG_IGNORE(-Wformat)
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    GCC_DIAG_IGNORE(-Wformat-security)

    /* integers, bases, signs */
    rc = swscanf_s(L"-17", L"%d", &a);
    WCKRC(1) WCKINT(a, -17)
    rc = swscanf_s(L"+5", L"%d", &a);
    WCKRC(1) WCKINT(a, 5)
    rc = swscanf_s(L"42", L"%u", &ua);
    WCKRC(1) WCKINT(ua, 42)
    rc = swscanf_s(L"017", L"%o", &ua);
    WCKRC(1) WCKINT(ua, 15)
    rc = swscanf_s(L"ff", L"%x", &ua);
    WCKRC(1) WCKINT(ua, 255)
    rc = swscanf_s(L"0xFF", L"%x", &ua);
    WCKRC(1) WCKINT(ua, 255)
    rc = swscanf_s(L"1aB", L"%X", &ua);
    WCKRC(1) WCKINT(ua, 0x1ab)
    rc = swscanf_s(L"0x1F", L"%i", &a);
    WCKRC(1) WCKINT(a, 31)
    rc = swscanf_s(L"017", L"%i", &a);
    WCKRC(1) WCKINT(a, 15)
    rc = swscanf_s(L"-0x10", L"%i", &a);
    WCKRC(1) WCKINT(a, -16)
    rc = swscanf_s(L"0 ", L"%i", &a);
    WCKRC(1) WCKINT(a, 0)
    rc = swscanf_s(L"0x", L"%i", &a); /* prefix without digits */
    WCKRC(0)
    rc = swscanf_s(L"0xg", L"%x", &a);
    WCKRC(0)
    rc = swscanf_s(L"089", L"%i", &a); /* octal stops at 8 */
    WCKRC(1) WCKINT(a, 0)
    rc = swscanf_s(L"-", L"%d", &a);
    WCKRC(0)
    rc = swscanf_s(L"abc", L"%d", &a);
    WCKRC(0)
    rc = swscanf_s(L"12abc", L"%d", &a);
    WCKRC(1) WCKINT(a, 12)
    rc = swscanf_s(L"1z", L"%2o", &a);
    WCKRC(1) WCKINT(a, 1)

    rc = swscanf_s(L"25", L"%i", &a); /* decimal via base 0 */
    WCKRC(1) WCKINT(a, 25)
    rc = swscanf_s(L"1 \t 2 ", L"%d  %d", &a, &b); /* whitespace run in fmt */
    WCKRC(2) WCKINT(b, 2)
    a = 77;
    rc = swscanf_s(L"12", L"%Ld", &a); /* size not valid for integers */
    WCKRC(1)

    /* size modifiers */
    sc = 0;
    rc = swscanf_s(L"300", L"%hhd", &sc);
    WCKRC(1) WCKINT(sc, (signed char)300)
    sh = 0;
    rc = swscanf_s(L"70000", L"%hd", &sh);
    WCKRC(1) WCKINT(sh, (short)70000)
    lv = 0;
    rc = swscanf_s(L"-123456789012", L"%ld", &lv);
    WCKRC(1) WCKINT(lv, -123456789012L)
    ulv = 0;
    rc = swscanf_s(L"123456789012", L"%lu", &ulv);
    WCKRC(1) WCKINT(ulv, 123456789012UL)
    zv = 0;
    rc = swscanf_s(L"123", L"%zu", &zv);
    WCKRC(1) WCKINT(zv, 123)
    pv = NULL;
    rc = swscanf_s(L"0x1234", L"%p", &pv);
    WCKRC(1) WCKINT((uintptr_t)pv, 0x1234)
    rc = swscanf_s(L"1234", L"%p", &pv);
    WCKRC(1) WCKINT((uintptr_t)pv, 0x1234)
    rc = swscanf_s(L"12", L"%*p");
    WCKRC(0)
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
    {
        long long ll = 0;
        unsigned long long ull = 0;
        rc = swscanf_s(L"-9223372036854775807", L"%lld", &ll);
        WCKRC(1) WCKINT(ll, -9223372036854775807LL)
        rc = swscanf_s(L"123456789012", L"%jd", &ll);
        WCKRC(1) WCKINT(ll, 123456789012LL)
        rc = swscanf_s(L"18446744073709551615", L"%llu", &ull);
        WCKRC(1)
        if (ull != 18446744073709551615ULL) {
            debug_printf("%s %u wrong ull\n", __FUNCTION__, __LINE__);
            errs++;
        }
        /* overflow clamps to the type limit */
        ull = 0;
        rc = swscanf_s(L"99999999999999999999999", L"%llu", &ull);
        WCKRC(1)
        if (ull != 18446744073709551615ULL) {
            debug_printf("%s %u wrong clamp\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = swscanf_s(L"-99999999999999999999999", L"%lld", &ll);
        WCKRC(1)
        rc = swscanf_s(L"0xFFFFFFFFFFFFFFFFFFFF", L"%llx", &ull);
        WCKRC(1)
        rc = swscanf_s(L"0777777777777777777777777", L"%lli", &ll);
        WCKRC(1)
        rc = swscanf_s(L"11111111111111111111", L"%2$lld %1$lld", &ll, &ll);
        WCKRC(1)
    }
#endif
#ifdef HAVE_STDDEF_H
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
    {
        ptrdiff_t td = 0;
        rc = swscanf_s(L"-7", L"%td", &td);
        WCKRC(1) WCKINT(td, -7)
    }
#endif
#endif

    /* width limits, multiple conversions, suppression */
    rc = swscanf_s(L"12345", L"%2d", &a);
    WCKRC(1) WCKINT(a, 12)
    rc = swscanf_s(L"12 34", L"%d %d", &a, &b);
    WCKRC(2) WCKINT(a, 12) WCKINT(b, 34)
    rc = swscanf_s(L"12", L"%d %d", &a, &b); /* partial match */
    WCKRC(1) WCKINT(a, 12)
    rc = swscanf_s(L"12 x", L"%d %d", &a, &b);
    WCKRC(1)
    rc = swscanf_s(L"1 2 ", L"%*d %d", &a);
    WCKRC(1) WCKINT(a, 2)
    rc = swscanf_s(L"7", L"%*d");
    WCKRC(0)
    rc = swscanf_s(L"", L"%d", &a);
    WCKRC(-1)
    rc = swscanf_s(L"   ", L"%d", &a);
    WCKRC(-1)
    rc = swscanf_s(L"", L"x");
    WCKRC(-1)
    rc = swscanf_s(L"x", L"x");
    WCKRC(0)
    rc = swscanf_s(L"y1", L"x%d", &a);
    WCKRC(0)
    rc = swscanf_s(L"x1 ", L"x%d", &a);
    WCKRC(1) WCKINT(a, 1)
    rc = swscanf_s(L"5%", L"%d%%", &a);
    WCKRC(1) WCKINT(a, 5)
    rc = swscanf_s(L"  %7 ", L"%%%d", &a);
    WCKRC(1) WCKINT(a, 7)
    rc = swscanf_s(L"%", L"%%");
    WCKRC(0)
    rc = swscanf_s(L"a", L"%%");
    WCKRC(0)
    rc = swscanf_s(L"", L"%%");
    WCKRC(-1)
    rc = swscanf_s(L"1,2 ", L"%d,%d", &a, &b);
    WCKRC(2) WCKINT(b, 2)
    rc = swscanf_s(L"1;2", L"%d,%d", &a, &b);
    WCKRC(1)
    rc = swscanf_s(L"5 6 ", L"%2$d %1$d", &a, &b);
    WCKRC(2) WCKINT(b, 5) WCKINT(a, 6)

    /* illegal %n behind a width or modifier is rejected by the core */
    errno = 0;
    rc = swscanf_s(L"5", L"%5n", &a);
    WCKRC(EOF) WCKERRNO(EINVAL)
    errno = 0;
    rc = swscanf_s(L"5", L"%hn", &a);
    WCKRC(EOF) WCKERRNO(EINVAL)
    errno = 0;
    rc = swscanf_s(L"5 ", L"%d%5n", &a, &b);
    WCKRC(EOF) WCKERRNO(EINVAL)

    /* unknown conversion */
    rc = swscanf_s(L"5", L"%y", &a);
    WCKRC(-1)
    rc = swscanf_s(L"5 ", L"%d%y", &a, &b);
    WCKRC(1)

    /* %c */
    buf[0] = buf[1] = buf[2] = 'z';
    rc = swscanf_s(L"abc", L"%c", buf, (rsize_t)1);
    WCKRC(1) WCKINT(buf[0], 'a') WCKINT(buf[1], 'z')
    rc = swscanf_s(L"abc", L"%3c", buf, (rsize_t)3);
    WCKRC(1) WCKINT(buf[2], 'c')
    rc = swscanf_s(L" b", L"%c", buf, (rsize_t)1); /* %c keeps whitespace */
    WCKRC(1) WCKINT(buf[0], ' ')
    rc = swscanf_s(L"a", L"%2c", buf, (rsize_t)2); /* input too short */
    WCKRC(0)
    rc = swscanf_s(L"", L"%c", buf, (rsize_t)1);
    WCKRC(-1)
    rc = swscanf_s(L"abc", L"%*c%c", buf, (rsize_t)1);
    WCKRC(1) WCKINT(buf[0], 'b')
    rc = swscanf_s(L"abc", L"%*3c");
    WCKRC(0)
    rc = swscanf_s(L"abc", L"%0c", buf, (rsize_t)1); /* width 0 => 1 */
    WCKRC(1)
    errno = 0;
    rc = swscanf_s(L"abc", L"%c", buf, (rsize_t)0);
    WCKRC(EOF) WCKERRNO(ESZEROL)
    errno = 0;
    rc = swscanf_s(L"abc", L"%s", buf, (rsize_t)0);
    WCKRC(EOF) WCKERRNO(ESZEROL)
    errno = 0;
    rc = swscanf_s(L"abc", L"%s", buf, (rsize_t)(RSIZE_MAX_STR + 1));
    WCKRC(EOF) WCKERRNO(ESZEROL)
    errno = 0;
    rc = swscanf_s(L"abc", L"%3c", buf, (rsize_t)2); /* too small */
    WCKRC(EOF) WCKERRNO(ESNOSPC)

    /* %s */
    rc = swscanf_s(L"  hello world", L"%s", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "hello")
    rc = swscanf_s(L"hello world", L"%3s", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "hel")
    rc = swscanf_s(L"one two", L"%s %s", buf, (rsize_t)sizeof(buf), buf2,
                  (rsize_t)sizeof(buf2));
    WCKRC(2) EXPSTR(buf2, "two")
    rc = swscanf_s(L"one two", L"%*s %s", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "two")
    rc = swscanf_s(L"one", L"%*s");
    WCKRC(0)
    errno = 0;
    rc = swscanf_s(L"abcdef", L"%s", buf, (rsize_t)4); /* needs 7 */
    WCKRC(EOF) WCKERRNO(ESNOSPC)
    rc = swscanf_s(L"abc", L"%s", buf, (rsize_t)4);
    WCKRC(1) EXPSTR(buf, "abc")
    /* unknown dest size (not statically known): dmax from volatile */
    {
        char *heap = (char *)malloc(16);
        wvsz = 16;
        rc = swscanf_s(L"heap", L"%s", heap, wvsz);
        WCKRC(1) EXPSTR(heap, "heap")
        free(heap);
    }

    /* %[ scansets */
    rc = swscanf_s(L"abcd", L"%[a-c]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "abc")
    rc = swscanf_s(L"abxd", L"%[^x]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "ab")
    rc = swscanf_s(L"]a]b", L"%[]a]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "]a]")
    rc = swscanf_s(L"b]a", L"%[^]a]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "b")
    rc = swscanf_s(L"-a-b", L"%[-a]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "-a-")
    rc = swscanf_s(L"a-b", L"%[^-]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "a")
    rc = swscanf_s(L"ab-c", L"%[ab-]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "ab-")
    rc = swscanf_s(L"0123xyz", L"%[0-9x-z]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "0123xyz")
    rc = swscanf_s(L"  abc", L"%[a-c]", buf, (rsize_t)sizeof(buf)); /* no skip */
    WCKRC(0)
    rc = swscanf_s(L"xyz", L"%[a-c]", buf, (rsize_t)sizeof(buf));
    WCKRC(0)
    rc = swscanf_s(L"abcdef", L"%3[a-z]", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "abc")
    rc = swscanf_s(L"abcdef", L"%*[a-c]%s", buf, (rsize_t)sizeof(buf));
    WCKRC(1) EXPSTR(buf, "def")
    rc = swscanf_s(L"abcdef", L"%*[a-z]");
    WCKRC(0)
    rc = swscanf_s(L"abc", L"%[abc", buf, (rsize_t)sizeof(buf)); /* unterminated */
    WCKRC(-1)
    errno = 0;
    rc = swscanf_s(L"abcdef", L"%[a-f]", buf, (rsize_t)3);
    WCKRC(EOF) WCKERRNO(ESNOSPC)
    rc = swscanf_s(L"", L"%[a-f]", buf, (rsize_t)3);
    WCKRC(-1)

    /* %m allocating conversions */
    {
        char *p1 = NULL;
        char *p2 = NULL;
        const char *longs = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuv"
                            "wxyzabcdefghijklmnopqrstuvwxyz";
        const wchar_t *wlongs = L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuv"
                                L"wxyzabcdefghijklmnopqrstuvwxyz";
        rc = swscanf_s(L"hello world", L"%ms", &p1);
        WCKRC(1)
        if (p1) {
            EXPSTR(p1, "hello")
            free(p1);
            p1 = NULL;
        }
        rc = swscanf_s(wlongs, L"%ms", &p1); /* forces realloc growth */
        WCKRC(1)
        if (p1) {
            EXPSTR(p1, longs)
            free(p1);
            p1 = NULL;
        }
        rc = swscanf_s(L"abc1", L"%m[a-c]", &p1);
        WCKRC(1)
        if (p1) {
            EXPSTR(p1, "abc")
            free(p1);
            p1 = NULL;
        }
        rc = swscanf_s(L"abcdef", L"%3mc", &p2);
        WCKRC(1)
        if (p2) {
            WCKINT(p2[2], 'c')
            free(p2);
            p2 = NULL;
        }
        rc = swscanf_s(L"xyz", L"%m[a-c]", &p1); /* nothing matches */
        WCKRC(0)
        rc = swscanf_s(L"one two", L"%*ms %ms", &p1);
        WCKRC(1)
        if (p1) {
            EXPSTR(p1, "two")
            free(p1);
            p1 = NULL;
        }
    }

    /* wide destinations from the byte scanner */
    {
        wchar_t wb[32];
        wchar_t *wp = NULL;
        rc = swscanf_s(L"wide str", L"%ls", wb, (rsize_t)32);
        WCKRC(1) WCKINT(wb[0], 'w') WCKINT(wb[3], 'e') WCKINT(wb[4], 0)
        rc = swscanf_s(L"wide str", L"%S", wb, (rsize_t)32);
        WCKRC(1) WCKINT(wb[0], 'w')
        rc = swscanf_s(L"abcdef", L"%3lc", wb, (rsize_t)3);
        WCKRC(1) WCKINT(wb[2], 'c')
        rc = swscanf_s(L"abcdef", L"%C", wb, (rsize_t)1);
        WCKRC(1) WCKINT(wb[0], 'a')
        rc = swscanf_s(L"abc1", L"%l[a-c]", wb, (rsize_t)32);
        WCKRC(1) WCKINT(wb[2], 'c') WCKINT(wb[3], 0)
        errno = 0;
        rc = swscanf_s(L"abcdef", L"%ls", wb, (rsize_t)3);
        WCKRC(EOF) WCKERRNO(ESNOSPC)
        rc = swscanf_s(L"*", L"%*ls");
        WCKRC(0)
        rc = swscanf_s(L"wide str", L"%mls", &wp);
        WCKRC(1)
        if (wp) {
            WCKINT(wp[0], 'w') WCKINT(wp[3], 'e') WCKINT(wp[4], 0)
            free(wp);
            wp = NULL;
        }
        rc = swscanf_s(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",
                      L"%mls", &wp); /* growth */
        WCKRC(1)
        if (wp) {
            WCKINT(wp[40], 'o')
            free(wp);
            wp = NULL;
        }
        rc = swscanf_s(L"abc1", L"%m[a-c]", &wp);
        WCKRC(1)
        free(wp);
        wp = NULL;
        {
            /* UTF-8: wide chars converted to multibyte; and in the plain
               C locale a char above 0x7f is not convertible */
            wchar_t wide[4];
            char mb[32];
            wide[0] = 0xe4;
            wide[1] = L'x';
            wide[2] = 0;
            SETLOCALE_UTF8;
            if (lc_cat && strstr(lc_cat, "UTF")) {
                rc = swscanf_s(wide, L"%s", mb, (rsize_t)32);
                WCKRC(1) WCKINT((unsigned char)mb[0], 0xc3)
                    WCKINT((unsigned char)mb[1], 0xa4) WCKINT(mb[2], 'x')
                rc = swscanf_s(wide, L"%s", mb, (rsize_t)2); /* too small */
                WCKRC(EOF)
                rc = swscanf_s(wide, L"%ms", &p1);
                WCKRC(1)
                free(p1);
                p1 = NULL;
                rc = swscanf_s(wide, L"%[^y]", mb, (rsize_t)32);
                WCKRC(1)
            }
            SETLOCALE_C;
            errno = 0;
#ifndef __APPLE__ /* its C locale converts any wchar_t */
            rc = swscanf_s(wide, L"%s", mb, (rsize_t)32); /* EILSEQ */
            WCKRC(EOF)
            rc = swscanf_s(wide, L"%mc", &p1);
            WCKRC(EOF)
#endif
        }
        {
            /* code points above 255 are no digits */
            wchar_t wide[3];
            wide[0] = 0x100;
            wide[1] = 0;
            rc = swscanf_s(wide, L"%d", &a);
            WCKRC(0)
        }
    }

    /* floating point */
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
    {
        float f = 0;
        double d = 0;
        long double ld = 0;
        wchar_t big[1600];
        int n;

        rc = swscanf_s(L"3.25", L"%f", &f);
        WCKRC(1) WCKINT(f * 100, 325)
        rc = swscanf_s(L"-.5", L"%f", &f);
        WCKRC(1) WCKINT(f * 10, -5)
        rc = swscanf_s(L"5.", L"%lf", &d);
        WCKRC(1) WCKINT(d, 5)
        rc = swscanf_s(L"+7", L"%lf", &d);
        WCKRC(1) WCKINT(d, 7)
        rc = swscanf_s(L"123.456", L"%5lf", &d);
        WCKRC(1) WCKINT(d * 10, 1234)
        rc = swscanf_s(L"1.5 2.5", L"%f %lf", &f, &d);
        WCKRC(2) WCKINT(d * 10, 25)
        rc = swscanf_s(L"abc", L"%f", &f);
        WCKRC(0)
        rc = swscanf_s(L"", L"%f", &f);
        WCKRC(-1)
        rc = swscanf_s(L"2.5", L"%*f");
        WCKRC(0)
        rc = swscanf_s(L"2.5", L"%Lf", &ld);
#ifndef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
        WCKRC(1) WCKINT(ld * 10, 25)
#endif
        /* size not valid for floats */
        rc = swscanf_s(L"2.5", L"%hf", &f);
        WCKRC(0)
        rc = swscanf_s(L"2.5", L"%hhf", &f);
        WCKRC(0)
        rc = swscanf_s(L"2.5", L"%jf", &f);
        WCKRC(0)

        /* inf and nan, any case */
        rc = swscanf_s(L"inf", L"%f", &f);
        WCKRC(1)
        if (!(f > 3e38f)) {
            debug_printf("%s %u  not inf\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = swscanf_s(L"-INF", L"%f", &f);
        WCKRC(1)
        if (!(f < -3e38f)) {
            debug_printf("%s %u  not -inf\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = swscanf_s(L"+Infinity", L"%lf", &d);
        WCKRC(1)
        if (!(d > 1e308)) {
            debug_printf("%s %u  not inf\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = swscanf_s(L"infinit", L"%lf", &d); /* partial infinity */
        WCKRC(0)
        rc = swscanf_s(L"infx", L"%f", &f);
        WCKRC(1)
        rc = swscanf_s(L"in", L"%f", &f);
        WCKRC(0)
        rc = swscanf_s(L"nan", L"%lf", &d);
        WCKRC(1)
        if (d == d) {
            debug_printf("%s %u  not nan\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = swscanf_s(L"-NaN", L"%lf", &d);
        WCKRC(1)
        rc = swscanf_s(L"nan(0x1f_a)", L"%f", &f);
        WCKRC(1)
        if (f == f) {
            debug_printf("%s %u  not nan\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = swscanf_s(L"nan(1", L"%f", &f); /* unterminated payload */
        WCKRC(0)
        rc = swscanf_s(L"nan()", L"%f", &f);
        WCKRC(1)
        rc = swscanf_s(L"nan(a-b)", L"%f", &f); /* illegal payload char */
        WCKRC(0)
        rc = swscanf_s(L"na", L"%f", &f);
        WCKRC(0)

#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
        rc = swscanf_s(L"1e3", L"%e", &f);
        WCKRC(1) WCKINT(f, 1000)
        rc = swscanf_s(L"1E-2", L"%lg", &d);
        WCKRC(1) WCKINT(d * 100, 1)
        rc = swscanf_s(L"-.5e1", L"%lE", &d);
        WCKRC(1) WCKINT(d, -5)
        rc = swscanf_s(L"2.5e+1x", L"%lG", &d);
        WCKRC(1) WCKINT(d, 25)
        rc = swscanf_s(L"1.2.3 ", L"%lf", &d); /* second radix point ends it */
        WCKRC(1) WCKINT(d * 10, 12)
        rc = swscanf_s(L"1e-5x", L"%lf", &d);
        WCKRC(1) WCKINT(d * 1e5, 1)
        /* in-memory input: no unget after EOF or after the width limit */
        {
            int ia = -1, ib = -1;
            float g0 = 0;
            wchar_t w1 = '?', w2 = '?';
            rc = swscanf_s(L"12", L"%d %d", &ia, &ib);
            WCKRC(1) WCKINT(ia, 12) WCKINT(ib, -1)
            rc = swscanf_s(L"12", L"%d%lc", &ia, &w1, 1);
            WCKRC(1) WCKINT(ia, 12)
            rc = swscanf_s(L"1234", L"%2d%d", &ia, &ib);
            WCKRC(2) WCKINT(ia, 12) WCKINT(ib, 34)
            rc = swscanf_s(L"1234", L"%2d%lc%lc", &ia, &w1, 1, &w2, 1);
            WCKRC(3) WCKINT(ia, 12) WCKINT(w1, '3') WCKINT(w2, '4')
            rc = swscanf_s(L"1234", L"%2x%lc", &ia, &w1, 1);
            WCKRC(2) WCKINT(ia, 0x12) WCKINT(w1, '3')
            rc = swscanf_s(L"1.25", L"%3f%lc", &g0, &w1, 1);
            WCKRC(2) WCKINT(g0 * 100, 120) WCKINT(w1, '5')
            rc = swscanf_s(L"7", L"%d%lc", &ia, &w1, 1);
            WCKRC(1) WCKINT(ia, 7)
        }
        /* invalid exponent: the e/p and what follows are pushed back exactly
   once, like musl (glibc rejects these inputs, so there is no oracle) */
        {
            wchar_t c1 = '?', c2 = '?';
            float g = -7;
            rc = swscanf_s(L"1ex", L"%f%lc%lc", &g, &c1, 1, &c2, 1);
            WCKRC(3) WCKINT(g, 1) WCKINT(c1, 'e') WCKINT(c2, 'x')
            rc = swscanf_s(L"1e-x", L"%f%lc%lc", &g, &c1, 1, &c2, 1);
            WCKRC(3) WCKINT(g, 1) WCKINT(c1, 'e') WCKINT(c2, '-')
            rc = swscanf_s(L"0x1pz", L"%f%lc%lc", &g, &c1, 1, &c2, 1);
            WCKRC(3) WCKINT(g, 1) WCKINT(c1, 'p') WCKINT(c2, 'z')
            rc = swscanf_s(L"0x1p-z", L"%f%lc%lc", &g, &c1, 1, &c2, 1);
            WCKRC(3) WCKINT(g, 1) WCKINT(c1, 'p') WCKINT(c2, '-')
            rc = swscanf_s(L"1e5x", L"%f%lc", &g, &c1, 1);
            WCKRC(2) WCKINT(g, 100000) WCKINT(c1, 'x')
            rc = swscanf_s(L"1ex", L"%f", &g);
            WCKRC(1) WCKINT(g, 1)
        }
        /* no digits at all: matching failure, nothing stored (glibc oracle) */
        {
            float g = -7;
            rc = swscanf_s(L"-", L"%f", &g);
            WCKRC(0) WCKINT(g, -7)
            rc = swscanf_s(L".", L"%f", &g);
            WCKRC(0) WCKINT(g, -7)
            rc = swscanf_s(L"0x", L"%f", &g);
            WCKRC(0) WCKINT(g, -7)
            rc = swscanf_s(L"0xg", L"%f", &g);
            WCKRC(0) WCKINT(g, -7)
            rc = swscanf_s(L"+", L"%f", &g);
            WCKRC(0) WCKINT(g, -7)
            rc = swscanf_s(L"-.x", L"%f", &g);
            WCKRC(0) WCKINT(g, -7)
            rc = swscanf_s(L"0xp", L"%f", &g);
            WCKRC(0) WCKINT(g, -7)
        }
        /* dropped digits keep the magnitude: 1 followed by 700 zeros, e-690 */
        {
            wchar_t big[800];
            double dv = 0;
            int k;
            big[0] = '1';
            for (k = 1; k <= 700; k++)
                big[k] = '0';
            big[701] = 'e';
            big[702] = '-';
            big[703] = '6';
            big[704] = '9';
            big[705] = '0';
            big[706] = 0;
            rc = swscanf_s(big, L"%lf", &dv);
            WCKRC(1) WCKINT(dv, 10000000000)
        }
        rc = swscanf_s(L"1e999", L"%lf", &d); /* overflow */
        WCKRC(1)
        if (!(d > 1e308)) {
            debug_printf("%s %u  no overflow to inf\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = swscanf_s(L"-1e999", L"%lf", &d);
        WCKRC(1)
        if (!(d < -1e308)) {
            debug_printf("%s %u  no overflow to -inf\n", __FUNCTION__,
                         __LINE__);
            errs++;
        }
        rc = swscanf_s(L"1e50", L"%f", &f); /* float overflow */
        WCKRC(1)
        if (!(f > 3e38f)) {
            debug_printf("%s %u  no float overflow\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = swscanf_s(L"1e-999", L"%lf", &d); /* underflow */
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"1e-50", L"%f", &f);
        WCKRC(1) WCKINT(f == 0, 1)
        rc = swscanf_s(L"4.9e-324", L"%lf", &d); /* denormal min */
        WCKRC(1) WCKINT(d > 0, 1)
        rc = swscanf_s(L"2.2250738585072011e-308", L"%lf", &d);
        WCKRC(1) WCKINT(d > 0, 1)
        rc = swscanf_s(L"1.7976931348623157e308", L"%lf", &d);
        WCKRC(1) WCKINT(d > 1e308, 1)
        rc = swscanf_s(L"1e99999999999999999999", L"%lf", &d); /* huge exponent */
        WCKRC(1)
        rc = swscanf_s(L"1e-99999999999999999999", L"%lf", &d);
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"0e99999999999999999999", L"%lf", &d);
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"0.0000000", L"%lf", &d);
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"-0.0", L"%lf", &d);
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"0.00000000000000000000000000001e30", L"%lf", &d);
        WCKRC(1) WCKINT(d > 0, 1)
        rc = swscanf_s(L"123456789012345678901234567890.5", L"%lf", &d);
        WCKRC(1) WCKINT(d > 1e29, 1)
        rc = swscanf_s(L"1.00000000000000000000000000000000000001", L"%lf", &d);
        WCKRC(1) WCKINT(d == 1, 1)
        rc = swscanf_s(L"9007199254740993", L"%lf", &d); /* round to even */
        WCKRC(1)
        rc = swscanf_s(L"1.5e1", L"%Lf", &ld);
#ifndef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
        WCKRC(1) WCKINT(ld, 15)
        rc = swscanf_s(L"1e4000", L"%Lf", &ld);
        WCKRC(1)
        rc = swscanf_s(L"1e-5000", L"%Lf", &ld);
        WCKRC(1) WCKINT(ld == 0, 1)
        rc = swscanf_s(L"3.14159265358979323846264338327950288", L"%Lf", &ld);
        WCKRC(1) WCKINT(ld > 3, 1)
        rc = swscanf_s(L"1e-4945", L"%Lf", &ld); /* long double denormal */
        WCKRC(1)
        rc = swscanf_s(L"3.6e-4951", L"%Lf", &ld);
        WCKRC(1)
        rc = swscanf_s(L"1.18973149535723176502e+4932", L"%Lf", &ld);
        WCKRC(1)
#endif

        /* very long mantissas */
        wmemset(big, L'1', 1500);
        big[1500] = '\0';
        rc = swscanf_s(big, L"%lf", &d);
        WCKRC(1) WCKINT(d > 1e308, 1)
        big[0] = '0';
        big[1] = '.';
        rc = swscanf_s(big, L"%lf", &d);
        WCKRC(1) WCKINT(d > 0.1, 1)
        wcscpy(big + 600, L"e-400"); /* 0.111... e-400 */
        rc = swscanf_s(big, L"%lf", &d);
        WCKRC(1)
        wmemset(big, L'0', 1500);
        big[1500] = '\0';
        big[1499] = '5';
        big[1] = '.';
        rc = swscanf_s(big, L"%lf", &d); /* tiny, long leading zeros */
        WCKRC(1) WCKINT(d == 0, 1)
        wmemset(big, L'9', 1500);
        big[1500] = '\0';
        wcscpy(big + 400, L".9999e-399");
        rc = swscanf_s(big, L"%lf", &d);
        WCKRC(1)
        wcscpy(big, L"1");
        for (n = 0; n < 700; n++)
            wcscat(big, L"0");
        wcscat(big, L"e-690");
        rc = swscanf_s(big, L"%lf", &d);
        WCKRC(1) WCKINT(d > 1e9, 1)
        /* many digits with many exponents */
        wcscpy(big, L"0.");
        for (n = 0; n < 40; n++)
            wcscat(big, L"1234567890");
        wcscat(big, L"e5");
        rc = swscanf_s(big, L"%lf", &d);
        WCKRC(1)
        rc = swscanf_s(big, L"%f", &f);
        WCKRC(1)
        /* denormal float and rounding of exactly half ulp */
        rc = swscanf_s(L"1.401298464324817e-45", L"%f", &f);
        WCKRC(1)
        rc = swscanf_s(L"7e-46", L"%f", &f);
        WCKRC(1)
        rc = swscanf_s(L"1.17549435e-38", L"%f", &f);
        WCKRC(1)
        rc = swscanf_s(L"16777217", L"%f", &f);
        WCKRC(1)
        rc = swscanf_s(L"3.4028236e38", L"%f", &f);
        WCKRC(1)
        rc = swscanf_s(L"3.40282347e38", L"%f", &f);
        WCKRC(1)
        rc = swscanf_s(L"0.1", L"%f", &f);
        WCKRC(1)
#endif /* !EXPONENTIAL */

        /* hex floats */
        rc = swscanf_s(L"0x1.8.8 ", L"%la", &d);
        WCKRC(1) WCKINT(d * 2, 3)
        rc = swscanf_s(L"0x10", L"%lf", &d);
        WCKRC(1) WCKINT(d, 16)
        rc = swscanf_s(L"0x1.8p1", L"%lf", &d);
        WCKRC(1) WCKINT(d, 3)
        rc = swscanf_s(L"0X.8P1", L"%a", &f);
        WCKRC(1) WCKINT(f, 1)
        rc = swscanf_s(L"-0x1p-2", L"%lA", &d);
        WCKRC(1) WCKINT(d * 4, -1)
        rc = swscanf_s(L"0x1P+2", L"%la", &d);
        WCKRC(1) WCKINT(d, 4)
        rc = swscanf_s(L"0x1p", L"%la", &d); /* incomplete exponent */
        WCKRC(1) WCKINT(d, 1)
        rc = swscanf_s(L"0x1p-", L"%la", &d);
        WCKRC(1) WCKINT(d, 1)
        rc = swscanf_s(L"0x1pz", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0x0p0", L"%la", &d);
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"0x0.0000p5", L"%la", &d);
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"0x1.fffffffffffffp1023", L"%la", &d); /* DBL_MAX */
        WCKRC(1) WCKINT(d > 1e308, 1)
        rc = swscanf_s(L"0x1p1024", L"%la", &d); /* overflow */
        WCKRC(1) WCKINT(d > 1e308, 1)
        rc = swscanf_s(L"-0x1p1024", L"%la", &d);
        WCKRC(1) WCKINT(d < -1e308, 1)
        rc = swscanf_s(L"0x1p-1074", L"%la", &d); /* min denormal */
        WCKRC(1) WCKINT(d > 0, 1)
        rc = swscanf_s(L"0x1p-1075", L"%la", &d); /* half: round to even */
        WCKRC(1)
        rc = swscanf_s(L"0x1.8p-1075", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0x1p-1100", L"%la", &d); /* underflow */
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"0x1p-99999999999999999999", L"%la", &d);
        WCKRC(1) WCKINT(d == 0, 1)
        rc = swscanf_s(L"0x1p99999999999999999999", L"%la", &d);
        WCKRC(1) WCKINT(d > 1e308, 1)
        rc = swscanf_s(L"0x1p-149", L"%a", &f); /* float min denormal */
        WCKRC(1) WCKINT(f > 0, 1)
        rc = swscanf_s(L"0x1p128", L"%a", &f); /* float overflow */
        WCKRC(1) WCKINT(f > 3e38f, 1)
        rc = swscanf_s(L"0x1.fffffep127", L"%a", &f);
        WCKRC(1)
        rc = swscanf_s(L"0x1.ffffffp127", L"%a", &f); /* rounds up to inf */
        WCKRC(1)
        rc = swscanf_s(L"0x1.0000000000000800000p0", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0x1.00000000000008p0", L"%la", &d); /* half ulp */
        WCKRC(1)
        rc = swscanf_s(L"0x1.00000000000018p0", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0x1.00000000000008000001p0", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0x1.00000000000007fffffp0", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0x3.8p0", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0xf.fp0", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0x1.2345678901234567890123456789p0", L"%la", &d);
        WCKRC(1) WCKINT(d > 1, 1)
        rc = swscanf_s(L"0x0.0000000000000000000000000000000001p0", L"%la", &d);
        WCKRC(1)
        rc = swscanf_s(L"0x1.8p0", L"%Lf", &ld);
#ifndef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
        WCKRC(1) WCKINT(ld * 2, 3)
        rc = swscanf_s(L"0x1p16384", L"%La", &ld);
        WCKRC(1)
        rc = swscanf_s(L"0x1p-16445", L"%La", &ld);
        WCKRC(1)
        rc = swscanf_s(L"0x1p-16446", L"%La", &ld);
        WCKRC(1)
        rc = swscanf_s(L"0x1.ffffffffffffffffp16383", L"%La", &ld);
        WCKRC(1)
        rc = swscanf_s(L"0x1.fffffffffffffffffp16383", L"%La", &ld);
        WCKRC(1)
        rc = swscanf_s(L"0x8p-3", L"%La", &ld);
        WCKRC(1)
#endif
        rc = swscanf_s(L"0x12345678", L"%5lf", &d); /* width stops inside */
        WCKRC(1)
    }
#endif /* !FLOAT */

    GCC_DIAG_RESTORE
    GCC_DIAG_RESTORE
    GCC_DIAG_RESTORE
    return errs;
}
#endif /* !HAVE_SWSCANF_S */

int main(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int num = 0;
    int errs = 0;
    int have_wine = 0;

    /*--------------------------------------------------*/

#ifdef __MINGW32__
    wcscpy(wstr1, L" ");
#endif
    print_msvcrt(use_msvcrt);

    /* wine msvcrt doesn't check fmt==NULL */
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_SWSCANF_S))
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    rc = swscanf_s(wstr1, NULL, NULL);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif
#else
    printf("Using wine\n");
    use_msvcrt = 1;
    have_wine = 1;
#endif

    /*--------------------------------------------------*/

    wstr2[0] = '\0';
    /* msvcrt doesn't check src==NULL neither */
#if !(defined(TEST_MSVCRT) && defined(HAVE_SWSCANF_S))
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty src")
    rc = swscanf_s(NULL, L"%ls", wstr2);
    ERREOF_MSVC(ESNULLP, EINVAL);
    WEXPSTR(wstr2, L"");
#endif
#endif

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"      24");
    /* msvcrt doesn't check for %n neither */
    rc = swscanf_s(wstr1, L"%ls %n", wstr2, LEN, &ind);
#ifdef HAVE_CT_BOS_OVR
    init_msvcrt(errno != EINVAL, &use_msvcrt);
#endif
    if (!use_msvcrt) {
        ERREOF(EINVAL);
        WEXPSTR(wstr2, L"");
    } else { /* msvcrt is pretty broken */
        ERR(1);
        ERRNO(0);
        WEXPSTR(wstr2, L"24");
    }

    rc = swscanf_s(wstr1, L"%%n");
#ifdef BSD_OR_NEWLIB_LIKE
    if (rc != 0) { /* BSD + cygwin they all return -1 on %%n */
        printf("%s %u wrong vswscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    } else
#endif
        ERR(0); /* e.g. cygwin32 vswscanf() returns -1 */
    ERRNO(0);

    rc = swscanf_s(wstr1, L"%ls %%n", wstr2, LEN);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong vswscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    } else
#endif
        ERR(1);
    ERRNO(0);
    WEXPSTR(wstr2, L"24");

    rc = swscanf_s(wstr1, L"%ls %%n", wstr2, 6);
    ERR(1);
    ERRNO(0);
    WEXPSTR(wstr2, L"24");

    rc = swscanf_s(wstr1, L"%s %%n", str3, 6);
    ERR(1);
    ERRNO(0);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    EXPSTR(str3, "24");
#else
    EXPSTR(str3, "2");
#endif

    rc = swscanf_s(wstr1, L" %d", &num);
    ERR(1);
    ERRNO(0);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/
    /* floating point / long long / long double / ptrdiff_t coverage,
       exercising --disable-float, --disable-float-exp,
       --disable-long-long, --disable-long-double and
       --disable-printf-ptrdiff */
    {
        float fval;
        double dval;
        long double ldval;
        long long llval;
#ifdef HAVE_STDDEF_H
        ptrdiff_t tval;
#endif

        /* %f: plain decimal, gated only by --disable-float */
        wcscpy(wstr1, L"3.5");
        fval = -1;
        rc = swscanf_s(wstr1, L"%f", &fval);
#ifdef PRINTF_DISABLE_SUPPORT_FLOAT
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (fval != 3.5f) {
            debug_printf("%s %u wrong float arg: %f\n", __FUNCTION__, __LINE__,
                         (double)fval);
            errs++;
        }
#endif

        /* %lf: double, same gate as %f */
        wcscpy(wstr1, L"-2.25");
        dval = -1;
        rc = swscanf_s(wstr1, L"%lf", &dval);
#ifdef PRINTF_DISABLE_SUPPORT_FLOAT
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (dval != -2.25) {
            debug_printf("%s %u wrong double arg: %f\n", __FUNCTION__, __LINE__,
                         dval);
            errs++;
        }
#endif

        /* %e: exponential notation, additionally gated by
           --disable-float-exp */
        wcscpy(wstr1, L"3.5e2");
        fval = -1;
        rc = swscanf_s(wstr1, L"%e", &fval);
#if defined(PRINTF_DISABLE_SUPPORT_FLOAT) ||                                   \
    defined(PRINTF_DISABLE_SUPPORT_EXPONENTIAL)
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (fval != 350.0f) {
            debug_printf("%s %u wrong float arg: %f\n", __FUNCTION__, __LINE__,
                         (double)fval);
            errs++;
        }
#endif

        /* %g: adaptive exponential notation, same gate as %e */
        wcscpy(wstr1, L"3.5e2");
        fval = -1;
        rc = swscanf_s(wstr1, L"%g", &fval);
#if defined(PRINTF_DISABLE_SUPPORT_FLOAT) ||                                   \
    defined(PRINTF_DISABLE_SUPPORT_EXPONENTIAL)
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (fval != 350.0f) {
            debug_printf("%s %u wrong float arg: %f\n", __FUNCTION__, __LINE__,
                         (double)fval);
            errs++;
        }
#endif

        /* %a: hex float notation, same gate as %e */
        wcscpy(wstr1, L"0x1.cp+1");
        fval = -1;
        rc = swscanf_s(wstr1, L"%a", &fval);
#if defined(PRINTF_DISABLE_SUPPORT_FLOAT) ||                                   \
    defined(PRINTF_DISABLE_SUPPORT_EXPONENTIAL)
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (fval != 3.5f) {
            debug_printf("%s %u wrong float arg: %f\n", __FUNCTION__, __LINE__,
                         (double)fval);
            errs++;
        }
#endif

        /* %Lf: long double, gated by --disable-long-double */
        wcscpy(wstr1, L"1.125");
        ldval = -1;
        rc = swscanf_s(wstr1, L"%Lf", &ldval);
#if defined(PRINTF_DISABLE_SUPPORT_FLOAT) ||                                   \
    defined(PRINTF_DISABLE_SUPPORT_LONG_DOUBLE)
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (ldval != 1.125L) {
            debug_printf("%s %u wrong long double arg: %Lf\n", __FUNCTION__,
                         __LINE__, ldval);
            errs++;
        }
#endif

        /* %lld: long long, gated by --disable-long-long */
        wcscpy(wstr1, L"123456789012");
        llval = -1;
        rc = swscanf_s(wstr1, L"%lld", &llval);
#ifdef PRINTF_DISABLE_SUPPORT_LONG_LONG
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (llval != 123456789012LL) {
            debug_printf("%s %u wrong long long arg: %lld\n", __FUNCTION__,
                         __LINE__, llval);
            errs++;
        }
#endif

#ifdef HAVE_STDDEF_H
        /* %td: ptrdiff_t, gated by --disable-printf-ptrdiff */
        wcscpy(wstr1, L"42");
        tval = -1;
        rc = swscanf_s(wstr1, L"%td", &tval);
#ifdef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (tval != 42) {
            debug_printf("%s %u wrong ptrdiff_t arg: %td\n", __FUNCTION__,
                         __LINE__, (ptrdiff_t)tval);
            errs++;
        }
#endif
#endif
    }

    /*--------------------------------------------------*/

    /* TODO
    rc = swscanf_s(wstr1, L"%s", NULL);
    ERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"aaaaaaaaaa");
    len1 = wcslen(wstr1);

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR(1)
    len2 = wcslen(wstr2);
    len3 = wcslen(wstr1);
    if (len3 != len2) {
#ifdef DEBUG
        len1 = wcslen(wstr1);
#endif
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
        errs++;
    }

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it simple");

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR(1);
    WEXPSTR(wstr1, L"keep it simple")

    /*--------------------------------------------------*/

    wstr1[0] = '\0';
    wstr2[0] = '\0';

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR_MSVC(-1, have_wine ? -1 : 0);
    WEXPNULL(wstr1);

    /*--------------------------------------------------*/

    wstr1[0] = '\0';
    wcscpy(wstr2, L"keep it simple");

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR_MSVC(-1, have_wine ? -1 : 0);
    WEXPSTR(wstr1, L"");

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"qqweqq");
    wcscpy(wstr2, L"keep it simple");

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    NOERR()
    WEXPSTR(wstr1, wstr2);

    /*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    wcscpy(wstr1, L"12345678901234567890");

    rc = swscanf_s(wstr1, L"%ls", &wstr1[7]);
    ERR(1);
    WEXPSTR(wstr1, L"123456712345678901234567890");

    wcscpy(wstr1, L"123456789");

    rc = swscanf_s(wstr1, L"%ls", &wstr1[8]);
    ERR(1);
    WEXPSTR(wstr1, L"12345678123456789");
    */

    /*--------------------------------------------------*/

#ifndef HAVE_SWSCANF_S
    errs += test_wscan_core();
#endif

    return (errs);
}
