/*------------------------------------------------------------------
 * test_u8len_s
 * File 'extu8/u8len_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

int test_u8len_s(void);

int test_u8len_s(void) {
    rsize_t len;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty str")
    len = u8len_s(NULL);
    if (len != 0) {
        debug_printf("%s %u  len=%u, expected 0\n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }
#endif

    len = u8len_s((const char8_t *)"");
    if (len != 0) {
        debug_printf("%s %u  len=%u, expected 0\n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }

    len = u8len_s((const char8_t *)"hello");
    if (len != 5) {
        debug_printf("%s %u  len=%u, expected 5\n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }

    /* byte-length, not character count: e-acute is 2 bytes */
    len = u8len_s((const char8_t *)"h\xC3\xA9llo");
    if (len != 6) {
        debug_printf("%s %u  len=%u, expected 6\n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8len_s()); }
