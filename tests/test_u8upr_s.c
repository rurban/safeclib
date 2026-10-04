/*------------------------------------------------------------------
 * test_u8upr_s
 * File 'extu8/u8upr_s.c'
 * Lines executed:81.40% of 43
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str[LEN];
int test_u8upr_s(void);

int test_u8upr_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8upr_s(NULL, LEN);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8upr_s(str, 0);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8upr_s(str, RSIZE_MAX_STR + 1);
    ERR(ESLEMAX)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str, "hello");
    rc = u8upr_s(str, LEN);
    ERR(EOK)
    if (strcmp((char *)str, "HELLO") != 0) {
        debug_printf("%s %u  str=%s, expected HELLO\n", __FUNCTION__, __LINE__,
                     str);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str, "Hello World");
    rc = u8upr_s(str, LEN);
    ERR(EOK)
    if (strcmp((char *)str, "HELLO WORLD") != 0) {
        debug_printf("%s %u  str=%s, expected 'HELLO WORLD'\n", __FUNCTION__,
                     __LINE__, str);
        errs++;
    }

    /*--------------------------------------------------*/

    /* multibyte content whose case mapping is locale-dependent (only
       ASCII is guaranteed under the default "C" locale); verify the
       ASCII runs around it are converted and byte-length is preserved */
    strcpy((char *)str, "caf\xC3\xA9 town");
    rc = u8upr_s(str, LEN);
    ERR(EOK)
    if (strncmp((char *)str, "CAF", 3) != 0 ||
        strcmp((char *)str + 5, " TOWN") != 0) {
        debug_printf("%s %u  str=%s, expected ascii runs uppered\n",
                     __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    /* already uppercase: unchanged */
    strcpy((char *)str, "ALREADY UPPER");
    rc = u8upr_s(str, LEN);
    ERR(EOK)
    if (strcmp((char *)str, "ALREADY UPPER") != 0) {
        debug_printf("%s %u  str=%s, expected 'ALREADY UPPER'\n", __FUNCTION__,
                     __LINE__, str);
        errs++;
    }

    /*--------------------------------------------------*/

    /* result must fit: too small dmax for the actual content byte-length */
    strcpy((char *)str, "hello");
    rc = u8upr_s(str, 3);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8upr_s()); }
