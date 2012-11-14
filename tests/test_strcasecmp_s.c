/*------------------------------------------------------------------
 * test_strcasecmp_s
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

    int ind;
    int std_ind;

    char   str1[LEN];
    char   str2[LEN];

/*--------------------------------------------------*/

    rc = strcasecmp_s(NULL, LEN, str2, &ind);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = strcasecmp_s(str1, LEN, NULL, &ind);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = strcasecmp_s(str1, LEN, str2, NULL);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    rc = strcasecmp_s(str1, 0, str2, &ind);
    if (rc != ESZEROL) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = strcasecmp_s(str1, RSIZE_MAX_STR+1, str2, &ind);
    if (rc != ESLEMAX) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strcasecmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

    std_ind = strcasecmp(str1, str2);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "KEEP IT SIMPLE");
    strcpy (str2, "keep it simple");

    rc = strcasecmp_s(str1, 1, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }
/*--------------------------------------------------*/

    strcpy (str1, "KEEP IT SIMPLE");
    strcpy (str2, "keep it simple");

    rc = strcasecmp_s(str1, 2, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "KEEP IT SIMPLE");
    strcpy (str2, "keep it simple");

    rc = strcasecmp_s(str1, 4, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "KEEP IT SIMPLE");
    strcpy (str2, "keep it simple");

    rc = strcasecmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

    std_ind = strcasecmp(str1, str2);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keeP it simple");

    rc = strcasecmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");

    rc = strcasecmp_s(str1, LEN, str1, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != 0) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "KEEP it simplified");
    strcpy (str2, "keep it simple");

    rc = strcasecmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != ('I' - 'E')) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "KEEP 1234567890");
    strcpy (str2, "keep it simple");

    rc = strcasecmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (ind != ('1' - 'I')) {
        printf("%s %u  Error ind=%d  rc=%d  %d \n",
                     __FUNCTION__, __LINE__,  ind, rc, ('1' - 'I'));
    }

/*--------------------------------------------------*/

    return (0);
}
