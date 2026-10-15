/*------------------------------------------------------------------
 * test_u8ncat_s
 * File 'extu8/u8ncat_s.c'
 * Lines executed:100.00% of 117
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t dest[LEN];
int test_u8ncat_s(void);

/* dest and src live in one buffer, so that their address order (dest < src,
   or dest > src) selects the branch in u8ncat_s, and so that overlapping
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
        strcpy((char *)d, "hi");
        strcpy((char *)s, "yo");
        rc = u8ncat_s(d, dm, s, 10);
        ERR(EOK)
        EXPSTR((char *)d, "hiyo")
        CHECK_SLACK_FILLED(&d[5], dm - 5);

        /* slen exhausted before the end of src */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)d, "hi");
        strcpy((char *)s, "yoyo");
        rc = u8ncat_s(d, dm, s, 2);
        ERR(EOK)
        EXPSTR((char *)d, "hiyo")
        CHECK_SLACK_FILLED(&d[5], dm - 5);

        /* slen truncates before a multi-byte character */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)d, "hi");
        strcpy((char *)s, "a\xC3\xA9");
        rc = u8ncat_s(d, dm, s, 2);
        ERR(EOK)
        EXPSTR((char *)d, "hia")
        CHECK_SLACK_FILLED(&d[4], dm - 4);

        /* slen == 0 */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)d, "hi");
        strcpy((char *)s, "yo");
        rc = u8ncat_s(d, dm, s, 0);
        ERR(EOK)
        EXPSTR((char *)d, "hi")
        CHECK_SLACK_FILLED(&d[3], dm - 3);

        /* illegal UTF-8 in src */
        strcpy((char *)d, "hi");
        strcpy((char *)s, "ab\xFF"
                          "cd");
        rc = u8ncat_s(d, dm, s, 5);
        ERR(EILSEQ)
    }

    /* dest not terminated within dmax */
    memset(buf, 'X', sizeof(buf));
    memset(d, 'a', 4);
    strcpy((char *)s, "yo");
    rc = u8ncat_s(d, 4, s, 2);
    ERR(ESUNTERM)

    /* overlapping objects */
    memset(buf, 'X', sizeof(buf));
    if (fwd) {
        /* src starts inside the dest string */
        d = buf;
        s = buf + 3;
        strcpy((char *)buf, "abcdef");
        rc = u8ncat_s(d, 16, s, 3);
        ERR(ESOVRLP)
        /* src starts at the end of dest */
        strcpy((char *)buf, "ab");
        s = buf + 2;
        rc = u8ncat_s(d, 16, s, 3);
        ERR(ESOVRLP)
    } else {
        d = buf + 2;
        s = buf;
        strcpy((char *)buf, "abcd");
        rc = u8ncat_s(d, 16, s, 8);
        ERR(ESOVRLP)
    }

    return errs;
}

int test_u8ncat_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/
    /* basic append, slen bigger than src's actual content but not
       bigger than its known object size */

    strcpy((char *)dest, "hello ");
    rc = u8ncat_s(dest, LEN, (char8_t *)"world", 6);
    ERR(EOK)
    EXPSTR((char *)dest, "hello world")

    /*--------------------------------------------------*/
    /* slen truncates the source before its NUL */

    strcpy((char *)dest, "hello ");
    rc = u8ncat_s(dest, LEN, (char8_t *)"world!!!", 5);
    ERR(EOK)
    EXPSTR((char *)dest, "hello world")

    /*--------------------------------------------------*/
    /* slen truncation must stop at a UTF-8 character boundary, not
       split a multi-byte sequence: "a" + e-acute (0xC3 0xA9) + "b" */

    {
        char8_t src[] = "a\xC3\xA9"
                        "b";
        strcpy((char *)dest, "x");
        rc = u8ncat_s(dest, LEN, src, 2);
        ERR(EOK)
        EXPSTR((char *)dest, "xa")
        if ((unsigned char)dest[2] != 0) {
            debug_printf("%s %u  dangling byte after truncation: %02X\n",
                         __FUNCTION__, __LINE__, (unsigned char)dest[2]);
            errs++;
        }
    }

    /*--------------------------------------------------*/
    /* illegal UTF-8 (overlong encoding of NUL) */

    {
        char8_t src[] = {(char8_t)0xC0, (char8_t)0x80, 0};
        strcpy((char *)dest, "x");
        rc = u8ncat_s(dest, LEN, src, 2);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* dest not big enough */

    strcpy((char *)dest, "hello");
    rc = u8ncat_s(dest, 7, (char8_t *)"world", 6);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* both address orders of dest and src */

    errs += check_layout(1);
    errs += check_layout(0);

    /*--------------------------------------------------*/
    /* constraint violations */

    rc = u8ncat_s(NULL, 0, (char8_t *)"abc", 0);
    ERR(EOK)

#ifndef HAVE_CT_BOS_OVR
    strcpy((char *)dest, "hi");
    EXPECT_BOS("empty dest")
    rc = u8ncat_s(NULL, LEN, (char8_t *)"abc", 3);
    ERR(ESNULLP)

    EXPECT_BOS("empty dmax")
    rc = u8ncat_s(dest, 0, (char8_t *)"abc", 3);
    ERR(ESZEROL)

    EXPECT_BOS("empty src")
    rc = u8ncat_s(dest, LEN, NULL, 3);
    ERR(ESNULLP)

    EXPECT_BOS("slen exceeds max")
    rc = u8ncat_s(dest, LEN, (char8_t *)"abc", RSIZE_MAX_STR + 1);
    ERR(ESLEMAX)

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    strcpy((char *)dest, "hi");
    EXPECT_BOS("slen exceeds src")
    rc = u8ncat_s(dest, LEN, (char8_t *)"abc", 10);
    ERR(EOVERFLOW)
#endif
#endif

    /*--------------------------------------------------*/
    /* dmax > RSIZE_MAX_STR is only checked for a dest of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        if (!hp)
            return errs + 1;
        strcpy((char *)hp, "x");
        rc = u8ncat_s(hp, RSIZE_MAX_STR + 1, (char8_t *)"abc", 3);
        ERR(ESLEMAX)
        rc = u8ncat_s(hp, hsz, (char8_t *)"abc", 3);
        ERR(EOK)
        EXPSTR((char *)hp, "xabc")
        free(hp);
    }

    return (errs);
}

int main(void) { return (test_u8ncat_s()); }
