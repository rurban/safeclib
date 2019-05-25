/*------------------------------------------------------------------
 * test_strismixed_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

int main(void) {
    bool rc;
    rsize_t len;
    char str[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

    len = 5;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strismixedcase_s(NULL, len);
    ERR(false)
#endif
    /*--------------------------------------------------*/

    len = 0;
    rc = strismixedcase_s("test", len);
    ERR(false)
    /*--------------------------------------------------*/

    len = RSIZE_MAX_STR + 1;
    rc = strismixedcase_s("test", len);
    ERR(false)
    /*--------------------------------------------------*/

    len = 9;
    rc = strismixedcase_s("", len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "AaBbCcDdEeFf");
    len = 5;

    rc = strismixedcase_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    rc = strismixedcase_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "NowISTHETimE");
    len = strlen(str);

    rc = strismixedcase_s(str, len);
    ERR(true)
    /*--------------------------------------------------*/

    strcpy(str, "qq21ego");
    len = strlen(str);

    rc = strismixedcase_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    strcpy(str, "1234");
    len = strlen(str);

    rc = strismixedcase_s(str, len);
    ERR(false)
    /*--------------------------------------------------*/

    return (errs);
}
