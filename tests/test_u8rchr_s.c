/*------------------------------------------------------------------
 * test_u8rchr_s
 * File 'extu8/u8rchr_s.c'
 * Lines executed:89.47% of 19
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str[LEN];
int test_u8rchr_s(void);

int test_u8rchr_s(void) {
    errno_t rc;
    char8_t *sub;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8rchr_s(NULL, LEN, 0, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty resultp")
    rc = u8rchr_s(str, LEN, 0, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8rchr_s(str, 0, 0, &sub);
    ERR(ESZEROL)
    SUBNULL()

    EXPECT_BOS("dest overflow")
    rc = u8rchr_s(str, RSIZE_MAX_STR + 1, 0, &sub);
    ERR(ESLEMAX)
    SUBNULL()

    EXPECT_BOS("ch overflow >255")
    rc = u8rchr_s(str, LEN, 256, &sub);
    ERR(ESLEMAX)
    SUBNULL()
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str, "h\xC3\xA9llo world");

    /* last occurrence of a repeated byte */
    rc = u8rchr_s(str, LEN, 'o', &sub);
    ERR(EOK)
    PTREQ(sub, &str[8])

    /* not found */
    rc = u8rchr_s(str, LEN, 'z', &sub);
    ERR(ESNOTFND)
    SUBNULL()

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* dmax > RSIZE_MAX_STR is only checked for a dest of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        if (!hp)
            return errs + 1;
        rc = u8rchr_s(hp, RSIZE_MAX_STR + 1, 'x', &sub);
        ERR(ESLEMAX)
        SUBNULL()
        free(hp);
    }
    {
        /* empty string */
        char8_t e[4] = {0, 0, 0, 0};
        rc = u8rchr_s(e, sizeof(e), 'a', &sub);
        ERR(ESZEROL)
        SUBNULL()
    }

    return (errs);
}

int main(void) { return (test_u8rchr_s()); }
