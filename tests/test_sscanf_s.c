/*------------------------------------------------------------------
 * test_sscanf_s
 * File 'io/sscanf_s.c'
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

#ifdef HAVE_SSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
static char str3[LEN];
int test_sscanf_s(void);

/*--------------------------------------------------*/
/* scanf core: conversions, modifiers, limits (the same engine drives
   fscanf_s, vsscanf_s, ...) */
#ifndef HAVE_SSCANF_S
#define CKRC(exp)                                                              \
    if (rc != (exp)) {                                                         \
        debug_printf("%s %u  Error rc=%d, expected %d\n", __FUNCTION__,        \
                     __LINE__, (int)rc, (int)(exp));                           \
        errs++;                                                                \
    }
#define CKINT(v, exp)                                                          \
    if ((long long)(v) != (long long)(exp)) {                                  \
        debug_printf("%s %u  Error value=%lld, expected %lld\n",               \
                     __FUNCTION__, __LINE__, (long long)(v),                   \
                     (long long)(exp));                                        \
        errs++;                                                                \
    }
#define CKERRNO(exp)                                                           \
    if (errno != (exp)) {                                                      \
        debug_printf("%s %u  Error errno=%d, expected %d\n", __FUNCTION__,     \
                     __LINE__, errno, (int)(exp));                             \
        errs++;                                                                \
    }
