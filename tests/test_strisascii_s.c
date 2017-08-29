/*------------------------------------------------------------------
 * test_strisascii_s
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
    rc = strisascii_s(NULL, len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 0;
    rc = strisascii_s("test", len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 99999;
    rc = strisascii_s("test", len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    /* empty string */
    rc = strisascii_s("", 2);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "ABCDEFGHIJK");

    rc = strisascii_s(str, 2);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "N");
    len = strlen(str);

    rc = strisascii_s(str, 1);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "N");
    len = strlen(str);

    rc = strisascii_s(str, 2);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "NowISTHETimE");
    len = strlen(str);

    rc = strisascii_s(str, len);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "qq21ego");

    rc = strisascii_s(str, 33);
    if (rc != true) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str, "1234");
    str[2] = -124;   /* special char: 132 */
    len = strlen(str);

    /* special char embedded */
    rc = strisascii_s(str, len);
    if (rc != false) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
