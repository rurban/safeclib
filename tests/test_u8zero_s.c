/*------------------------------------------------------------------
 * test_u8zero_s
 * File 'extu8/u8zero_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_u8zero_s(void);

int test_u8zero_s(void) {
    errno_t rc;
    rsize_t max_len;
    char8_t str1[LEN];
    uint32_t j;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8zero_s(NULL, 5);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8zero_s(str1, 0);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8zero_s(str1, RSIZE_MAX_STR + 1);
    ERR(ESLEMAX)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "abc");
    max_len = 1;
    rc = u8zero_s(str1, max_len);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != '\0') {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy((char *)str1, "h\xC3\xA9llo");
    rc = u8zero_s(str1, LEN);
    ERR(EOK)
    if (str1[0] != '\0') {
        debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
        errs++;
    }
    CHECK_SLACK(str1, LEN);

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8zero_s()); }
