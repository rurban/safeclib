/*------------------------------------------------------------------
 * test_u8nset_s
 * File 'extu8/u8nset_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_u8nset_s(void);

int test_u8nset_s(void) {
    errno_t rc;
    char8_t str1[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8nset_s(NULL, LEN, 'x', 3);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8nset_s(str1, 0, 'x', 3);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8nset_s(str1, RSIZE_MAX_STR + 1, 'x', 3);
    ERR(ESLEMAX)
#endif

    strcpy((char *)str1, "abc");
#ifndef HAVE_CT_BOS_OVR
    rc = u8nset_s(str1, LEN, 256, 3);
    ERR(ESLEMAX)
#endif

    strcpy((char *)str1, "abc");
#ifndef HAVE_CT_BOS_OVR
    rc = u8nset_s(str1, 5, 'x', 6);
    ERR(ESNOSPC)
#endif

    /*--------------------------------------------------*/

    /* n(3) < strlen("hello")==5: u8nset_s only touches the first n bytes;
     * anything beyond is untouched, live remaining string content, not
     * null-slack (that only applies once the terminator is reached). */
    strcpy((char *)str1, "hello");
    rc = u8nset_s(str1, LEN, 'x', 3);
    ERR(EOK)
    EXPSTR((char *)str1, "xxxlo")

    /*--------------------------------------------------*/

    /* n(10) > strlen("hi")==2: the scan reaches the terminator, so the
     * rest of dmax is null-slack and must be cleared. */
    strcpy((char *)str1, "hi");
    rc = u8nset_s(str1, LEN, 'z', 10);
    ERR(EOK)
    EXPSTR((char *)str1, "zz")
    CHECK_SLACK(&str1[2], LEN - 2);

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8nset_s()); }
