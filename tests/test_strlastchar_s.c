/*------------------------------------------------------------------
 * test_strlastchar_s
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

    char *last;
    char  str1[LEN];

/*--------------------------------------------------*/

    rc = strlastchar_s(NULL, LEN, 'a', &last);
    if (rc != ESNULLP) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

    if (last) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    rc = strlastchar_s(str1, LEN, 'a', NULL);
    if (rc != ESNULLP) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    rc = strlastchar_s(str1, 0, 'a', &last);
    if (rc != ESZEROL) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

    if (last) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    rc = strlastchar_s(str1, RSIZE_MAX_STR+1, 'a', &last);
    if (rc != ESLEMAX) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

    if (last) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    str1[0] = '\0';

    rc = strlastchar_s(str1, LEN, 'a', &last);
    if (rc != ESNOTFND) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

    if (last) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "Keep it simple");

    rc = strlastchar_s(str1, 5, 'z', &last);
    if (rc != ESNOTFND) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

    if (last) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "Keep it simplezz");

    rc = strlastchar_s(str1, LEN, 'z', &last);
    if (rc != EOK) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

    if (last != &str1[15]) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "Keep it simple");

    rc = strlastchar_s(str1, LEN, 'K', &last);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (last != &str1[0]) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "kEEp it simple");

    rc = strlastchar_s(str1, LEN, 'E', &last);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (last != &str1[2]) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "kEep it Simple");

    rc = strlastchar_s(str1, LEN, 'S', &last);
    if (rc != EOK) {
        printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
    }

    if (last != &str1[8]) {
        printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
    }

/*--------------------------------------------------*/
/*--------------------------------------------------*/

    return (0);
}
