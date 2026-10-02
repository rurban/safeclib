/*------------------------------------------------------------------
 * test_u8str_s
 * File 'extu8/u8str_s.c'
 * Lines executed:68.09% of 47
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8str_s(void);

int test_u8str_s(void) {
    errno_t rc;
    char8_t *sub;
    int errs = 0;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8str_s(NULL, LEN, str2, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty src")
    rc = u8str_s(str1, LEN, NULL, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty substringp")
    rc = u8str_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8str_s(str1, 0, str2, LEN, &sub);
    ERR(ESZEROL)
    SUBNULL()
#endif

    strcpy((char *)str1, "caf\xC3\xA9 world");
    strcpy((char *)str2, "\xC3\xA9");

    /* multibyte sequence at codepoint boundary */
    rc = u8str_s(str1, LEN, str2, sizeof("\xC3\xA9"), &sub);
    ERR(EOK)
    PTREQ(sub, &str1[3])

    /* substring in the middle */
    strcpy((char *)str2, "world");
    rc = u8str_s(str1, LEN, str2, sizeof("world"), &sub);
    ERR(EOK)
    PTREQ(sub, &str1[6])

    /* empty source and source equal to dest return dest */
    *str2 = '\0';
    rc = u8str_s(str1, LEN, str2, 0, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    rc = u8str_s(str1, LEN, str1, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    strcpy((char *)str2, "xyz");
    rc = u8str_s(str1, LEN, str2, sizeof("xyz"), &sub);
    ERR(ESNOTFND)
    SUBNULL()

    /* a short bounded destination cannot contain the substring */
    strcpy((char *)str2, "world");
    rc = u8str_s(str1, 6, str2, sizeof("world"), &sub);
    ERR(ESNOTFND)
    SUBNULL()

    return (errs);
}

int main(void) { return (test_u8str_s()); }
