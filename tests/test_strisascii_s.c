/*------------------------------------------------------------------
 * test_strisascii_s
 * File 'extstr/strisascii_s.c'
 * Lines executed:100.00% of 8
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

int main(void) {
    bool rc;

    uint32_t len;
    char str[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

    len = 5;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strisascii_s(NULL, len);
    ERR(false)

    len = 0;
    EXPECT_BOS("empty dest or dmax")
    rc = strisascii_s("test", 0);
    ERR(false)

    EXPECT_BOS("dest overflow")
    rc = strisascii_s(str, RSIZE_MAX_STR + 1);
    ERR(false)

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strisascii_s(str, LEN + 1);
    ERR(false)

    /* literal strings once fixed via const decl. fragile */
    EXPECT_BOS_TODO("dest overflow")
    rc = strisascii_s("test", RSIZE_MAX_STR + 1);
    ERR(false)

    EXPECT_BOS_TODO("dest overflow")
    rc = strisascii_s("", 2);
    ERR(false)
#endif
#endif

    /*--------------------------------------------------*/

    strcpy(str, "ABCDEFGHIJK");

    rc = strisascii_s(str, 2);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    rc = strisascii_s(str, 1);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    rc = strisascii_s(str, 2);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "NowISTHETimE");
    len = strlen(str);

    rc = strisascii_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "qq21ego");

    rc = strisascii_s(str, 33);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "1234");
    str[2] = -124; /* special char: 132 */
    len = strlen(str);

    /* special char embedded */
    rc = strisascii_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    return (errs);
}
