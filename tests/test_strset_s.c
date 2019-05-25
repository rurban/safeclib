/*------------------------------------------------------------------
 * test_strset_s
 * File 'strset_s.c'
 * Lines executed:93.75% of 16
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
    int value = 0;
    int len;
    uint32_t j;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strset_s(NULL, 5, value);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strset_s(str1, 0, 0);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = strset_s(str1, RSIZE_MAX_STR + 1, 0);
    ERR(ESLEMAX);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strset_s(str1, LEN + 1, 0);
    ERR(EOVERFLOW)
#endif

    EXPECT_BOS("value overflow >255")
    rc = strset_s(str1, LEN, 256);
    ERR(ESLEMAX);
#endif
    /*--------------------------------------------------*/

    strcpy(str1, "abc");
    value = 20;
    max_len = 1;
    rc = strset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != (char)value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "abc");
    value = 0;
    max_len = 2;
    rc = strset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != (char)value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "abc");
    max_len = 3;
    rc = strset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != (char)value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "abc");
    max_len = 4;
    rc = strset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < max_len; j++) {
        if (str1[j] != (char)value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }
    CHECK_SLACK(&str1[3], 1);

    /*--------------------------------------------------*/

    strcpy(str1, "abc");
    max_len = 5;
    rc = strset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < 3; j++) {
        if (str1[j] != (char)value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }
    CHECK_SLACK(&str1[3], 2);

    /*--------------------------------------------------*/

    strcpy(str1, "abc");
    max_len = LEN;
    rc = strset_s(str1, max_len, value);
    ERR(EOK)
    for (j = 0; j < 3; j++) {
        if (str1[j] != (char)value) {
            debug_printf("%s %u   Error rc=%u \n", __FUNCTION__, __LINE__, rc);
            errs++;
        }
    }
    CHECK_SLACK(&str1[3], LEN - 3);

    /*--------------------------------------------------*/

    strcpy(str1, "Now is the time for all data to be zeroed");
    len = strlen(str1);
    max_len = strlen("Now is the ");

    rc = strset_s(str1, max_len, 0);
    ERR(EOK)
    if (strcmp(&str1[max_len], "time for all data to be zeroed")) {
        debug_printf("%s %u ERROR  --%s-- \n", __FUNCTION__, __LINE__,
                     &str1[max_len]);
        errs++;
    }
    CHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    return (errs);
}
