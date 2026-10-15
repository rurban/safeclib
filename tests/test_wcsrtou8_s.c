/*------------------------------------------------------------------
 * test_wcsrtou8_s
 * File 'extu8/wcsrtou8_s.c'
 * Lines executed:93.18% of 44
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static volatile size_t vlen = LEN;

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
    EXPECT_BOS("empty ps") {
        const wchar_t *p = L"hi";
        n = 0;
        rc = wcsrtou8_s(&n, dest, LEN, &p, LEN, NULL);
        ERR(ESNULLP)
    }
#endif

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* no room in dest: nothing consumed */

    memset(&ps, 0, sizeof(ps));
    {
        const wchar_t *p = L"abc";
        const wchar_t *p0 = p;
        char8_t one[1];

        n = 99;
        rc = wcsrtou8_s(&n, one, 1, &p, vlen, &ps);
        ERR(EOK)
        if (n != 0 || p != p0 || one[0] != '\0') {
            debug_printf("%s %u  n=%zu, p advanced\n", __FUNCTION__, __LINE__,
                         n);
            errs++;
        }
    }

    /*--------------------------------------------------*/
    /* dest overlaps src */

    memset(&ps, 0, sizeof(ps));
    {
        wchar_t ov[8];
        const wchar_t *p = ov;
        n = 0;
        rc = wcsrtou8_s(&n, (char8_t *)(void *)ov, sizeof(ov), &p, vlen, &ps);
        ERR(ESOVRLP)
    }

    /*--------------------------------------------------*/
    /* dmax > RSIZE_MAX_STR is only checked for a dest of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        const wchar_t *p = L"abc";
        if (!hp)
            return errs + 1;
        memset(&ps, 0, sizeof(ps));
        n = 0;
        rc = wcsrtou8_s(&n, hp, RSIZE_MAX_STR + 1, &p, 3, &ps);
        ERR(ESLEMAX)
        rc = wcsrtou8_s(&n, hp, 16, &p, 3, &ps);
        ERR(EOK)
        if (n != 3 || strcmp((char *)hp, "abc") != 0) {
            debug_printf("%s %u  wrong result\n", __FUNCTION__, __LINE__);
            errs++;
        }
        free(hp);
    }

    return (errs);
}

int main(void) { return (test_wcsrtou8_s()); }
