/*------------------------------------------------------------------
 * test_strtolowercase_s
 * File 'extstr/strtolowercase_s.c'
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
    rc = strtolowercase_s(NULL, len);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strtolowercase_s(str, 0);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = strtolowercase_s(str, RSIZE_MAX_STR + 1);
    ERR(ESLEMAX)
#endif

    /*--------------------------------------------------*/

    strcpy(str, "N");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    ERR(EOK)
    if (strcmp(str, "n")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str, "n");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    ERR(EOK)
    if (strcmp(str, "n")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str, "NOWISTHETIM3");

    rc = strtolowercase_s(str, 25);
    ERR(EOK)
    if (strcmp(str, "nowisthetim3")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str, "NOWISTHETIME");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    ERR(EOK)
    if (strcmp(str, "nowisthetime")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str, "qqeRo");
    len = strlen(str);

    rc = strtolowercase_s(str, len);
    ERR(EOK)
    if (strcmp(str, "qqero")) {
        debug_printf("%s %u   Error -%s- \n", __FUNCTION__, __LINE__, str);
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
