/*------------------------------------------------------------------
 * test_u8pbrk_s
 * File 'extu8/u8pbrk_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str[LEN];
int test_u8pbrk_s(void);

int test_u8pbrk_s(void) {
    errno_t rc;
    char8_t *sub;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8pbrk_s(NULL, LEN, (char8_t *)"ow", 3, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty src")
    rc = u8pbrk_s(str, LEN, NULL, 3, &sub);
    ERR(ESNULLP)

    EXPECT_BOS("empty firstp")
    rc = u8pbrk_s(str, LEN, (char8_t *)"ow", 3, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8pbrk_s(str, 0, (char8_t *)"ow", 3, &sub);
    ERR(ESZEROL)
    SUBNULL()
#endif

    strcpy((char *)str, "hello world");
    rc = u8pbrk_s(str, LEN, (char8_t *)"ow", 0, &sub);
    ERR(ESZEROL)
    SUBNULL()

    /*--------------------------------------------------*/

    rc = u8pbrk_s(str, LEN, (char8_t *)"ow", 3, &sub);
    ERR(EOK)
    PTREQ(sub, &str[4])

    rc = u8pbrk_s(str, LEN, (char8_t *)"xyz", 4, &sub);
    ERR(ESNOTFND)
    SUBNULL()

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8pbrk_s()); }
