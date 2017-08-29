/*------------------------------------------------------------------
 * test_strfirstsame_s
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
    rsize_t ind;

    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = strfirstsame_s(NULL, LEN, str2, &ind);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strfirstsame_s(str1, LEN, NULL, &ind);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strfirstsame_s(str1, LEN, str2, NULL);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strfirstsame_s(str1, 0, str2, &ind);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strfirstsame_s(str1, RSIZE_MAX_STR+1, str2, &ind);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strfirstsame_s(str1, LEN, str2, &ind);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "kEEP it simple");
    strcpy (str2, "keep it simple");

    rc = strfirstsame_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
               __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "Keep it simple");
    strcpy (str2, "keep it simple");

    rc = strfirstsame_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 1) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "KEEP it simple");
    strcpy (str2, "keep it simple");

    rc = strfirstsame_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 4) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP_IT_SIMPLe");

    rc = strfirstsame_s(str1, 13, str2, &ind);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP_IT_SIMPLe");

    rc = strfirstsame_s(str1, 14, str2, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 13) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");

    /* same string for src and dest */
    rc = strfirstsame_s(str1, LEN, str1, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "KEEP it simple");
    strcpy (str2, "keep it simple");

    rc = strfirstsame_s(str1, 1, str2, &ind);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP_IT_SIMPLE");

    rc = strfirstsame_s(str1, 5, str2, &ind);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP_IT_SIMPLE");

    rc = strfirstsame_s(str1, LEN, str2, &ind);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
