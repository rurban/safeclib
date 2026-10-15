/*------------------------------------------------------------------
 * test_vsnprintf_s
 * File 'wchar/vsnprintf_s.c'
 * Lines executed:97.03% of 607
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <wchar.h>
#include <float.h>
#include <math.h>

// #ifdef HAVE_VSNPRINTF_S
// #define HAVE_NATIVE 1
// #else
// #define HAVE_NATIVE 0
// #endif
#include "test_msvcrt.h"

#if defined(_WIN32) && defined(HAVE_VSNPRINTF_S)
#define USE_MSVCRT
int vsnprintf_s(char *dest, size_t dmax, size_t maxcount, const char *fmt,
                va_list arglist);
#endif

#define LEN (128)

// the same probing as in vsnprintf_s.c
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
#define PRINTF_SUPPORT_FLOAT
#endif
#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_SUPPORT_EXPONENTIAL
#endif
#ifndef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
#ifdef HAVE_LONG_DOUBLE
#define PRINTF_SUPPORT_LONG_DOUBLE
#endif
#endif

static char str1[LEN];
static char str2[LEN];
static inline int vtprintf_s(char *restrict dest, rsize_t dmax,
                             const char *restrict fmt, ...) BOS_CHK(dest)
    BOS_NULL(fmt);
#ifndef HAVE_CT_BOS_OVR
static inline int vtprintf_s_chk(char *restrict dest, rsize_t dmax,
                                 rsize_t destbos, const char *restrict fmt, ...)
    BOS_CHK(dest) BOS_NULL(fmt);
#endif
int test_vsnprintf_s(void);

#ifndef USE_MSVCRT
/* exact return code and result */
#define CHK(erc, estr, dmax, ...)                                              \
    rc = vtprintf_s(str1, dmax, __VA_ARGS__);                                  \
    if (rc != (erc) || strcmp(str1, (estr)) != 0) {                            \
        debug_printf("%s %u  Expected %d \"%s\", got %d \"%s\"\n",            \
                     __FUNCTION__, __LINE__, (int)(erc), (estr), (int)rc,      \
                     str1);                                                    \
        errs++;                                                                \
    }
/* same result as the native snprintf (for the formats delegated to libc) */
#define CHKLIBC(fmt, arg)                                                      \
    rc = vtprintf_s(str1, LEN, fmt, arg);                                      \
    len2 = snprintf(str2, LEN, fmt, arg);                                      \
    if (rc != len2 || strcmp(str1, str2) != 0) {                               \
        debug_printf("%s %u  Expected %d \"%s\", got %d \"%s\"\n",            \
                     __FUNCTION__, __LINE__, (int)len2, str2, (int)rc, str1);  \
        errs++;                                                                \
    }
/* known deviations from C99: only check for a sane, terminated result */
#define CHKSANE(fmt, arg)                                                      \
    rc = vtprintf_s(str1, LEN, fmt, arg);                                      \
    if (rc <= 0 || (size_t)rc != strlen(str1)) {                               \
        debug_printf("%s %u  \"%s\" got %d \"%s\"\n", __FUNCTION__,           \
                     __LINE__, fmt, (int)rc, str1);                            \
        errs++;                                                                \
    }
#endif

static inline int vtprintf_s(char *restrict dest, rsize_t dmax,
                             const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
#ifdef USE_MSVCRT
    rc = vsnprintf_s(dest, dmax, 20, fmt, ap);
#else
    rc = vsnprintf_s(dest, dmax, fmt, ap);
#endif
    va_end(ap);
    return rc;
}

#ifndef HAVE_CT_BOS_OVR
static inline int vtprintf_s_chk(char *restrict dest, rsize_t dmax,
                                 rsize_t destbos, const char *restrict fmt,
                                 ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
#ifdef USE_MSVCRT
    rc = vsnprintf_s(dest, dmax, 20, fmt, ap);
#else
    rc = _vsnprintf_s_chk(dest, dmax, destbos, fmt, ap);
#endif
    va_end(ap);
    return rc;
}
#endif

