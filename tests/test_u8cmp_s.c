/*------------------------------------------------------------------
 * test_u8cmp_s
 * File 'extu8/u8cmp_s.c'
 * Lines executed:69.81% of 53
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8cmp_s(void);

int test_u8cmp_s(void) {
    errno_t rc;
    int result;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8cmp_s(NULL, LEN, str2, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8cmp_s(str1, LEN, NULL, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty resultp")
    rc = u8cmp_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8cmp_s(str1, 0, str2, &result);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8cmp_s(str1, RSIZE_MAX_STR + 1, str2, &result);
    ERR(ESLEMAX)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "hello");
    strcpy((char *)str2, "hello");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str1, "hello");
    strcpy((char *)str2, "help");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str1, "help");
    strcpy((char *)str2, "hello");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* multibyte sequences compare bytewise when not canonically equal */
    strcpy((char *)str1, "h\xC3\xA9llo");
    strcpy((char *)str2, "hello");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    /* canonical equivalence: precomposed e-acute (U+00E9 = C3 A9) must
       compare equal to the decomposed form (e U+0065 + combining
       acute U+0301 = 65 CC 81), since u8cmp_s normalizes to NFC first */
    strcpy((char *)str1, "caf\xC3\xA9");
    strcpy((char *)str2, "cafe\xCC\x81");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0 (canonically equal)\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8cmp_s()); }
