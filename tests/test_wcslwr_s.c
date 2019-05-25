/*------------------------------------------------------------------
 * test_wcslwr_s
 * File 'wcslwr_s.c'
 * Lines executed:100.00% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

int main(void) {
    errno_t rc;
    rsize_t len;
    wchar_t str[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

    wcscpy(str, L"untouched");
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("src overflow or empty")
    rc = wcslwr_s(NULL, 5);
    ERR(ESNULLP);
    WEXPSTR(str, L"untouched");

    wcscpy(str, L"untouched");
    EXPECT_BOS("src overflow or empty")
    rc = wcslwr_s(str, RSIZE_MAX_WSTR + 1);
    ERR(ESLEMAX);
    WEXPSTR(str, L"untouched");

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    wcscpy(str, L"untouched");
    EXPECT_BOS("src overflow or empty")
    rc = wcslwr_s(str, LEN + 1);
    ERR(EOVERFLOW);
    WEXPSTR(str, L"untouched");
#endif
#endif

    /*--------------------------------------------------*/

    wcscpy(str, L"untouched");
    rc = wcslwr_s(str, 0);
    ERR(EOK); /* and untouched */
    WEXPSTR(str, L"untouched");

    rc = wcslwr_s(NULL, 0);
    ERR(EOK); /* and untouched */
    WEXPSTR(str, L"untouched");

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
