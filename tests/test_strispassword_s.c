/*------------------------------------------------------------------
 * test_strispassword_s
 * File 'extstr/strispassword_s.c'
 * Lines executed:97.44% of 39
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN 64

int main(void) {
    bool rc;
    uint32_t len;
    char str[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strispassword_s(NULL, LEN);
    ERR(false)

    EXPECT_BOS("empty dest or dmax")
    rc = strispassword_s("Test4You&", 0);
    ERR(false)

    strcpy(str, "Test4You&");
    EXPECT_BOS("dest overflow")
    rc = strispassword_s(str, RSIZE_MAX_STR + 1);
    ERR(false)

    EXPECT_BOS("dest overflow")
    rc = strispassword_s("Test4You&", SAFE_STR_PASSWORD_MAX_LENGTH + 1);
    ERR(false)

    EXPECT_BOS("dest overflow")
    rc = strispassword_s("", 9);
    ERR(false)

    EXPECT_BOS("dest overflow")
    rc = strispassword_s("", LEN);
    ERR(false)

#endif

    /*--------------------------------------------------*/

    //EXPECT_BOS("dest is too short")
    rc = strispassword_s("a", 1);
    ERR(false)

    strcpy(str, "Test4You*123");
    len = 8;

    rc = strispassword_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "Test4You*");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "Test4You*Test4You*Test4You*");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "Eest!22/");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "pa$$W0rD");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(true)

    strcpy(str, "<a]$b}0X_D");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(true)

    /*--------------------------------------------------*/

    strcpy(str, "pa$$W0rD f");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(false)

    strcpy(str, "Test");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    return (errs);
}
