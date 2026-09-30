/*------------------------------------------------------------------
 * test_u8fccmp_s
 * File 'extu8/u8fccmp_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8fccmp_s(void);

int test_u8fccmp_s(void) {
    errno_t rc;
    int result;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8fccmp_s(NULL, LEN, str2, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8fccmp_s(str1, LEN, NULL, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty resultp")
    rc = u8fccmp_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8fccmp_s(str1, 0, str2, &result);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "Hello");
    strcpy((char *)str2, "hello");

    /* ASCII foldcase makes the strings equal */
    rc = u8fccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    strcpy((char *)str2, "help");

    rc = u8fccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* multibyte foldcase: U+00C9 (E-acute) folds against lowercase e-acute */
    strcpy((char *)str1, "caf\xC3\x89");
    strcpy((char *)str2, "caf\xC3\xA9");

    rc = u8fccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* multibyte foldcase: folded U+00C9 (e-acute) sorts after plain 'e' */
    strcpy((char *)str2, "cafe");

    rc = u8fccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    /* canonical equivalence + fold-case combined: precomposed uppercase
       E-acute vs. decomposed lowercase e + combining acute must compare
       equal, since u8fccmp_s normalizes to NFC before fold-casing */
    strcpy((char *)str1, "CAF\xC3\x89");
    strcpy((char *)str2, "cafe\xCC\x81");

    rc = u8fccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0 (case+norm equal)\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    return (errs);
}

int main(void) { return (test_u8fccmp_s()); }
