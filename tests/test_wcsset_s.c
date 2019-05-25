/*------------------------------------------------------------------
 * test_wcsset_s
 * File 'wcsset_s.c'
 * Lines executed:94.12% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)
int main(void);

int main(void) {
    errno_t rc;
    rsize_t max_len;
    wchar_t str1[LEN];
    wchar_t value = L'\0';
    uint32_t j;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = wcsset_s(NULL, 5, value);
    ERR(ESNULLP)
    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dmax")
    rc = wcsset_s(str1, 0, 0);
    ERR(ESZEROL)

    /*--------------------------------------------------*/

#if SIZEOF_WCHAR_T > 2
    EXPECT_BOS("value overflow >0x10ffff")
    rc = wcsset_s(str1, LEN, 0x110000);
    ERR(ESLEMAX);
#endif

    EXPECT_BOS("dest overflow")
    rc = wcsset_s(str1, RSIZE_MAX_WSTR + 1, 0);
    ERR(ESLEMAX);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = wcsset_s(str1, LEN + 1, 0);
    ERR(EOVERFLOW);
    WCHECK_SLACK(str1, LEN);
#endif

#endif

    /*--------------------------------------------------*/

    wcscpy(str1, L"abc");
    value = 20;
    max_len = 1;
    rc = wcsset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"abc");
    value = 0;
    max_len = 2;
    rc = wcsset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"abc");
    max_len = 3;
    rc = wcsset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"abc");
    max_len = 4;
    rc = wcsset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < 3; j++) {
        if (str1[j] != value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }
    WCHECK_SLACK(&str1[3], 1);

    /*--------------------------------------------------*/

    wcscpy(str1, L"abc");
    max_len = LEN;
    rc = wcsset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < 3; j++) {
        if (str1[j] != value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }
    WCHECK_SLACK(&str1[3], LEN - 3);

    /*--------------------------------------------------*/

    wcscpy(str1, L"Now is the time for all data to be zeroed");
    max_len = wcslen(L"Now is the ");

    rc = wcsset_s(str1, max_len, 0);
    ERR(EOK)
    if (wcscmp(&str1[max_len], L"time for all data to be zeroed")) {
        debug_printf("%s %u ERROR  --%ls-- \n", __FUNCTION__, __LINE__,
                     &str1[max_len]);
        errs++;
    }
    /*--------------------------------------------------*/

    return (errs);
}
