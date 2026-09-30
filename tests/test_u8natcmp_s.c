/*------------------------------------------------------------------
 * test_u8natcmp_s
 * File 'extu8/u8natcmp_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8natcmp_s(void);

int test_u8natcmp_s(void) {
    errno_t rc;
    int result;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8natcmp_s(NULL, LEN, str2, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8natcmp_s(str1, LEN, NULL, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty resultp")
    rc = u8natcmp_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8natcmp_s(str1, 0, str2, &result);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    /* digit runs compare numerically: file2 < file10 */
    strcpy((char *)str1, "file10");
    strcpy((char *)str2, "file2");

    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* equal strings */
    strcpy((char *)str1, "file2");
    strcpy((char *)str2, "file2");

    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* plain lexicographic difference still applies outside digit runs */
    strcpy((char *)str1, "abc");
    strcpy((char *)str2, "abd");

    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* u8natfccmp_s folds ASCII case */
    strcpy((char *)str1, "File2");
    strcpy((char *)str2, "file2");

    rc = u8natfccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8natcmp_s()); }
