/*------------------------------------------------------------------
 * test_wcsnset_s
 * File 'wcsnset_s.c'
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
    rsize_t max_len;
    wchar_t str1[LEN];
    wchar_t value = L'\0';
    uint32_t i;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcsnset_s(NULL, 5, value, 5);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = wcsnset_s(str1, 0, 0, 5);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = wcsnset_s(str1, RSIZE_MAX_WSTR+1, 0, 5);
    ERR(ESLEMAX);

    rc = wcsnset_s(str1, LEN-1, L' ', LEN);
    ERR(ESNOSPC);

/*--------------------------------------------------*/

    value = L' ';
    max_len = 1;
    rc = wcsnset_s(str1, LEN, value, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != value) {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    value = 0;
    max_len = 2;
    rc = wcsnset_s(str1, LEN, value, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != value) {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    max_len = 3;
    rc = wcsnset_s(str1, LEN, value, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
        if (str1[i] != value) {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    max_len = LEN;
    rc = wcsnset_s(str1, LEN, value, max_len);
    ERR(EOK)
    for (i=0; i<max_len; i++) {
       if (str1[i] != value) {
           debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
           errs++;
       }
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"Now is the time for all data to be zeroed");
    max_len = wcslen(L"Now is the ");

    rc = wcsnset_s(str1, LEN, 0, max_len);
    ERR(EOK)
    if (wcscmp(&str1[max_len], L"time for all data to be zeroed") ) {
        debug_printf("%s %u ERROR  --%ls-- \n",
               __FUNCTION__, __LINE__,
               &str1[max_len]);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
