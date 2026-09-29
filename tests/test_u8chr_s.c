/*------------------------------------------------------------------
 * test_u8chr_s
 * File 'extu8/u8chr_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str[LEN];
int test_u8chr_s(void);

int test_u8chr_s(void) {
    errno_t rc;
    char8_t *sub;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8chr_s(NULL, LEN, 0, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty resultp")
    rc = u8chr_s(str, LEN, 0, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8chr_s(str, 0, 0, &sub);
    ERR(ESZEROL)
    SUBNULL()

    EXPECT_BOS("dest overflow")
    rc = u8chr_s(str, RSIZE_MAX_STR + 1, 0, &sub);
    ERR(ESLEMAX)
    SUBNULL()

    EXPECT_BOS("ch overflow >255")
    rc = u8chr_s(str, LEN, 256, &sub);
    ERR(ESLEMAX)
    SUBNULL()
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str, "h\xC3\xA9llo world");

    /* at beginning */
    rc = u8chr_s(str, LEN, 'h', &sub);
    ERR(EOK)
    PTREQ(sub, &str[0])

    /* in the middle */
    rc = u8chr_s(str, LEN, 'o', &sub);
    ERR(EOK)
    PTREQ(sub, &str[5])

    /* not found */
    rc = u8chr_s(str, LEN, 'z', &sub);
    ERR(ESNOTFND)
    SUBNULL()

    /* the terminating NUL is a valid search target */
    rc = u8chr_s(str, LEN, 0, &sub);
    ERR(EOK)
    PTREQ(sub, &str[strlen((char *)str)])

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8chr_s()); }
