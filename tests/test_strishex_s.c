/*------------------------------------------------------------------
 * test_strishex_s
 * File 'extstr/strishex_s.c'
 * Lines executed:100.00% of 12
 *
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
    rc = strishex_s(NULL, len);
    ERR(false)
#endif
    /*--------------------------------------------------*/

    str[0] = '\0';
    rc = strishex_s(str, 5);
    ERR(false)
    /*--------------------------------------------------*/

    len = 0;
    rc = strishex_s("1234", len);
    ERR(false)
    /*--------------------------------------------------*/

    len = RSIZE_MAX_STR + 1;
    rc = strishex_s("1234", len);
    ERR(false)
    /*--------------------------------------------------*/

    len = 9;
    rc = strishex_s("", len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "123456789");
    len = 6;

    rc = strishex_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "1");
    len = strlen(str);

    rc = strishex_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "12");
    len = strlen(str);

    rc = strishex_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "1Af");
    len = strlen(str);

    rc = strishex_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "FF");
    len = strlen(str);

    rc = strishex_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "1abzd");
    len = strlen(str);

    /* non hex char in string */
    rc = strishex_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    return (errs);
}
