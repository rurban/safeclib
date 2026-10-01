/*------------------------------------------------------------------
 * test_wcsrtou8_s
 * File 'extu8/wcsrtou8_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_wcsrtou8_s(void);

int test_wcsrtou8_s(void) {
    errno_t rc;
    int errs = 0;
    size_t n;
    mbstate_t ps;
    char8_t dest[LEN];

    /*--------------------------------------------------*/
    /* one-shot conversion */

    memset(&ps, 0, sizeof(ps));
    {
        const wchar_t *p = L"hi\x00E9";
        n = 0;
        rc = wcsrtou8_s(&n, dest, LEN, &p, LEN, &ps);
        ERR(EOK)
        if (n != 4) {
            debug_printf("%s %u  n=%zu, expected 4\n", __FUNCTION__, __LINE__,
                         n);
            errs++;
        }
        EXPSTR((char *)dest, "hi\xC3\xA9")
        WEXPNULL(p)
    }

    /*--------------------------------------------------*/
    /* restartable: convert one wide char at a time */

    memset(&ps, 0, sizeof(ps));
    {
        const wchar_t *p = L"ab";
        char8_t chunk[4];

        n = 0;
        rc = wcsrtou8_s(&n, chunk, 4, &p, 1, &ps);
        ERR(EOK)
        EXPSTR((char *)chunk, "a")

        n = 0;
        rc = wcsrtou8_s(&n, chunk, 4, &p, 1, &ps);
        ERR(EOK)
        EXPSTR((char *)chunk, "b")
        WEXPNULL(p)
    }

    /*--------------------------------------------------*/
    /* illegal wide character: encoded surrogate half */

    memset(&ps, 0, sizeof(ps));
    {
        wchar_t wsrc[2];
        const wchar_t *p;
        wsrc[0] = 0xD800;
        wsrc[1] = 0;
        p = wsrc;
        n = 0;
        rc = wcsrtou8_s(&n, dest, LEN, &p, LEN, &ps);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* null ps */

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty ps")
    {
        const wchar_t *p = L"hi";
        n = 0;
        rc = wcsrtou8_s(&n, dest, LEN, &p, LEN, NULL);
        ERR(ESNULLP)
    }
#endif

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_wcsrtou8_s()); }
