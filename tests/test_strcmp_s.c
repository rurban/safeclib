/*------------------------------------------------------------------
 * test_strcmp_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )
#define SHORT_LEN  ( 5 )

static char   str1[LEN];
static char   str2[LEN];

int test_strcmp_s (void)
{
    errno_t rc;
    int ind;
    int std_ind;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strcmp_s(NULL, LEN, str2, &ind);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcmp_s(str1, LEN, NULL, &ind);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcmp_s(str1, LEN, str2, NULL);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcmp_s(str1, 0, str2, &ind);
    if (rc != ESZEROL) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcmp_s(str1, RSIZE_MAX_STR+1, str2, &ind);
    if (rc != ESLEMAX) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strcmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, ind, rc);
        errs++;
    }
    std_ind = strcmp(str1, str2);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep it simple");

    rc = strcmp_s(str1, 5, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        printf("%s %u  ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    /*   K - k ==  -32  */
    strcpy (str1, "Keep it simple");
    strcpy (str2, "keep it simple");

    rc = strcmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != (-32)) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
        errs++;
    }
    std_ind = strcmp(str1, str2);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    /*   p - P ==  32  */
    strcpy (str1, "keep it simple");
    strcpy (str2, "keeP it simple");

    rc = strcmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 32) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
        errs++;
    }
    std_ind = strcmp(str1, str2);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");

    rc = strcmp_s(str1, LEN, str1, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind != 0) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
        errs++;
    }
    /* be sure the results are the same as strcmp */
    std_ind = strcmp(str1, str1);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simplified");
    strcpy (str2, "keep it simple");

    rc = strcmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind <= 0) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
        errs++;
    }
    /* be sure the results are the same as strcmp */
    std_ind = strcmp(str1, str2);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep it simplified");

    rc = strcmp_s(str1, LEN, str2, &ind);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (ind >= 0) {
        printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, rc);
        errs++;
    }
    /* be sure the results are the same as strcmp */
    std_ind = strcmp(str1, str2);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strcmp_s());
}
#endif
