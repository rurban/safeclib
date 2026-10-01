/*------------------------------------------------------------------
 * test_wctou8_s
 * File 'extu8/wctou8_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_wctou8_s(void);

int test_wctou8_s(void) {
    errno_t rc;
    int errs = 0;
    int n;
    char8_t dest[LEN];

    /*--------------------------------------------------*/
    /* ASCII */

    n = 0;
    rc = wctou8_s(&n, dest, LEN, L'A');
    ERR(EOK)
    if (n != 1) {
        debug_printf("%s %u  n=%d, expected 1\n", __FUNCTION__, __LINE__, n);
        errs++;
    }
    EXPSTR((char *)dest, "A")

    /*--------------------------------------------------*/
    /* 3-byte codepoint: U+2713 CHECK MARK */

    n = 0;
    rc = wctou8_s(&n, dest, LEN, 0x2713);
    ERR(EOK)
    if (n != 3) {
        debug_printf("%s %u  n=%d, expected 3\n", __FUNCTION__, __LINE__, n);
        errs++;
    }
    EXPSTR((char *)dest, "\xE2\x9C\x93")

    /*--------------------------------------------------*/
    /* illegal: codepoint beyond U+10FFFF */
    /* a literal >0xffff wchar_t constant truncates on platforms with a
       2-byte wchar_t (Windows/cygwin), so this check needs a real
       wchar_t wide enough to hold it */

#if !defined(HAVE_CT_BOS_OVR) && SIZEOF_WCHAR_T > 2
    n = 0;
    rc = wctou8_s(&n, dest, LEN, 0x110000);
    ERR(EILSEQ)
#endif

    /*--------------------------------------------------*/
    /* dest too small */

    n = 0;
    rc = wctou8_s(&n, dest, 1, 0x2713);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/
    /* null retvalp */

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty retvalp")
    rc = wctou8_s(NULL, dest, LEN, L'A');
    ERR(ESNULLP)
#endif

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_wctou8_s()); }
