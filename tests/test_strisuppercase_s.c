/*------------------------------------------------------------------
 * test_strisuppercase_s
 * File 'extstr/strisuppercase_s.c'
 * Lines executed:100.00% of 18
 *
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
    EXPECT_BOS("empty str")
    rc = strisuppercase_s(NULL, len);
    ERR(false)
#endif
/*--------------------------------------------------*/

    len = 99999;
    rc = strisuppercase_s("test", len);
    ERR(false)
/*--------------------------------------------------*/

    len = 0;
    rc = strisuppercase_s("test", len);
    ERR(false)
/*--------------------------------------------------*/

    str[0] = '\0';
    rc = strisuppercase_s(str, 5);
    ERR(false)
/*--------------------------------------------------*/

    strcpy (str, "ABCDEFGHIGHIJ");
    len = 7;

    rc = strisuppercase_s(str, len);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "ABCDEFGHIGHIJ");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "qqWe go");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    ERR(false)
/*--------------------------------------------------*/

    strcpy (str, "1234");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    ERR(false)
/*--------------------------------------------------*/

    strcpy (str, "!@#$%^&*()");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    ERR(false)
/*--------------------------------------------------*/

    return (errs);
}
