/*------------------------------------------------------------------
 * test_strnset_s
 * File 'strnset_s.c'
 * Lines executed:100.00% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
    errno_t rc;
    rsize_t max_len = 5;
    char   str1[LEN];
    int value = 0;
    uint32_t i;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strnset_s(NULL, 5, value, 5);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = strnset_s(str1, 0, 0, 5);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = strnset_s(str1, RSIZE_MAX_STR+1, 0, 5);
    ERR(ESLEMAX);

    rc = strnset_s(str1, LEN, 256, 5);
    ERR(ESLEMAX);

/*--------------------------------------------------*/

    rc = strnset_s(str1, 5, 0, 6);
    ERR(ESNOSPC);

/*--------------------------------------------------*/

    value = 20;
    max_len = 1;
    rc = strnset_s(str1, LEN, value, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != (char)value) {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    value = 0;
    max_len = 2;
    rc = strnset_s(str1, LEN, value, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != (char)value) {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    max_len = 3;
    rc = strnset_s(str1, LEN, value, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
        if (str1[i] != (char)value) {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    max_len = LEN;
    rc = strnset_s(str1, LEN, value, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != (char)value) {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    strcpy(str1, "Now is the time for all data to be zeroed");
    max_len = strlen("Now is the ");

    rc = strnset_s(str1, LEN, 0, max_len);
    ERR(EOK)
    if (strcmp(&str1[max_len], "time for all data to be zeroed") ) {
        debug_printf("%s %u ERROR  --%s-- \n",
               __FUNCTION__, __LINE__,
               &str1[max_len]);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
