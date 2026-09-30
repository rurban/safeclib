/*------------------------------------------------------------------
 * test_u8cat_s
 * File 'extu8/u8cat_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
int test_u8cat_s(void);

int test_u8cat_s(void) {
    errno_t rc;
    int errs = 0;
    /*--------------------------------------------------*/
    /* basic concatenation */

    strcpy((char *)str1, "hello ");
    rc = u8cat_s(str1, LEN, (char8_t *)"world");
    ERR(EOK)
    EXPSTR((char *)str1, "hello world")

    /*--------------------------------------------------*/
    /* valid multi-byte UTF-8 is appended untouched */

    strcpy((char *)str1, "check: ");
    rc = u8cat_s(str1, LEN, (char8_t *)"\xE2\x9C\x93"); /* U+2713 CHECK MARK */
    ERR(EOK)
    EXPSTR((char *)str1, "check: \xE2\x9C\x93")
    /*--------------------------------------------------*/
    /* overlong 2-byte encoding of NUL (0xC0 0x80) is illegal UTF-8 */

    strcpy((char *)str1, "x");
    rc = u8cat_s(str1, LEN, (char8_t *)"\xC0\x80");
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* truncated 3-byte sequence (missing final continuation byte) */

    strcpy((char *)str1, "x");
    rc = u8cat_s(str1, LEN, (char8_t *)"a\xE2\x9C");
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* stray continuation byte with no lead byte */

    strcpy((char *)str1, "x");
    rc = u8cat_s(str1, LEN, (char8_t *)"\x80" "bc");
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* encoded surrogate half (0xED 0xA0 0x80 = U+D800) is illegal */

    strcpy((char *)str1, "x");
    rc = u8cat_s(str1, LEN, (char8_t *)"\xED\xA0\x80");
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* codepoint beyond U+10FFFF (0xF4 0x90 0x80 0x80 = U+110000) */

    strcpy((char *)str1, "x");
    rc = u8cat_s(str1, LEN, (char8_t *)"\xF4\x90\x80\x80");
    ERR(EILSEQ)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8cat_s()); }
