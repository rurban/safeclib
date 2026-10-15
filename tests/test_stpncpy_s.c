/*------------------------------------------------------------------
 * test_stpncpy_s
 * File 'stpncpy_s.c'
 * Lines executed:94.62% of 93
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include "config.h"

#ifdef HAVE_STPNCPY_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif

#define MAX (128)
#define LEN (128)

static char str1[LEN];
static char str2[LEN];
static volatile size_t vzero = 0;

int test_strncpy_s(void);

int test_stpncpy_s(void) {
    errno_t rc;
    int len;
    char *dest;
    int errs = 0;
    rsize_t slen = LEN;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    dest = stpncpy_s(NULL, LEN, str2, slen, &rc);
    ERR(ESNULLP);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);

    EXPECT_BOS("empty dest or dmax")
    dest = stpncpy_s(str1, 0, str2, slen, &rc);
    ERR(ESZEROL);

    /*--------------------------------------------------*/

#ifndef HAVE_ASAN /* With asan no BOS? */
    if (_BOS_KNOWN(str1)) {
        EXPECT_BOS("dest overflow")
        dest = stpncpy_s(str1, LEN + 1, str2, slen, &rc);
        ERR(EOVERFLOW);   /* dmax exceeds dest */
        EXPSTR(str1, ""); /* cleared */
        CHECK_SLACK(str1, len);
    } else {
#ifdef HAVE___BUILTIN_OBJECT_SIZE
        debug_printf("%s %u  Warning unknown str1 size\n", __FUNCTION__,
                     __LINE__);
#endif
    }
#endif

    EXPECT_BOS("dest overflow")
    dest = stpncpy_s(str1, (RSIZE_MAX_STR + 1), str2, slen, &rc);
    ERR(ESLEMAX);

    EXPECT_BOS("dest overflow")
    dest = stpncpy_s(str1, (size_t)-1L, str2, slen, &rc);
    ERR(ESLEMAX);

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);

    EXPECT_BOS("empty src")
    dest = stpncpy_s(str1, 5, NULL, slen, &rc);
    ERR(ESNULLP);
    CHECK_SLACK(str1, 5);
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';
    slen = 10;

    dest = stpncpy_s(str1, LEN / 2, str2, slen, &rc);
    ERR(EOK)
    CHECK_SLACK(str1, LEN / 2);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    len = strlen(str1);

    /* test overlap */
    GCC_PUSH_WARN_RESTRICT
    dest = stpncpy_s(str1, LEN, str1, slen, &rc);
    GCC_POP_WARN_RESTRICT
    ERR(EOK); // OK if the same
    EXPSTR(str1, "aaaaa");

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    slen = strlen(str1);

    dest = stpncpy_s(&str1[0], LEN, &str1[5], slen - 5, &rc);
    ERR(ESOVRLP); // TODO
    CHECK_SLACK(str1, LEN);

    /*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");
    slen = strlen(str1);

    dest = stpncpy_s(&str1[5], LEN - 5, &str1[0], slen, &rc);
    ERR(ESOVRLP);
    CHECK_SLACK(&str1[5], LEN - 5);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    slen = len = strlen(str1);

    dest = stpncpy_s(str2, LEN, str1, slen, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str2[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");
    str2[0] = '\0';
    slen = 10;

    dest = stpncpy_s(str1, LEN, str2, slen, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(str1, LEN);

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(&str2[0], "keep it simple");
    slen = len = strlen(str2);

    dest = stpncpy_s(str1, LEN, str2, slen, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    slen = len = strlen(str2);

    dest = stpncpy_s(str1, LEN, str2, slen, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    slen = strlen(str2);

    dest = stpncpy_s(str1, 1, str2, slen, &rc);
    ERR(ESNOSPC);
    CHECK_SLACK(str1, 1);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    dest = stpncpy_s(str1, 2, str2, slen, &rc);
    ERR(ESNOSPC);
    CHECK_SLACK(str1, 2);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "it");

    dest = stpncpy_s(str1, 3, str2, 2, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[2], 1);

    /*--------------------------------------------------*/

    strcpy(str1, "qq12345weqeqeqeq");
    strcpy(str2, "it");

    dest = stpncpy_s(str1, 10, str2, 2, &rc);
    ERR(EOK);
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[2], 8);
    /*for (i=0; i<10; i++) {
        debug_printf("str1[%i] = %d \n", i, str1[i]);
    }*/
    /*--------------------------------------------------*/

    (void)dest;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty errp")
    dest = stpncpy_s(str1, LEN, str2, slen, NULL);
    if (dest != NULL) {
        errs++;
    }
