/*------------------------------------------------------------------
 * test_wcscpy_s
 * File 'wchar/wcscpy_s.c'
 * Lines executed:100.00% of 45
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

int test_wcscpy_s (void)
{
    errno_t rc;
    int     ind;
    size_t  len;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcscpy_s(NULL, LEN, str2);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    rc = wcscpy_s(str1, 5, NULL);
    ERR(ESNULLP)
    WCHECK_SLACK(str1, 5);

/*--------------------------------------------------*/

    rc = wcscpy_s(str1, 0, str2);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = wcscpy_s(str1, (RSIZE_MAX_STR+1), str2);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = L'\0';

    rc = wcscpy_s(str1, LEN/2, str2);
    ERR(EOK)
    WCHECK_SLACK(str1, LEN/2);

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    rc = wcscpy_s(str1, LEN, str1);
    ERR(EOK)

/*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");

    rc = wcscpy_s(&str1[0], LEN, &str1[5]);
    ERR(ESOVRLP);
    WCHECK_SLACK(str1, LEN);

    wcscpy(str1, L"keep it simple");
    rc = wcscpy_s(&str1[5], LEN-5, &str1[0]);
    ERR(ESOVRLP)
    WCHECK_SLACK(&str1[5], LEN-5);

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
    WCHECK_SLACK(&str1[len], LEN-len);

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, LEN, str2);
    ERR(EOK);
    WEXPSTR(str1, str2);
    WCHECK_SLACK(&str1[len], LEN-len);

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, 1, str2);
    ERR(ESNOSPC)
    WCHECK_SLACK(str1, 1);

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, 2, str2);
    ERR(ESNOSPC)
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

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcscpy_s());
}
#endif
