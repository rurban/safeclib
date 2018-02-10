/*------------------------------------------------------------------
 * test_vsnprintf_s
 * File 'wchar/vsnprintf_s.c'
 * Lines executed:100.00% of 23
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#ifdef HAVE_VSNPRINTF_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#if defined(_WIN32) && defined(HAVE_VSNPRINTF_S)
#define USE_MSVCRT
int vsnprintf_s(char *dest, size_t dmax, size_t maxcount,const char *fmt, va_list arglist);
#endif

#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];

static inline int
vtprintf_s (char *restrict dest, rsize_t dmax, const char *restrict fmt, ...)
    BOS_CHK(dest) BOS_NULL(fmt)
{
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

int test_vsnprintf_s (void)
{
    errno_t rc;
    int32_t  ind;
    int32_t  len2;
    int32_t  len3;
    int errs = 0;
    int have_wine = 0;

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
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_VSNPRINTF_S))
    EXPECT_BOS("empty fmt")
    rc = vtprintf_s(str1, LEN, NULL);
    NEGERR_MSVC(ESNULLP, EOF);
#else
    printf("Using wine\n");
    have_wine = 1;    
#endif

    EXPECT_BOS("dest overflow")
    rc = vtprintf_s(str1, (RSIZE_MAX_STR+1), "%s", str2);
    NEGERR_MSVC(ESLEMAX, have_wine?EOF:0);
#endif

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 1, "%s", str2);
    if (!use_msvcrt) {
        ERR(14);
    } else {
        ERR(-1);
    }
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 2, "%s", str2);
    if (!use_msvcrt) {
        ERR(14);
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
        debug_printf("%s %u lengths wrong: %d  %u  %u \n",
                     __FUNCTION__, __LINE__, len1, len2, len3);
#endif
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 5, "%s", str2);
#ifndef __MINGW32__
    NOERRNULL() /* no ENOSPC */
    EXPSTR(str1, "keep")
#else
    ERR(-1);
    EXPSTR(str1, "")
#endif

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 2, "%s", str2);
#ifndef __MINGW32__
    ERR(14) /* sic! unsafe */
    EXPSTR(str1, "k")
#else
    ERR(-1);
    EXPSTR(str1, "")
#endif

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
#ifndef __MINGW32__
    ERR(14) /* sic! unsafe */
#else
    ERR(-1);
    EXPSTR(str1, "")
#endif

/*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 52, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

/*--------------------------------------------------*/

    strcpy(str1, "12345678901234567890");

    rc = vtprintf_s(str1, 8, "%s", &str1[7]);
#ifndef __MINGW32__
    ERR(13) /* sic! unsafe */
#else
    ERR(-1);
    EXPSTR(str1, "")
#endif

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
int main (void)
{
    return (test_vsnprintf_s());
}
#endif
