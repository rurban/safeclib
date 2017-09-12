/*------------------------------------------------------------------
 * test_wcslwr_s
 * File 'wcslwr_s.c'
 * Lines executed:100.00% of 15
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
    errno_t rc;
    int ind;
    rsize_t len;
    wchar_t str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    len = 5;
    rc = wcslwr_s(NULL, len);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    len = 0;
    rc = wcslwr_s(L"test", len);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    len = 99999;
    rc = wcslwr_s(L"test", len);
    ERR(ESLEMAX)

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
