/*------------------------------------------------------------------
 * test_u8towcs_s
 * File 'extu8/u8towcs_s.c'
 * Lines executed:82.05% of 39
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#define HAVE_NATIVE 0
#include "test_msvcrt.h"

#define LEN (128)

#ifdef HAVE_WCHAR_H
int test_u8towcs_s(void);

int test_u8towcs_s(void) {
    errno_t rc;
    int errs = 0;
    size_t n;
    wchar_t wdest[LEN];

    /*--------------------------------------------------*/
    /* ASCII-only */

    n = 0;
    rc = u8towcs_s(&n, wdest, LEN, (const char8_t *)"hello", LEN);
    ERR(EOK)
    if (n != 5) {
        debug_printf("%s %u  n=%zu, expected 5\n", __FUNCTION__, __LINE__, n);
        errs++;
    }
    WEXPSTR(wdest, L"hello")

    /*--------------------------------------------------*/
    /* multi-byte codepoints: e-acute (2 bytes), check mark (3 bytes) */

    n = 0;
    rc = u8towcs_s(&n, wdest, LEN, (const char8_t *)"\xC3\xA9\xE2\x9C\x93",
                   LEN);
    ERR(EOK)
    if (n != 2) {
        debug_printf("%s %u  n=%zu, expected 2\n", __FUNCTION__, __LINE__, n);
        errs++;
    }
    if (wdest[0] != 0xE9) {
        debug_printf("%s %u  wdest[0]=U+%04X, expected U+00E9\n",
                     __FUNCTION__, __LINE__, (unsigned)wdest[0]);
        errs++;
    }
    if (wdest[1] != 0x2713) {
        debug_printf("%s %u  wdest[1]=U+%04X, expected U+2713\n",
                     __FUNCTION__, __LINE__, (unsigned)wdest[1]);
        errs++;
    }

    /*--------------------------------------------------*/
    /* illegal UTF-8 (overlong encoding of NUL) */

    n = 0;
    rc = u8towcs_s(&n, wdest, LEN, (const char8_t *)"\xC0\x80", LEN);
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* null src */

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty src")
    n = 0;
    rc = u8towcs_s(&n, wdest, LEN, NULL, LEN);
    ERR(ESNULLP)
#endif

    /*--------------------------------------------------*/
    /* dest too small (dmax leaves no room) */

    n = 0;
    rc = u8towcs_s(&n, wdest, 2, (const char8_t *)"abc", LEN);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/

    return (errs);
}
#endif

#ifndef __KERNEL__
int main(void) {
#ifdef HAVE_WCHAR_H
    return (test_u8towcs_s());
#else
    return 0;
#endif
}
#endif
