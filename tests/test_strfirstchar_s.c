/*------------------------------------------------------------------
 * test_strfirstchar_s
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
    rsize_t  len;

    char *first;
    char  str1[LEN];

/*--------------------------------------------------*/

    rc = strfirstchar_s(NULL, LEN, 'a', &first);
    if (rc != ESNULLP) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

    if (first) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    rc = strfirstchar_s(str1, LEN, 'a', NULL);
    if (rc != ESNULLP) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    rc = strfirstchar_s(str1, 0, 'a', &first);
    if (rc != ESZEROL) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

    if (first) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    rc = strfirstchar_s(str1, RSIZE_MAX_STR+1, 'a', &first);
    if (rc != ESLEMAX) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

    if (first) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    str1[0] = '\0';

    rc = strfirstchar_s(str1, LEN, 'a', &first);
    if (rc != ESNOTFND) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

    if (first) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "Keep it simple");

    rc = strfirstchar_s(str1, LEN, 'z', &first);
    if (rc != ESNOTFND) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

    if (first) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "kEEp it simple");

    rc = strfirstchar_s(str1, LEN, 'E', &first);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (first != &str1[1]) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simpleZ");
    len = strlen(str1);

    rc = strfirstchar_s(str1, len, 'Z', &first);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (first != &str1[14]) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simpleZZ");

    rc = strfirstchar_s(str1, LEN, 'Z', &first);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (first != &str1[14]) {
        printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, first, rc);
    }

/*--------------------------------------------------*/

    return (0);
}
