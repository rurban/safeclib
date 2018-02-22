/*------------------------------------------------------------------
 * test_strispassword_s
 * File 'extstr/strispassword_s.c'
 * Lines executed:100.00% of 44
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN 64

int main()
{
    bool rc;
    uint32_t len;
    char   str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strispassword_s(NULL, LEN);
    ERR(false)

    EXPECT_BOS("empty dest or dmax")
    rc = strispassword_s("Test4You&", 0);
    ERR(false)

    EXPECT_BOS("dest overflow")
    rc = strispassword_s("Test4You&", 999);
    ERR(false)

    EXPECT_BOS("dest overflow")
    rc = strispassword_s("", 9);
    ERR(false)

    EXPECT_BOS("dest overflow")
    rc = strispassword_s("", LEN);
    ERR(false)
#endif

/*--------------------------------------------------*/

    strcpy (str, "Test4You*123");
    len = 8;

    rc = strispassword_s(str, len);
    ERR(false)
/*--------------------------------------------------*/

    strcpy (str, "Test4You*");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "Test4You*Test4You*Test4You*");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "Eest!22/");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(false)
/*--------------------------------------------------*/

    strcpy (str, "pa$$W0rD");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(true)

    strcpy (str, "<a]$b}0X_D");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(true)

/*--------------------------------------------------*/

    strcpy (str, "pa$$W0rD f");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(false)

    strcpy (str, "Test");
    len = strlen(str);

    rc = strispassword_s(str, len);
    ERR(false)
/*--------------------------------------------------*/

    return (errs);
}