int test_vsnprintf_s(void) {
    errno_t rc;
    int32_t len2;
    int32_t len3;
    int errs = 0;
#ifndef HAVE_CT_BOS_OVR
    int have_wine = 0;
#endif

    /*--------------------------------------------------*/

    /* not testable
      rc = vtprintf_s(str1, LEN, "%s", NULL);
      NEGERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest or dmax")
    rc = vtprintf_s(str1, 0, "%s", str2);
    init_msvcrt(rc == -ESZEROL, &use_msvcrt);
    NEGERR_MSVC(ESZEROL, EOF);

    /* wine msvcrt doesn't check fmt==NULL */
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) &&                         \
      defined(HAVE_VSNPRINTF_S))
    EXPECT_BOS("empty fmt")
    rc = vtprintf_s(str1, LEN, NULL);
    NEGERR_MSVC(ESNULLP, EOF);
#else
    printf("Using wine\n");
    have_wine = 1;
#endif

    EXPECT_BOS("dest overflow")
    rc = vtprintf_s(str1, (RSIZE_MAX_STR + 1), "%s", str2);
    if (have_wine) {
        ERR(EOF);
    } else {
        NEGERR_MSVC(ESLEMAX, 0);
    }

    if (_BOS_KNOWN(str1)) {
        EXPECT_BOS("dest overflow")
        rc = vtprintf_s_chk(str1, LEN + 1, _BOS_KNOWN(str1), "%s", str2);
        NEGERR_MSVC(EOVERFLOW, 0); /* dmax exceeds dest */
        CHECK_SLACK(str1, LEN);    /* cleared */
    }
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 1, "%s", str2);
    if (!use_msvcrt) {
        ERR(-ESNOSPC);
    } else {
        ERR(-1);
    }
    EXPNULL(str1)

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 2, "%s", str2);
    if (!use_msvcrt) {
        ERR(-ESNOSPC);
    } else {
        ERR(-1);
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    len2 = strlen(str2);

    rc = vtprintf_s(str1, 50, "%s", str2);
    ERR(len2)
    len3 = strlen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        int len1 = strlen(str1);
        debug_printf("%s %u lengths wrong: %d  %" PRId32 "  %" PRId32 " \n",
                     __FUNCTION__, __LINE__, len1, len2, len3);
#endif
        errs++;
    }

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 5, "%s", str2);
#ifndef USE_MSVCRT
    ERR(-ESNOSPC);
#else
    ERR(-1);
#endif
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 2, "%s", str2);
#ifndef USE_MSVCRT
    ERR(-ESNOSPC);
#else
    ERR(-1);
#endif
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 20, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = vtprintf_s(str1, LEN, "%s", str2);
    ERR(0)
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, LEN, "%s", str2);
    NOERR()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, LEN, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 12, "%s", str2);
#ifndef USE_MSVCRT
    ERR(-ESNOSPC);
#else
    ERR(-1);
#endif
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 52, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    strcpy(str1, "12345678901234567890");

    rc = vtprintf_s(str1, 8, "%s", &str1[7]);
#ifndef USE_MSVCRT
    EXPSTR(str1, "")
#else
    ERR(-1);
#endif
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    strcpy(str1, "123456789");

    rc = vtprintf_s(str1, 9, "%s", &str1[8]);
    ERR(1) /* overlapping allowed */
    EXPSTR(str1, "9")

    /*--------------------------------------------------*/

    strcpy(str2, "123");
    strcpy(str1, "keep it simple");

    rc = vtprintf_s(str2, 31, "%s", &str1[0]);
    NOERRNULL()
    EXPSTR(str2, "keep it simple")

    /*--------------------------------------------------*/

    strcpy(str2, "1234");
    strcpy(str1, "56789");

    rc = vtprintf_s(str2, 10, "%s", str1);
    NOERRNULL()
    EXPSTR(str2, "56789")

    /*--------------------------------------------------*/

