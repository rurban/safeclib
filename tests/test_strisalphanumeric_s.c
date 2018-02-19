/*------------------------------------------------------------------
 * test_strisalphanumeric_s.c
 * File 'extstr/strisalphanumeric_s.c'
 * Lines executed:100.00% of 20
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
    bool rc;

    uint32_t len;
    char   str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    len = 5;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strisalphanumeric_s(NULL, len);
    ERR(false)
#endif
/*--------------------------------------------------*/

    len = 0;
    rc = strisalphanumeric_s("test", len);
    ERR(false)
/*--------------------------------------------------*/

    /* exceeds max */
    len = 99999;
    rc = strisalphanumeric_s("test", len);
    ERR(false)
/*--------------------------------------------------*/

    /* empty string */
    rc = strisalphanumeric_s("", LEN);
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
    str[2] = -124;          /* special char: 132 */
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
