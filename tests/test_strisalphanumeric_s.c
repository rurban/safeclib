/*------------------------------------------------------------------
 * test_strisalphanumeric_s.c
 * File 'extstr/strisalphanumeric_s.c'
 * Lines executed:91.67% of 12
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
    rc = strisalphanumeric_s(NULL, len);
    ERR(false)

    /* empty string */
    *str = 0;
    EXPECT_BOS("dest overflow")
    rc = strisalphanumeric_s(str, RSIZE_MAX_STR + 1);
    ERR(false)

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strisalphanumeric_s(str, LEN + 1);
    ERR(false)

    EXPECT_BOS_TODO("dest overflow")
    rc = strisalphanumeric_s("", 2);
    ERR(false)
#endif

#endif
    /*--------------------------------------------------*/

    len = 0;
    rc = strisalphanumeric_s("test", len);
    ERR(false)
    /*--------------------------------------------------*/

    /* exceeds max */
    len = RSIZE_MAX_STR + 1;
    rc = strisalphanumeric_s("test", len);
    ERR(false)

    /*--------------------------------------------------*/

    strcpy(str, "123456789");
    len = 4;

    rc = strisalphanumeric_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "N");

    rc = strisalphanumeric_s(str, 1);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    rc = strisalphanumeric_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "NowISTHETimE4us");
    len = strlen(str);

    rc = strisalphanumeric_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "qq21ego");
    len = strlen(str);

    rc = strisalphanumeric_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "1234");
    str[2] = -124; /* special char: 132 */
    len = strlen(str);

    /* special char embedded */
    rc = strisalphanumeric_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "!@#$%^&*(){}[]:;\"'?/.>,<");
    len = strlen(str);

    rc = strisalphanumeric_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    return (errs);
}
