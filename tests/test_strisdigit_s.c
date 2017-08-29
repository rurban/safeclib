/*------------------------------------------------------------------
 * test_strisdigit_s
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
    rc = strisdigit_s(NULL, len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 0;
    rc = strisdigit_s("1234", len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 99999;
    rc = strisdigit_s("1234", len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 9;
    rc = strisdigit_s("", len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "123456789");
    len = 4;

    rc = strisdigit_s(str, len);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "1");
    len = strlen(str);

    rc = strisdigit_s(str, len);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "12");
    len = strlen(str);

    rc = strisdigit_s(str, len);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "1abcd");
    len = strlen(str);

    rc = strisdigit_s(str, len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "abcd");
    len = strlen(str);

    rc = strisdigit_s(str, len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
