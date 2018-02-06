/*------------------------------------------------------------------
 * test_strzero_s
 * File 'extstr/strzero_s.c'
 * Lines executed:100.00% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
    errno_t rc;
    rsize_t max_len;
    char   str1[LEN];
    uint32_t i;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strzero_s(NULL, 5);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = strzero_s(str1, 0);
    ERR(ESZEROL)

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = strzero_s(str1, RSIZE_MAX_STR+1);
    ERR(ESLEMAX)
#endif
/*--------------------------------------------------*/

    strcpy(str1, "abc");
    max_len = 1;
    rc = strzero_s(str1, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != '\0') {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    strcpy(str1, "abc\0xxxxxxxxxxxxxxxxxxx");
    max_len = 2;
    rc = strzero_s(str1, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != '\0') {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    strcpy(str1, "abc\0xxxxxxxxxxxxxxxxxxx");
    max_len = 3;
    rc = strzero_s(str1, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != '\0') {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    strcpy(str1, "abc\0xxxxxxxxxxxxxxxxxxx");
    max_len = 4;
    rc = strzero_s(str1, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != '\0') {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }
    CHECK_SLACK(&str1[3], 1);

/*--------------------------------------------------*/

    strcpy(str1, "abc\0xxxxxxxxxxxxxxxxxxx");
    rc = strzero_s(str1, LEN);
    ERR(EOK)
    for (i=0; i<3; i++) {
       if (str1[i] != '\0') {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }
    CHECK_SLACK(&str1[3], LEN-3);

/*--------------------------------------------------*/

    strcpy(str1, "Now is the time for all data to be zeroed");
    max_len = strlen("Now is the ");

    rc = strzero_s(str1, max_len);
    ERR(EOK)
    if (strcmp(&str1[max_len], "time for all data to be zeroed") ) {
        debug_printf("%s %u ERROR  --%s-- \n",
               __FUNCTION__, __LINE__,
               &str1[max_len]);
        errs++;
    }
    CHECK_SLACK(str1, max_len);

/*--------------------------------------------------*/

    return (errs);
}
