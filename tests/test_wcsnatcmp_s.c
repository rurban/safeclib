/*------------------------------------------------------------------
 * test_wcsnatcmp_s
 * File 'wcsnatcmp_s.c'
 * Lines executed:42.31% of 104
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

static wchar_t str1[LEN];
static wchar_t str2[LEN];
int test_wcsnatcmp_s(void);

int test_wcsnatcmp_s(void) {
    errno_t rc;
    int ind;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = wcsnatcmp_s(NULL, LEN, str2, LEN, &ind);
    ERR(ESNULLP)
    INDZERO()

    EXPECT_BOS("empty src")
    rc = wcsnatcmp_s(str1, LEN, NULL, LEN, &ind);
    ERR(ESNULLP)
    INDZERO()

    EXPECT_BOS("empty resultp")
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = wcsnatcmp_s(str1, 0, str2, LEN, &ind);
    ERR(ESZEROL)
    INDZERO()

    EXPECT_BOS("empty src or smax")
    rc = wcsnatcmp_s(str1, LEN, str2, 0, &ind);
    ERR(ESZEROL)
    INDZERO()

    EXPECT_BOS("dest overflow")
    rc = wcsnatcmp_s(str1, RSIZE_MAX_STR + 1, str2, LEN, &ind);
    ERR(ESLEMAX)
    INDZERO()

    EXPECT_BOS("src overflow")
    rc = wcsnatcmp_s(str1, LEN, str2, RSIZE_MAX_STR + 1, &ind);
    ERR(ESLEMAX)
    INDZERO()
#endif

    /*--------------------------------------------------*/

    str1[0] = L'\0';
    str2[0] = L'\0';

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    wcscpy(str2, L"keep it simple");

    rc = wcsnatcmp_s(str1, 5, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    wcscpy(str1, L"Keep it simple");
    wcscpy(str2, L"keep it simple");

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    wcscpy(str2, L"keeP it simple");

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");

    rc = wcsnatcmp_s(str1, LEN, str1, LEN, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simplified");
    wcscpy(str2, L"keep it simple");

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(<= 0)

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    wcscpy(str2, L"keep it simplified");

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(>= 0)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_wcsnatcmp_s()); }
