/*------------------------------------------------------------------
 * test_strisuppercase_s
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
    rc = strisuppercase_s(NULL, len);
    if (rc != false) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 99999;
    rc = strisuppercase_s("test", len);
    if (rc != false) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 0;
    rc = strisuppercase_s("test", len);
    if (rc != false) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    str[0] = '\0';
    rc = strisuppercase_s(str, 5);
    if (rc != false) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "ABCDEFGHIGHIJ");
    len = 7;

    rc = strisuppercase_s(str, len);
    if (rc != true) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "ABCDEFGHIGHIJ");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    if (rc != true) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "qqWe go");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    if (rc != false) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "1234");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    if (rc != false) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "!@#$%^&*()");
    len = strlen(str);

    rc = strisuppercase_s(str, len);
    if (rc != false) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
