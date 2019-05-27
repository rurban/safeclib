/*------------------------------------------------------------------
 * test_wcsncat_s
 * File 'wchar/wcsncat_s.c'
 * Lines executed:93.55% of 93
 *
 * wine tested with wine-2.0.4
 * msvcrt.dll 7.0.9200.16384 (win8 64bit default)
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCSNCAT_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static wchar_t str1[LEN];
static wchar_t str2[LEN];
int test_wcsncat_s(void);

int test_wcsncat_s(void) {
    errno_t rc;
    size_t len;
    int errs = 0;
    int have_wine = 0;

    /*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"aaaaa");
    print_msvcrt(use_msvcrt);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest or dmax")
    rc = wcsncat_s(NULL, LEN, str2, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"b");

    EXPECT_BOS("empty slen")
    rc = wcsncat_s(str1, 1, str2, 0);
    if (use_msvcrt && rc == 0)
        have_wine = 1;
    ERR_MSVC(ESZEROL, have_wine ? 0 : EINVAL);
    WEXPSTR(str1, have_wine ? L"a" : L"");

    EXPECT_BOS("empty dest or dmax")
    rc = wcsncat_s(NULL, 1, str2, 1);
    ERR_MSVC(ESNULLP, EINVAL);

    /* silent ok as in the msvcrt, but with clang compile-time error */
    /*EXPECT_BOS("empty dest") EXPECT_BOS("empty dest or dmax")*/
    rc = wcsncat_s(NULL, 0, str2, 0);
    if (have_wine) {
        ERR(EINVAL);
    } else {
        ERR(EOK);
    }

    wcscpy(str1, L"a");
    EXPECT_BOS("empty slen")
    rc = wcsncat_s(str1, 0, str2, 0);
    ERR_MSVC(ESZEROL, EINVAL);
    WEXPSTR(str1, L"a");

    wcscpy(str1, L"a");
    EXPECT_BOS("empty dest or dmax")
    rc = wcsncat_s(str1, 0, str2, LEN);
    ERR_MSVC(ESZEROL, EINVAL);

    wcscpy(str1, L"a");
    EXPECT_BOS("dest overflow")
    rc = wcsncat_s(str1, (RSIZE_MAX_WSTR + 1), str2, 1);
    ERR_MSVC(ESLEMAX, EOK);
    if (!use_msvcrt) {
        WEXPSTR(str1, L"a");
    } else {
        WEXPSTR(str1, L"ab");
    }

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    wcscpy(str1, L"untouched");
    EXPECT_BOS("dest overflow")
    rc = wcsncat_s(str1, LEN + 1, str2, 5);
    ERR_MSVC(EOVERFLOW, 0);
    WEXPSTR(str1, L"untouched");
#endif

    wcscpy(str1, L"a");
    EXPECT_BOS("src overflow or empty")
    rc = wcsncat_s(str1, LEN, str2, (RSIZE_MAX_WSTR + 1));
    ERR_MSVC(ESLEMAX, 0);
    if (!use_msvcrt) {
        WEXPNULL(str1);
    } else {
        WEXPSTR(str1, L"ab");
    }

    wcscpy(str1, L"a");
    EXPECT_BOS("src overflow or empty")
    rc = wcsncat_s(str1, LEN, NULL, LEN);
    ERR_MSVC(ESNULLP, EINVAL);
    if (!have_wine)
        WCHECK_SLACK(str1, LEN);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    wcscpy(str1, L"ab");
    EXPECT_BOS("src overflow or empty")
    rc = wcsncat_s(str1, LEN, str2, LEN + 1);
    ERR_MSVC(EOVERFLOW, 0);
    if (!use_msvcrt) {
        WEXPNULL(str1);
    } else {
        WEXPSTR(str1, L"ab");
    }
#endif

