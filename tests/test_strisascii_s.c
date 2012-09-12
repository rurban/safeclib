/*------------------------------------------------------------------
 * test_strisascii_s
 *
 *
 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "safe_str_lib.h"


#define LEN   ( 128 )


int main()
{
    boolean_t rc;

    uint32_t len;
    char   str[LEN];

/*--------------------------------------------------*/

    len = 5;
    rc = strisascii_s(NULL, len);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    len = 0;
    rc = strisascii_s("test", len);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    len = 99999;
    rc = strisascii_s("test", len);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    /* empty string */ 
    rc = strisascii_s("", 2);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "ABCDEFGHIJK");

    rc = strisascii_s(str, 2);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "N");
    len = strlen(str);

    rc = strisascii_s(str, 1);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "N");
    len = strlen(str);

    rc = strisascii_s(str, 2);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "NowISTHETimE");
    len = strlen(str);

    rc = strisascii_s(str, len);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "qq21ego");

    rc = strisascii_s(str, 33);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "1234");
    str[2] = 132;   /* special char */
    len = strlen(str);

    /* special char embedded */ 
    rc = strisascii_s(str, len);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    return (0);
}

