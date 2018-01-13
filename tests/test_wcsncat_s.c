/*------------------------------------------------------------------
 * test_wcsncat_s
 * File 'wchar/wcsncat_s.c'
 * Lines executed:97.06% of 68
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

int test_wcsncat_s (void)
{
    errno_t rc;
    size_t len;
    int errs = 0;

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");

    rc = wcsncat_s(NULL, LEN, str2, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = wcsncat_s(str1, 0, str2, LEN);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = wcsncat_s(str1, (RSIZE_MAX_STR+1), str2, LEN);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    rc = wcsncat_s(str1, (RSIZE_MAX_STR), str2, (RSIZE_MAX_STR+1));
    ERR(ESLEMAX);
    WEXPSTR(str1, L"");
/*--------------------------------------------------*/

    rc = wcsncat_s(str1, LEN, NULL, LEN);
    ERR(ESNULLP)
    WCHECK_SLACK(str1, LEN);
/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 1, str2, LEN);
    ERR(ESUNTERM)
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 1);

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 2, str2, LEN);
    ERR(ESUNTERM)
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 2);

/*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"b");

    rc = wcsncat_s(str1, 2, str2, 1);
    ERR(ESNOSPC)
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 2);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 8, &str1[3], 4);
    ERR(ESOVRLP);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 8);

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 4, &str1[3], 4);
    ERR(ESOVRLP);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 4);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcd");

    rc = wcsncat_s(&str1[0], 3, &str1[3], 4);
    ERR(ESUNTERM);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 3);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcdefgh");

    rc = wcsncat_s(&str1[3], 5, &str1[0], 4);
    ERR(ESUNTERM);
    WEXPNULL(&str1[3])
    WCHECK_SLACK(&str1[3], 5);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcdefgh");

    rc = wcsncat_s(&str1[3], 12, &str1[0], 4);
    ERR(ESOVRLP);
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
    ERR(ESNOSPC)
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
