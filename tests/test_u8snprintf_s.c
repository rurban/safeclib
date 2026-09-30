/*------------------------------------------------------------------
 * test_u8snprintf_s
 * File 'extu8/u8snprintf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
int test_u8snprintf_s(void);

int test_u8snprintf_s(void) {
    errno_t rc;
    int ind;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8snprintf_s(NULL, LEN, "%s", "x");
    NEGERR(ESNULLP)

    EXPECT_BOS("empty fmt")
    rc = u8snprintf_s(str1, LEN, NULL);
    NEGERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8snprintf_s(str1, 0, "%s", "x");
    NEGERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    rc = u8snprintf_s(str1, LEN, "%s %n", "x", &ind);
    NEGERR(EINVAL)

    /*--------------------------------------------------*/

    /* multi-byte utf-8 payload round-trips like any other byte data */
    rc = u8snprintf_s(str1, LEN, "caf\xC3\xA9 %d", 42);
    ERR(8)
    EXPSTR((char *)str1, "caf\xC3\xA9 42")

    /*--------------------------------------------------*/

    /* truncating: unlike u8sprintf_s, no ESNOSPC, buffer is truncated */
    strcpy((char *)str1, "aaaaaaaaaa");
    rc = u8snprintf_s(str1, 5, "%s", "keep it simple");
    ERR(-ESNOSPC);
    EXPSTR((char *)str1, "")

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8snprintf_s()); }
