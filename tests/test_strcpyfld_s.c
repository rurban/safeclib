/*------------------------------------------------------------------
 * test_strcpyfld_s
 * File 'extstr/strcpyfld_s.c'
 * Lines executed:94.44% of 36
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
    strcpy(str1, "aaaaa");
    strcpy(str2, "bbb");
    len = 5;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("dest overflow or empty")
    rc = strcpyfld_s(NULL, LEN, str2, LEN);
    ERR(ESNULLP);

    EXPECT_BOS("dest overflow or empty") EXPECT_BOS("slen overflow >dmax")
    rc = strcpyfld_s(str1, 0, str2, 5);
    ERR(ESZEROL); /* and untouched */
    EXPSTR(str1, "aaaaa");

    EXPECT_BOS("dest overflow or empty")
    rc = strcpyfld_s(str1, (RSIZE_MAX_STR + 1), str2, 5);
    ERR(ESLEMAX); /* and cleared */
    if (_BOS_KNOWN(str1)) {
      EXPSTR(str1, "");
      CHECK_SLACK(str1, 5);
    }

    strcpy(str1, "aaaaa");
    slen = len + 1; /* ESLEMAX over ESNOSPC */
    EXPECT_BOS("dest overflow or empty")
    rc = strcpyfld_s(str1, (RSIZE_MAX_STR + 1), str2, 6);
    ERR(ESLEMAX); /* and cleared */
    if (_BOS_KNOWN(str1)) {
      EXPSTR(str1, "");
      CHECK_SLACK(str1, 5);
    }

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    EXPECT_BOS("src overflow or empty")
    rc = strcpyfld_s(str1, 5, NULL, 5);
    ERR(ESNULLP); /* and cleared */
    CHECK_SLACK(str1, 5);

    strcpy(str1, "aaaaa");
    EXPECT_BOS("slen overflow >dmax")
    rc = strcpyfld_s(str1, LEN - 1, str2, LEN);
    ERR(ESNOSPC); /* and cleared */
    CHECK_SLACK(str1, 5);
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    strcpy(str2, "bbb");
    len = 5;

    rc = strcpyfld_s(str1, len - 1, str2, len);
    ERR(ESNOSPC); /* and cleared */
    CHECK_SLACK(str1, len - 1);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    len = 5;
    rc = strcpyfld_s(str1, len, str2, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    rc = strcpyfld_s(NULL, len, str2, 0);
    ERR(EOK); /* and untouched */

    rc = strcpyfld_s(str1, 0, str2, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    rc = strcpyfld_s(str1, len, NULL, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 1;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 1;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
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
    len = 2;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 2;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
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
    len = 2;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 2;

    rc = strcpyfld_s(str2, len, str1, slen);
    ERR(EOK)

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 10;

    rc = strcpyfld_s(str1, len, &str1[5], 10);
    ERR(ESOVRLP); /* and cleared */
    CHECK_SLACK(str1, len);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 10;

    rc = strcpyfld_s(&str1[5], len, &str1[0], 10);
    ERR(ESOVRLP); /* and cleared */
    CHECK_SLACK(&str1[5], len);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 3;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 3;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
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
    len = strlen(str1);

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = strlen(str2);
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
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
    len = strlen(str1) + 2;
    str1[20] = '\0';

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = strlen(str1);
    str2[0] = '\0';
    str2[20] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
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
