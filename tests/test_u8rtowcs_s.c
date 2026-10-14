/*------------------------------------------------------------------
 * test_u8rtowcs_s
 * File 'extu8/u8rtowcs_s.c'
 * Lines executed:85.71% of 42
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static volatile size_t vlen = LEN;
static volatile size_t vrmw = RSIZE_MAX_WSTR + 1;

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
    EXPECT_BOS("empty ps") {
        const char8_t *p = (const char8_t *)"hi";
        n = 0;
        rc = u8rtowcs_s(&n, wdest, LEN, &p, LEN, NULL);
        ERR(ESNULLP)
    }
#endif

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* no room in dest: nothing consumed */

    memset(&ps, 0, sizeof(ps));
    {
        const char8_t *p = (const char8_t *)"abc";
        const char8_t *p0 = p;
        wchar_t one[1];

        n = 99;
        rc = u8rtowcs_s(&n, one, 1, &p, vlen, &ps);
        ERR(EOK)
        if (n != 0 || p != p0 || one[0] != L'\0') {
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
        const char8_t *p = (const char8_t *)(void *)ov;
        n = 0;
        rc = u8rtowcs_s(&n, ov, 8, &p, vlen, &ps);
        ERR(ESOVRLP)
    }

    /*--------------------------------------------------*/
    /* dmax/len > RSIZE_MAX_WSTR is only checked for a dest of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 8 * sizeof(wchar_t);
        wchar_t *hp = (wchar_t *)malloc(hsz);
        const char8_t *p = (const char8_t *)"abc";
        if (!hp)
            return errs + 1;
        memset(&ps, 0, sizeof(ps));
        n = 0;
        rc = u8rtowcs_s(&n, hp, RSIZE_MAX_WSTR + 1, &p, 3, &ps);
        ERR(ESLEMAX)
        rc = u8rtowcs_s(&n, hp, 8, &p, vrmw, &ps);
        ERR(ESLEMAX)
        rc = u8rtowcs_s(&n, hp, 8, &p, 3, &ps);
        ERR(EOK)
        if (n != 3 || hp[0] != L'a' || hp[2] != L'c' || hp[3] != L'\0') {
            debug_printf("%s %u  wrong result\n", __FUNCTION__, __LINE__);
            errs++;
        }
        free(hp);
    }

    return (errs);
}

int main(void) { return (test_u8rtowcs_s()); }
