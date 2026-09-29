/*------------------------------------------------------------------
 * test_wcstou8_s
 * File 'extu8/wcstou8_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_wcstou8_s(void);

int test_wcstou8_s(void) {
    errno_t rc;
    int errs = 0;
    size_t n;
    char8_t dest[LEN];

    /*--------------------------------------------------*/
    /* ASCII-only */

    n = 0;
    rc = wcstou8_s(&n, dest, LEN, L"hello", LEN);
    ERR(EOK)
    if (n != 5) {
        debug_printf("%s %u  n=%zu, expected 5\n", __FUNCTION__, __LINE__, n);
        errs++;
    }
    EXPSTR((char *)dest, "hello")

    /*--------------------------------------------------*/
    /* multi-byte codepoints: e-acute (U+00E9), check mark (U+2713) */

    {
        wchar_t wsrc[3] = {0xE9, 0x2713, 0};
        n = 0;
        rc = wcstou8_s(&n, dest, LEN, wsrc, LEN);
        ERR(EOK)
        EXPSTR((char *)dest, "\xC3\xA9\xE2\x9C\x93")
    }

    /*--------------------------------------------------*/
    /* illegal wide character: encoded surrogate half */

    {
        wchar_t wsrc[2] = {0xD800, 0};
        n = 0;
        rc = wcstou8_s(&n, dest, LEN, wsrc, LEN);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* null src */

    EXPECT_BOS("empty src")
    n = 0;
    rc = wcstou8_s(&n, dest, LEN, NULL, LEN);
    ERR(ESNULLP)

    /*--------------------------------------------------*/
    /* dest too small */

    n = 0;
    rc = wcstou8_s(&n, dest, 2, L"abc", LEN);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_wcstou8_s()); }
