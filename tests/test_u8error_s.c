/*------------------------------------------------------------------
 * test_u8error_s
 * File 'extu8/u8error_s.c'
 * Lines executed:100.00% of 3
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_u8error_s(void);

int test_u8error_s(void) {
    errno_t rc;
    char8_t str1[LEN];
    size_t len;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8error_s(NULL, LEN, ESNULLP);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8error_s(str1, 0, ESNULLP);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    rc = u8error_s(str1, LEN, ESNULLP);
    ERR(EOK)
    if (str1[0] == '\0') {
        debug_printf("%s %u  empty error message\n", __FUNCTION__, __LINE__);
        errs++;
    }

    len = u8errorlen_s(ESNULLP);
    if (len != strlen((char *)str1)) {
        debug_printf("%s %u  len=%zu, expected %zu\n", __FUNCTION__, __LINE__,
                     len, strlen((char *)str1));
        errs++;
    }

    /*--------------------------------------------------*/

    /* dmax too small for the message: truncate with "..." */
    rc = u8error_s(str1, 6, ESNULLP);
    ERR(EOK)
    if (strlen((char *)str1) >= 6 ||
        strcmp((char *)&str1[strlen((char *)str1) - 3], "...")) {
        debug_printf("%s %u  expected truncated \"...\", got \"%s\"\n",
                     __FUNCTION__, __LINE__, (char *)str1);
        errs++;
    }

    /* dmax way too small to even fit "..." */
    rc = u8error_s(str1, 2, ESNULLP);
    ERR(ESLEMIN)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8error_s()); }
