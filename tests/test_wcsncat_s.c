/*------------------------------------------------------------------
 * test_wcsncat_s
 * File 'wchar/wcsncat_s.c'
 * Lines executed:88.24% of 68
 *
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
    int ind;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcsncat_s(NULL, LEN, str2, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = wcsncat_s(str1, LEN, NULL, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = wcsncat_s(str1, 0, str2, LEN);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = wcsncat_s(str1, (RSIZE_MAX_STR+1), str2, LEN);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    rc = wcsncat_s(str1, (RSIZE_MAX_STR), str2, (RSIZE_MAX_STR+1));
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 1, str2, LEN);
    ERR(ESUNTERM)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 2, str2, LEN);
    ERR(ESUNTERM)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"b");

    rc = wcsncat_s(str1, 2, str2, 1);
    ERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    wcscpy(str1, L"a");
    wcscpy(str2, L"b");

    rc = wcsncat_s(str1, 3, str2, 1);
    ERR(EOK)
    WEXPSTR(str1, L"ab")

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcsncat_s(str1, 50, str2, LEN);
    ERR(EOK)
    WEXPSTR(str1, L"aaaaaaaaaakeep it simple")

/*--------------------------------------------------*/
/* TR example */

    wcscpy(str1, L"good");
    wcscpy(str2, L"bye");

    rc = wcsncat_s(str1, 100, str2, 100);
    ERR(EOK)
    WEXPSTR(str1, L"goodbye");

/*--------------------------------------------------*/
/* TR example */

    wcscpy(str1, L"hello");

    rc = wcsncat_s(str1, 6, L"", 1);
    ERR(EOK)
    WEXPSTR(str1, L"hello");

/*--------------------------------------------------*/
/* TR example */

    wcscpy(str1, L"hello");

    rc = wcsncat_s(str1, 6, L"X", 2);
    ERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/
/* TR example */

    wcscpy(str1, L"abc");

    rc = wcsncat_s(str1, 7, L"defghijklmn", 3);
    ERR(EOK)
    WEXPSTR(str1, L"abcdef");

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
