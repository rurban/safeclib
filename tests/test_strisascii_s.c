/*------------------------------------------------------------------
 * test_strisascii_s
 * File 'extstr/strisascii_s.c'
 * Lines executed:100.00% of 16
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
    bool rc;

    uint32_t len;
    char   str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    len = 5;
    rc = strisascii_s(NULL, len);
    ERR(false)
/*--------------------------------------------------*/

    len = 0;
    rc = strisascii_s("test", len);
    ERR(false)
/*--------------------------------------------------*/

    len = 99999;
    rc = strisascii_s("test", len);
    ERR(false)
/*--------------------------------------------------*/

    /* empty string */
    rc = strisascii_s("", 2);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "ABCDEFGHIJK");

    rc = strisascii_s(str, 2);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "N");
    len = strlen(str);

    rc = strisascii_s(str, 1);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "N");
    len = strlen(str);

    rc = strisascii_s(str, 2);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "NowISTHETimE");
    len = strlen(str);

    rc = strisascii_s(str, len);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "qq21ego");

    rc = strisascii_s(str, 33);
    ERR(true)
/*--------------------------------------------------*/

    strcpy (str, "1234");
    str[2] = -124;   /* special char: 132 */
    len = strlen(str);

    /* special char embedded */
    rc = strisascii_s(str, len);
    ERR(false)
/*--------------------------------------------------*/

    return (errs);
}
