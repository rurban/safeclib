/*------------------------------------------------------------------
 * test_u8cmp_s
 * File 'extu8/u8cmp_s.c'
 * Lines executed:98.11% of 53
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8cmp_s(void);

int test_u8cmp_s(void) {
    errno_t rc;
    int result;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8cmp_s(NULL, LEN, str2, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8cmp_s(str1, LEN, NULL, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty resultp")
    rc = u8cmp_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8cmp_s(str1, 0, str2, &result);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8cmp_s(str1, RSIZE_MAX_STR + 1, str2, &result);
    ERR(ESLEMAX)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "hello");
    strcpy((char *)str2, "hello");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str1, "hello");
    strcpy((char *)str2, "help");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str1, "help");
    strcpy((char *)str2, "hello");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* multibyte sequences compare bytewise when not canonically equal */
    strcpy((char *)str1, "h\xC3\xA9llo");
    strcpy((char *)str2, "hello");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    /* canonical equivalence: precomposed e-acute (U+00E9 = C3 A9) must
       compare equal to the decomposed form (e U+0065 + combining
       acute U+0301 = 65 CC 81), since u8cmp_s normalizes to NFC first */
    strcpy((char *)str1, "caf\xC3\xA9");
    strcpy((char *)str2, "cafe\xCC\x81");

    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0 (canonically equal)\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /*--------------------------------------------------*/

    /* empty strings (minimum scratch size) */
    str1[0] = 0;
    str2[0] = 0;
    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    /* src not terminated within its bound (needs the known src size) */
    memset(str2, 'a', LEN);
    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(ESUNTERM)
#endif

    /*--------------------------------------------------*/

    /* codepoint beyond U+10FFFF in dest or src is rejected by normalization */
    strcpy((char *)str1, "a\xF4\x90\x80\x80");
    strcpy((char *)str2, "a");
    rc = u8cmp_s(str1, LEN, str2, &result);
    ERR(ESLEMAX)
    rc = u8cmp_s(str2, LEN, str1, &result);
    ERR(ESLEMAX)

    /*--------------------------------------------------*/

    /* long strings need heap scratch buffers */
    {
        static char8_t big1[400], big2[400];
        memset(big1, 'a', 300);
        big1[300] = 0;
        memset(big2, 'a', 300);
        big2[300] = 0;
        rc = u8cmp_s(big1, sizeof(big1), big2, &result);
        ERR(EOK)
        if (result != 0) {
            debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__,
                         __LINE__, result);
            errs++;
        }
        big2[299] = 'b';
        rc = u8cmp_s(big1, sizeof(big1), big2, &result);
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
        rc = u8cmp_s(big1, sizeof(big1), str2, &result);
        ERR(ESLEMAX)
        rc = u8cmp_s(str2, LEN, big1, &result);
        ERR(ESLEMAX)
        rc = u8cmp_s(big2, sizeof(big2), big1, &result);
        ERR(ESLEMAX)
    }

    /*--------------------------------------------------*/

    /* unknown dest size: dmax > RSIZE_MAX_STR */
    {
        static volatile size_t usize = 16;
        char8_t *ubuf = (char8_t *)malloc(usize);
        if (ubuf) {
            strcpy((char *)ubuf, "abc");
            strcpy((char *)str2, "abc");
            rc = u8cmp_s(ubuf, RSIZE_MAX_STR + 1, str2, &result);
            ERR(ESLEMAX)
            rc = u8cmp_s(ubuf, usize, str2, &result);
            ERR(EOK)
            if (result != 0) {
                debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__,
                             __LINE__, result);
                errs++;
            }
            free(ubuf);
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8cmp_s()); }