static volatile size_t vsz;
static int test_scan_core(void);
static int test_scan_core(void) {
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

    GCC_DIAG_IGNORE(-Wformat)
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    GCC_DIAG_IGNORE(-Wformat-security)

    /* integers, bases, signs */
    rc = sscanf_s("-17", "%d", &a);
    CKRC(1) CKINT(a, -17)
    rc = sscanf_s("+5", "%d", &a);
    CKRC(1) CKINT(a, 5)
    rc = sscanf_s("42", "%u", &ua);
    CKRC(1) CKINT(ua, 42)
    rc = sscanf_s("017", "%o", &ua);
    CKRC(1) CKINT(ua, 15)
    rc = sscanf_s("ff", "%x", &ua);
    CKRC(1) CKINT(ua, 255)
    rc = sscanf_s("0xFF", "%x", &ua);
    CKRC(1) CKINT(ua, 255)
    rc = sscanf_s("1aB", "%X", &ua);
    CKRC(1) CKINT(ua, 0x1ab)
    rc = sscanf_s("0x1F", "%i", &a);
    CKRC(1) CKINT(a, 31)
    rc = sscanf_s("017", "%i", &a);
    CKRC(1) CKINT(a, 15)
    rc = sscanf_s("-0x10", "%i", &a);
    CKRC(1) CKINT(a, -16)
    rc = sscanf_s("0", "%i", &a);
    CKRC(1) CKINT(a, 0)
    rc = sscanf_s("0x", "%i", &a); /* prefix without digits */
    CKRC(0)
    rc = sscanf_s("0xg", "%x", &a);
    CKRC(0)
    rc = sscanf_s("089", "%i", &a); /* octal stops at 8 */
    CKRC(1) CKINT(a, 0)
    rc = sscanf_s("-", "%d", &a);
    CKRC(0)
    rc = sscanf_s("abc", "%d", &a);
    CKRC(0)
    rc = sscanf_s("12abc", "%d", &a);
    CKRC(1) CKINT(a, 12)
    rc = sscanf_s("1z", "%2o", &a);
    CKRC(1) CKINT(a, 1)

    rc = sscanf_s("25", "%i", &a); /* decimal via base 0 */
    CKRC(1) CKINT(a, 25)
    rc = sscanf_s("1 \t 2", "%d  %d", &a, &b); /* whitespace run in fmt */
    CKRC(2) CKINT(b, 2)
    a = 77;
    rc = sscanf_s("12", "%Ld", &a); /* size not valid for integers */
    CKRC(1)

    /* size modifiers */
    sc = 0;
    rc = sscanf_s("300", "%hhd", &sc);
    CKRC(1) CKINT(sc, (signed char)300)
    sh = 0;
    rc = sscanf_s("70000", "%hd", &sh);
    CKRC(1) CKINT(sh, (short)70000)
    lv = 0;
    rc = sscanf_s("-123456789012", "%ld", &lv);
    CKRC(1) CKINT(lv, -123456789012L)
    ulv = 0;
    rc = sscanf_s("123456789012", "%lu", &ulv);
    CKRC(1) CKINT(ulv, 123456789012UL)
    zv = 0;
    rc = sscanf_s("123", "%zu", &zv);
    CKRC(1) CKINT(zv, 123)
    pv = NULL;
    rc = sscanf_s("0x1234", "%p", &pv);
    CKRC(1) CKINT((uintptr_t)pv, 0x1234)
    rc = sscanf_s("1234", "%p", &pv);
    CKRC(1) CKINT((uintptr_t)pv, 0x1234)
    rc = sscanf_s("12", "%*p");
    CKRC(0)
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
    {
        long long ll = 0;
        unsigned long long ull = 0;
        rc = sscanf_s("-9223372036854775807", "%lld", &ll);
        CKRC(1) CKINT(ll, -9223372036854775807LL)
        rc = sscanf_s("123456789012", "%jd", &ll);
        CKRC(1) CKINT(ll, 123456789012LL)
        rc = sscanf_s("18446744073709551615", "%llu", &ull);
        CKRC(1)
        if (ull != 18446744073709551615ULL) {
            debug_printf("%s %u wrong ull\n", __FUNCTION__, __LINE__);
            errs++;
        }
        /* overflow clamps to the type limit */
        ull = 0;
        rc = sscanf_s("99999999999999999999999", "%llu", &ull);
        CKRC(1)
        if (ull != 18446744073709551615ULL) {
            debug_printf("%s %u wrong clamp\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = sscanf_s("-99999999999999999999999", "%lld", &ll);
        CKRC(1)
        rc = sscanf_s("0xFFFFFFFFFFFFFFFFFFFF", "%llx", &ull);
        CKRC(1)
        rc = sscanf_s("0777777777777777777777777", "%lli", &ll);
        CKRC(1)
        rc = sscanf_s("11111111111111111111", "%2$lld %1$lld", &ll, &ll);
        CKRC(1)
    }
#endif
#ifdef HAVE_STDDEF_H
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
    {
        ptrdiff_t td = 0;
        rc = sscanf_s("-7", "%td", &td);
        CKRC(1) CKINT(td, -7)
    }
#endif
#endif

    /* width limits, multiple conversions, suppression */
    rc = sscanf_s("12345", "%2d%3d", &a, &b);
    CKRC(2) CKINT(a, 12) CKINT(b, 345)
    rc = sscanf_s("12 34", "%d %d", &a, &b);
    CKRC(2) CKINT(a, 12) CKINT(b, 34)
    rc = sscanf_s("12", "%d %d", &a, &b); /* partial match */
    CKRC(1) CKINT(a, 12)
    rc = sscanf_s("12 x", "%d %d", &a, &b);
    CKRC(1)
    rc = sscanf_s("1 2", "%*d %d", &a);
    CKRC(1) CKINT(a, 2)
    rc = sscanf_s("7", "%*d");
    CKRC(0)
    rc = sscanf_s("", "%d", &a);
    CKRC(-1)
    rc = sscanf_s("   ", "%d", &a);
    CKRC(-1)
    rc = sscanf_s("", "x");
    CKRC(-1)
    rc = sscanf_s("x", "x");
    CKRC(0)
    rc = sscanf_s("y1", "x%d", &a);
    CKRC(0)
    rc = sscanf_s("x1", "x%d", &a);
    CKRC(1) CKINT(a, 1)
    rc = sscanf_s("5%", "%d%%", &a);
    CKRC(1) CKINT(a, 5)
    rc = sscanf_s("  %7", "%%%d", &a);
    CKRC(1) CKINT(a, 7)
    rc = sscanf_s("%", "%%");
    CKRC(0)
    rc = sscanf_s("a", "%%");
    CKRC(0)
    rc = sscanf_s("", "%%");
    CKRC(-1)
    rc = sscanf_s("1,2", "%d,%d", &a, &b);
    CKRC(2) CKINT(b, 2)
    rc = sscanf_s("1;2", "%d,%d", &a, &b);
    CKRC(1)
    rc = sscanf_s("5 6", "%2$d %1$d", &a, &b);
    CKRC(2) CKINT(b, 5) CKINT(a, 6)

    /* illegal %n behind a width or modifier is rejected by the core */
    errno = 0;
    rc = sscanf_s("5", "%5n", &a);
    CKRC(EOF) CKERRNO(EINVAL)
    errno = 0;
    rc = sscanf_s("5", "%hn", &a);
    CKRC(EOF) CKERRNO(EINVAL)
    errno = 0;
    rc = sscanf_s("5", "%d%5n", &a, &b);
    CKRC(EOF) CKERRNO(EINVAL)

    /* unknown conversion */
    rc = sscanf_s("5", "%y", &a);
    CKRC(-1)
    rc = sscanf_s("5", "%d%y", &a, &b);
    CKRC(1)

    /* %c */
    buf[0] = buf[1] = buf[2] = 'z';
    rc = sscanf_s("abc", "%c", buf, (rsize_t)1);
    CKRC(1) CKINT(buf[0], 'a') CKINT(buf[1], 'z')
    rc = sscanf_s("abc", "%3c", buf, (rsize_t)3);
    CKRC(1) CKINT(buf[2], 'c')
    rc = sscanf_s(" b", "%c", buf, (rsize_t)1); /* %c keeps whitespace */
    CKRC(1) CKINT(buf[0], ' ')
    rc = sscanf_s("a", "%2c", buf, (rsize_t)2); /* input too short */
    CKRC(0)
    rc = sscanf_s("", "%c", buf, (rsize_t)1);
    CKRC(-1)
    rc = sscanf_s("abc", "%*c%c", buf, (rsize_t)1);
    CKRC(1) CKINT(buf[0], 'b')
    rc = sscanf_s("abc", "%*3c");
    CKRC(0)
    rc = sscanf_s("abc", "%0c", buf, (rsize_t)1); /* width 0 => 1 */
    CKRC(1)
    errno = 0;
    rc = sscanf_s("abc", "%c", buf, (rsize_t)0);
    CKRC(EOF) CKERRNO(ESZEROL)
    errno = 0;
    rc = sscanf_s("abc", "%s", buf, (rsize_t)0);
    CKRC(EOF) CKERRNO(ESZEROL)
    errno = 0;
    rc = sscanf_s("abc", "%s", buf, (rsize_t)(RSIZE_MAX_STR + 1));
    CKRC(EOF) CKERRNO(ESZEROL)
    errno = 0;
    rc = sscanf_s("abc", "%3c", buf, (rsize_t)2); /* too small */
    CKRC(EOF) CKERRNO(ESNOSPC)

    /* %s */
    rc = sscanf_s("  hello world", "%s", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "hello")
    rc = sscanf_s("hello world", "%3s", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "hel")
    rc = sscanf_s("one two", "%s %s", buf, (rsize_t)sizeof(buf), buf2,
                  (rsize_t)sizeof(buf2));
    CKRC(2) EXPSTR(buf2, "two")
    rc = sscanf_s("one two", "%*s %s", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "two")
    rc = sscanf_s("one", "%*s");
    CKRC(0)
    errno = 0;
    rc = sscanf_s("abcdef", "%s", buf, (rsize_t)4); /* needs 7 */
    CKRC(EOF) CKERRNO(ESNOSPC)
    rc = sscanf_s("abc", "%s", buf, (rsize_t)4);
    CKRC(1) EXPSTR(buf, "abc")
    /* unknown dest size (not statically known): dmax from volatile */
    {
        char *heap = (char *)malloc(16);
        vsz = 16;
        rc = sscanf_s("heap", "%s", heap, vsz);
        CKRC(1) EXPSTR(heap, "heap")
        free(heap);
    }

    /* %[ scansets */
    rc = sscanf_s("abcd", "%[a-c]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "abc")
    rc = sscanf_s("abxd", "%[^x]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "ab")
    rc = sscanf_s("]a]b", "%[]a]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "]a]")
    rc = sscanf_s("b]a", "%[^]a]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "b")
    rc = sscanf_s("-a-b", "%[-a]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "-a-")
    rc = sscanf_s("a-b", "%[^-]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "a")
    rc = sscanf_s("ab-c", "%[ab-]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "ab-")
    rc = sscanf_s("0123xyz", "%[0-9x-z]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "0123xyz")
    rc = sscanf_s("  abc", "%[a-c]", buf, (rsize_t)sizeof(buf)); /* no skip */
    CKRC(0)
    rc = sscanf_s("xyz", "%[a-c]", buf, (rsize_t)sizeof(buf));
    CKRC(0)
    rc = sscanf_s("abcdef", "%3[a-z]", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "abc")
    rc = sscanf_s("abcdef", "%*[a-c]%s", buf, (rsize_t)sizeof(buf));
    CKRC(1) EXPSTR(buf, "def")
    rc = sscanf_s("abcdef", "%*[a-z]");
    CKRC(0)
    rc = sscanf_s("abc", "%[abc", buf, (rsize_t)sizeof(buf)); /* unterminated */
    CKRC(-1)
    errno = 0;
    rc = sscanf_s("abcdef", "%[a-f]", buf, (rsize_t)3);
    CKRC(EOF) CKERRNO(ESNOSPC)
    rc = sscanf_s("", "%[a-f]", buf, (rsize_t)3);
    CKRC(-1)

    /* %m allocating conversions */
    {
        char *p1 = NULL;
        char *p2 = NULL;
        const char *longs = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuv"
                            "wxyzabcdefghijklmnopqrstuvwxyz";
        rc = sscanf_s("hello world", "%ms", &p1);
        CKRC(1)
        if (p1) {
            EXPSTR(p1, "hello")
            free(p1);
            p1 = NULL;
        }
        rc = sscanf_s(longs, "%ms", &p1); /* forces realloc growth */
        CKRC(1)
        if (p1) {
            EXPSTR(p1, longs)
            free(p1);
            p1 = NULL;
        }
        rc = sscanf_s("abc1", "%m[a-c]", &p1);
        CKRC(1)
        if (p1) {
            EXPSTR(p1, "abc")
            free(p1);
            p1 = NULL;
        }
        rc = sscanf_s("abcdef", "%3mc", &p2);
        CKRC(1)
        if (p2) {
            CKINT(p2[2], 'c')
            free(p2);
            p2 = NULL;
        }
        rc = sscanf_s("xyz", "%m[a-c]", &p1); /* nothing matches */
        CKRC(0)
        rc = sscanf_s("one two", "%*ms %ms", &p1);
        CKRC(1)
        if (p1) {
            EXPSTR(p1, "two")
            free(p1);
            p1 = NULL;
        }
    }

#ifndef SAFECLIB_DISABLE_WCHAR
    /* wide destinations from the byte scanner */
    {
        wchar_t wb[32];
        wchar_t *wp = NULL;
        rc = sscanf_s("wide str", "%ls", wb, (rsize_t)32);
        CKRC(1) CKINT(wb[0], 'w') CKINT(wb[3], 'e') CKINT(wb[4], 0)
        rc = sscanf_s("wide str", "%S", wb, (rsize_t)32);
        CKRC(1) CKINT(wb[0], 'w')
        rc = sscanf_s("abcdef", "%3lc", wb, (rsize_t)3);
        CKRC(1) CKINT(wb[2], 'c')
        rc = sscanf_s("abcdef", "%C", wb, (rsize_t)1);
        CKRC(1) CKINT(wb[0], 'a')
        rc = sscanf_s("abc1", "%l[a-c]", wb, (rsize_t)32);
        CKRC(1) CKINT(wb[2], 'c') CKINT(wb[3], 0)
        errno = 0;
        rc = sscanf_s("abcdef", "%ls", wb, (rsize_t)3);
        CKRC(EOF) CKERRNO(ESNOSPC)
        rc = sscanf_s("*", "%*ls");
        CKRC(0)
        rc = sscanf_s("wide str", "%mls", &wp);
        CKRC(1)
        if (wp) {
            CKINT(wp[0], 'w') CKINT(wp[3], 'e') CKINT(wp[4], 0)
            free(wp);
            wp = NULL;
        }
        rc = sscanf_s("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",
                      "%mls", &wp); /* growth */
        CKRC(1)
        if (wp) {
            CKINT(wp[40], 'o')
            free(wp);
            wp = NULL;
        }
        rc = sscanf_s("abc1", "%m[a-c]", &wp);
        CKRC(1)
        free(wp);
        wp = NULL;
        {
            /* UTF-8: multibyte chars, then a truncated sequence */
            SETLOCALE_UTF8;
            if (lc_cat && strstr(lc_cat, "UTF")) {
                rc = sscanf_s("\xc3\xa4x", "%ls", wb, (rsize_t)32);
                CKRC(1) CKINT(wb[0], 0xe4) CKINT(wb[1], 'x')
                rc = sscanf_s("a\xc3", "%ls", wb, (rsize_t)32); /* -2 */
                CKRC(-1)
                rc = sscanf_s("\xffz", "%ls", wb, (rsize_t)32); /* EILSEQ */
                CKRC(-1)
            }
            SETLOCALE_C;
        }
        {
            /* a high byte is an illegal sequence in a UTF-8 or ASCII
               locale: input failure */
            const char bad[] = "\xff\xfe";
            errno = 0;
            rc = sscanf_s(bad, "%ls", wb, (rsize_t)32);
            if (rc != -1 && rc != 1 && rc != 0) {
                debug_printf("%s %u  Error rc=%d\n", __FUNCTION__, __LINE__,
                             rc);
                errs++;
            }
        }
    }
#endif

    /* floating point */
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
    {
        float f = 0;
        double d = 0;
        long double ld = 0;
        char big[1600];
        int n;

        rc = sscanf_s("3.25", "%f", &f);
        CKRC(1) CKINT(f * 100, 325)
        rc = sscanf_s("-.5", "%f", &f);
        CKRC(1) CKINT(f * 10, -5)
        rc = sscanf_s("5.", "%lf", &d);
        CKRC(1) CKINT(d, 5)
        rc = sscanf_s("+7", "%lf", &d);
        CKRC(1) CKINT(d, 7)
        rc = sscanf_s("123.456", "%5lf", &d);
        CKRC(1) CKINT(d * 10, 1234)
        rc = sscanf_s("1.5 2.5", "%f %lf", &f, &d);
        CKRC(2) CKINT(d * 10, 25)
        rc = sscanf_s("abc", "%f", &f);
        CKRC(0)
        rc = sscanf_s("", "%f", &f);
        CKRC(-1)
        rc = sscanf_s("2.5", "%*f");
        CKRC(0)
        rc = sscanf_s("2.5", "%Lf", &ld);
#ifndef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
        CKRC(1) CKINT(ld * 10, 25)
#endif
        /* size not valid for floats */
        rc = sscanf_s("2.5", "%hf", &f);
        CKRC(0)
        rc = sscanf_s("2.5", "%hhf", &f);
        CKRC(0)
        rc = sscanf_s("2.5", "%jf", &f);
        CKRC(0)

        /* inf and nan, any case */
        rc = sscanf_s("inf", "%f", &f);
        CKRC(1)
        if (!(f > 3e38f)) {
            debug_printf("%s %u  not inf\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = sscanf_s("-INF", "%f", &f);
        CKRC(1)
        if (!(f < -3e38f)) {
            debug_printf("%s %u  not -inf\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = sscanf_s("+Infinity", "%lf", &d);
        CKRC(1)
        if (!(d > 1e308)) {
            debug_printf("%s %u  not inf\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = sscanf_s("infinit", "%lf", &d); /* partial infinity */
        CKRC(0)
        rc = sscanf_s("infx", "%f", &f);
        CKRC(1)
        rc = sscanf_s("in", "%f", &f);
        CKRC(0)
        rc = sscanf_s("nan", "%lf", &d);
        CKRC(1)
        if (d == d) {
            debug_printf("%s %u  not nan\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = sscanf_s("-NaN", "%lf", &d);
        CKRC(1)
        rc = sscanf_s("nan(0x1f_a)", "%f", &f);
        CKRC(1)
        if (f == f) {
            debug_printf("%s %u  not nan\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = sscanf_s("nan(1", "%f", &f); /* unterminated payload */
        CKRC(0)
        rc = sscanf_s("nan()", "%f", &f);
        CKRC(1)
        rc = sscanf_s("nan(a-b)", "%f", &f); /* illegal payload char */
        CKRC(0)
        rc = sscanf_s("na", "%f", &f);
        CKRC(0)

#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
        rc = sscanf_s("1e3", "%e", &f);
        CKRC(1) CKINT(f, 1000)
        rc = sscanf_s("1E-2", "%lg", &d);
        CKRC(1) CKINT(d * 100, 1)
        rc = sscanf_s("-.5e1", "%lE", &d);
        CKRC(1) CKINT(d, -5)
        rc = sscanf_s("2.5e+1x", "%lG", &d);
        CKRC(1) CKINT(d, 25)
        rc = sscanf_s("1e", "%lf", &d); /* incomplete exponent */
        CKRC(1) CKINT(d, 1)
        rc = sscanf_s("1e+", "%lf", &d);
        CKRC(1) CKINT(d, 1)
        rc = sscanf_s("1.2.3 ", "%lf", &d); /* second radix point ends it */
        CKRC(1) CKINT(d * 10, 12)
        rc = sscanf_s("1e-5x", "%lf", &d);
        CKRC(1) CKINT(d * 1e5, 1)
        /* invalid exponent: the e/p and what follows are pushed back exactly
   once, like musl (glibc rejects these inputs, so there is no oracle) */
        {
            char c1 = '?', c2 = '?';
            float g = -7;
            rc = sscanf_s("1ex", "%f%c%c", &g, &c1, 1, &c2, 1);
            CKRC(3) CKINT(g, 1) CKINT(c1, 'e') CKINT(c2, 'x')
            rc = sscanf_s("1e-x", "%f%c%c", &g, &c1, 1, &c2, 1);
            CKRC(3) CKINT(g, 1) CKINT(c1, 'e') CKINT(c2, '-')
            rc = sscanf_s("0x1pz", "%f%c%c", &g, &c1, 1, &c2, 1);
            CKRC(3) CKINT(g, 1) CKINT(c1, 'p') CKINT(c2, 'z')
            rc = sscanf_s("0x1p-z", "%f%c%c", &g, &c1, 1, &c2, 1);
            CKRC(3) CKINT(g, 1) CKINT(c1, 'p') CKINT(c2, '-')
            rc = sscanf_s("1e5x", "%f%c", &g, &c1, 1);
            CKRC(2) CKINT(g, 100000) CKINT(c1, 'x')
            rc = sscanf_s("1ex", "%f", &g);
            CKRC(1) CKINT(g, 1)
        }
        /* no digits at all: matching failure, nothing stored (glibc oracle) */
        {
            float g = -7;
            rc = sscanf_s("-", "%f", &g);
            CKRC(0) CKINT(g, -7)
            rc = sscanf_s(".", "%f", &g);
            CKRC(0) CKINT(g, -7)
            rc = sscanf_s("0x", "%f", &g);
            CKRC(0) CKINT(g, -7)
            rc = sscanf_s("0xg", "%f", &g);
            CKRC(0) CKINT(g, -7)
            rc = sscanf_s("+", "%f", &g);
            CKRC(0) CKINT(g, -7)
            rc = sscanf_s("-.x", "%f", &g);
            CKRC(0) CKINT(g, -7)
            rc = sscanf_s("0xp", "%f", &g);
            CKRC(0) CKINT(g, -7)
        }
        /* dropped digits keep the magnitude: 1 followed by 700 zeros, e-690 */
        {
            char big[800];
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
            rc = sscanf_s(big, "%lf", &dv);
            CKRC(1) CKINT(dv, 10000000000)
        }
        rc = sscanf_s("1e999", "%lf", &d); /* overflow */
        CKRC(1)
        if (!(d > 1e308)) {
            debug_printf("%s %u  no overflow to inf\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = sscanf_s("-1e999", "%lf", &d);
        CKRC(1)
        if (!(d < -1e308)) {
            debug_printf("%s %u  no overflow to -inf\n", __FUNCTION__,
                         __LINE__);
            errs++;
        }
        rc = sscanf_s("1e50", "%f", &f); /* float overflow */
        CKRC(1)
        if (!(f > 3e38f)) {
            debug_printf("%s %u  no float overflow\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = sscanf_s("1e-999", "%lf", &d); /* underflow */
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("1e-50", "%f", &f);
        CKRC(1) CKINT(f == 0, 1)
        rc = sscanf_s("4.9e-324", "%lf", &d); /* denormal min */
        CKRC(1) CKINT(d > 0, 1)
        rc = sscanf_s("2.2250738585072011e-308", "%lf", &d);
        CKRC(1) CKINT(d > 0, 1)
        rc = sscanf_s("1.7976931348623157e308", "%lf", &d);
        CKRC(1) CKINT(d > 1e308, 1)
        rc = sscanf_s("1e99999999999999999999", "%lf", &d); /* huge exponent */
        CKRC(1)
        rc = sscanf_s("1e-99999999999999999999", "%lf", &d);
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("0e99999999999999999999", "%lf", &d);
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("0.0000000", "%lf", &d);
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("-0.0", "%lf", &d);
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("0.00000000000000000000000000001e30", "%lf", &d);
        CKRC(1) CKINT(d > 0, 1)
        rc = sscanf_s("123456789012345678901234567890.5", "%lf", &d);
        CKRC(1) CKINT(d > 1e29, 1)
        rc = sscanf_s("1.00000000000000000000000000000000000001", "%lf", &d);
        CKRC(1) CKINT(d == 1, 1)
        rc = sscanf_s("9007199254740993", "%lf", &d); /* round to even */
        CKRC(1)
        rc = sscanf_s("1.5e1", "%Lf", &ld);
#ifndef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
        CKRC(1) CKINT(ld, 15)
        rc = sscanf_s("1e4000", "%Lf", &ld);
        CKRC(1)
        rc = sscanf_s("1e-5000", "%Lf", &ld);
        CKRC(1) CKINT(ld == 0, 1)
        rc = sscanf_s("3.14159265358979323846264338327950288", "%Lf", &ld);
        CKRC(1) CKINT(ld > 3, 1)
        rc = sscanf_s("1e-4945", "%Lf", &ld); /* long double denormal */
        CKRC(1)
        rc = sscanf_s("3.6e-4951", "%Lf", &ld);
        CKRC(1)
        rc = sscanf_s("1.18973149535723176502e+4932", "%Lf", &ld);
        CKRC(1)
#endif

        /* very long mantissas */
        memset(big, '1', 1500);
        big[1500] = '\0';
        rc = sscanf_s(big, "%lf", &d);
        CKRC(1) CKINT(d > 1e308, 1)
        big[0] = '0';
        big[1] = '.';
        rc = sscanf_s(big, "%lf", &d);
        CKRC(1) CKINT(d > 0.1, 1)
        strcpy(big + 600, "e-400"); /* 0.111... e-400 */
        rc = sscanf_s(big, "%lf", &d);
        CKRC(1)
        memset(big, '0', 1500);
        big[1500] = '\0';
        big[1499] = '5';
        big[1] = '.';
        rc = sscanf_s(big, "%lf", &d); /* tiny, long leading zeros */
        CKRC(1) CKINT(d == 0, 1)
        memset(big, '9', 1500);
        big[1500] = '\0';
        strcpy(big + 400, ".9999e-399");
        rc = sscanf_s(big, "%lf", &d);
        CKRC(1)
        strcpy(big, "1");
        for (n = 0; n < 700; n++)
            strcat(big, "0");
        strcat(big, "e-690");
        rc = sscanf_s(big, "%lf", &d);
        CKRC(1) CKINT(d > 1e9, 1)
        /* many digits with many exponents */
        strcpy(big, "0.");
        for (n = 0; n < 40; n++)
            strcat(big, "1234567890");
        strcat(big, "e5");
        rc = sscanf_s(big, "%lf", &d);
        CKRC(1)
        rc = sscanf_s(big, "%f", &f);
        CKRC(1)
        /* denormal float and rounding of exactly half ulp */
        rc = sscanf_s("1.401298464324817e-45", "%f", &f);
        CKRC(1)
        rc = sscanf_s("7e-46", "%f", &f);
        CKRC(1)
        rc = sscanf_s("1.17549435e-38", "%f", &f);
        CKRC(1)
        rc = sscanf_s("16777217", "%f", &f);
        CKRC(1)
        rc = sscanf_s("3.4028236e38", "%f", &f);
        CKRC(1)
        rc = sscanf_s("3.40282347e38", "%f", &f);
        CKRC(1)
        rc = sscanf_s("0.1", "%f", &f);
        CKRC(1)
#endif /* !EXPONENTIAL */

        /* hex floats */
        rc = sscanf_s("0x1.8.8 ", "%la", &d);
        CKRC(1) CKINT(d * 2, 3)
        rc = sscanf_s("0x10", "%lf", &d);
        CKRC(1) CKINT(d, 16)
        rc = sscanf_s("0x1.8p1", "%lf", &d);
        CKRC(1) CKINT(d, 3)
        rc = sscanf_s("0X.8P1", "%a", &f);
        CKRC(1) CKINT(f, 1)
        rc = sscanf_s("-0x1p-2", "%lA", &d);
        CKRC(1) CKINT(d * 4, -1)
        rc = sscanf_s("0x1P+2", "%la", &d);
        CKRC(1) CKINT(d, 4)
        rc = sscanf_s("0x1p", "%la", &d); /* incomplete exponent */
        CKRC(1) CKINT(d, 1)
        rc = sscanf_s("0x1p-", "%la", &d);
        CKRC(1) CKINT(d, 1)
        rc = sscanf_s("0x1pz", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0x0p0", "%la", &d);
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("0x0.0000p5", "%la", &d);
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("0x1.fffffffffffffp1023", "%la", &d); /* DBL_MAX */
        CKRC(1) CKINT(d > 1e308, 1)
        rc = sscanf_s("0x1p1024", "%la", &d); /* overflow */
        CKRC(1) CKINT(d > 1e308, 1)
        rc = sscanf_s("-0x1p1024", "%la", &d);
        CKRC(1) CKINT(d < -1e308, 1)
        rc = sscanf_s("0x1p-1074", "%la", &d); /* min denormal */
        CKRC(1) CKINT(d > 0, 1)
        rc = sscanf_s("0x1p-1075", "%la", &d); /* half: round to even */
        CKRC(1)
        rc = sscanf_s("0x1.8p-1075", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0x1p-1100", "%la", &d); /* underflow */
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("0x1p-99999999999999999999", "%la", &d);
        CKRC(1) CKINT(d == 0, 1)
        rc = sscanf_s("0x1p99999999999999999999", "%la", &d);
        CKRC(1) CKINT(d > 1e308, 1)
        rc = sscanf_s("0x1p-149", "%a", &f); /* float min denormal */
        CKRC(1) CKINT(f > 0, 1)
        rc = sscanf_s("0x1p128", "%a", &f); /* float overflow */
        CKRC(1) CKINT(f > 3e38f, 1)
        rc = sscanf_s("0x1.fffffep127", "%a", &f);
        CKRC(1)
        rc = sscanf_s("0x1.ffffffp127", "%a", &f); /* rounds up to inf */
        CKRC(1)
        rc = sscanf_s("0x1.0000000000000800000p0", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0x1.00000000000008p0", "%la", &d); /* half ulp */
        CKRC(1)
        rc = sscanf_s("0x1.00000000000018p0", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0x1.00000000000008000001p0", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0x1.00000000000007fffffp0", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0x3.8p0", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0xf.fp0", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0x1.2345678901234567890123456789p0", "%la", &d);
        CKRC(1) CKINT(d > 1, 1)
        rc = sscanf_s("0x0.0000000000000000000000000000000001p0", "%la", &d);
        CKRC(1)
        rc = sscanf_s("0x1.8p0", "%Lf", &ld);
#ifndef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
        CKRC(1) CKINT(ld * 2, 3)
        rc = sscanf_s("0x1p16384", "%La", &ld);
        CKRC(1)
        rc = sscanf_s("0x1p-16445", "%La", &ld);
        CKRC(1)
        rc = sscanf_s("0x1p-16446", "%La", &ld);
        CKRC(1)
        rc = sscanf_s("0x1.ffffffffffffffffp16383", "%La", &ld);
        CKRC(1)
        rc = sscanf_s("0x1.fffffffffffffffffp16383", "%La", &ld);
        CKRC(1)
        rc = sscanf_s("0x8p-3", "%La", &ld);
        CKRC(1)
#endif
        rc = sscanf_s("0x12345678", "%5lf", &d); /* width stops inside */
        CKRC(1)
        rc = sscanf_s("1e5", "%2lf", &d); /* width stops after e */
        CKRC(1) CKINT(d, 1)
    }
#endif /* !FLOAT */

    GCC_DIAG_RESTORE
    GCC_DIAG_RESTORE
    GCC_DIAG_RESTORE
    return errs;
}
#endif /* !HAVE_SSCANF_S */


int test_sscanf_s(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int num = 0;
    int errs = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    EXPECT_BOS("empty fmt")
    rc = sscanf_s(str1, NULL, NULL);
    GCC_DIAG_RESTORE
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    str2[0] = '\0';
    EXPECT_BOS("empty buffer")
    rc = sscanf_s(NULL, "%s", str2);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "      24");
    GCC_DIAG_IGNORE(-Wformat)
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s %n", str2, LEN, &ind);
    GCC_DIAG_RESTORE
    ERREOF(EINVAL);

    rc = sscanf_s(str1, "%%n");
    ERR(0);
    ERRNO(0);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s %%n", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1);
    ERRNO(0);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s %%n", str2, 6);
    GCC_DIAG_RESTORE
    ERR(1);
    ERRNO(0);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s %%n", str3, 6);
    GCC_DIAG_RESTORE
    ERR(1);
    ERRNO(0);
    EXPSTR(str3, "24");

    rc = sscanf_s(str1, "%%n");
    ERR(0);
    ERRNO(0);

    rc = sscanf_s(str1, " %d", &num);
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
        strcpy(str1, "3.5");
        fval = -1;
        rc = sscanf_s(str1, "%f", &fval);
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
        strcpy(str1, "-2.25");
        dval = -1;
        rc = sscanf_s(str1, "%lf", &dval);
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
        strcpy(str1, "3.5e2");
        fval = -1;
        rc = sscanf_s(str1, "%e", &fval);
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
        strcpy(str1, "3.5e2");
        fval = -1;
        rc = sscanf_s(str1, "%g", &fval);
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
        strcpy(str1, "0x1.cp+1");
        fval = -1;
        rc = sscanf_s(str1, "%a", &fval);
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
        strcpy(str1, "1.125");
        ldval = -1;
        rc = sscanf_s(str1, "%Lf", &ldval);
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
        strcpy(str1, "123456789012");
        llval = -1;
        rc = sscanf_s(str1, "%lld", &llval);
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
        strcpy(str1, "42");
        tval = -1;
        rc = sscanf_s(str1, "%td", &tval);
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
    rc = sscanf_s(str1, "%s", NULL);
    ERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    len1 = strlen(str1);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1)
    len2 = strlen(str2);
    len3 = strlen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        len1 = strlen(str1);
#endif
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1);
    EXPSTR(str1, "keep it simple")

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(-1)
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(-1)
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    NOERR()
    EXPSTR(str1, str2);

    /*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    strcpy(str1, "12345678901234567890");

    rc = sscanf_s(str1, "%s", &str1[7]);
    ERR(1);
    EXPSTR(str1, "123456712345678901234567890");

    strcpy(str1, "123456789");

    rc = sscanf_s(str1, "%s", &str1[8]);
    ERR(1);
    EXPSTR(str1, "12345678123456789");
    */

    /*--------------------------------------------------*/

#ifndef HAVE_SSCANF_S
    errs += test_scan_core();
#endif

    return (errs);
}

int main(void) { return (test_sscanf_s()); }
