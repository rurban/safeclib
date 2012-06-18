/*------------------------------------------------------------------
 * test_strtouppercase_s     
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
    errno_t rc;
    rsize_t len;
    char str[LEN];

/*--------------------------------------------------*/

    len = 5;
    rc = strtouppercase_s(NULL, len);
    if (rc != ESNULLP) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    len = 0;
    rc = strtouppercase_s("test", len);
    if (rc != ESZEROL) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    /* empty string */ 
    len = 5;
    rc = strtouppercase_s("", len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    len = 99999;
    rc = strtouppercase_s("test", len);
    if (rc != ESLEMAX) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "n");
    len = strlen(str);

    rc = strtouppercase_s(str, len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    if (strcmp(str, "N") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
    }

/*--------------------------------------------------*/

    strcpy (str, "N");
    len = strlen(str);

    rc = strtouppercase_s(str, len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    if (strcmp(str, "N") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
    }

/*--------------------------------------------------*/

    strcpy (str, "nowisthetime");
    len = strlen(str);

    rc = strtouppercase_s(str, len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    if (strcmp(str, "NOWISTHETIME") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
    }

/*--------------------------------------------------*/

    strcpy (str, "qqeRo");
    len = strlen(str);

    rc = strtouppercase_s(str, len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    if (strcmp(str, "QQERO") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
    }

/*--------------------------------------------------*/

    strcpy (str, "1234");
    len = strlen(str);

    rc = strtouppercase_s(str, 22);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    return (0);
}
