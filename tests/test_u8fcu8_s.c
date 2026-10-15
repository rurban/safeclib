/*------------------------------------------------------------------
 * test_u8fcu8_s
 * File 'extu8/u8fcu8_s.c'
 * Lines executed:97.37% of 76
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
static char8_t *sub;
static volatile size_t vzero = 0;

int test_u8fcu8_s(void);

int test_u8fcu8_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8fcu8_s(NULL, LEN, str2, 5, &sub);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8fcu8_s(str1, LEN, NULL, 5, &sub);
    ERR(ESNULLP)

    EXPECT_BOS("empty substring")
    rc = u8fcu8_s(str1, LEN, str2, 5, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8fcu8_s(str1, 0, str2, 5, &sub);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str1, "Hello World");
    strcpy((char *)str2, "world");

    rc = u8fcu8_s(str1, LEN, str2, 5, &sub);
    ERR(EOK)
    if (!sub || strcmp((char *)sub, "World") != 0) {
        debug_printf("%s %u  sub=%s, expected \"World\"\n", __FUNCTION__,
                     __LINE__, sub ? (char *)sub : "(null)");
        errs++;
    }

    /*--------------------------------------------------*/

    /* case-insensitive match spanning a multi-byte utf-8 codepoint:
       Latin capital E with acute (U+00C9, C3 89) folds to the same
       codepoint as lowercase e-acute (U+00E9, C3 A9) */
    strcpy((char *)str1, "caf\xC3\x89 today");
    strcpy((char *)str2, "caf\xC3\xA9");

    rc = u8fcu8_s(str1, LEN, str2, 5, &sub);
    ERR(EOK)
    if (!sub || sub != str1) {
        debug_printf("%s %u  sub mismatch\n", __FUNCTION__, __LINE__);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str1, "Hello World");
    strcpy((char *)str2, "xyz");

    rc = u8fcu8_s(str1, LEN, str2, 3, &sub);
    ERR(ESNOTFND)
    if (sub != NULL) {
        debug_printf("%s %u  expected NULL substring\n", __FUNCTION__,
                     __LINE__);
        errs++;
    }

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/

    /* pattern longer than the remaining haystack */
    strcpy((char *)str1, "ab");
    strcpy((char *)str2, "abc");
    rc = u8fcu8_s(str1, LEN, str2, 3, &sub);
    ERR(ESNOTFND)
    if (sub != NULL) {
        debug_printf("%s %u  expected NULL substring\n", __FUNCTION__,
                     __LINE__);
        errs++;
    }

    /*--------------------------------------------------*/

    /* zero slen */
    rc = u8fcu8_s(str1, LEN, str2, vzero, &sub);
    ERR(ESZEROL)

#if defined(HAVE___BUILTIN_OBJECT_SIZE) && !defined(HAVE_CT_BOS_OVR)
    EXPECT_BOS("slen exceeds src")
    rc = u8fcu8_s(str1, LEN, str2, LEN + 1, &sub);
    ERR(EOVERFLOW)
#endif

    /*--------------------------------------------------*/

    /* illegal UTF-8 in the pattern */
    strcpy((char *)str1, "Hello");
    strcpy((char *)str2, "\xC0\x80");
    rc = u8fcu8_s(str1, LEN, str2, 2, &sub);
    ERR(EILSEQ)

    /* illegal UTF-8 in the haystack, reached while matching */
    strcpy((char *)str1, "a\xC0\x80z");
    strcpy((char *)str2, "az");
    rc = u8fcu8_s(str1, LEN, str2, 2, &sub);
    ERR(EILSEQ)

    /*--------------------------------------------------*/

    /* unknown dest and src sizes: dmax and slen exceeding RSIZE_MAX_STR */
    {
        static volatile size_t usize = 32;
        char8_t *ubuf = (char8_t *)malloc(usize);
        char8_t *usrc = (char8_t *)malloc(usize);
        if (ubuf && usrc) {
            strcpy((char *)ubuf, "Hello World");
            strcpy((char *)usrc, "WORLD");
            rc = u8fcu8_s(ubuf, RSIZE_MAX_STR + 1, usrc, 5, &sub);
            ERR(ESLEMAX)
            rc = u8fcu8_s(ubuf, usize, usrc, RSIZE_MAX_STR + 1, &sub);
            ERR(ESLEMAX)
            rc = u8fcu8_s(ubuf, usize, usrc, 5, &sub);
            ERR(EOK)
            if (sub != ubuf + 6) {
                debug_printf("%s %u  sub mismatch\n", __FUNCTION__, __LINE__);
                errs++;
            }
        }
        free(ubuf);
        free(usrc);
    }

    return (errs);
}

int main(void) { return (test_u8fcu8_s()); }