#ifndef USE_MSVCRT
    /* integer conversions: flags, widths, precisions, bases, length modifiers */
    CHK(2, "12", LEN, "%d", 12)
    CHK(3, " 12", LEN, "% d", 12)
    CHK(3, "-12", LEN, "% d", -12)
    CHK(3, "+12", LEN, "%+d", 12)
    CHK(5, "-0042", LEN, "%05d", -42)
    CHK(5, "00042", LEN, "%05d", 42)
    CHK(5, "+0042", LEN, "%+05d", 42)
    CHK(5, " 0042", LEN, "% 05d", 42)
    CHK(6, "42   |", LEN, "%-5d|", 42)
    CHK(5, "00042", LEN, "%.5d", 42)
    CHK(8, "  -00042", LEN, "%8.5d", -42)
    CHK(1, "|", LEN, "%.0d|", 0)
    CHK(6, "42   |", LEN, "%*d|", -5, 42)
    CHK(6, "   42|", LEN, "%*d|", 5, 42)
    CHK(3, "42|", LEN, "%.*d|", -5, 42)
    CHK(4, "abc|", LEN, "%.*s|", 3, "abcdef")
    CHK(2, "ff", LEN, "%x", 255)
    CHK(3, "ABC", LEN, "%X", 0xABCu)
    CHK(4, "0xff", LEN, "%#x", 255u)
    CHK(4, "0XFF", LEN, "%#X", 255u)
    CHK(1, "0", LEN, "%#x", 0u)
    CHK(3, "010", LEN, "%#o", 8u)
    CHK(4, "  01", LEN, "%#4o", 1u)
    CHK(8, "     010", LEN, "%#8o", 8u)
    CHK(6, "0x00ff", LEN, "%#06x", 255u)
    CHK(10, "0x000000ff", LEN, "%#010x", 255u)
    CHK(4, "0xff", LEN, "%#4x", 255u)
    CHK(5, "0x005", LEN, "%#.3x", 5u)
    CHK(4, "0xff", LEN, "%#2x", 255u)
    CHK(4, "0xff", LEN, "%#03x", 255u)
    CHK(5, "0x0ff", LEN, "%#05x", 255u)
    CHK(2, "01", LEN, "%#1o", 1u)
    CHK(3, "010", LEN, "%#02o", 8u)
    CHK(5, "00010", LEN, "%#05o", 8u)
    CHK(5, "00010", LEN, "%#.5o", 8u)
    CHK(10, "0b00000101", LEN, "%#010b", 5u)
    CHK(3, "101", LEN, "%b", 5u)
    CHK(5, "0b101", LEN, "%#b", 5u)
    CHK(8, "   0b101", LEN, "%#8b", 5u)
    CHK(1, "0", LEN, "%#b", 0u)
    CHK(6, "ABCDEF", LEN, "%llX", 0xABCDEFull)
    CHK(8, "0xabcdef", LEN, "%#llx", 0xABCDEFull)
    CHK(2, "-5", LEN, "%lld", -5ll)
    CHK(2, "+5", LEN, "%+lld", 5ll)
    CHK(1, "5", LEN, "%llu", 5ull)
    CHK(2, "-5", LEN, "%ld", -5L)
    CHK(1, "5", LEN, "%lu", 5UL)
    CHK(1, "5", LEN, "%zu", (size_t)5)
    CHK(2, "-5", LEN, "%td", (ptrdiff_t)-5)
    CHK(1, "7", LEN, "%jd", (intmax_t)7)
    CHK(2, "44", LEN, "%hhd", 300)
    CHK(2, "44", LEN, "%hhu", 300u)
    CHK(4, "4464", LEN, "%hd", 70000)
    CHK(4, "4464", LEN, "%hu", 70000u)
    CHK(3, "1%|", LEN, "%d%%|", 1)
    /* %p is zero padded to the pointer size */
    rc = vtprintf_s(str1, LEN, "%p", (void *)0x1234);
    if (rc != (int)(sizeof(void *) * 2) || strcmp(&str1[rc - 4], "1234") != 0) {
        debug_printf("%s %u  %%p got %d \"%s\"\n", __FUNCTION__, __LINE__,
                     (int)rc, str1);
        errs++;
    }
    /* width exceeds max */
    CHK(-ESLEMAX, "", LEN, "%2147483647d", 5)

    /* chars and strings */
    CHK(2, "a|", LEN, "%c|", 'a')
    CHK(6, "    a|", LEN, "%5c|", 'a')
    CHK(6, "a    |", LEN, "%-5c|", 'a')
    CHK(6, "   ab|", LEN, "%5s|", "ab")
    CHK(6, "ab   |", LEN, "%-5s|", "ab")
    CHK(2, "a|", LEN, "%.1s|", "ab")
    CHK(6, "    a|", LEN, "%5.1s|", "ab")
    CHK(6, "a    |", LEN, "%-5.1s|", "ab")

    /* dmax too small: the output functions fail at different places */
    CHK(-ESNOSPC, "", 3, "abcdef")
    CHK(-ESNOSPC, "", 3, "abc%%")
    CHK(-ESNOSPC, "", 3, "abc%c", 'x')
