/*------------------------------------------------------------------
 * test_strisuppercase_s
 * File 'extstr/strisuppercase_s.c'
 * Lines executed:100.00% of 10
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
    rc = strisuppercase_s(NULL, len);
    ERR(false)

    len = 0;
    EXPECT_BOS("empty dest or dmax")
    rc = strisuppercase_s("test", 0);
    ERR(false)

    EXPECT_BOS("dest overflow")
    rc = strisuppercase_s(str, 99999);
    ERR(false)

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow") /* but see test_strtouppercase_s.c */
    rc = strisuppercase_s("", 2);
    ERR(false)

    /*printf("%u BOS: %d %d/%d %d - %lu/%lu\n", __LINE__, _BOS_KNOWN("test"),
       SAFEC_CONSTP("test"), _BOS_KNOWN(str), SAFEC_CONSTP(str), BOS("test"),
       BOS(str));*/
    /* once fixed by adding prototypes and const: clang literal string bug */
    EXPECT_BOS("dest overflow")
    rc = strisuppercase_s("test", 99999);
    ERR(false)
#endif

#endif

    /*--------------------------------------------------*/

    str[0] = '\0';
    rc = strisuppercase_s(str, 5);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "ABCDEFGHIGHIJ");
    len = 7;

    rc = strisuppercase_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "ABCDEFGHIGHIJ");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "qqWe go");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "1234");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "!@#$%^&*()");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    return (errs);
}
