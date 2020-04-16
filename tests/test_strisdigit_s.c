/*------------------------------------------------------------------
 * test_strisdigit_s
 * File 'extstr/strisdigit_s.c'
 * Lines executed:90.00% of 10
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
    rc = strisdigit_s(NULL, len);
    ERR(false)
#endif
    /*--------------------------------------------------*/

    len = 0;
    rc = strisdigit_s("1234", len);
    ERR(false)
    /*--------------------------------------------------*/

    len = RSIZE_MAX_STR + 1;
    rc = strisdigit_s("1234", len);
    ERR(false)
    /*--------------------------------------------------*/

    len = 9;
    rc = strisdigit_s("", len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "123456789");
    len = 4;

    rc = strisdigit_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "1");
    len = strlen(str);

    rc = strisdigit_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "12");
    len = strlen(str);

    rc = strisdigit_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "1abcd");
    len = strlen(str);

    rc = strisdigit_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "abcd");
    len = strlen(str);

    rc = strisdigit_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    return (errs);
}
