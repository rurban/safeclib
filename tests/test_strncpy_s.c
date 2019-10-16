/*------------------------------------------------------------------
 * test_strncpy_s
 * File 'str/strncpy_s.c'
 * Lines executed:95.83% of 72
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#if defined(TEST_MSVCRT) && defined(HAVE_STRNCPY_S)
#undef HAVE_CT_BOS_OVR
#undef strncpy_s
EXTERN errno_t strncpy_s(char *restrict dest, rsize_t dmax,
                         const char *restrict src, rsize_t slen);
#endif

#ifdef HAVE_STRNCPY_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX (128)
#define LEN (128)

static char str1[LEN];
static char str2[LEN];
static char dest[LEN];
int test_strncpy_s(void);

int test_strncpy_s(void) {
    errno_t rc;
    rsize_t nlen;
    int errs = 0;

    /*--------------------------------------------------*/

    nlen = 5;

#if defined(TEST_MSVCRT) && defined(HAVE_STRNCPY_S)
    use_msvcrt = true;
#endif
    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strncpy_s(NULL, LEN, str2, nlen);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    strcpy(str2, "b");

    EXPECT_BOS("empty dest")
    rc = strncpy_s(NULL, LEN, str2, 5);
    ERR_MSVC(ESNULLP, EINVAL); /* and untouched */

    EXPECT_BOS("empty dest")
    rc = strncpy_s(NULL, LEN, str2, 0);
    ERR_MSVC(ESNULLP, 0); /* and untouched */

    EXPECT_BOS("empty dest or dmax")
    rc = strncpy_s(str1, 0, str2, 0);
    ERR_MSVC(ESZEROL, 0); /* and untouched */
    EXPSTR(str1, "aaaaa")
#endif

    strcpy(str1, "aaaaa");
    rc = strncpy_s(str1, 5, NULL, 0);
    ERR(EOK); /* and cleared */
    EXPSTR(str1, "")

    /*--------------------------------------------------*/
    strcpy(str1, "aaaaa");
    rc = strncpy_s(str1, LEN, str2, 0);
    ERR(EOK); /* and cleared */
    EXPSTR(str1, "")

    strcpy(str1, "aaaaa");
    rc = strncpy_s(str1, 1, str2, 0);
    ERR(EOK) /* and cleared */
    EXPSTR(str1, "")

    strcpy(str1, "aaaaa");

