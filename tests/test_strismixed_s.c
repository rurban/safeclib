/*------------------------------------------------------------------
 * test_strismixed_s
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
    rsize_t len;
    char   str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    len = 5;
    rc = strismixedcase_s(NULL, len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 0;
    rc = strismixedcase_s("test", len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = RSIZE_MAX_STR+1;
    rc = strismixedcase_s("test", len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 9;
    rc = strismixedcase_s("", len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "AaBbCcDdEeFf");
    len = 5;

    rc = strismixedcase_s(str, len);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    rc = strismixedcase_s(str, len);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "NowISTHETimE");
    len = strlen(str);

    rc = strismixedcase_s(str, len);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "qq21ego");
    len = strlen(str);

    rc = strismixedcase_s(str, len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "1234");
    len = strlen(str);

    rc = strismixedcase_s(str, len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
