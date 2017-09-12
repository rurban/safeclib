/*------------------------------------------------------------------
 * test_wcsupr_s
 * File 'wcsupr_s.c'
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
    rc = wcsupr_s(NULL, len);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    len = 0;
    rc = wcsupr_s(L"test", len);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    len = 99999;
    rc = wcsupr_s(L"test", len);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    wcscpy(str, L"n");
    len = wcslen(str);

    rc = wcsupr_s(str, len);
    ERR(EOK);
    WEXPSTR(str, L"N");

/*--------------------------------------------------*/

    wcscpy(str, L"N");
    len = wcslen(str);

    rc = wcsupr_s(str, len);
    ERR(EOK)
    WEXPSTR(str, L"N");

/*--------------------------------------------------*/

    wcscpy(str, L"nowisthetim3");

    rc = wcsupr_s(str, 25);
    ERR(EOK)
    WEXPSTR(str, L"NOWISTHETIM3");

/*--------------------------------------------------*/

    wcscpy(str, L"nowisthetime");
    len = wcslen(str);

    rc = wcsupr_s(str, len);
    ERR(EOK)
    WEXPSTR(str, L"NOWISTHETIME");

/*--------------------------------------------------*/

    wcscpy(str, L"qqeRo");
    len = wcslen(str);

    rc = wcsupr_s(str, len);
    ERR(EOK)
    WEXPSTR(str, L"QQERO");

/*--------------------------------------------------*/

    wcscpy(str, L"1234");
    len = wcslen(str);

    rc = wcsupr_s(str, 4);
    ERR(EOK)
    WEXPSTR(str, L"1234");

/*--------------------------------------------------*/

    return (errs);
}
