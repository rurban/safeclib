/*------------------------------------------------------------------
 * test_strtolowercase_s
 * File 'extstr/strtolowercase_s.c'
 * Lines executed:100.00% of 16
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
    errno_t rc;
    rsize_t len;
    char str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    len = 5;
    rc = strtolowercase_s(NULL, len);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    len = 0;
    rc = strtolowercase_s("test", len);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    len = 99999;
    rc = strtolowercase_s("test", len);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    ERR(EOK)
    if (strcmp(str, "n") ) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "n");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    ERR(EOK)
    if (strcmp(str, "n") ) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "NOWISTHETIM3");

    rc = strtolowercase_s(str, 25);
    ERR(EOK)
    if (strcmp(str, "nowisthetim3") ) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "NOWISTHETIME");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    ERR(EOK)
    if (strcmp(str, "nowisthetime") ) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "qqeRo");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    ERR(EOK)
    if (strcmp(str, "qqero") ) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str, "1234");
    len = strlen(str);

    rc = strtolowercase_s(str, 4);
    ERR(EOK)
/*--------------------------------------------------*/

    return (errs);
}
