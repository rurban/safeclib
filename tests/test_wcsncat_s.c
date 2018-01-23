/*------------------------------------------------------------------
 * test_wcsncat_s
 * File 'wchar/wcsncat_s.c'
 * Lines executed:97.06% of 68
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCSNCAT_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

int test_wcsncat_s (void)
{
    errno_t rc;
    size_t len;
    int errs = 0;

/*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"aaaaa");
    if (use_msvcrt)
        printf("Using msvcrt...\n");

    /* probe for msvcrt or safec.dll being active */
    rc = wcsncat_s(NULL, LEN, str2, LEN);
    if ( use_msvcrt && rc == ESNULLP ) {
        printf("safec.dll overriding msvcrt.dll\n");
        use_msvcrt = false;
    }
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    rc = wcsncat_s(str1, 0, str2, LEN);
    ERR_MSVC(ESZEROL, EINVAL);

/*--------------------------------------------------*/

    rc = wcsncat_s(str1, (RSIZE_MAX_STR+1), str2, LEN);
    ERR_MSVC(ESLEMAX, EOK);
    if (!use_msvcrt) {
        WEXPSTR(str1, L"a");
    } else {
        WEXPNULL(str1);
    }

/*--------------------------------------------------*/

    rc = wcsncat_s(str1, (RSIZE_MAX_STR), str2, (RSIZE_MAX_STR+1));
    ERR_MSVC(ESLEMAX, 0);
    WEXPNULL(str1)

/*--------------------------------------------------*/

    rc = wcsncat_s(str1, LEN, NULL, LEN);
    ERR_MSVC(ESNULLP, EINVAL);
    WCHECK_SLACK(str1, LEN);
/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 1, str2, LEN);
    ERR_MSVC(ESUNTERM, EINVAL);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 1);

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 2, str2, LEN);
    ERR_MSVC(ESUNTERM, EINVAL);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 2);

/*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"b");

    rc = wcsncat_s(str1, 2, str2, 1);
    ERR_MSVC(ESNOSPC, ERANGE);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 2);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 8, &str1[3], 4);
    ERR_MSVC(ESOVRLP, ERANGE);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 8);

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 4, &str1[3], 4);
    ERR_MSVC(ESOVRLP, EINVAL);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 4);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 3, &str1[3], 4);
    ERR_MSVC(ESUNTERM, EINVAL);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 3);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcdefgh");

    rc = wcsncat_s(&str1[3], 5, &str1[0], 4);
    ERR_MSVC(ESUNTERM, EINVAL);
    WEXPNULL(&str1[3])
    WCHECK_SLACK(&str1[3], 5);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcdefgh");

    rc = wcsncat_s(&str1[3], 12, &str1[0], 4);
    ERR_MSVC(ESOVRLP, EOK);
    WEXPNULL(&str1[3])
    WCHECK_SLACK(&str1[3], 12);

/*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"b");

    rc = wcsncat_s(str1, 3, str2, 1);
    ERR(EOK)
    WEXPSTR(str1, L"ab")
    WCHECK_SLACK(&str1[2], 3-2);

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 50, str2, LEN);
    ERR(EOK)
    WEXPSTR(str1, L"aaaaaaaaaakeep it simple")
    len = wcslen(str1);
    WCHECK_SLACK(&str1[len], 50-len);

/*--------------------------------------------------*/
/* TR example */

    wcscpy(str1, L"good");
    wcscpy(str2, L"bye");

    rc = wcsncat_s(str1, 100, str2, 100);
    ERR(EOK)
    WEXPSTR(str1, L"goodbye");
    len = wcslen(str1);
    WCHECK_SLACK(&str1[len], 100-len);

/*--------------------------------------------------*/
/* TR example */

    wcscpy(str1, L"hello");

    rc = wcsncat_s(str1, 6, L"", 1);
    ERR(EOK)
    WEXPSTR(str1, L"hello");
    len = wcslen(str1);
    WCHECK_SLACK(&str1[len], 6-len);

/*--------------------------------------------------*/
/* TR example */

    wcscpy(str1, L"hello");

    rc = wcsncat_s(str1, 6, L"X", 2);
    ERR_MSVC(ESNOSPC, ERANGE);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 6);

/*--------------------------------------------------*/
/* TR example */

    wcscpy(str1, L"abc");

    rc = wcsncat_s(str1, 7, L"defghijklmn", 3);
    ERR(EOK)
    WEXPSTR(str1, L"abcdef");
    len = wcslen(str1);
    WCHECK_SLACK(&str1[len], 7-len);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcsncat_s());
}
#endif
