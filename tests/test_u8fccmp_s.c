/*------------------------------------------------------------------
 * test_u8fccmp_s
 * File 'extu8/u8fccmp_s.c'
 * Lines executed:91.30% of 92
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

    /*--------------------------------------------------*/

    /* empty strings (minimum normalization scratch size) */
    str1[0] = 0;
    str2[0] = 0;
    rc = u8fccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }
    strcpy((char *)str2, "a");
    rc = u8fccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }
    rc = u8fccmp_s(str2, LEN, str1, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    /* codepoint beyond U+10FFFF in dest or src is rejected by normalization */
    strcpy((char *)str1, "a\xF4\x90\x80\x80");
    strcpy((char *)str2, "a");
    rc = u8fccmp_s(str1, LEN, str2, &result);
    ERR(ESLEMAX)
    rc = u8fccmp_s(str2, LEN, str1, &result);
    ERR(ESLEMAX)

    /*--------------------------------------------------*/

    /* long strings need heap scratch buffers (> 128 bytes normalized) */
    {
        static char8_t big1[400], big2[400];
        memset(big1, 'A', 300);
        big1[300] = 0;
        memset(big2, 'a', 300);
        big2[300] = 0;
        rc = u8fccmp_s(big1, sizeof(big1), big2, &result);
        ERR(EOK)
        if (result != 0) {
            debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__,
                         __LINE__, result);
            errs++;
        }
        big2[299] = 'b';
        rc = u8fccmp_s(big1, sizeof(big1), big2, &result);
        ERR(EOK)
        if (result >= 0) {
            debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__,
                         __LINE__, result);
            errs++;
        }

        /* too-large codepoint after heap allocation of dest, or of src */
        big1[10] = (char8_t)0xF4;
        big1[11] = (char8_t)0x90;
        big1[12] = (char8_t)0x80;
        big1[13] = (char8_t)0x80;
        rc = u8fccmp_s(big1, sizeof(big1), str2, &result);
        ERR(ESLEMAX)
        rc = u8fccmp_s(str2, LEN, big1, &result);
        ERR(ESLEMAX)
        rc = u8fccmp_s(big2, sizeof(big2), big1, &result);
        ERR(ESLEMAX)
    }

    /*--------------------------------------------------*/

    /* unknown dest size: dmax > RSIZE_MAX_STR */
    {
        static volatile size_t usize = 16;
        char8_t *ubuf = (char8_t *)malloc(usize);
        if (ubuf) {
            strcpy((char *)ubuf, "Ab");
            strcpy((char *)str2, "aB");
            rc = u8fccmp_s(ubuf, RSIZE_MAX_STR + 1, str2, &result);
            ERR(ESLEMAX)
            rc = u8fccmp_s(ubuf, usize, str2, &result);
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

int main(void) { return (test_u8fccmp_s()); }
