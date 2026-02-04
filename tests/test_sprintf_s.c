/*------------------------------------------------------------------
 * test_sprintf_s
 * File 'str/sprintf_s.c'
 * Lines executed:100.00% of 5
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#ifdef HAVE_STDDEF_H
#include <stddef.h> // do we have ptrdiff_t?
#endif

#ifdef PRINTF_INCLUDE_CONFIG_H
#include "printf_config.h"
#endif

// support for the long long types (%llu or %p)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
# ifdef HAVE_LONG_LONG
#  define PRINTF_SUPPORT_LONG_LONG
# else
#  undef PRINTF_SUPPORT_LONG_LONG
# endif
#endif

// support for the long double types (%Lf %Le %Lg %La)
// default: probed
#ifndef PRINTF_DISABLE_SUPPORT_LONG_DOUBLE
# ifdef HAVE_LONG_DOUBLE
#  define PRINTF_SUPPORT_LONG_DOUBLE
# else
#  undef PRINTF_SUPPORT_LONG_DOUBLE
# endif
#endif

// support for the floating point type (%f)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
#define PRINTF_SUPPORT_FLOAT
#endif

// support for exponential floating point notation (%e/%g)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_SUPPORT_EXPONENTIAL
#endif

// support for the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
#define PRINTF_SUPPORT_PTRDIFF_T
#endif

#if defined(TEST_MSVCRT) && defined(HAVE_SPRINTF_S)
#ifdef SAFECLIB_HAVE_C99
#undef sprintf_s
EXTERN int sprintf_s(char *restrict dest, rsize_t dmax,
                     const char *restrict fmt, ...);
#endif
#endif

//#ifdef HAVE_SPRINTF_S
//#define HAVE_NATIVE 1
//#else
//#define HAVE_NATIVE 0
//#endif
#include "test_msvcrt.h"

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
int test_sprintf_s(void);

int test_sprintf_s(void) {
    errno_t rc;
    int ind;
    int len2;
    int len3;
    int errs = 0;

    /*--------------------------------------------------*/

    str2[0] = '\0';

#ifndef HAVE_CT_BOS_OVR
    print_msvcrt(use_msvcrt);
    /* older windows sprintf_s segv here */
    EXPECT_BOS("empty dest")
    rc = sprintf_s(NULL, LEN, "%s", str2);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    ERR_MSVC(-ESNULLP, -1);
#if defined(__aarch64__) && defined(__linux__)
    /* ubuntu aarch64 glibc is broken here, fedora works fine. ubuntu returns ENOMEM. */
    if (errno == 12 && is_ubuntu())
        broken_errno = 1;
#endif
    ERRNO_MSVC(0, EINVAL);

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = sprintf_s(str1, RSIZE_MAX_STR + 1, "%s", str2);
    ERR_MSVC(-ESLEMAX, 0);
    ERRNO(0);

    /*--------------------------------------------------*/

    /* wine msvcrt doesn't check fmt==NULL */
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_SPRINTF_S))
    EXPECT_BOS("empty fmt")
    rc = sprintf_s(str1, LEN, NULL);
    ERR_MSVC(-ESNULLP, -1);
    ERRNO_MSVC(0, EINVAL);
 #endif

    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dmax")
    rc = sprintf_s(str1, 0, "%s", str2);
    ERR_MSVC(-ESZEROL, -1);
    ERRNO_MSVC(0, EINVAL);
#endif
    /*--------------------------------------------------*/

    rc = sprintf_s(str1, LEN, "%s %n", str2, &ind);
    ERR(-EINVAL);
    errno = 0;

    rc = sprintf_s(str1, LEN, "%s %%n", str2);
    ERR(3)

    rc = sprintf_s(str1, LEN, "%%n");
    ERR(2);

    /*--------------------------------------------------*/

    GCC_DIAG_IGNORE(-Wformat)
    rc = sprintf_s(str1, LEN, "%s", NULL);
    GCC_DIAG_RESTORE
    ERR(-ESNULLP);
    ERRNO_MSVC(0, EINVAL); // ??

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 1, "%s", str2);
#ifdef HAVE_CT_BOS_OVR
    print_msvcrt(use_msvcrt);