#ifndef SAFECLIB_DISABLE_WCHAR
    CHK(-ESNOSPC, "", 3, "abc%lc", (wint_t)'x')
#endif
    CHK(-ESNOSPC, "", 3, "%5d", 5)
    CHK(-ESNOSPC, "", 5, "%-10d", 5)
    CHK(-ESNOSPC, "", 3, "%x", 0xffff)
    CHK(-ESNOSPC, "", 5, "%10c", 'a')
    CHK(-ESNOSPC, "", 5, "%-10c", 'a')
    CHK(-ESNOSPC, "", 5, "%10s", "a")
    CHK(-ESNOSPC, "", 5, "%-10s", "ab")
    CHK(-ESNOSPC, "", 8, "%9s", "abc")
    /* just fits: truncated by the terminating \0 */
    CHK(4, "0xf", 4, "%#x", 255u)

    /* illegal formats */
    CHK(-1, "", LEN, "%5n", 1)
    CHK(-1, "", LEN, "%hhn", 1)
    CHK(-1, "", LEN, "abc%")
    CHK(-1, "", LEN, "%l")
#ifndef ENABLE_ILLEGAL_FORMATSPECIFIER
    CHK(-1, "", LEN, "%y", 1)
    CHK(-1, "", LEN, "%Ld", 1)
#endif

#ifndef SAFECLIB_DISABLE_WCHAR
    /* wide strings and chars */
    CHK(4, "abc|", LEN, "%ls|", L"abc")
    CHK(3, "ab|", LEN, "%.2ls|", L"abc")
    CHK(6, "  abc|", LEN, "%5ls|", L"abc")
    CHK(6, "abc  |", LEN, "%-5ls|", L"abc")
    CHK(2, "x|", LEN, "%lc|", (wint_t)'x')
    CHK(6, "    x|", LEN, "%5lc|", (wint_t)'x')
    CHK(6, "x    |", LEN, "%-5lc|", (wint_t)'x')
    CHK(-ESNULLP, "", LEN, "%ls", (wchar_t *)NULL)
    CHK(-ESNOSPC, "", 3, "%ls|", L"abc")
    CHK(-ESNOSPC, "", 3, "%ls|", L"abcd") /* %ls arg exceeds dmax */
    CHK(-ESNOSPC, "", 3, "%s|", "abcd")   /* %s arg exceeds dmax */
    CHK(-ESNOSPC, "", 8, "%9ls", L"abc")
    CHK(-ESNOSPC, "", 5, "%10ls", L"a")
    CHK(-ESNOSPC, "", 5, "%-10ls", L"ab")
    /* %lc is appended, not copied to the start */
    CHK(3, "abx", LEN, "ab%lc", (wint_t)'x')
    CHK(5, "abcd|", LEN, "abcd%lc", (wint_t)'|')
    /* surrogates are not convertible: wctomb fails */
    CHK(-EILSEQ, "", LEN, "%lc|", (wint_t)0xD800)
    CHK(-EILSEQ, "", LEN, "%lc|", (wint_t)0x7fffffff)
    /* not convertible with wcstombs_s: negative EILSEQ */
    CHK(-EILSEQ, "", LEN, "%ls|", L"ab\xD800")
#endif
    EXPNULL(str1)

