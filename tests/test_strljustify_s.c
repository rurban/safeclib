/*------------------------------------------------------------------
 * test_strljustify_s
 * File 'extstr/strljustify_s.c'
 * Lines executed:100.00% of 33
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
    errno_t rc;
    int ind;
    uint32_t len;
    char   str[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    len = 5;
    rc = strljustify_s(NULL, len);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    len = 0;
    rc = strljustify_s("test", len);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    len = 99999;
    rc = strljustify_s("test", len);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    /* empty string */
    str[0] = '\0';
    rc = strljustify_s(str, 12);
    ERR(EOK)
    if (str[0] != '\0') {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }

    /* whitespace only */
    strcpy (str, " ");
    rc = strljustify_s(str, 12);
    ERR(EOK)
    if (str[0] != '\0') {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy (str, "ABCDEFGHIJK");
    len = 2;

    /* unterminated */
    rc = strljustify_s(str, len);
    ERR(ESUNTERM)
    if (str[0] != '\0') {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy (str, "      ABCDEFGHIJK");
    len = 5;

    /* unterminated */
    rc = strljustify_s(str, len);
    ERR(ESUNTERM)
    if (str[0] != '\0') {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy (str, "A");
    len = 1;

    /* a one char string will be emptied - str[0]=='\0' */
    rc = strljustify_s(str, len);
    ERR(EOK)
    if (str[0] != '\0') {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy (str, "ABC");
    len = 2;

    /* this will be unterminated */
    rc = strljustify_s(str, len);
    ERR(ESUNTERM)
    if (str[0] != '\0') {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy (str, "ABC");
    len = 5;

    /* this will be unterminated */
    rc = strljustify_s(str, len);
    ERR(EOK)
    ind = strcmp(str, "ABC");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy(str, "   B ");
    len = strlen(str);

    rc = strljustify_s(str, len);
    ERR(EOK)
    ind = strcmp(str, "B ");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s-  ind=%d \n",
                     __FUNCTION__, __LINE__,  str, ind);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy(str, "   B ");
    len = strlen(str);

    rc = strljustify_s(str, len);
    ERR(EOK)
    ind = strcmp(str, "B ");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy(str, "   C ");
    len = strlen(str);

    rc = strljustify_s(str, len);
    ERR(EOK)
    ind = strcmp(str, "C ");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy(str, "   NowISTHETimE       ");
    len = strlen(str);

    rc = strljustify_s(str, len);
    ERR(EOK)
    ind = strcmp(str, "NowISTHETimE       ");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str);
        errs++;
    }
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy (str, " qq21ego ");
    len = strlen(str);

    rc = strljustify_s(str, len);
    ERR(EOK)
/*--------------------------------------------------*/

    strzero_s(str, LEN);
    strcpy (str, "   1234   ");
    len = strlen(str);

    rc = strljustify_s(str, len);
    ERR(EOK)
/*--------------------------------------------------*/

    return (errs);
}
