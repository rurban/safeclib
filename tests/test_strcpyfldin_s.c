/*------------------------------------------------------------------
 * test_strcpyfldin_s
 * File 'extstr/strcpyfldin_s.c'
 * Lines executed:94.12% of 34
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX (128)
#define LEN (128)

int main(void) {
    errno_t rc;
    uint32_t j;
    rsize_t len;
    rsize_t slen;

    char str1[LEN];
    char str2[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("dest overflow or empty")
    rc = strcpyfldin_s(NULL, LEN, str2, LEN);
    ERR(ESNULLP);

    strcpy(str1, "aaaaa");
    EXPECT_BOS("dest overflow or empty") EXPECT_BOS("slen overflow >dmax")
    rc = strcpyfldin_s(str1, 0, str2, LEN);
    ERR(ESZEROL); /* and untouched */
    EXPSTR(str1, "aaaaa");

    EXPECT_BOS("dest overflow or empty")
    rc = strcpyfldin_s(str1, (RSIZE_MAX_STR + 1), str2, LEN);
    ERR(ESLEMAX); /* and untouched */
    EXPSTR(str1, "aaaaa");

    EXPECT_BOS("src overflow or empty")
    rc = strcpyfldin_s(str1, LEN, NULL, LEN);
    ERR(ESNULLP); /* and cleared */
    CHECK_SLACK(str1, LEN);

    strcpy(str1, "aaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    EXPECT_BOS("slen overflow >dmax")
    rc = strcpyfldin_s(str1, LEN - 1, str2, LEN);
    ERR(ESNOSPC); /* and cleared */
    CHECK_SLACK(str1, LEN - 1);
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    strcpy(str2, "01234567890123456789");
    len = strlen(str1);

    rc = strcpyfldin_s(str1, len, str2, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    rc = strcpyfldin_s(NULL, len, str2, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    rc = strcpyfldin_s(str1, 0, str2, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    rc = strcpyfldin_s(str1, len, NULL, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = strlen(str1);
    slen = strlen(str2);

    rc = strcpyfldin_s(str1, len, str2, slen);
    ERR(ESNOSPC); /* and cleared */
    CHECK_SLACK(str1, len);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 1;
    rc = strcpyfldin_s(str1, len, str2, len);
    ERR(EOK)
    for (j = 0; j < len; j++) {
        if (str1[j] != str2[j]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                         __FUNCTION__, __LINE__, j, str1[j], j, str2[j], rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 2;
    rc = strcpyfldin_s(str1, len, str2, len);
    ERR(EOK)
    for (j = 0; j < len; j++) {
        if (str1[j] != str2[j]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                         __FUNCTION__, __LINE__, j, str1[j], j, str2[j], rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 3;
    rc = strcpyfldin_s(str1, len, str2, len);
    ERR(EOK)
    for (j = 0; j < len; j++) {
        if (str1[j] != str2[j]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                         __FUNCTION__, __LINE__, j, str1[j], j, str2[j], rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    /* same string in dest and src */
    len = LEN;
    rc = strcpyfldin_s(str1, len, str1, len);
    ERR(ESOVRLP); /* and cleared */
    CHECK_SLACK(str1, len);

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple for best results ");
    len = strlen(str1);

    /* overlap */
    rc = strcpyfldin_s(&str1[0], len, &str1[5], len);
    ERR(ESOVRLP); /* and cleared */
    CHECK_SLACK(str1, len);

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple for best results ");
    len = 20;

    /* overlap */
    rc = strcpyfldin_s(&str1[10], len, &str1[0], len);
    ERR(ESOVRLP); /* and cleared */
    CHECK_SLACK(&str1[10], len);

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    len = strlen(str1);

    strcpy(str2, "KEEP IT SIMPLE PLEASE");

    rc = strcpyfldin_s(str1, len, str2, len);
    ERR(EOK)
    for (j = 0; j < len; j++) {
        if (str1[j] != str2[j]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                         __FUNCTION__, __LINE__, j, str1[j], j, str2[j], rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "always keep it simple");
    len = strlen(str1);

    strcpy(str2, "keep it simple");
    slen = strlen(str2);

    rc = strcpyfldin_s(str1, len, str2, slen);
    ERR(EOK)
    for (j = 0; j < slen; j++) {
        if (str1[j] != str2[j]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                         __FUNCTION__, __LINE__, j, str1[j], j, str2[j], rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}