#ifdef PRINTF_SUPPORT_FLOAT
    CHK(8, "1.500000", LEN, "%f", 1.5)
    CHK(9, "-1.500000", LEN, "%f", -1.5)
    CHK(14, "1.500000000000", LEN, "%.12f", 1.5)
    CHK(8, "-0001.50", LEN, "%08.2f", -1.5)
    CHK(9, "+1.500000", LEN, "%+f", 1.5)
    CHK(9, " 1.500000", LEN, "% f", 1.5)
    CHK(11, "1.5       |", LEN, "%-10.1f|", 1.5)
    CHK(11, "       1.5|", LEN, "%10.1f|", 1.5)
    CHK(11, "-0000001.5|", LEN, "%010.1f|", -1.5)
    CHK(4, "1.00", LEN, "%.2f", 0.999)
    CHK(5, "10.00", LEN, "%.2f", 9.999)
    CHK(1, "2", LEN, "%.0f", 1.7)
    CHK(8, "0.000000", LEN, "%f", 0.0)
    CHK(3, "nan", LEN, "%f", NAN)
    CHK(3, "NAN", LEN, "%F", NAN)
    CHK(3, "inf", LEN, "%f", INFINITY)
    CHK(4, "+inf", LEN, "%+f", INFINITY)
    CHK(3, "INF", LEN, "%F", INFINITY)
    CHK(4, "+INF", LEN, "%+F", INFINITY)
    CHK(4, "-inf", LEN, "%f", -INFINITY)
    CHK(4, "-INF", LEN, "%F", -INFINITY)
    CHK(9, "     inf|", LEN, "%8f|", INFINITY)
    CHK(9, "-inf    |", LEN, "%-8f|", -INFINITY)
    /* too large for %f: all digits as libc */
    CHKLIBC("%f", 1e20)
    CHKLIBC("%f", -1e20)
    CHKLIBC("%f", 1e10)
    CHKLIBC("%.2f", 123456789012.5)
    CHKLIBC("%+f", 1e10)
    CHKLIBC("%020.1f", 1e10)
    /* does not fit into dmax */
    CHK(-ESNOSPC, "", LEN, "%f", 1e300)
    /* round half to even */
    CHK(1, "2", LEN, "%.0f", 2.5)
    CHK(1, "0", LEN, "%.0f", 0.5)
    CHK(1, "2", LEN, "%.0f", 1.5)
    CHK(1, "4", LEN, "%.0f", 3.5)
    CHK(1, "4", LEN, "%.0f", 4.5)
    CHK(2, "-2", LEN, "%.0f", -2.5)
    CHKLIBC("%f", -0.0)
    /* too small dmax */
    CHK(-ESNOSPC, "", 5, "%f", 123.456)
    CHK(-ESNOSPC, "", 3, "%-6.1f|", 1.5)
    CHK(-ESNOSPC, "", 3, "%8f", NAN)
    /* truncated */
    CHK(5, "  1.", 5, "%5.1f", 1.5)
    CHK(3, "na", 3, "%f", NAN)

#ifdef PRINTF_SUPPORT_EXPONENTIAL
    CHK(12, "1.500000e+00", LEN, "%e", 1.5)
    CHK(12, "1.500000E+00", LEN, "%E", 1.5)
    CHK(13, "-1.500000e+00", LEN, "%e", -1.5)
    CHK(12, "1.000000e-05", LEN, "%e", 1e-5)
    CHK(13, "1.000000e-100", LEN, "%e", 1e-100)
    CHK(8, "1.23e+04", LEN, "%.2e", 12345.678)
    CHK(16, "   1.500000e+00|", LEN, "%15e|", 1.5)
    CHK(16, "1.500000e+00   |", LEN, "%-15e|", 1.5)
    CHK(16, "0001.500000e+00|", LEN, "%015e|", 1.5)
    CHK(16, "-1.500000E+100 |", LEN, "%-15E|", -1.5e100)
    CHK(5, "2e+01", LEN, "%.0e", 15.0)
    CHK(3, "nan", LEN, "%e", NAN)
    CHK(3, "inf", LEN, "%e", INFINITY)
    CHK(4, "-inf", LEN, "%e", -INFINITY)
    /* zero and the exponent 100 */
    CHK(12, "0.000000e+00", LEN, "%e", 0.0)
    CHK(13, "1.000000e+100", LEN, "%e", 1e100)
    CHKLIBC("%e", 1e100)
    CHKLIBC("%e", 1e10)
    CHKLIBC("%e", -0.0)
    CHK(3, "1.5", LEN, "%g", 1.5)
    CHK(3, "1.5", LEN, "%G", 1.5)
    CHK(6, "100000", LEN, "%g", 100000.0)
    CHK(5, "1e-05", LEN, "%g", 1e-5)
    CHK(5, "1E-05", LEN, "%G", 1e-5)
    CHK(6, "0.0001", LEN, "%g", 0.0001)
    CHK(7, "1.50000", LEN, "%#g", 1.5)
    CHK(8, "1.23e-05", LEN, "%.3g", 0.00001234)
    CHK(11, "       1.5|", LEN, "%10g|", 1.5)
    CHK(11, "1.5       |", LEN, "%-10g|", 1.5)
    CHK(11, "1e-10     |", LEN, "%-10g|", 1e-10)
    CHK(1, "2", LEN, "%.0g", 1.5)
    CHK(3, "1.5", LEN, "%.10g", 1.5)
    /* %g of 1e6, >= 1000 with precision, zero: same as libc */
    CHKLIBC("%g", 1e6)
    CHKLIBC("%.3g", 1234.5)
    CHKLIBC("%g", 0.0)
    CHKLIBC("%.0g", 0.0)
    CHKLIBC("%g", 1e-5)
    CHKLIBC("%g", 123456789.0)
    CHKLIBC("%.0g", 2.5)
    CHKLIBC("%+g", 1234.5)
    CHK(3, "1.5", LEN, "%g", 1.5)
    /* %e writes the exponent char after the mantissa */
    CHK(-ESNOSPC, "", 8, "%e", 1.5)
    CHK(-ESNOSPC, "", 9, "%e", 1.5)
    CHK(-ESNOSPC, "", 5, "%e", 123.456)
    CHK(-ESNOSPC, "", 9, "%15e|", 1.5)
    CHK(-ESNOSPC, "", 11, "%-15e|", 1.5)
    CHK(-ESNOSPC, "", 11, "%-15E|", 1.5)
    CHK(8, "1.5e+00", 8, "%.1e|", 1.5)

    /* %a is delegated to libc */
    CHKLIBC("%a", 1.0)
    CHKLIBC("%A", 1.0)
    CHKLIBC("%a", 1.5)
    CHKLIBC("%.2a", 1.0)
    CHKLIBC("%a|", 1.5)
    CHKLIBC("%A|", 1.5)
    CHK(3, "nan", LEN, "%a", NAN)
    CHK(3, "NAN", LEN, "%A", NAN)
    CHK(3, "inf", LEN, "%a", INFINITY)
    CHK(3, "INF", LEN, "%A", INFINITY)
    CHK(4, "-inf", LEN, "%a", -INFINITY)
    CHK(4, "+inf", LEN, "%+a", INFINITY)
    CHK(4, "+INF", LEN, "%+A", INFINITY)
    CHK(-ESNOSPC, "", 5, "%a", 1.0)