#endif

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 1, str2, LEN);
    ERR_MSVC(ESUNTERM, EINVAL);
    if (!have_wine) {
        WEXPNULL(str1);
        WCHECK_SLACK(str1, 1);
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 2, str2, LEN);
    ERR_MSVC(ESUNTERM, EINVAL);
    if (!have_wine) {
        WEXPNULL(str1);
        if (!use_msvcrt)
            WCHECK_SLACK(str1, 2);
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"b");

    rc = wcsncat_s(str1, 2, str2, 1);
    ERR_MSVC(ESNOSPC, ERANGE);
    WEXPNULL(str1)
    if (!use_msvcrt)
        WCHECK_SLACK(str1, 2);

    /*--------------------------------------------------*/

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 8, &str1[3], 4);
    if (!have_wine) {
        ERR_MSVC(ESOVRLP, ERANGE);
        WEXPNULL(str1);
        if (!use_msvcrt)
            WCHECK_SLACK(str1, 8);
    }

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 4, &str1[3], 4);
    ERR_MSVC(ESOVRLP, EINVAL);
    if (!have_wine) {
        WEXPNULL(str1);
        if (!use_msvcrt)
            WCHECK_SLACK(str1, 4);
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 3, &str1[3], 4);
    ERR_MSVC(ESUNTERM, EINVAL);
    if (!have_wine) {
        WEXPNULL(str1);
        if (!use_msvcrt)
            WCHECK_SLACK(str1, 3);
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"abcdefgh");

    rc = wcsncat_s(&str1[3], 5, &str1[0], 4);
    ERR_MSVC(ESUNTERM, EINVAL);
    if (!have_wine) {
        WEXPNULL(&str1[3]);
        if (!use_msvcrt)
            WCHECK_SLACK(&str1[3], 5);
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"abcdefgh");

    rc = wcsncat_s(&str1[3], 12, &str1[0], 4);
    ERR_MSVC(ESOVRLP, EOK);
    if (!use_msvcrt) {
        WEXPNULL(&str1[3]);
        WCHECK_SLACK(&str1[3], 12);
    } else
        WEXPSTR(str1, L"abcdefghabcd");

    /*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"b");

    rc = wcsncat_s(str1, 3, str2, 1);
    ERR(EOK)
    WEXPSTR(str1, L"ab")
    WCHECK_SLACK(&str1[2], 3 - 2);

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 50, str2, LEN);
    ERR(EOK)
    WEXPSTR(str1, L"aaaaaaaaaakeep it simple")
    len = wcslen(str1);
    WCHECK_SLACK(&str1[len], 50 - len);

    /*--------------------------------------------------*/
    /* TR example */

    wcscpy(str1, L"good");
    wcscpy(str2, L"bye");

    rc = wcsncat_s(str1, 100, str2, 100);
    ERR(EOK)
    WEXPSTR(str1, L"goodbye");
    len = wcslen(str1);
    if (!use_msvcrt)
        WCHECK_SLACK(&str1[len], 100 - len);

    /*--------------------------------------------------*/
    /* TR example */

    wcscpy(str1, L"hello");

    rc = wcsncat_s(str1, 6, L"", 1);
    ERR(EOK)
    WEXPSTR(str1, L"hello");
    len = wcslen(str1);
    WCHECK_SLACK(&str1[len], 6 - len);

    /*--------------------------------------------------*/
    /* TR example */

    wcscpy(str1, L"hello");

    rc = wcsncat_s(str1, 6, L"X", 2);
    ERR_MSVC(ESNOSPC, ERANGE);
    WEXPNULL(str1)
    if (!use_msvcrt)
        WCHECK_SLACK(str1, 6);

    /*--------------------------------------------------*/
    /* TR example */

    wcscpy(str1, L"abc");

    rc = wcsncat_s(str1, 7, L"defghijklmn", 3);
    ERR(EOK)
    WEXPSTR(str1, L"abcdef");
    len = wcslen(str1);
    WCHECK_SLACK(&str1[len], 7 - len);

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_wcsncat_s()); }
#endif
