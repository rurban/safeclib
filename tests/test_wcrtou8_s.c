/*------------------------------------------------------------------
 * test_wcrtou8_s
 * File 'extu8/wcrtou8_s.c'
 * Lines executed:95.00% of 20
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_wcrtou8_s(void);

int test_wcrtou8_s(void) {
    errno_t rc;
    int errs = 0;
    size_t n;
    mbstate_t ps;
    char8_t dest[LEN];

    memset(&ps, 0, sizeof(ps));

    /*--------------------------------------------------*/
    /* ASCII */

    n = 0;
    rc = wcrtou8_s(&n, dest, LEN, L'A', &ps);
    ERR(EOK)
    if (n != 1) {
        debug_printf("%s %u  n=%zu, expected 1\n", __FUNCTION__, __LINE__, n);
        errs++;
    }
    EXPSTR((char *)dest, "A")

    /*--------------------------------------------------*/
    /* 3-byte codepoint: U+2713 CHECK MARK */

    n = 0;
    rc = wcrtou8_s(&n, dest, LEN, 0x2713, &ps);
    ERR(EOK)
    if (n != 3) {
        debug_printf("%s %u  n=%zu, expected 3\n", __FUNCTION__, __LINE__, n);
        errs++;
    }
    EXPSTR((char *)dest, "\xE2\x9C\x93")

    /*--------------------------------------------------*/
    /* 4-byte codepoint: U+1F600 GRINNING FACE */
    /* a literal non-BMP wchar_t constant truncates on platforms with a
       2-byte wchar_t (Windows/cygwin), so this needs a real wchar_t
       that can hold it */

#if SIZEOF_WCHAR_T > 2
    n = 0;
    rc = wcrtou8_s(&n, dest, LEN, 0x1F600, &ps);
    ERR(EOK)
    if (n != 4) {
        debug_printf("%s %u  n=%zu, expected 4\n", __FUNCTION__, __LINE__, n);
        errs++;
    }
    EXPSTR((char *)dest, "\xF0\x9F\x98\x80")
#endif

    /*--------------------------------------------------*/
    /* illegal: encoded surrogate half */

    n = 0;
    rc = wcrtou8_s(&n, dest, LEN, 0xD800, &ps);
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* dest too small */

    n = 0;
    rc = wcrtou8_s(&n, dest, 1, 0x2713, &ps);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/
    /* null ps */

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty ps")
    n = 0;
    rc = wcrtou8_s(&n, dest, LEN, L'A', NULL);
    ERR(ESNULLP)
#endif

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_wcrtou8_s()); }