#endif /* PRINTF_SUPPORT_EXPONENTIAL */

#ifdef PRINTF_SUPPORT_LONG_DOUBLE
    CHKLIBC("%Lf", 1.5L)
    CHKLIBC("%Lf|", 3.0L)
    CHKLIBC("%Lf", 1e30L)
    CHK(3, "inf", LEN, "%Lf", HUGE_VALL)
    CHK(4, "-inf", LEN, "%Lf", -HUGE_VALL)
    CHK(4, "+inf", LEN, "%+Lf", HUGE_VALL)
    CHK(4, "+INF", LEN, "%+LF", HUGE_VALL)
    CHK(4, "-INF", LEN, "%LF", -HUGE_VALL)
    CHK(3, "nan", LEN, "%Lf", (long double)NAN)
    CHK(3, "NAN", LEN, "%LF", (long double)NAN)
    CHK(-ESNOSPC, "", 5, "%Lf", 123.456L)
#ifdef PRINTF_SUPPORT_EXPONENTIAL
    CHKLIBC("%Le", 1.5L)
    CHKLIBC("%Le|", 1.5L)
    CHKLIBC("%LE", 1.5L)
    CHKLIBC("%Lg", 1.5L)
    CHKLIBC("%LG", 1.5L)
    CHKLIBC("%La", 1.0L)
    CHKLIBC("%LA", 1.0L)
    CHKLIBC("%La|", 1.0L)
    CHK(3, "inf", LEN, "%Le", HUGE_VALL)
    CHK(3, "inf", LEN, "%La", HUGE_VALL)
    CHK(-ESNOSPC, "", 5, "%La", 1.0L)
#endif
#endif /* PRINTF_SUPPORT_LONG_DOUBLE */
#endif /* PRINTF_SUPPORT_FLOAT */

    /* an illegal %L with integer conversions */
#if defined(PRINTF_SUPPORT_LONG_DOUBLE) && !defined(ENABLE_ILLEGAL_FORMATSPECIFIER)
    CHK(-1, "", LEN, "%Lx", 1)
#endif
#endif /* USE_MSVCRT */

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_vsnprintf_s()); }
#endif