#endif

    /*--------------------------------------------------*/

    /* dest == src without a terminator within dmax */
    strcpy(str1, "aaaaa");
    GCC_PUSH_WARN_RESTRICT
    dest = stpncpy_s(str1, 3, str1, 3, &rc);
    GCC_POP_WARN_RESTRICT
    ERR(ESNOSPC);
    if (dest != NULL) {
        errs++;
    }

    /*--------------------------------------------------*/

    /* slen == 0 with dest below and above src: empty result */
    {
        char buf[64];

        memset(buf, 'x', sizeof(buf));
        strcpy(&buf[32], "abc");
        dest = stpncpy_s(&buf[0], 16, &buf[32], vzero, &rc);
        ERR(EOK);
        if (dest != &buf[0]) {
            errs++;
        }
#ifdef SAFECLIB_STR_NULL_SLACK
        if (buf[0] != '\0') {
            errs++;
        }
#endif

        memset(buf, 'x', sizeof(buf));
        strcpy(&buf[0], "abc");
        dest = stpncpy_s(&buf[32], 16, &buf[0], vzero, &rc);
        ERR(EOK);
        if (dest == NULL) {
            errs++;
        }
#ifdef SAFECLIB_STR_NULL_SLACK
        if (buf[32] != '\0') {
            errs++;
        }
#endif
    }

    /*--------------------------------------------------*/

    /* dmax > RSIZE_MAX_STR with unknown dest size, slen > RSIZE_MAX_STR
       with unknown src size */
    {
        static volatile size_t hsz = 16;
        char *hp = (char *)malloc(hsz);
        char *hs = (char *)malloc(hsz);

        if (!hp || !hs) {
            return errs + 1;
        }
        memset(hp, 'x', hsz - 1);
        hp[hsz - 1] = '\0';
        memset(hs, 'y', hsz - 1);
        hs[hsz - 1] = '\0';
        dest = stpncpy_s(hp, RSIZE_MAX_STR + 1, hs, 4, &rc);
        ERR(ESLEMAX);
        if (dest != NULL) {
            errs++;
        }
        dest = stpncpy_s(hp, 16, hs, RSIZE_MAX_STR + 1, &rc);
        ERR(ESLEMAX);
        if (dest != NULL) {
            errs++;
        }
        free(hp);
        free(hs);
    }

    /*--------------------------------------------------*/

    /* slen exceeds the known size of src, unknown at compile-time */
    {
        static volatile size_t xlen = 9;
        char s4[4];

        memcpy(s4, "abc", 4);
        dest = stpncpy_s(str1, LEN, s4, xlen, &rc);
        if (dest != NULL || rc == EOK) {
            debug_printf("%s %u  Error rc=%d \n", __FUNCTION__, __LINE__,
                         (int)rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    /* unterminated src of known size copied fully: no room for the NUL,
       the two orders put dest once below and once above src */
    {
        static char p[4];
        static char q[4];

        memset(p, 'x', sizeof(p));
        memset(q, 'y', sizeof(q));
        dest = stpncpy_s(p, 4, q, 4, &rc);
        ERR(ESNOSPC);
        if (dest != NULL) {
            errs++;
        }

        /* the failed call above nulled p */
        memset(p, 'x', sizeof(p));
        memset(q, 'y', sizeof(q));
        dest = stpncpy_s(q, 4, p, 4, &rc);
        ERR(ESNOSPC);
        if (dest != NULL) {
            errs++;
        }
    }

    /*--------------------------------------------------*/

    /* terminated src in a larger known-size array, slen == sizeof array;
       the return value points at the written NUL */
    {
        char arr4[4] = "ab";
        char arr2[4] = "ab";
        char d[16];

        memset(d, 'x', sizeof(d));
        dest = stpncpy_s(d, 16, arr4, 4, &rc);
        ERR(EOK);
        EXPSTR(d, "ab");
        if (dest != &d[2]) {
            debug_printf("%s %u  wrong ptr\n", __FUNCTION__, __LINE__);
            errs++;
        }
        /* dest above src */
        memset(d, 'x', sizeof(d));
        dest = stpncpy_s(&d[8], 8, arr2, 4, &rc);
        ERR(EOK);
        EXPSTR(&d[8], "ab");
        if (dest != &d[10]) {
            errs++;
        }
    }

    /*--------------------------------------------------*/

    /* return value points at the written NUL, not at the dest end */
    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep");
    dest = stpncpy_s(str1, LEN, str2, 4, &rc);
    ERR(EOK);
    EXPSTR(str1, "keep");
    if (dest != &str1[4]) {
        errs++;
    }
    strcpy(str1, "qqweqeqeqeq");
    dest = stpncpy_s(str1, LEN, str2, 2, &rc);
    ERR(EOK);
    EXPSTR(str1, "ke");
    if (dest != &str1[2]) {
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_stpncpy_s()); }
#endif
