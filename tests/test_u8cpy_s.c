/*------------------------------------------------------------------
 * test_u8cpy_s
 * File 'extu8/u8cpy_s.c'
 * Lines executed:45.90% of 61
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t dest[LEN];
int test_u8cpy_s(void);

int test_u8cpy_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/
    /* basic copy */

    rc = u8cpy_s(dest, LEN, (char8_t *)"hello");
    ERR(EOK)
    EXPSTR((char *)dest, "hello")

    /*--------------------------------------------------*/
    /* valid multi-byte UTF-8 is copied untouched */

    rc = u8cpy_s(dest, LEN, (char8_t *)"h\xC3\xA9llo \xE2\x9C\x93");
    ERR(EOK)
    EXPSTR((char *)dest, "h\xC3\xA9llo \xE2\x9C\x93")

    /*--------------------------------------------------*/
    /* overlong 2-byte encoding of NUL (0xC0 0x80) is illegal UTF-8 */

    rc = u8cpy_s(dest, LEN, (char8_t *)"\xC0\x80");
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* truncated 3-byte sequence (missing final continuation byte) */

    rc = u8cpy_s(dest, LEN, (char8_t *)"a\xE2\x9C");
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* stray continuation byte with no lead byte */

    rc = u8cpy_s(dest, LEN, (char8_t *)"\x80" "bc");
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* encoded surrogate half (0xED 0xA0 0x80 = U+D800) is illegal */

    rc = u8cpy_s(dest, LEN, (char8_t *)"\xED\xA0\x80");
    ERR(EILSEQ)

    /*--------------------------------------------------*/
    /* codepoint beyond U+10FFFF (0xF4 0x90 0x80 0x80 = U+110000) */

    rc = u8cpy_s(dest, LEN, (char8_t *)"\xF4\x90\x80\x80");
    ERR(EILSEQ)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8cpy_s()); }
