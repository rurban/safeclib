/*------------------------------------------------------------------
 * test_u8set_s
 * File 'extu8/u8set_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_u8set_s(void);

int test_u8set_s(void) {
    errno_t rc;
    char8_t str1[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8set_s(NULL, LEN, 'x');
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8set_s(str1, 0, 'x');
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8set_s(str1, RSIZE_MAX_STR + 1, 'x');
    ERR(ESLEMAX)
#endif

    strcpy((char *)str1, "abc");
#ifndef HAVE_CT_BOS_OVR
    rc = u8set_s(str1, LEN, 256);
    ERR(ESLEMAX)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "hello");
    rc = u8set_s(str1, LEN, 'x');
    ERR(EOK)
    EXPSTR((char *)str1, "xxxxx")
    CHECK_SLACK(&str1[5], LEN - 5);

    /*--------------------------------------------------*/

    /* only up to the terminating NUL, not past it */
    strcpy((char *)str1, "hi");
    rc = u8set_s(str1, LEN, 'z');
    ERR(EOK)
    EXPSTR((char *)str1, "zz")

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8set_s()); }