#endif
    init_msvcrt(rc == -ESNOSPC, &use_msvcrt);
    ERR_MSVC(-ESNOSPC, -1); /* ERANGE */
    ERRNO_MSVC(0, ERANGE);
    EXPNULL(str1)

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 2, "%s", str2);
    ERR_MSVC(-ESNOSPC, -1);
    ERRNO_MSVC(0, ERANGE);
    EXPNULL(str1)

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    len2 = strlen(str2);

    rc = sprintf_s(str1, 50, "%s", str2);
    ERR(len2)
    len3 = strlen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        int len1 = strlen(str1);
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, len1, len2, len3);
#endif
        errs++;
    }

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 1, "%s", str2);
    ERR_MSVC(-ESNOSPC, -1);
    ERRNO_MSVC(0, ERANGE);
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 2, "%s", str2);
    ERR_MSVC(-ESNOSPC, -1);
    ERRNO_MSVC(0, ERANGE);
    EXPNULL(str1)

    str1[0] = '\0';

    rc = sprintf_s(str1, 5, "%ld", -10000000L);
    ERR_MSVC(-ESNOSPC, -1);
    ERRNO_MSVC(0, ERANGE);
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 20, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = sprintf_s(str1, LEN, "%s", str2);
    ERR(0)
    EXPNULL(str1)

    strcpy(str1, "qqweqq");

    rc = sprintf_s(str1, LEN, "%s", "");
    ERR(0)
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, LEN, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, LEN, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, "keep it simple")

    /*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 12, "%s", str2);
    ERR_MSVC(-ESNOSPC, -1);
    ERRNO_MSVC(0, ERANGE);

    /*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 52, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, "keep it simple")

    /*--------------------------------------------------*/

    strcpy(str1, "12345678901234567890");

    rc = sprintf_s(str1, 8, "%s", &str1[7]);
    ERR_MSVC(-ESNOSPC, -1);
    ERRNO_MSVC(0, ERANGE);
    EXPNULL(str1)

    /*--------------------------------------------------*/

    strcpy(str1, "123456789");

    rc = sprintf_s(str1, 9, "%s", &str1[8]);
    ERR(1) /* overlapping allowed */
    EXPSTR(str1, "9")

    /*--------------------------------------------------*/

    strcpy(str2, "123");
    strcpy(str1, "keep it simple");

    rc = sprintf_s(str2, 31, "%s", &str1[0]);
    NOERRNULL()
    EXPSTR(str2, "keep it simple")

    /*--------------------------------------------------*/

    strcpy(str2, "1234");
    strcpy(str1, "56789");

    rc = sprintf_s(str2, 10, "%s", str1);
    ERR(5)
    EXPSTR(str2, "56789")

    // padding
    rc = sprintf_s(str2, 10, "[%6s]", str1);
    ERR(8)
    EXPSTR(str2, "[ 56789]")
    rc = sprintf_s(str2, 10, "[%-6s]", str1);
    ERR(8)
    EXPSTR(str2, "[56789 ]")
    rc = sprintf_s(str2, 10, "[%*s]", 6, str1);
    ERR(8)
    EXPSTR(str2, "[ 56789]")

    // truncating
    rc = sprintf_s(str2, 10, "%.4s", str1);
    ERR(4)
    EXPSTR(str2, "5678")
    rc = sprintf_s(str2, 10, "%.*s", 4, str1);
    ERR(4)
    EXPSTR(str2, "5678")

    rc = sprintf_s(str2, 10, "%c", 'A');
    ERR(1)
    EXPSTR(str2, "A")

    rc = sprintf_s(str2, LEN, "%i", 1);
    ERR(1)
    EXPSTR(str2, "1")
    rc = sprintf_s(str2, LEN, "%d", -1);
    ERR(2)
    EXPSTR(str2, "-1")
    rc = sprintf_s(str2, LEN, "%+i", 1);
    ERR(2)
    EXPSTR(str2, "+1")
    rc = sprintf_s(str2, LEN, "%.3i", 1);
    ERR(3)
    EXPSTR(str2, "001")
    rc = sprintf_s(str2, LEN, "%.0i", 1);
    ERR(1)
    EXPSTR(str2, "1")

    rc = sprintf_s(str2, LEN, "%hhu", (unsigned char)1);
    ERR(1)
    EXPSTR(str2, "1")
    rc = sprintf_s(str2, LEN, "%hu", (unsigned short)1);
    ERR(1)
    EXPSTR(str2, "1")
    rc = sprintf_s(str2, LEN, "%u", 1U);
    ERR(1)
    EXPSTR(str2, "1")
    rc = sprintf_s(str2, LEN, "%zu", (size_t)1U);
    ERR(1)
    EXPSTR(str2, "1")
    // 64-bit only:
    //if (sizeof(intmax_t) == sizeof(long))
#if SIZEOF_SIZE_T == 8
        rc = sprintf_s(str2, LEN, "%ju", 1UL);
#else
        rc = sprintf_s(str2, LEN, "%ju", 1ULL);
#endif
    ERR(1)
    EXPSTR(str2, "1")
    // else rc 19, 6223306920690712577

#ifdef PRINTF_SUPPORT_LONG_LONG
    rc = sprintf_s(str2, LEN, "%llu", 1ULL);
    ERR(1)
    EXPSTR(str2, "1")
