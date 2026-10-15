/*------------------------------------------------------------------
 * test_u8spn_s
 * File 'extu8/u8spn_s.c'
 * Lines executed:100.00% of 38
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str[LEN];
int test_u8spn_s(void);

int test_u8spn_s(void) {
    errno_t rc;
    rsize_t cnt;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8spn_s(NULL, LEN, (char8_t *)"a", 2, &cnt);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8spn_s(str, LEN, NULL, 2, &cnt);
    ERR(ESNULLP)

    EXPECT_BOS("empty countp")
    rc = u8spn_s(str, LEN, (char8_t *)"a", 2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8spn_s(str, 0, (char8_t *)"a", 2, &cnt);
    ERR(ESZEROL)
#endif

    strcpy((char *)str, "aaabbb");
#ifndef HAVE_CT_BOS_OVR
    rc = u8spn_s(str, LEN, (char8_t *)"a", 0, &cnt);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    /* prefix of a's, stops at the first b */
    rc = u8spn_s(str, LEN, (char8_t *)"a", 2, &cnt);
    ERR(EOK)
    if (cnt != 3) {
        debug_printf("%s %u  cnt=%u, expected 3\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt);
        errs++;
    }

    /* first byte itself isn't in the set: prefix length 0 */
    rc = u8spn_s(str, LEN, (char8_t *)"b", 2, &cnt);
    ERR(EOK)
    if (cnt != 0) {
        debug_printf("%s %u  cnt=%u, expected 0\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt);
        errs++;
    }

    /* everything matches */
    rc = u8spn_s(str, LEN, (char8_t *)"ab", 3, &cnt);
    ERR(EOK)
    if (cnt != strlen((char *)str)) {
        debug_printf("%s %u  cnt=%u, expected %u\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt, (unsigned)strlen((char *)str));
        errs++;
    }

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* slen beyond the src object: known src size */
#ifndef HAVE_CT_BOS_OVR
    strcpy((char *)str, "aaabbb");
    EXPECT_BOS("slen exceeds src")
    rc = u8spn_s(str, LEN, (char8_t *)"a", 3, &cnt);
    ERR(EOVERFLOW)
    if (cnt != 0) {
        debug_printf("%s %u  cnt=%u, expected 0\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt);
        errs++;
    }
    EXPECT_BOS("slen exceeds max")
    rc = u8spn_s(str, LEN, (char8_t *)"a", RSIZE_MAX_STR + 1, &cnt);
    ERR(ESLEMAX)
    if (cnt != 0) {
        debug_printf("%s %u  cnt=%u, expected 0\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt);
        errs++;
    }
#endif

    /*--------------------------------------------------*/
    /* dmax/slen > RSIZE_MAX_STR is only checked for objects of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        if (!hp)
            return errs + 1;
        strcpy((char *)hp, "aab");
        rc = u8spn_s(hp, RSIZE_MAX_STR + 1, (char8_t *)"a", 2, &cnt);
        ERR(ESLEMAX)
        if (cnt != 0) {
        debug_printf("%s %u  cnt=%u, expected 0\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt);
        errs++;
    }
        rc = u8spn_s(str, LEN, hp, RSIZE_MAX_STR + 1, &cnt);
        ERR(ESLEMAX)
        if (cnt != 0) {
        debug_printf("%s %u  cnt=%u, expected 0\n", __FUNCTION__, __LINE__,
                     (unsigned)cnt);
        errs++;
    }
        rc = u8spn_s(str, LEN, hp, 2, &cnt);
        ERR(EOK)
        if (cnt != 3) {
            debug_printf("%s %u  cnt=%u, expected 3\n", __FUNCTION__, __LINE__,
                         (unsigned)cnt);
            errs++;
        }
        free(hp);
    }

    return (errs);
}

int main(void) { return (test_u8spn_s()); }
