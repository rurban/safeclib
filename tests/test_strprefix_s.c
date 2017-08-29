/*------------------------------------------------------------------
 * test_strprefix_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )
#define SHORT_LEN  ( 5 )


int main()
{
    errno_t rc;
    rsize_t len;

    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = strprefix_s(NULL, LEN, str2);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strprefix_s(str1, LEN, NULL);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strprefix_s(str1, 0, str2);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strprefix_s(str1, RSIZE_MAX_STR+1, str2);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strprefix_s(str1, LEN, str2);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strprefix_s(str1, 55, str2);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "kerp");

    rc = strprefix_s(str1, 55, str2);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "ke");
    strcpy(str2, "kerp");

    rc = strprefix_s(str1, 2, str2);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strprefix_s(str1, 4, str2);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keeeep");

    rc = strprefix_s(str1, 4, str2);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");

    rc = strprefix_s(str1, 4, "");
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it sipmle and very long");

    len = strlen(str1);

    rc = strprefix_s(str1, len, str2);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it simple");

    len = strlen(str1);

    rc = strprefix_s(str1, len, str2);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
