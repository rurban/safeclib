/*------------------------------------------------------------------
 * test_strlastsame_s
 * File 'extstr/strlastsame_s.c'
 * Lines executed:95.24% of 21
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
    rc = strlastsame_s(NULL, LEN, str2, &ind);
    ERR(ESNULLP)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    EXPECT_BOS("empty src")
    rc = strlastsame_s(str1, LEN, NULL, &ind);
    ERR(ESNULLP)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    EXPECT_BOS("empty resultp")
    rc = strlastsame_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)
    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dmax")
    rc = strlastsame_s(str1, 0, str2, &ind);
    ERR(ESZEROL)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = strlastsame_s(str1, RSIZE_MAX_STR + 1, str2, &ind);
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

    rc = strlastsame_s(str1, LEN, str2, &ind);
    ERR(ESNOTFND)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "Keep it simple");
    strcpy(str2, "keep_IT_SIMPLISTIC");

    rc = strlastsame_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 3) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "kEEP_IT_SIMPLE");
    strcpy(str2, "keep it simplistic");

    rc = strlastsame_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "kkEEP_IT_SIMPLE");
    strcpy(str2, "kkeep it simplistic");

    rc = strlastsame_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 1) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keeP_IT_SIMPLe");

    rc = strlastsame_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 13) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");

    /* same string for src and dest */
    rc = strlastsame_s(str1, LEN, str1, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    if (ind != 13) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "KEEP it simple");
    strcpy(str2, "keep it simple");

    rc = strlastsame_s(str1, 1, str2, &ind);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "KEEP it simple");
    strcpy(str2, "Keep it simple");

    rc = strlastsame_s(str1, 1, str2, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "KEEP_IT_SIMPLE");

    rc = strlastsame_s(str1, 5, str2, &ind);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "KEEP_IT_SIMPLE");

    rc = strlastsame_s(str1, LEN, str2, &ind);
    if (rc != ESNOTFND) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n", __FUNCTION__, __LINE__,
                     (int)ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    return (errs);
}
