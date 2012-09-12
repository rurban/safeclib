/*------------------------------------------------------------------
 * test_strcspn_s
 *
 *
 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "safe_str_lib.h"


#define LEN   ( 128 )
#define SHORT_LEN  ( 5 )


int main()
{
    errno_t rc;
    uint32_t count;
    int std_count;

    char   str1[LEN];
    char   str2[LEN];


/*--------------------------------------------------*/

    rc = strcspn_s(NULL, LEN, str2, LEN, &count);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 0) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    rc = strcspn_s(str1, LEN, NULL, LEN, &count);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 0) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    rc = strcspn_s(str1, LEN, str2, LEN, NULL);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    rc = strcspn_s(str1, 0, str2, LEN, &count);
    if (rc != ESZEROL) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 0) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    rc = strcspn_s(str1, LEN, str2, 0, &count);
    if (rc != ESZEROL) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 0) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    rc = strcspn_s(str1, RSIZE_MAX_STR+1, str2, LEN, &count);
    if (rc != ESLEMAX) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 0) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    rc = strcspn_s(str1, RSIZE_MAX_STR, str2, RSIZE_MAX_STR+1, &count);
    if (rc != ESLEMAX) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 0) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 0) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

    std_count = strcspn(str1, str2);
    if (count != std_count) {
        printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  count, std_count, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP");

    rc = strcspn_s(str1, 1, str2, LEN, &count);
    if (rc != EOK) {   
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 1) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP");

    rc = strcspn_s(str1, 2, str2, LEN, &count);
    if (rc != EOK) {   
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 2) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP");

    rc = strcspn_s(str1, 3, str2, LEN, &count);
    if (rc != EOK) {   
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 3) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }


/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP");

    rc = strcspn_s(str1, 6, str2, LEN, &count);
    if (rc != EOK) { 
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 6) {
        printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, count, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "ABCDEF");

    rc = strcspn_s(str1, 6, str2, 2, &count);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (count != 6) {
        printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  count, std_count, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    std_count = strcspn(str1, str2);
    if (count != std_count) {
        printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  count, std_count, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "notincluded");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    std_count = strcspn(str1, str2);
    if (count != std_count) {
        printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  count, std_count, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "1234567890");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    std_count = strcspn(str1, str2);
    if (count != std_count) {
        printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  count, std_count, rc);
    }

/*--------------------------------------------------*/

    return (0);
}

