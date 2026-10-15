/*------------------------------------------------------------------
 * test_u8cat_s
 * File 'extu8/u8cat_s.c'
 * Lines executed:98.65% of 74
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
    rc = u8cat_s(str1, LEN,
                 (char8_t *)"\x80"
                            "bc");
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
    /* src above dest in one buffer (dest < src branch) */
    {
        static char8_t buf[256];
        char8_t *d = buf, *s = buf + 100;

        memset(buf, 0, sizeof(buf));
        strcpy((char *)d, "ab");
        strcpy((char *)s, "c\xE2\x9C\x93");
        rc = u8cat_s(d, 16, s);
        ERR(EOK)
        EXPSTR((char *)d, "abc\xE2\x9C\x93")

        /* illegal sequence in src */
        memset(buf, 0, sizeof(buf));
        strcpy((char *)d, "ab");
        strcpy((char *)s, "\xC0\x80");
        rc = u8cat_s(d, 64, s);
        ERR(EILSEQ)

        /* not enough space: "ab" + "cdef" in 4 */
        memset(buf, 0, sizeof(buf));
        strcpy((char *)d, "ab");
        strcpy((char *)s, "cdef");
        rc = u8cat_s(d, 4, s);
        ERR(ESNOSPC)

        /* dest unterminated within dmax */
        memset(buf, 'a', 100);
        strcpy((char *)s, "cd");
        rc = u8cat_s(d, 10, s);
        ERR(ESUNTERM)

        /* dest runs into src before its terminator */
        memset(buf, 'a', 100);
        memset(s, 'x', 20);
        rc = u8cat_s(d, 200, s);
        ERR(ESOVRLP)

        /* src is the terminator of dest */
        memset(buf, 0, sizeof(buf));
        strcpy((char *)d, "ab");
        rc = u8cat_s(d, 64, d + 2);
        ERR(ESOVRLP)

        /* src below dest: not enough space */
        memset(buf, 0, sizeof(buf));
        strcpy((char *)(buf + 100), "ab");
        strcpy((char *)buf, "cdef");
        rc = u8cat_s(buf + 100, 4, buf);
        ERR(ESNOSPC)

        /* src below dest, dest unterminated */
        memset(buf, 0, sizeof(buf));
        memset(buf + 100, 'b', 20);
        strcpy((char *)buf, "cd");
        rc = u8cat_s(buf + 100, 10, buf);
        ERR(ESUNTERM)

        /* src below dest and runs into dest */
        memset(buf, 0, sizeof(buf));
        memset(buf, 'a', 5);
        memset(buf + 5, 'b', 2);
        rc = u8cat_s(buf + 5, 100, buf);
        ERR(ESOVRLP)

        /* src below dest: illegal sequence */
        memset(buf, 0, sizeof(buf));
        strcpy((char *)(buf + 100), "ab");
        strcpy((char *)buf, "\xE2\x9C");
        rc = u8cat_s(buf + 100, 64, buf);
        ERR(EILSEQ)
    }
    /*--------------------------------------------------*/
    /* unknown dest size: dmax > RSIZE_MAX_STR */
    {
        static volatile size_t usize = 32;
        char8_t *ubuf = (char8_t *)malloc(usize);
        if (ubuf) {
            ubuf[0] = 0;
            rc = u8cat_s(ubuf, RSIZE_MAX_STR + 1, (char8_t *)"a");
            ERR(ESLEMAX)
            rc = u8cat_s(ubuf, usize, (char8_t *)"ab");
            ERR(EOK)
            EXPSTR((char *)ubuf, "ab")
            free(ubuf);
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8cat_s()); }
