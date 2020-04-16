/*------------------------------------------------------------------
 * test_strtouppercase_s
 * File 'extstr/strtouppercase_s.c'
 * Lines executed:91.67% of 12
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

int main(void) {
    errno_t rc;
    rsize_t len;
    char str[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

    len = 5;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strtouppercase_s(NULL, len);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strtouppercase_s(str, 0);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = strtouppercase_s(str, RSIZE_MAX_STR + 1);
    ERR(ESLEMAX)

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strtouppercase_s(str, LEN + 1);
    ERR(EOVERFLOW)

    EXPECT_BOS("dest overflow") /* flapping */
    rc = strtouppercase_s((char *)"", 2);
    ERR(EOVERFLOW)

    /* empty string: static */
    rc = strtouppercase_s((char *)"", 1);
    ERR(EOK)
#endif
#endif
    /*--------------------------------------------------*/

    /* dynamic */
    *str = '\0';
    rc = strtouppercase_s(str, 5);
    ERR(EOK)

    /*--------------------------------------------------*/

    strcpy(str, "n");
    len = strlen(str);

    debug_printf("debug - 05\n");
    rc = strtouppercase_s(str, len);
    ERR(EOK)
    debug_printf("debug - 06\n");

    if (strcmp(str, "N")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    debug_printf("debug - 07\n");
    rc = strtouppercase_s(str, len);
    ERR(EOK)
    debug_printf("debug - 08\n");
    if (strcmp(str, "N")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str, "nowisthetime");
    len = strlen(str);

    debug_printf("debug - 09\n");
    rc = strtouppercase_s(str, len);
    ERR(EOK)
    debug_printf("debug - 10\n");
    if (strcmp(str, "NOWISTHETIME")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str, "qqeRo");
    len = strlen(str);

    debug_printf("debug - 11\n");
    rc = strtouppercase_s(str, len);
    ERR(EOK)
    debug_printf("debug - 12\n");
    if (strcmp(str, "QQERO")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str, "1234");
    len = strlen(str);

    debug_printf("debug - 13\n");
    rc = strtouppercase_s(str, 22);
    ERR(EOK)
    /*--------------------------------------------------*/

    return (errs);
}
