/*------------------------------------------------------------------
 * test_u8coll_s
 * File 'extu8/u8coll_s.c'
 * Lines executed:90.91% of 11
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8coll_s(void);

int test_u8coll_s(void) {
    errno_t rc;
    int result;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8coll_s(NULL, LEN, str2, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8coll_s(str1, LEN, NULL, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty resultp")
    rc = u8coll_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8coll_s(str1, 0, str2, &result);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "hello");
    strcpy((char *)str2, "hello");

    rc = u8coll_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    strcpy((char *)str2, "world");

    rc = u8coll_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* multibyte utf-8 content round-trips through locale collation */
    strcpy((char *)str1, "caf\xC3\xA9");
    strcpy((char *)str2, "caf\xC3\xA9");

    rc = u8coll_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8coll_s()); }
