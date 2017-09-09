/*------------------------------------------------------------------
 * test_strprefix_s
 * File 'extstr/strprefix_s.c'
 * Lines executed:100.00% of 22
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
    rsize_t len;

    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = strprefix_s(NULL, LEN, str2);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = strprefix_s(str1, LEN, NULL);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = strprefix_s(str1, 0, str2);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = strprefix_s(str1, RSIZE_MAX_STR+1, str2);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strprefix_s(str1, LEN, str2);
    ERR(ESNOTFND)
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strprefix_s(str1, 55, str2);
    ERR(EOK)
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "kerp");

    rc = strprefix_s(str1, 55, str2);
    ERR(ESNOTFND)
/*--------------------------------------------------*/

    strcpy(str1, "ke");
    strcpy(str2, "kerp");

    rc = strprefix_s(str1, 2, str2);
    ERR(EOK)
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strprefix_s(str1, 4, str2);
    ERR(EOK)
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keeeep");

    rc = strprefix_s(str1, 4, str2);
    ERR(ESNOTFND)
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");

    rc = strprefix_s(str1, 4, "");
    ERR(ESNOTFND)
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it sipmle and very long");

    len = strlen(str1);

    rc = strprefix_s(str1, len, str2);
    ERR(ESNOTFND)
/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it simple");

    len = strlen(str1);

    rc = strprefix_s(str1, len, str2);
    ERR(EOK)
/*--------------------------------------------------*/

    return (errs);
}
