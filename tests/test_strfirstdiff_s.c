/*------------------------------------------------------------------
 * test_strfirstdiff_s
 * File 'extstr/strfirstdiff_s.c'
 * Lines executed:94.44% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)
#define SHORT_LEN (5)

int main(void) {
    errno_t rc;
    rsize_t ind;

    char str1[LEN];
    char str2[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strfirstdiff_s(NULL, LEN, str2, &ind);
    ERR(ESNULLP)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    EXPECT_BOS("empty src")
    rc = strfirstdiff_s(str1, LEN, NULL, &ind);
    ERR(ESNULLP)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    EXPECT_BOS("empty resultp")
    rc = strfirstdiff_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)
    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dmax")
    rc = strfirstdiff_s(str1, 0, str2, &ind);
    ERR(ESZEROL)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }

    EXPECT_BOS("dest overflow")
    rc = strfirstdiff_s(str1, RSIZE_MAX_STR + 1, str2, &ind);
    ERR(ESLEMAX)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
#endif
    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strfirstdiff_s(str1, LEN, str2, &ind);
    ERR(ESNODIFF)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "Keep it simple");
    strcpy(str2, "keep it simple");

    rc = strfirstdiff_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "kEeP it simple");
    strcpy(str2, "keep it simple");

    rc = strfirstdiff_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 1) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keEp IT simple");

    rc = strfirstdiff_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 2) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");

    rc = strfirstdiff_s(str1, LEN, str1, &ind);
    ERR(ESNODIFF)
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it simple");

    rc = strfirstdiff_s(str1, 1, str2, &ind);
    ERR(ESNODIFF)
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simplE");
    strcpy(str2, "keep it simple");

    rc = strfirstdiff_s(str1, 13, str2, &ind);
    ERR(ESNODIFF)
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simplE");
    strcpy(str2, "keep it simple");

    rc = strfirstdiff_s(str1, 14, str2, &ind);
    ERR(EOK)
    if (ind != 13) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simplE");
    strcpy(str2, "keep it simple");

    rc = strfirstdiff_s(str1, 55, str2, &ind);
    ERR(EOK)
    if (ind != 13) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    return (errs);
}
