/*------------------------------------------------------------------
 * test_strislowercase_s
 * File 'extstr/strislowercase_s.c'
 * Lines executed:100.00% of 10
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
    rc = strislowercase_s(NULL, len);
    ERR(false)
#endif
    /*--------------------------------------------------*/

    len = RSIZE_MAX_STR + 1;
    rc = strislowercase_s("test", len);
    ERR(false)
    /*--------------------------------------------------*/

    len = 0;
    rc = strislowercase_s("test", len);
    ERR(false)
    /*--------------------------------------------------*/

    str[0] = '\0';
    rc = strislowercase_s(str, 5);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "qqweqeqeqeq");
    len = 3;

    rc = strislowercase_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "qqweqeqeqeq");
    len = strlen(str);

    rc = strislowercase_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "qqWe go");
    len = strlen(str);

    rc = strislowercase_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "1234");
    len = strlen(str);

    rc = strislowercase_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    return (errs);
}
