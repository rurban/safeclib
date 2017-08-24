/*------------------------------------------------------------------
 * test_strtolowercase_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )


int main()
{
    errno_t rc;
    rsize_t len;
    char str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    len = 5;
    rc = strtolowercase_s(NULL, len);
    if (rc != ESNULLP) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 0;
    rc = strtolowercase_s("test", len);
    if (rc != ESZEROL) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    len = 99999;
    rc = strtolowercase_s("test", len);
    if (rc != ESLEMAX) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
    if (strcmp(str, "n") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "n");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
    if (strcmp(str, "n") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "NOWISTHETIM3");

    rc = strtolowercase_s(str, 25);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
    if (strcmp(str, "nowisthetim3") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "NOWISTHETIME");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
    if (strcmp(str, "nowisthetime") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "qqeRo");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
    if (strcmp(str, "qqero") ) {
        printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "1234");
    len = strlen(str);

    rc = strtolowercase_s(str, 4);
    if (rc != EOK) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/


    return (errs);
}
