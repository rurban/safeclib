/*------------------------------------------------------------------
 * test_u8ncpy_s
 * File 'extu8/u8ncpy_s.c'
 * Lines executed:100.00% of 87
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t dest[LEN];
int test_u8ncpy_s(void);

/* dest and src live in one buffer, so that their address order (dest < src,
   or dest > src) selects the branch in u8ncpy_s, and so that overlapping
   objects can be constructed. */
static int check_layout(int fwd) {
    static char8_t buf[256];
    char8_t *d = fwd ? buf : buf + 128;
    char8_t *s = fwd ? buf + 128 : buf;
    errno_t rc;
    int errs = 0;
    rsize_t dmaxs[2] = {128, 16}; /* memset and per byte slack paths */
    int i;

    for (i = 0; i < 2; i++) {
        rsize_t dm = dmaxs[i];

        /* src ends before slen */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "yo");
        rc = u8ncpy_s(d, dm, s, 10);
        ERR(EOK)
        EXPSTR((char *)d, "yo")
        CHECK_SLACK(&d[3], dm - 3);

        /* slen truncates before a multi-byte character */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "a\xC3\xA9");
        rc = u8ncpy_s(d, dm, s, 2);
        ERR(EOK)
        EXPSTR((char *)d, "a")
        CHECK_SLACK(&d[1], dm - 1);

        /* slen == 0 */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "yo");
        rc = u8ncpy_s(d, dm, s, 0);
        ERR(EOK)
        EXPSTR((char *)d, "")

        /* illegal UTF-8 in src */
        strcpy((char *)s, "ab\xFF" "cd");
        rc = u8ncpy_s(d, dm, s, 5);
        ERR(EILSEQ)
    }

    /* not enough room for src */
    strcpy((char *)s, "abcdef");
    rc = u8ncpy_s(d, 3, s, 6);
    ERR(ESNOSPC)

    /* overlapping objects */
    memset(buf, 'X', sizeof(buf));
    if (fwd) {
        d = buf;
        s = buf + 2;
    } else {
        d = buf + 2;
        s = buf;
    }
    strcpy((char *)buf, "abcdefgh");
    rc = u8ncpy_s(d, 16, s, 8);
    ERR(ESOVRLP)

    return errs;
}


int test_u8ncpy_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/
    /* basic copy */

    rc = u8ncpy_s(dest, LEN, (char8_t *)"hello", 5);
    ERR(EOK)
    EXPSTR((char *)dest, "hello")

    /*--------------------------------------------------*/
    /* valid multi-byte UTF-8 is copied untouched */

    {
        char8_t src[] = "h\xC3\xA9llo";
        rc = u8ncpy_s(dest, LEN, src, sizeof(src) - 1);
        ERR(EOK)
        EXPSTR((char *)dest, "h\xC3\xA9llo")
    }

    /*--------------------------------------------------*/
    /* overlong 2-byte encoding of NUL (0xC0 0x80) is illegal UTF-8 */

    {
        char8_t src[] = {(char8_t)0xC0, (char8_t)0x80, 0};
        rc = u8ncpy_s(dest, LEN, src, 2);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* encoded surrogate half (0xED 0xA0 0x80 = U+D800) is illegal */

    {
        char8_t src[] = {(char8_t)0xED, (char8_t)0xA0, (char8_t)0x80, 0};
        rc = u8ncpy_s(dest, LEN, src, 3);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* slen truncation must stop at a character boundary, not split a
       multi-byte sequence: "a" + e-acute (0xC3 0xA9) + "b", slen=2
       cannot fit the 2-byte e-acute after 'a', so only "a" is copied,
       never a dangling lead byte 0xC3. */

    {
        char8_t src[] = "a\xC3\xA9"
                        "b";
        memset(dest, 'X', sizeof(dest));
        rc = u8ncpy_s(dest, LEN, src, 2);
        ERR(EOK)
        EXPSTR((char *)dest, "a")
        if ((unsigned char)dest[1] != 0) {
            debug_printf("%s %u  dangling byte after truncation: %02X\n",
                         __FUNCTION__, __LINE__, (unsigned char)dest[1]);
            errs++;
        }
    }

    /*--------------------------------------------------*/
    /* slen exactly fits the multi-byte character */

    {
        char8_t src[] = "a\xC3\xA9"
                        "b";
        rc = u8ncpy_s(dest, LEN, src, 3);
        ERR(EOK)
        EXPSTR((char *)dest, "a\xC3\xA9")
    }

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* both address orders of dest and src */

    errs += check_layout(1);
    errs += check_layout(0);

    /*--------------------------------------------------*/
    /* constraint violations */

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8ncpy_s(NULL, LEN, (char8_t *)"abc", 3);
    ERR(ESNULLP)

    EXPECT_BOS("empty dmax")
    rc = u8ncpy_s(dest, 0, (char8_t *)"abc", 3);
    ERR(ESZEROL)

    EXPECT_BOS("empty src")
    rc = u8ncpy_s(dest, LEN, NULL, 3);
    ERR(ESNULLP)

    EXPECT_BOS("slen exceeds max")
    rc = u8ncpy_s(dest, LEN, (char8_t *)"abc", RSIZE_MAX_STR + 1);
    ERR(ESLEMAX)

    EXPECT_BOS("slen exceeds src")
    rc = u8ncpy_s(dest, LEN, (char8_t *)"abc", 10);
    ERR(EOVERFLOW)
#endif

    /*--------------------------------------------------*/
    /* dmax > RSIZE_MAX_STR is only checked for a dest of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        if (!hp)
            return errs + 1;
        rc = u8ncpy_s(hp, RSIZE_MAX_STR + 1, (char8_t *)"abc", 3);
        ERR(ESLEMAX)
        rc = u8ncpy_s(hp, hsz, (char8_t *)"abc", 3);
        ERR(EOK)
        EXPSTR((char *)hp, "abc")
        free(hp);
    }

    return (errs);
}

int main(void) { return (test_u8ncpy_s()); }
