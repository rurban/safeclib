/*------------------------------------------------------------------
 * test_snprintf_s
 * File 'snprintf_s.c'
 * Lines executed:87.10% of 31
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#if defined(TEST_MSVCRT) && defined(HAVE_SNPRINTF_S)
#ifdef SAFECLIB_HAVE_C99
#undef snprintf_s
EXTERN int snprintf_s(char *restrict dest, rsize_t dmax,
                      const char *restrict fmt, ...);
#endif
#endif

#ifdef HAVE_SNPRINTF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
int test_snprintf_s(void);

int test_snprintf_s(void) {
    errno_t rc;
    int ind;
    int len2;
    int len3;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = snprintf_s(NULL, LEN, "%s", str2);
    NEGERR(ESNULLP)

    /* not even with dmax=0, use sprintf_s then */
    EXPECT_BOS("empty dest") EXPECT_BOS("empty dest or dmax")
    rc = snprintf_s(NULL, 0, "%s", str2);
    NEGERR(ESNULLP)

    strcpy(str1, "123456");
    EXPECT_BOS("empty dest or dmax")
    rc = snprintf_s(str1, 0, "%s", str2);
    NEGERR(ESZEROL)

    EXPECT_BOS("empty fmt")
    rc = snprintf_s(str1, LEN, NULL);
    NEGERR(ESNULLP)

    EXPECT_BOS("dest overflow")
    rc = snprintf_s(str1, RSIZE_MAX_STR + 1, "%s", str2);
    NEGERR(ESLEMAX)

    /* only with c99 __VA_ARGS__ we can pass destbos */
#ifdef SAFECLIB_HAVE_C99
    if (_BOS_KNOWN(str1)) {
        EXPECT_BOS("dest overflow")
        rc = snprintf_s(str1, LEN + 1, "%s", str2);
        NEGERR(EOVERFLOW);      /* dmax exceeds dest */
        CHECK_SLACK(str1, LEN); /* cleared */
    }
#endif
#endif

    /*--------------------------------------------------*/

    str2[0] = '\0';
    rc = snprintf_s(str1, LEN, "%s %n", str2, &ind);
    NEGERR(EINVAL)

    rc = snprintf_s(str1, LEN, "%s %%n", str2);
    ERR(3)

    rc = snprintf_s(str1, LEN, "%%n");
    ERR(2);

    /*--------------------------------------------------*/

    /* TODO
    rc = snprintf_s(str1, LEN, "%p", NULL);
    NEGERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    strcpy(str1, "123456");
    strcpy(str2, "keep it simple");

    rc = snprintf(str1, 1, "%s", str2);
    /* number of characters (not including the terminating null character)
       which would have been written to buffer if dmax was ignored */
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERR(14); /* but truncated, written only 1 */
    EXPSTR(str1, "");
    if ((ind = memcmp(str1, "\00023456\000", 7))) {
        debug_printf("%s %u snprintf truncation: %d \"\\x%x%s\"\n",
                     __FUNCTION__, __LINE__, ind, str1[0], &str1[1]);
        errs++;
    }
#else
    ERR(-1);
    /* EXPSTR(str1, "k23456"); */ /* oops, insecure */
#endif

    strcpy(str1, "123456");
    rc = snprintf_s(str1, 1, "%s", str2);
    /* number of characters not including the terminating null
     * character (which is always written as long as buffer is not a
     * null pointer and bufsz is not zero and not greater than
     * RSIZE_MAX), which would have been written to buffer if bufsz
     * was ignored. */
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERR(14); /* but truncated, written only 1, the \0 */
#else
    ERR(-1);
#endif
    EXPSTR(str1, ""); /* at least we are secure */
    if ((ind = memcmp(str1, "\00023456\000", 7))) {
        debug_printf("%s %u snprintf_s truncation: %d \"\\x%x%s\"\n",
                     __FUNCTION__, __LINE__, ind, str1[0], &str1[1]);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "123456");
    strcpy(str2, "keep it simple");

    rc = snprintf(str1, 2, "%s", str2);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERR(14); /* but truncated, written only 2: k\0 */
    EXPSTR(str1, "k")
    if ((ind = memcmp(str1, "k\0003456\000", 7))) {
        debug_printf("%s %u snprintf truncation: %d \"%s\"\n", __FUNCTION__,
                     __LINE__, ind, str1);
        errs++;
    }
#else
    ERR(-1);
    /* mingw is entirely insecure => "ke3456" */
#endif

    strcpy(str1, "123456");
    rc = snprintf_s(str1, 2, "%s", str2);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERR(14); /* but truncated, written only 2: k\0 */
    EXPSTR(str1, "k")
    if ((ind = memcmp(str1, "k\0003456\000", 7))) {
        debug_printf("%s %u snprintf truncation: %d \"%s\"\n", __FUNCTION__,
                     __LINE__, ind, str1);
        errs++;
    }
#else
    ERR(-1);
    EXPSTR(str1, "")
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    len2 = strlen(str2);

    rc = snprintf_s(str1, 50, "%s", str2);
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

    rc = snprintf_s(str1, 5, "%s", str2);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    NOERRNULL() /* no ENOSPC */
    EXPSTR(str1, "keep")
#else
    EXPSTR(str1, "")
#endif

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = snprintf_s(str1, 2, "%s", str2);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    NOERRNULL()
    EXPSTR(str1, "k")
#else
    EXPSTR(str1, "")
#endif

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = snprintf_s(str1, 20, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = snprintf_s(str1, LEN, "%s", str2);
    ERR(0)
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = snprintf_s(str1, LEN, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    rc = snprintf_s(str1, LEN, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = snprintf_s(str1, 12, "%s", str2);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERR(14) /* sic! unsafe */
#else
    ERR(-1);
    EXPSTR(str1, "")
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = snprintf_s(str1, 52, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    strcpy(str1, "12345678901234567890");

    rc = snprintf_s(str1, 8, "%s", &str1[7]);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERR(13) /* sic! unsafe */
#else
    ERR(-1);
    EXPSTR(str1, "")
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "123456789");

    rc = snprintf_s(str1, 9, "%s", &str1[8]);
    ERR(1) /* overlapping allowed */
    EXPSTR(str1, "9")

    /*--------------------------------------------------*/

    strcpy(str2, "123");
    strcpy(str1, "keep it simple");

    rc = snprintf_s(str2, 31, "%s", &str1[0]);
    NOERRNULL()
    EXPSTR(str2, "keep it simple")

    /*--------------------------------------------------*/

    strcpy(str2, "1234");
    strcpy(str1, "56789");

    rc = snprintf_s(str2, 10, "%s", str1);
    NOERRNULL()
    EXPSTR(str2, "56789")

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_snprintf_s()); }
#endif
