/*------------------------------------------------------------------
 * test_wcscpy_s
 * File 'wchar/wcscpy_s.c'
 * Lines executed:95.92% of 49
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCSCPY_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX (128)
#define LEN (128)

static wchar_t str1[LEN];
static wchar_t str2[LEN];
int test_wcscpy_s(void);

int test_wcscpy_s(void) {
    errno_t rc;
    size_t len;
    int errs = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = wcscpy_s(NULL, LEN, str2);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    EXPECT_BOS("empty src")
    rc = wcscpy_s(str1, 5, NULL);
    ERR_MSVC(ESNULLP, EINVAL);
    WCHECK_SLACK(str1, 5);

    /*--------------------------------------------------*/

    wcscpy(str1, L"untouched");
    EXPECT_BOS("empty dest or dmax")
    rc = wcscpy_s(str1, 0, str2);
    ERR_MSVC(ESZEROL, EINVAL);
    WEXPSTR(str1, L"untouched");

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = wcscpy_s(str1, (RSIZE_MAX_STR + 1), str2);
    ERR_MSVC(ESLEMAX, 0);
#ifdef HAVE___BUILTIN_OBJECT_SIZE
    WEXPSTR(str1, L""); /* cleared if known destsize */
#else
    WEXPSTR(str1, L"untouched");
#endif

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    wcscpy(str1, L"untouched");
    EXPECT_BOS("dest overflow")
    rc = wcscpy_s(str1, LEN + 1, str2);
    ERR_MSVC(EOVERFLOW, 0);
    WEXPSTR(str1, L""); /* cleared if known destsize */
#endif
#endif

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = L'\0';

    rc = wcscpy_s(str1, LEN / 2, str2);
    ERR(EOK)
    WCHECK_SLACK(str1, LEN / 2);

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    GCC_PUSH_WARN_RESTRICT
    rc = wcscpy_s(str1, LEN, str1);
    GCC_POP_WARN_RESTRICT
    ERR(EOK)

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");

    rc = wcscpy_s(&str1[0], LEN, &str1[5]);
    ERR_MSVC(ESOVRLP, 0);
    if (!use_msvcrt) {
        WCHECK_SLACK(str1, LEN);
    } else { /* windows overlap with dest < src */
        WEXPSTR(str1, L"it simple");
    }

    wcscpy(str1, L"keep it simple");
    rc = wcscpy_s(&str1[5], LEN - 5, &str1[0]);
    /* Windows forbids overlap with dest > src */
    ERR_MSVC(ESOVRLP, ERANGE);
    WCHECK_SLACK(&str1[5], LEN - 5);

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    str2[0] = L'\0';

    rc = wcscpy_s(str1, LEN, str2);
    ERR(EOK)
    WCHECK_SLACK(str1, LEN);

    /*--------------------------------------------------*/

    str1[0] = L'\0';
    wcscpy(str2, L"keep it simple");
    len = wcslen(str2);

    rc = wcscpy_s(str1, LEN, str2);
    ERR(EOK);
    WEXPSTR(str1, str2);
    WCHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, LEN, str2);
    ERR(EOK);
    WEXPSTR(str1, str2);
    WCHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, 1, str2);
    ERR_MSVC(ESNOSPC, ERANGE);
    WCHECK_SLACK(str1, 1);

    /*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, 2, str2);
    ERR_MSVC(ESNOSPC, ERANGE);
    WCHECK_SLACK(str1, 2);

    /*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"it");

    rc = wcscpy_s(str1, 3, str2);
    ERR(EOK)
    WEXPSTR(str1, str2);
    WCHECK_SLACK(&str1[2], 1);

    /*--------------------------------------------------*/

    wcscpy(str1, L"it");
    wcscpy(str2, L"qqweqeqeqeq");

    rc = wcscpy_s(str2, 3, str1);
    ERR(EOK)
    WEXPSTR(str1, str2);
    WCHECK_SLACK(&str1[2], 1);

    /*--------------------------------------------------*/

    wcscpy(str1, L"qq12345weqeqeqeq");
    wcscpy(str2, L"it");

    rc = wcscpy_s(str1, 10, str2);
    ERR(EOK)
    WEXPSTR(str1, str2);
    WCHECK_SLACK(&str1[2], 8);

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_wcscpy_s()); }
