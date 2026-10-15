/*------------------------------------------------------------------
 * test_u8lwr_s
 * File 'extu8/u8lwr_s.c'
 * Lines executed:93.02% of 43
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str[LEN];
int test_u8lwr_s(void);

int test_u8lwr_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8lwr_s(NULL, LEN);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8lwr_s(str, 0);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8lwr_s(str, RSIZE_MAX_STR + 1);
    ERR(ESLEMAX)
#endif

    /*--------------------------------------------------*/

    strcpy((char *)str, "HELLO");
    rc = u8lwr_s(str, LEN);
    ERR(EOK)
    if (strcmp((char *)str, "hello") != 0) {
        debug_printf("%s %u  str=%s, expected hello\n", __FUNCTION__, __LINE__,
                     str);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str, "Hello World");
    rc = u8lwr_s(str, LEN);
    ERR(EOK)
    if (strcmp((char *)str, "hello world") != 0) {
        debug_printf("%s %u  str=%s, expected 'hello world'\n", __FUNCTION__,
                     __LINE__, str);
        errs++;
    }

    /*--------------------------------------------------*/

    /* multibyte content whose case mapping is locale-dependent (only
       ASCII is guaranteed under the default "C" locale) passes through
       unchanged bytes for the non-ASCII sequence itself */
    strcpy((char *)str, "CAF\xC3\x89 TOWN");
    rc = u8lwr_s(str, LEN);
    ERR(EOK)
    if (strncmp((char *)str, "caf", 3) != 0 ||
        strcmp((char *)str + 5, " town") != 0) {
        debug_printf("%s %u  str=%s, expected ascii runs lowered\n",
                     __FUNCTION__, __LINE__, str);
        errs++;
    }
    /*--------------------------------------------------*/

    /* already lowercase: unchanged */
    strcpy((char *)str, "already lower");
    rc = u8lwr_s(str, LEN);
    ERR(EOK)
    if (strcmp((char *)str, "already lower") != 0) {
        debug_printf("%s %u  str=%s, expected 'already lower'\n", __FUNCTION__,
                     __LINE__, str);
        errs++;
    }

    /*--------------------------------------------------*/

    /* result must fit: too small dmax for the actual content byte-length */
    strcpy((char *)str, "HELLO");
    rc = u8lwr_s(str, 3);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/

    /* full dmax uses the heap scratch buffer */
    strcpy((char *)str, "HEAP Buffer");
    rc = u8lwr_s(str, LEN);
    ERR(EOK)
    if (strcmp((char *)str, "heap buffer") != 0) {
        debug_printf("%s %u  str=%s, expected 'heap buffer'\n", __FUNCTION__,
                     __LINE__, str);
        errs++;
    }

    /*--------------------------------------------------*/

    /* illegal UTF-8 (overlong NUL), stack and heap scratch buffers */
    strcpy((char *)str, "A\xC0\x80");
    rc = u8lwr_s(str, 4);
    ERR(EILSEQ)
    strcpy((char *)str, "A\xC0\x80");
    rc = u8lwr_s(str, LEN);
    ERR(EILSEQ)

    /*--------------------------------------------------*/

    /* unknown dest size: dmax > RSIZE_MAX_STR */
    {
        static volatile size_t usize = 64;
        char8_t *ubuf = (char8_t *)malloc(usize);
        if (ubuf) {
            strcpy((char *)ubuf, "ABC");
            rc = u8lwr_s(ubuf, RSIZE_MAX_STR + 1);
            ERR(ESLEMAX)
            rc = u8lwr_s(ubuf, usize);
            ERR(EOK)
            EXPSTR((char *)ubuf, "abc")
            free(ubuf);
        }
    }

    return (errs);
}

int main(void) { return (test_u8lwr_s()); }
