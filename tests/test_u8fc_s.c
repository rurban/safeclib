/*------------------------------------------------------------------
 * test_u8fc_s
 * File 'extu8/u8fc_s.c'
 * Lines executed:90.48% of 42
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t dest[LEN];
static char8_t src[LEN];
int test_u8fc_s(void);

int test_u8fc_s(void) {
    errno_t rc;
    rsize_t len;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8fc_s(NULL, LEN, src, &len);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8fc_s(dest, 0, src, &len);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8fc_s(dest, RSIZE_MAX_STR + 1, src, &len);
    ERR(ESLEMAX)
#endif

    strcpy((char *)src, "hi");
#ifndef HAVE_CT_BOS_OVR
    rc = u8fc_s(dest, LEN, NULL, &len);
    ERR(ESNULLP)
#endif

    /*--------------------------------------------------*/

    /* simple ASCII fold via towlower */
    strcpy((char *)src, "Hello World");
    rc = u8fc_s(dest, LEN, src, &len);
    ERR(EOK)
    if (strcmp((char *)dest, "hello world") != 0) {
        debug_printf("%s %u  dest=%s, expected 'hello world'\n", __FUNCTION__,
                     __LINE__, dest);
        errs++;
    }
    if (len != strlen("hello world")) {
        debug_printf("%s %u  len=%zu, expected %zu\n", __FUNCTION__, __LINE__,
                     (size_t)len, strlen("hello world"));
        errs++;
    }

    /*--------------------------------------------------*/

    /* full fold-case multi-char expansion: U+00DF (sharp s) -> "ss" */
    strcpy((char *)src, "stra\xC3\x9F" "e");
    rc = u8fc_s(dest, LEN, src, &len);
    ERR(EOK)
    if (strcmp((char *)dest, "strasse") != 0) {
        debug_printf("%s %u  dest=%s, expected strasse\n", __FUNCTION__,
                     __LINE__, dest);
        errs++;
    }

    /*--------------------------------------------------*/

    /* multibyte simple fold: U+00C9 -> U+00E9 */
    strcpy((char *)src, "CAF\xC3\x89");
    rc = u8fc_s(dest, LEN, src, &len);
    ERR(EOK)
    if (strcmp((char *)dest, "caf\xC3\xA9") != 0) {
        debug_printf("%s %u  dest=%s, expected caf+e-acute\n", __FUNCTION__,
                     __LINE__, dest);
        errs++;
    }

    /*--------------------------------------------------*/

    /* result must fit */
    strcpy((char *)src, "Hello World");
    rc = u8fc_s(dest, 3, src, &len);
    ERR(ESNOSPC)
    if (len != strlen("hello world")) {
        debug_printf("%s %u  len=%zu, expected %zu even on ESNOSPC\n",
                     __FUNCTION__, __LINE__, (size_t)len,
                     strlen("hello world"));
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8fc_s()); }