#endif
#ifdef PRINTF_SUPPORT_FLOAT
#ifdef PRINTF_SUPPORT_EXPONENTIAL
    rc = sprintf_s(str2, LEN, "%f", 0.1f);
    NOERRNULL()
    EXPSTR(str2, "0.100000")
    rc = sprintf_s(str2, LEN, "%.1f", 0.1f);
    NOERRNULL()
    EXPSTR(str2, "0.1")
    rc = sprintf_s(str2, LEN, "%.0f", 1.5);
    NOERRNULL()
    EXPSTR(str2, "2")
    rc = sprintf_s(str2, LEN, "%05.2f", 1.5);
    NOERRNULL()
    EXPSTR(str2, "01.50")
    rc = sprintf_s(str2, LEN, "%.2f", 1.5);
    NOERRNULL()
    EXPSTR(str2, "1.50")
    rc = sprintf_s(str2, LEN, "%5.2f", 1.5);
    NOERRNULL()
    EXPSTR(str2, " 1.50")
    rc = sprintf_s(str2, LEN, "%F", 0.1f);
    NOERRNULL()
    EXPSTR(str2, "0.100000")
    rc = sprintf_s(str2, LEN, "%g", 0.1);
    NOERRNULL()
    EXPSTR(str2, "0.1")
    rc = sprintf_s(str2, LEN, "%#g", 0.1); // alternate form
    NOERRNULL()
    EXPSTR(str2, "0.100000")
    rc = sprintf_s(str2, LEN, "%G", 0.1);
    NOERRNULL()
    EXPSTR(str2, "0.1")
    rc = sprintf_s(str2, LEN, "%g", 0.1f);
    NOERRNULL()
    EXPSTR(str2, "0.1")
    rc = sprintf_s(str2, LEN, "%g", 1.0);
    NOERRNULL()
    EXPSTR(str2, "1")
    rc = sprintf_s(str2, LEN, "%lg", 0.1);
    NOERRNULL()
    EXPSTR(str2, "0.1")
    rc = sprintf_s(str2, LEN, "%a", 0.1f);
    NOERRNULL()
    EXPSTR(str2, "0x1.99999ap-4")

    rc = sprintf_s(str2, LEN, "%g", 0.0/0.0);
    NOERRNULL()
    EXPSTR(str2, "nan")
    rc = sprintf_s(str2, LEN, "%G", 0.0/0.0);
    NOERRNULL()
    EXPSTR(str2, "NAN")
    rc = sprintf_s(str2, LEN, "%g", 1.0/0.0);
    NOERRNULL()
    EXPSTR(str2, "inf")
    rc = sprintf_s(str2, LEN, "%g", -1.0/0.0);
    NOERRNULL()
    EXPSTR_OR(str2, "-inf", "inf") // "inf" on BSD/mingw
    rc = sprintf_s(str2, LEN, "%G", 1.0/0.0);
    NOERRNULL()
    EXPSTR(str2, "INF")
    rc = sprintf_s(str2, LEN, "%G", -1.0/0.0);
    NOERRNULL()
    EXPSTR_OR(str2, "-INF", "INF") // "INF" on BSD/mingw
    rc = sprintf_s(str2, LEN, "%+g", 1.0/0.0);
    NOERRNULL()
    EXPSTR(str2, "+inf")
    rc = sprintf_s(str2, LEN, "%+G", 1.0/0.0);
    NOERRNULL()
    EXPSTR(str2, "+INF")
#endif
#endif
    {
    const long double ld = 0.1;
#ifdef PRINTF_SUPPORT_LONG_DOUBLE
    rc = sprintf_s(str2, LEN, "%Ld", 1LL);
    ERR(-1)
    rc = sprintf_s(str2, LEN, "%Li", 1LL);
    ERR(-1)

#ifdef PRINTF_SUPPORT_FLOAT
#ifdef PRINTF_SUPPORT_EXPONENTIAL
    rc = sprintf_s(str2, LEN, "%Le", ld);
    NOERRNULL()
    EXPSTR(str2, "1.000000e-01")
    rc = sprintf_s(str2, LEN, "%LE", ld);
    NOERRNULL()
    EXPSTR(str2, "1.000000E-01")
    rc = sprintf_s(str2, LEN, "%Lf", ld);
    NOERRNULL()
    EXPSTR(str2, "0.100000")
    rc = sprintf_s(str2, LEN, "%LF", ld);
    NOERRNULL()
    EXPSTR(str2, "0.100000")
    rc = sprintf_s(str2, LEN, "%Lg", ld);
    NOERRNULL()
    EXPSTR(str2, "0.1")
    rc = sprintf_s(str2, LEN, "%LG", ld);
    NOERRNULL()
    EXPSTR(str2, "0.1")
    rc = sprintf_s(str2, LEN, "%La", ld);
    NOERRNULL()
    // OR-case with macOS, aarch64
    EXPSTR_OR(str2, "0xc.cccccccccccdp-7", "0x1.999999999999ap-4")
#endif
#endif
#endif
    }
#ifdef PRINTF_SUPPORT_PTRDIFF_T
    {
#ifdef HAVE_STDDEF_H
    const ptrdiff_t pd = str2 - str1;
    rc = sprintf_s(str2, LEN, "%td", pd);
#else
    rc = sprintf_s(str2, LEN, "%td", str2 - str1);
#endif
    NOERRNULL()
    if (str2 - str1)
        EXPNSTR(str2, "0")
    else
        EXPSTR(str2, "0")
    // invalid length
    GCC_DIAG_IGNORE(-Wformat)
#ifdef HAVE_STDDEF_H
    rc = sprintf_s(str2, LEN, "%t", pd);
#else
    rc = sprintf_s(str2, LEN, "%t", str2 - str1);
#endif
    GCC_DIAG_RESTORE
    ERR(-1)
    }
#endif

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_sprintf_s()); }
#endif
