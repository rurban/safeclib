/*------------------------------------------------------------------
 * test_strzero_s
 * File 'extstr/strzero_s.c'
 * Lines executed:92.31% of 13
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

int main(void) {
    errno_t rc;
    rsize_t max_len;
    char str1[LEN];
    uint32_t j;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strzero_s(NULL, 5);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strzero_s(str1, 0);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = strzero_s(str1, RSIZE_MAX_STR + 1);
    ERR(ESLEMAX)

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strzero_s(str1, LEN + 1);
    ERR(EOVERFLOW)
#endif
#endif
    /*--------------------------------------------------*/

    strcpy(str1, "abc");
    max_len = 1;
    rc = strzero_s(str1, max_len);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != '\0') {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "abc\0xxxxxxxxxxxxxxxxxxx");
    max_len = 2;
    rc = strzero_s(str1, max_len);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != '\0') {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "abc\0xxxxxxxxxxxxxxxxxxx");
    max_len = 3;
    rc = strzero_s(str1, max_len);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != '\0') {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "abc\0xxxxxxxxxxxxxxxxxxx");
    max_len = 4;
    rc = strzero_s(str1, max_len);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != '\0') {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }
    CHECK_SLACK(&str1[3], 1);

    /*--------------------------------------------------*/

    strcpy(str1, "abc\0xxxxxxxxxxxxxxxxxxx");
    rc = strzero_s(str1, LEN);
    ERR(EOK)
    for (j = 0; j < 3; j++) {
        if (str1[j] != '\0') {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }
    CHECK_SLACK(&str1[3], LEN - 3);

    /*--------------------------------------------------*/

    strcpy(str1, "Now is the time for all data to be zeroed");
    max_len = strlen("Now is the ");

    rc = strzero_s(str1, max_len);
    ERR(EOK)
    if (strcmp(&str1[max_len], "time for all data to be zeroed")) {
        debug_printf("%s %u ERROR  --%s-- \n", __FUNCTION__, __LINE__,
                     &str1[max_len]);
        errs++;
    }
    CHECK_SLACK(str1, max_len);

    /*--------------------------------------------------*/

    return (errs);
}
