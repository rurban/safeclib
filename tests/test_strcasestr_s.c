/*------------------------------------------------------------------
 * test_strcasestr_s
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
    char *sub;
    char *std_sub;

    rsize_t len1;
    rsize_t len2;

    char   str1[LEN];
    char   str2[LEN];


/*--------------------------------------------------*/

    rc = strcasestr_s(NULL, LEN, str2, LEN, &sub);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, LEN, NULL, LEN, &sub);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, LEN, str2, LEN, NULL);
    if (rc != ESNULLP) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, 0, str2, LEN, &sub);
    if (rc != ESZEROL) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, RSIZE_MAX_STR+1, str2, LEN, &sub);
    if (rc != ESLEMAX) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, LEN, str2, 0, &sub);
    if (rc != ESZEROL) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, LEN, str2, RSIZE_MAX_STR+1, &sub);
    if (rc != ESLEMAX) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    *str1 = '\0';
    *str2 = '\0';

    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != str1) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    /* compare to legacy */
    std_sub = strcasestr(str1, str2);
    if (sub != std_sub) {
        printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    *str1 = '\0';
    strcpy(str1, "key");

    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != str1) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    /* compare to legacy */
    std_sub = strcasestr(str1, str2);
    if (sub != std_sub) {
        printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    *str2 = '\0';

    /* str2 being empty, must return str1 */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != str1) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    /* compare to legacy */
    std_sub = strcasestr(str1, str2);
    if (sub != std_sub) {
        printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "kee");

    /* substring at beginning */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[0]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "eep it");

    /* substring in the middle */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[1]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "ethe");

    /* substring in the middle */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[15]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "er");

    len1 = strlen(str1);
    len2 = strlen(str2);

    /* substring at the end */
    rc = strcasestr_s(str1, len1, str2, len2, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[18]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    /* compare to legacy */
    std_sub = strcasestr(str1, str2);
    if (sub != std_sub) {
        printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "er");

    len1 = strlen(str1);
    len2 = strlen(str2);

    /* substring at the end */
    rc = strcasestr_s(str1, len1, str2, 2, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[18]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    /* compare to legacy */
    std_sub = strcasestr(str1, str2);
    if (sub != std_sub) {
        printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strcasestr_s(str1, 3, str2, LEN, &sub);
    if (rc != ESNOTFND) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != NULL) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strcasestr_s(str1, LEN, str2, 1, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[5]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strcasestr_s(str1, LEN, str2, 2, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[5]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strcasestr_s(str1, LEN, str2, 5, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[5]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "1234");

    len1 = strlen(str1);
    rc = strcasestr_s(str1, len1, str2, LEN, &sub);
    if (rc != ESNOTFND) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != NULL) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "IT ALL");

    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[5]) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "EEP");

    /* validate */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    if (rc != EOK) {
        printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (sub != &str1[1]) {
        printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

    /* compare to legacy */
    std_sub = strcasestr(str1, str2);
    if (sub != std_sub) {
        printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

/*--------------------------------------------------*/

    return (0);
}
