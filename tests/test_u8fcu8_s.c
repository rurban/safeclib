/*------------------------------------------------------------------
 * test_u8fcu8_s
 * File 'extu8/u8fcu8_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
static char8_t *sub;
int test_u8fcu8_s(void);

int test_u8fcu8_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8fcu8_s(NULL, LEN, str2, 5, &sub);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8fcu8_s(str1, LEN, NULL, 5, &sub);
    ERR(ESNULLP)

    EXPECT_BOS("empty substring")
    rc = u8fcu8_s(str1, LEN, str2, 5, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8fcu8_s(str1, 0, str2, 5, &sub);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "Hello World");
    strcpy((char *)str2, "world");

    rc = u8fcu8_s(str1, LEN, str2, 5, &sub);
    ERR(EOK)
    if (!sub || strcmp((char *)sub, "World") != 0) {
        debug_printf("%s %u  sub=%s, expected \"World\"\n", __FUNCTION__,
                     __LINE__, sub ? (char *)sub : "(null)");
        errs++;
    }

    /*--------------------------------------------------*/

    /* case-insensitive match spanning a multi-byte utf-8 codepoint:
       Latin capital E with acute (U+00C9, C3 89) folds to the same
       codepoint as lowercase e-acute (U+00E9, C3 A9) */
    strcpy((char *)str1, "caf\xC3\x89 today");
    strcpy((char *)str2, "caf\xC3\xA9");

    rc = u8fcu8_s(str1, LEN, str2, 5, &sub);
    ERR(EOK)
    if (!sub || sub != str1) {
        debug_printf("%s %u  sub mismatch\n", __FUNCTION__, __LINE__);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str1, "Hello World");
    strcpy((char *)str2, "xyz");

    rc = u8fcu8_s(str1, LEN, str2, 3, &sub);
    ERR(ESNOTFND)
    if (sub != NULL) {
        debug_printf("%s %u  expected NULL substring\n", __FUNCTION__,
                     __LINE__);
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8fcu8_s()); }
