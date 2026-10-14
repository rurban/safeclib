/*------------------------------------------------------------------
 * test_u8icmp_s
 * File 'extu8/u8icmp_s.c'
 * Lines executed:81.82% of 22
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
static volatile size_t vrms = RSIZE_MAX_STR + 1;

int test_u8icmp_s(void);

int test_u8icmp_s(void) {
    errno_t rc;
    int result;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8icmp_s(NULL, LEN, str2, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8icmp_s(str1, LEN, NULL, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty resultp")
    rc = u8icmp_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8icmp_s(str1, 0, str2, &result);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "identifier");
    strcpy((char *)str2, "identifier");

    rc = u8icmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    strcpy((char *)str2, "identifier2");

    rc = u8icmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* equal NFC strings with multibyte content */
    strcpy((char *)str1, "caf\xC3\xA9");
    strcpy((char *)str2, "caf\xC3\xA9");

    rc = u8icmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/

    /* first differing byte stops the comparison */
    strcpy((char *)str1, "abc");
    strcpy((char *)str2, "abd");
    rc = u8icmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }
    rc = u8icmp_s(str2, LEN, str1, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    /* both strings equal and unterminated within the src bound */
    memset(str1, 'a', LEN);
    memset(str2, 'a', LEN);
    rc = u8icmp_s(str1, LEN, str2, &result);
    ERR(ESUNTERM)

    /*--------------------------------------------------*/

    /* unknown dest size: dmax > RSIZE_MAX_STR */
    {
        static volatile size_t usize = 16;
        char8_t *ubuf = (char8_t *)malloc(usize);
        if (ubuf) {
            strcpy((char *)ubuf, "abc");
            strcpy((char *)str2, "abc");
            rc = u8icmp_s(ubuf, vrms, str2, &result);
            ERR(ESLEMAX)
            rc = u8icmp_s(ubuf, usize, str2, &result);
            ERR(EOK)
            if (result != 0) {
                debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__,
                             __LINE__, result);
                errs++;
            }
            free(ubuf);
        }
    }

    return (errs);
}

int main(void) { return (test_u8icmp_s()); }
