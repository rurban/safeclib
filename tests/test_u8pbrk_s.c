/*------------------------------------------------------------------
 * test_u8pbrk_s
 * File 'extu8/u8pbrk_s.c'
 * Lines executed:100.00% of 32
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

    /*--------------------------------------------------*/
    /* set string longer than slen: not found */
    strcpy((char *)str, "hello world");
    rc = u8pbrk_s(str, LEN, (char8_t *)"xyz", 2, &sub);
    ERR(ESNOTFND)
    SUBNULL()

#if defined(HAVE___BUILTIN_OBJECT_SIZE) && !defined(HAVE_CT_BOS_OVR)
    EXPECT_BOS("slen exceeds src")
    rc = u8pbrk_s(str, LEN, (char8_t *)"ow", 10, &sub);
    ERR(EOVERFLOW)
    SUBNULL()
#endif

    /*--------------------------------------------------*/
    /* dmax/slen > RSIZE_MAX_STR is only checked for objects of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        if (!hp)
            return errs + 1;
        strcpy((char *)hp, "ow");
        strcpy((char *)str, "hello world"); /* cleared by the overflow above */
        rc = u8pbrk_s(hp, RSIZE_MAX_STR + 1, (char8_t *)"ow", 3, &sub);
        ERR(ESLEMAX)
        SUBNULL()
        rc = u8pbrk_s(str, LEN, hp, RSIZE_MAX_STR + 1, &sub);
        ERR(ESLEMAX)
        SUBNULL()
        rc = u8pbrk_s(str, LEN, hp, 3, &sub);
        ERR(EOK)
        PTREQ(sub, &str[4])
        free(hp);
    }

    return (errs);
}

int main(void) { return (test_u8pbrk_s()); }
