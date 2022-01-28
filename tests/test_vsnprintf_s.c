/*------------------------------------------------------------------
 * test_vsnprintf_s
 * File 'wchar/vsnprintf_s.c'
 * Lines executed:40.12% of 486
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

//#ifdef HAVE_VSNPRINTF_S
//#define HAVE_NATIVE 1
//#else
//#define HAVE_NATIVE 0
//#endif
#include "test_msvcrt.h"

#if defined(_WIN32) && defined(HAVE_VSNPRINTF_S)
#define USE_MSVCRT
int vsnprintf_s(char *dest, size_t dmax, size_t maxcount, const char *fmt,
                va_list arglist);
#endif

#define LEN (128)

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
        debug_printf("%s %u lengths wrong: %d  %u  %u \n", __FUNCTION__,
                     __LINE__, len1, len2, len3);
#endif
        errs++;
    }

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 5, "%s", str2);
#ifndef HAVE_MINGW32
    ERR(-ESNOSPC);
#else
    ERR(-1);
#endif
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 2, "%s", str2);
#ifndef HAVE_MINGW32
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
#ifndef HAVE_MINGW32
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
#ifndef HAVE_MINGW32
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

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_vsnprintf_s()); }
#endif
