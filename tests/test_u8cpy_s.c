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

    rc = u8cpy_s(dest, LEN,
                 (char8_t *)"\x80"
                            "bc");
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
    /* same pointer is a no-op */
    strcpy((char *)dest, "same");
    rc = u8cpy_s(dest, LEN, dest);
    ERR(EOK)
    EXPSTR((char *)dest, "same")

    /*--------------------------------------------------*/
    /* src above dest in one buffer (dest < src branch) */
    {
        static char8_t buf[256];
        char8_t *d = buf, *s = buf + 100;

        memset(buf, 0, sizeof(buf));
        strcpy((char *)s, "h\xC3\xA9llo");
        rc = u8cpy_s(d, 64, s);
        ERR(EOK)
        EXPSTR((char *)d, "h\xC3\xA9llo")
        rc = u8cpy_s(d, 16, s);
        ERR(EOK)
        EXPSTR((char *)d, "h\xC3\xA9llo")

        strcpy((char *)s, "a\xE2\x9C");
        rc = u8cpy_s(d, 64, s);
        ERR(EILSEQ)

        strcpy((char *)s, "abcdef");
        rc = u8cpy_s(d, 4, s);
        ERR(ESNOSPC)

        /* src below dest: not enough space */
        strcpy((char *)buf, "abcdef");
        rc = u8cpy_s(buf + 100, 4, buf);
        ERR(ESNOSPC)

        /* src below dest: ok */
        strcpy((char *)buf, "h\xC3\xA9llo");
        rc = u8cpy_s(buf + 100, 64, buf);
        ERR(EOK)
        EXPSTR((char *)(buf + 100), "h\xC3\xA9llo")

        /* src below dest: illegal sequence */
        strcpy((char *)buf, "\xF4\x90\x80\x80");
        rc = u8cpy_s(buf + 100, 64, buf);
        ERR(EILSEQ)

        /* src below dest and runs into dest */
        memset(buf, 'a', 10);
        rc = u8cpy_s(buf + 3, 64, buf);
        ERR(ESOVRLP)
    }
    /*--------------------------------------------------*/
    /* unknown dest size: dmax > RSIZE_MAX_STR */
    {
        static volatile size_t usize = 32;
        char8_t *ubuf = (char8_t *)malloc(usize);
        if (ubuf) {
            rc = u8cpy_s(ubuf, RSIZE_MAX_STR + 1, (char8_t *)"a");
            ERR(ESLEMAX)
            rc = u8cpy_s(ubuf, usize, (char8_t *)"ab");
            ERR(EOK)
            EXPSTR((char *)ubuf, "ab")
            free(ubuf);
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8cpy_s()); }
