/*------------------------------------------------------------------
 * test_wcslwr_s
 * File 'wcslwr_s.c'
 * Lines executed:100.00% of 14
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
    errno_t rc;
    rsize_t len;
    wchar_t str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcslwr_s(NULL, 5);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    wcscpy(str, L"test");
    rc = wcslwr_s(str, 0);
    ERR(EOK); /* and untouched */
    WEXPSTR(str, L"test");

    rc = wcslwr_s(NULL, 0);
    ERR(EOK); /* and untouched */
    WEXPSTR(str, L"test");

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    wcscpy(str, L"test");
    rc = wcslwr_s(str, 99999);
    ERR(ESLEMAX); /* and untouched */
    WEXPSTR(str, L"test");
#endif

/*--------------------------------------------------*/

    wcscpy(str, L"N");
    len = wcslen(str);

    rc = wcslwr_s(str, len);
    ERR(EOK);
    WEXPSTR(str, L"n");

/*--------------------------------------------------*/

    wcscpy(str, L"n");
    len = wcslen(str);

    rc = wcslwr_s(str, len);
    ERR(EOK)
    WEXPSTR(str, L"n");

/*--------------------------------------------------*/

    wcscpy(str, L"NOWISTHETIM3");

    rc = wcslwr_s(str, 25);
    ERR(EOK)
    WEXPSTR(str, L"nowisthetim3");

/*--------------------------------------------------*/

    wcscpy(str, L"NOWISTHETIME");
    len = wcslen(str);

    rc = wcslwr_s(str, len);
    ERR(EOK)
    WEXPSTR(str, L"nowisthetime");

/*--------------------------------------------------*/

    wcscpy(str, L"qqeRo");
    len = wcslen(str);

    rc = wcslwr_s(str, len);
    ERR(EOK)
    WEXPSTR(str, L"qqero");

/*--------------------------------------------------*/

    wcscpy(str, L"1234");
    len = wcslen(str);

    rc = wcslwr_s(str, 4);
    ERR(EOK)
    WEXPSTR(str, L"1234");

/*--------------------------------------------------*/

    return (errs);
}
