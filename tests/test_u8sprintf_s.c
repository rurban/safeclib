/*------------------------------------------------------------------
 * test_u8sprintf_s
 * File 'extu8/u8sprintf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
int test_u8sprintf_s(void);

int test_u8sprintf_s(void) {
    errno_t rc;
    int ind;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8sprintf_s(NULL, LEN, "%s", "x");
    NEGERR(ESNULLP)

    EXPECT_BOS("empty fmt")
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = u8sprintf_s(str1, LEN, NULL, 0);
    GCC_DIAG_RESTORE
    NEGERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8sprintf_s(str1, 0, "%s", "x");
    NEGERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    rc = u8sprintf_s(str1, LEN, "%s %n", "x", &ind);
    NEGERR(EINVAL)

    /*--------------------------------------------------*/

    /* multi-byte utf-8 payload round-trips like any other byte data */
    rc = u8sprintf_s(str1, LEN, "caf\xC3\xA9 %d", 42);
    ERR(8)
    EXPSTR((char *)str1, "caf\xC3\xA9 42")

    /*--------------------------------------------------*/

    strcpy((char *)str1, "aaaaaaaaaa");
    rc = u8sprintf_s(str1, 2, "%s", "keep it simple");
    ERR(-ESNOSPC);
    EXPNULL((char *)str1)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8sprintf_s()); }