#ifndef HAVE_CT_BOS_OVR
    nlen = 5;
    /*EXPECT_BOS("empty buf")*/
    rc = strncpy_s(str1, 5, NULL, nlen);
    ERR_MSVC(ESNULLP, EINVAL); /* and cleared */
    if (!use_msvcrt)
        CHECK_SLACK(str1, 5);

    strcpy(str1, "a");
    EXPECT_BOS("empty dest or dmax")
    rc = strncpy_s(str1, 0, str2, 0);
    ERR_MSVC(ESZEROL, 0); /* and untouched */
    EXPSTR(str1, "a")

    /*--------------------------------------------------*/

    strcpy(str1, "aa");
    nlen = 5;
    EXPECT_BOS("empty dest or dmax")
    rc = strncpy_s(str1, 0, str2, nlen);
    ERR_MSVC(ESZEROL, EINVAL); /* and untouched */
    EXPSTR(str1, "aa")

    /*--------------------------------------------------*/

    strcpy(str1, "aaa");
    if (_BOS_KNOWN(str1)) {
        EXPECT_BOS("dest overflow")
        rc = strncpy_s(str1, LEN + 1, str2, nlen);
        ERR(EOVERFLOW); /* dmax exceeds dest */
        if (!use_msvcrt) {
            EXPSTR(str1, ""); /* cleared */
            CHECK_SLACK(str1, 4);
        }
#ifdef HAVE___BUILTIN_OBJECT_SIZE
    } else {
        debug_printf("%s %u  Error unknown str1 object_size\n", __FUNCTION__,
                     __LINE__);
        errs++;
#endif
    }

    strcpy(str1, "aa");
    EXPECT_BOS("dest overflow")
    rc = strncpy_s(str1, (RSIZE_MAX_STR + 1), str2, nlen);
    ERR_MSVC(ESLEMAX, 0); /* different MAX */
    if (!_BOS_KNOWN(str1)) {
        EXPSTR(str1, "aa")
    } else if (!use_msvcrt) {
        EXPSTR(str1, "") /* cleared */
    } else {
        EXPSTR(str1, "b")
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

    EXPECT_BOS("src overflow or empty")
    rc = strncpy_s(str1, 5, str2, (RSIZE_MAX_STR + 1));
    ERR_MSVC(ESLEMAX, EINVAL); /* and cleared */
    EXPSTR(str1, "");

    /*--------------------------------------------------*/

    // dont clear the const string, but dest.
    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    EXPECT_BOS("slen exceeds src")
    rc = strncpy_s(str1, 32, "ROString", 31);
    ERR(EOVERFLOW); /* slen exceeds src */
    if (!use_msvcrt) {
        CHECK_SLACK(str1, 32);
    } else {
        EXPSTR(str1, "");
    }
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';
    nlen = 5;

    rc = strncpy_s(&str1[0], LEN / 2, &str2[0], nlen);
    ERR(EOK); /* and cleared */
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN / 2);
    } else {
        EXPSTR(str1, "");
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 5;

    /* test overlap */
    GCC_PUSH_WARN_RESTRICT
    rc = strncpy_s(str1, LEN, str1, nlen);
    GCC_POP_WARN_RESTRICT
    ERR_MSVC(ESOVRLP, 0); /* and cleared/mangled */
#ifdef HAVE_CT_BOS_OVR
    init_msvcrt(rc == ESOVRLP, &use_msvcrt);
#endif
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN);
    } else {
        EXPSTR(str1, "aaaaa");
    }
    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 18;

    rc = strncpy_s(&str1[0], LEN, &str1[5], nlen);
    ERR_MSVC(ESOVRLP, 0); /* and cleared/mangled */
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN);
    } else {
        EXPSTR(str1, "aaaaaaaaaaaaaaaaaa");
    }

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    str2[0] = '\0';

    nlen = 10;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK); /* and cleared */
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN);
    } else {
        EXPSTR(str1, "")
    }

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    nlen = 20;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    EXPSTR(str1, str2)
    if (!use_msvcrt) {
        CHECK_SLACK(&str1[14], LEN - 14);
    }

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    nlen = 32;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    EXPSTR(str1, str2)
    if (!use_msvcrt) {
        CHECK_SLACK(&str1[14], LEN - 14);
    }

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strncpy_s(str1, 1, str2, nlen);
    ERR_MSVC(ESNOSPC, EINVAL);
    if (!use_msvcrt) {
        CHECK_SLACK(str1, 1);
    } else {
        EXPSTR(str1, "");
    }

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strncpy_s(str1, 2, str2, nlen);
    ERR_MSVC(ESNOSPC, EINVAL); /* and cleared */
    if (*str1 != '\0') {
        debug_printf("%s %u -%s-  Error rc=%u \n", __FUNCTION__, __LINE__, str1,
                     rc);
        errs++;
    }
    /*--------------------------------------------------*/
    /* TR example */

    strcpy(dest, "                            ");
    strcpy(str1, "hello");

    rc = strncpy_s(dest, 6, str1, 100);
    ERR(EOK)
    EXPSTR(dest, str1)
    if (!use_msvcrt) {
        CHECK_SLACK(&str1[5], 1);
    }

    /*--------------------------------------------------*/
    /* TR example */

    strcpy(dest, "                            ");
    strcpy(str2, "goodbye");

    rc = strncpy_s(dest, 5, str2, 7);
    ERR_MSVC(ESNOSPC, EINVAL);
    if (!use_msvcrt) {
        CHECK_SLACK(dest, 5);
    }

    /*--------------------------------------------------*/
    /* TR example */

    strcpy(dest, "                            ");
    strcpy(str2, "goodbye");

    rc = strncpy_s(dest, 5, str2, 4);
    ERR(EOK)
    EXPSTR(dest, "good")
    CHECK_SLACK(&dest[4], 5 - 4);

    /*--------------------------------------------------*/

    strcpy(dest, "                            ");
    strcpy(str2, "good");

    /*   strnlen("good") < 5   */
    rc = strncpy_s(dest, 5, str2, 8);
    ERR(EOK)
    EXPSTR(dest, "good")
    CHECK_SLACK(&dest[4], 5 - 4);

    /*--------------------------------------------------*/

    strcpy(str1, "qq12345weqeqeqeq");
    strcpy(str2, "it");

    nlen = 10;
    rc = strncpy_s(str1, 10, str2, nlen);
    ERR(EOK)
    EXPSTR(str1, str2)

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strncpy_s()); }
#endif
