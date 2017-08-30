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
    int errs = 0;

/*--------------------------------------------------*/

    rc = strlastchar_s(NULL, LEN, 'a', &last);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
    if (last) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strlastchar_s(str1, LEN, 'a', NULL);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strlastchar_s(str1, 0, 'a', &last);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
    if (last) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strlastchar_s(str1, RSIZE_MAX_STR+1, 'a', &last);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
    if (last) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    str1[0] = '\0';

    rc = strlastchar_s(str1, LEN, 'a', &last);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
    if (last) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "Keep it simple");

    rc = strlastchar_s(str1, 5, 'z', &last);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
    if (last) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "Keep it simplezz");

    rc = strlastchar_s(str1, LEN, 'z', &last);
    if (rc != EOK) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
    if (last != &str1[15]) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "Keep it simple");

    rc = strlastchar_s(str1, LEN, 'K', &last);
    ERR(EOK)
    if (last != &str1[0]) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "kEEp it simple");

    rc = strlastchar_s(str1, LEN, 'E', &last);
    ERR(EOK)
    if (last != &str1[2]) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "kEep it Simple");

    rc = strlastchar_s(str1, LEN, 'S', &last);
    ERR(EOK)
    if (last != &str1[8]) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, last, rc);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
