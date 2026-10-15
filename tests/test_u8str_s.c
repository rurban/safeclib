/*------------------------------------------------------------------
 * test_u8str_s
 * File 'extu8/u8str_s.c'
 * Lines executed:100.00% of 47
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8str_s(void);

int test_u8str_s(void) {
    errno_t rc;
    char8_t *sub;
    int errs = 0;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8str_s(NULL, LEN, str2, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty src")
    rc = u8str_s(str1, LEN, NULL, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty substringp")
    rc = u8str_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8str_s(str1, 0, str2, LEN, &sub);
    ERR(ESZEROL)
    SUBNULL()
#endif

    strcpy((char *)str1, "caf\xC3\xA9 world");
    strcpy((char *)str2, "\xC3\xA9");

    /* multibyte sequence at codepoint boundary */
    rc = u8str_s(str1, LEN, str2, sizeof("\xC3\xA9"), &sub);
    ERR(EOK)
    PTREQ(sub, &str1[3])

    /* substring in the middle */
    strcpy((char *)str2, "world");
    rc = u8str_s(str1, LEN, str2, sizeof("world"), &sub);
    ERR(EOK)
    PTREQ(sub, &str1[6])

    /* empty source and source equal to dest return dest */
    *str2 = '\0';
    rc = u8str_s(str1, LEN, str2, 0, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    rc = u8str_s(str1, LEN, str1, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    strcpy((char *)str2, "xyz");
    rc = u8str_s(str1, LEN, str2, sizeof("xyz"), &sub);
    ERR(ESNOTFND)
    SUBNULL()

    /* a short bounded destination cannot contain the substring */
    strcpy((char *)str2, "world");
    rc = u8str_s(str1, 6, str2, sizeof("world"), &sub);
    ERR(ESNOTFND)
    SUBNULL()

    /* slen 0 with a non-empty src */
    strcpy((char *)str1, "caf\xC3\xA9 world");
    strcpy((char *)str2, "abc");
    rc = u8str_s(str1, LEN, str2, 0, &sub);
    ERR(ESZEROL)
    SUBNULL()

    /* slen > dmax, the actual src is longer than dmax */
    strcpy((char *)str2, "world");
    rc = u8str_s(str1, 4, str2, 6, &sub);
    ERR(ESNOTFND)
    SUBNULL()

    /* slen > dmax, but the actual src fits */
    strcpy((char *)str2, "ca");
    rc = u8str_s(str1, 4, str2, 6, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

#ifndef HAVE_CT_BOS_OVR
#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("slen exceeds src")
    rc = u8str_s(str1, LEN, (char8_t *)"ab", 10, &sub);
    ERR(EOVERFLOW)
    SUBNULL()
#endif

    EXPECT_BOS("slen exceeds max")
    rc = u8str_s(str1, LEN, (char8_t *)"ab", RSIZE_MAX_STR + 1, &sub);
    ERR(ESLEMAX)
    SUBNULL()
#endif

    /* dmax/slen > RSIZE_MAX_STR is only checked for objects of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        if (!hp)
            return errs + 1;
        strcpy((char *)hp, "ab");
        rc = u8str_s(hp, RSIZE_MAX_STR + 1, str2, 3, &sub);
        ERR(ESLEMAX)
        SUBNULL()
        rc = u8str_s(str1, LEN, hp, RSIZE_MAX_STR + 1, &sub);
        ERR(ESLEMAX)
        SUBNULL()
        strcpy((char *)str1, "xxab");
        rc = u8str_s(str1, LEN, hp, 3, &sub);
        ERR(EOK)
        PTREQ(sub, &str1[2])
        free(hp);
    }

    return (errs);
}

int main(void) { return (test_u8str_s()); }
