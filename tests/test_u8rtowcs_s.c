/*------------------------------------------------------------------
 * test_u8rtowcs_s
 * File 'extu8/u8rtowcs_s.c'
 * Lines executed:85.71% of 42
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_u8rtowcs_s(void);

int test_u8rtowcs_s(void) {
    errno_t rc;
    int errs = 0;
    size_t n;
    mbstate_t ps;
    wchar_t wdest[LEN];

    /*--------------------------------------------------*/
    /* one-shot conversion */

    memset(&ps, 0, sizeof(ps));
    {
        const char8_t *p = (const char8_t *)"hi\xC3\xA9";
        n = 0;
        rc = u8rtowcs_s(&n, wdest, LEN, &p, LEN, &ps);
        ERR(EOK)
        if (n != 3) {
            debug_printf("%s %u  n=%zu, expected 3\n", __FUNCTION__, __LINE__,
                         n);
            errs++;
        }
        WEXPSTR(wdest, L"hi\x00E9")
        EXPNULL(p)
    }

    /*--------------------------------------------------*/
    /* restartable: convert in two chunks of 2 chars each */

    memset(&ps, 0, sizeof(ps));
    {
        const char8_t *p = (const char8_t *)"abcdef";
        wchar_t chunk[3];

        n = 0;
        rc = u8rtowcs_s(&n, chunk, 3, &p, 2, &ps);
        ERR(EOK)
        if (n != 2 || chunk[0] != L'a' || chunk[1] != L'b') {
            debug_printf("%s %u  chunk1 wrong\n", __FUNCTION__, __LINE__);
            errs++;
        }

        n = 0;
        rc = u8rtowcs_s(&n, chunk, 3, &p, 2, &ps);
        ERR(EOK)
        if (n != 2 || chunk[0] != L'c' || chunk[1] != L'd') {
            debug_printf("%s %u  chunk2 wrong\n", __FUNCTION__, __LINE__);
            errs++;
        }

        n = 0;
        rc = u8rtowcs_s(&n, chunk, 3, &p, 2, &ps);
        ERR(EOK)
        if (n != 2 || chunk[0] != L'e' || chunk[1] != L'f') {
            debug_printf("%s %u  chunk3 wrong\n", __FUNCTION__, __LINE__);
            errs++;
        }
        EXPNULL(p)
    }

    /*--------------------------------------------------*/
    /* illegal UTF-8 */

    memset(&ps, 0, sizeof(ps));
    {
        const char8_t *p = (const char8_t *)"\xC0\x80";
        n = 0;
        rc = u8rtowcs_s(&n, wdest, LEN, &p, LEN, &ps);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* null ps */

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty ps")
    {
        const char8_t *p = (const char8_t *)"hi";
        n = 0;
        rc = u8rtowcs_s(&n, wdest, LEN, &p, LEN, NULL);
        ERR(ESNULLP)
    }
#endif

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8rtowcs_s()); }
