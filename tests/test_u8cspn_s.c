/*------------------------------------------------------------------
 * test_u8cspn_s
 * File 'extu8/u8cspn_s.c'
 * Lines executed:83.87% of 31
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str[LEN];
int test_u8cspn_s(void);

int test_u8cspn_s(void) {
    errno_t rc;
    rsize_t cnt;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8cspn_s(NULL, LEN, (char8_t *)"ow", 3, &cnt);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8cspn_s(str, LEN, NULL, 3, &cnt);
    ERR(ESNULLP)

    EXPECT_BOS("empty countp")
    rc = u8cspn_s(str, LEN, (char8_t *)"ow", 3, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8cspn_s(str, 0, (char8_t *)"ow", 3, &cnt);
    ERR(ESZEROL)
#endif

    strcpy((char *)str, "hello world");
    rc = u8cspn_s(str, LEN, (char8_t *)"ow", 0, &cnt);
    ERR(ESZEROL)

    /*--------------------------------------------------*/

    /* "hell" excludes 'o'/'w' -> prefix length 4 */
    rc = u8cspn_s(str, LEN, (char8_t *)"ow", 3, &cnt);
    ERR(EOK)
    if (cnt != 4) {
        debug_printf("%s %u  cnt=%u, expected 4\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt);
        errs++;
    }

    /* none of the exclusion set occurs: whole string is the prefix */
    rc = u8cspn_s(str, LEN, (char8_t *)"xyz", 4, &cnt);
    ERR(EOK)
    if (cnt != strlen((char *)str)) {
        debug_printf("%s %u  cnt=%u, expected %u\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt, (unsigned)strlen((char *)str));
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8cspn_s()); }
