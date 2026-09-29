/*------------------------------------------------------------------
 * test_u8u8_s
 * File 'extu8/u8u8_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8u8_s(void);

int test_u8u8_s(void) {
    errno_t rc;
    char8_t *sub;
    int errs = 0;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8u8_s(NULL, LEN, str2, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty src")
    rc = u8u8_s(str1, LEN, NULL, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty substringp")
    rc = u8u8_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)
#endif

    strcpy((char *)str1, "caf\xC3\xA9 world");
    strcpy((char *)str2, "world");

    /* current u8u8_s implementation is literal u8str_s search */
    rc = u8u8_s(str1, LEN, str2, sizeof("world"), &sub);
    ERR(EOK)
    PTREQ(sub, &str1[6])

    strcpy((char *)str2, "xyz");
    rc = u8u8_s(str1, LEN, str2, sizeof("xyz"), &sub);
    ERR(ESNOTFND)
    SUBNULL()

    return (errs);
}

int main(void) { return (test_u8u8_s()); }
