/*------------------------------------------------------------------
 * test_strncat_s
 * File 'str/strncat_s.c'
 * Lines executed:93.33% of 90
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_STRNCAT_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"
#if defined(TEST_MSVCRT) && defined(HAVE_STRNCAT_S)
#undef HAVE_CT_BOS_OVR
#undef strncat_s
EXTERN errno_t strncat_s(char *restrict dest, rsize_t dmax,
                         const char *restrict src, rsize_t slen);
#endif

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
int test_strncat_s(void);

int test_strncat_s(void) {
    errno_t rc;
    int len;
    int errs = 0;

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");

#if defined(TEST_MSVCRT) && defined(HAVE_STRNCAT_S)
    use_msvcrt = true;
#endif
    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest or dmax")
    rc = strncat_s(NULL, LEN, str1, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    strcpy(str1, "a");
    strcpy(str2, "bcde");

    EXPECT_BOS("empty dest or dmax")
    rc = strncat_s(NULL, 1, str2, 0);
    ERR_MSVC(ESNULLP, EINVAL);

    EXPECT_BOS("empty dest or dmax")
    rc = strncat_s(str1, 0, str2, 0);
    ERR_MSVC(ESZEROL, EINVAL);
    EXPSTR(str1, "a");
#endif

    rc = strncat_s(NULL, 0, str2, 0);
#ifdef HAVE_CT_BOS_OVR
    init_msvcrt(rc == EOK, &use_msvcrt);
#endif
    ERR_MSVC(EOK, EINVAL);

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 1, str2, 0);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESZEROL, EINVAL);
    EXPSTR(str1, "");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 2, str2, 0);
    GCC_POP_WARN_DMAX
    ERR(EOK);
    EXPSTR(str1, "");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 3, str2, 0);
    GCC_POP_WARN_DMAX
    ERR(EOK);
    EXPSTR(str1, "");

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    strcpy(str1, "a");
    EXPECT_BOS("empty dest or dmax")
    rc = strncat_s(str1, 0, str2, LEN);
    ERR_MSVC(ESZEROL, EINVAL);

    /*--------------------------------------------------*/

    strcpy(str1, "ab");
    if (!use_msvcrt) {
        /* with clang-5 compile-time already checks these errors */
        if (_BOS_KNOWN(str1)) {
            EXPECT_BOS("dest overflow")
            rc = strncat_s(str1, LEN + 1, str2, LEN);
            ERR(EOVERFLOW);
            EXPSTR(str1, ""); /* cleared */
            CHECK_SLACK(str1, 2);
        } else {
#ifdef HAVE___BUILTIN_OBJECT_SIZE
            debug_printf("%s %u  Warning unknown str1 size\n", __FUNCTION__,
                         __LINE__);
#endif
        }
    }

    strcpy(str1, "a");
    /* valid with the windows sec_api */
    EXPECT_BOS("dest overflow")
    rc = strncat_s(str1, (RSIZE_MAX_STR + 1), str2, LEN);
    ERR_MSVC(ESLEMAX, 0);
    if (!_BOS_KNOWN(str1)) {
        EXPSTR(str1, "a")
    } else if (!use_msvcrt) { /* cleared, because BOS is known */
        EXPSTR(str1, "")
    } else {
        EXPSTR(str1, "abcde") /* valid */
    }

    strcpy(str1, "a");
    EXPECT_BOS("src overflow or empty")
    rc = strncat_s(str1, LEN, str2, (RSIZE_MAX_STR + 1));
    ERR_MSVC(ESLEMAX, 0);
    if (!use_msvcrt) {
        EXPSTR(str1, ""); /* cleared */
    } else {
        EXPSTR(str1, "abcde") /* valid */
    }

    strcpy(str1, "abc");
    if (!use_msvcrt) {
        if (_BOS_KNOWN(str1)) {
            EXPECT_BOS("dest overflow")
            rc = strncat_s(str1, LEN + 1, str2, LEN);
            ERR(EOVERFLOW);   /* dmax exceeds dest */
            EXPSTR(str1, ""); /* cleared */
            CHECK_SLACK(str1, 4);

            rc = strncat_s(str1, LEN - 1, str2, LEN);
            ERR(0);
        } else {
#ifdef HAVE___BUILTIN_OBJECT_SIZE
            debug_printf("%s %u  Error unknown str1 object_size\n",
                         __FUNCTION__, __LINE__);
            errs++;
#endif
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    EXPECT_BOS("slen exceeds src")
    rc = strncat_s(str1, 32, "ROString", 31);
    ERR_MSVC(EOVERFLOW, EINVAL);
    if (!use_msvcrt) {
        CHECK_SLACK(str1, 32);
    } else {
        EXPSTR(str1, "");
    }

    /*--------------------------------------------------*/

    strcpy(str1, "a");
    EXPECT_BOS("src overflow or empty")
    rc = strncat_s(str1, LEN, NULL, LEN);
    ERR_MSVC(ESNULLP, EINVAL);
    /*EXPSTR(str1, ""); */
    if (!use_msvcrt)
        CHECK_SLACK(str1, 1);

#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 1, str2, LEN);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESUNTERM, EINVAL);
    if (!use_msvcrt)
        CHECK_SLACK(str1, 1);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 2, str2, LEN);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESUNTERM, EINVAL);
    if (!use_msvcrt)
        CHECK_SLACK(str1, 2);

    /*--------------------------------------------------*/

    strcpy(str1, "a");
    strcpy(str2, "b");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 2, str2, 1);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESNOSPC, ERANGE);
    if (!use_msvcrt)
        CHECK_SLACK(str1, 2);

    /*--------------------------------------------------*/

    strcpy(str1, "abcd");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(&str1[0], 8, &str1[3], 4);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESOVRLP, ERANGE);
    EXPNULL(str1)
    if (!use_msvcrt)
        CHECK_SLACK(str1, 8);

    strcpy(str1, "abcd");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(&str1[0], 4, &str1[3], 4);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESOVRLP, EINVAL);
    EXPNULL(str1)
    if (!use_msvcrt)
        CHECK_SLACK(str1, 4);

    /*--------------------------------------------------*/

    strcpy(str1, "abcd");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(&str1[0], 3, &str1[3], 4);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESUNTERM, EINVAL);
    EXPNULL(str1)
    if (!use_msvcrt)
        CHECK_SLACK(str1, 3);

    /*--------------------------------------------------*/

    strcpy(str1, "abcdefgh");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(&str1[3], 5, &str1[0], 4);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESUNTERM, EINVAL);
    EXPNULL(&str1[3])
    if (!use_msvcrt)
        CHECK_SLACK(&str1[3], 5);

    /*--------------------------------------------------*/

    strcpy(str1, "abcdefgh");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(&str1[3], 12, &str1[0], 4);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESOVRLP, EOK);
    if (!use_msvcrt) {
        EXPNULL(&str1[3]);
        CHECK_SLACK(&str1[3], 12);
    } else {
        EXPSTR(str1, "abcdefghabcd"); /* Windows appends! */
    }

    /*--------------------------------------------------*/

    strcpy(str1, "a");
    strcpy(str2, "b");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 3, str2, 1);
    GCC_POP_WARN_DMAX
    ERR(EOK)
    EXPSTR(str1, "ab");
    if (!use_msvcrt)
        CHECK_SLACK(&str1[2], 3 - 2);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 50, str2, LEN);
    GCC_POP_WARN_DMAX
    ERR(EOK)
    EXPSTR(str1, "aaaaaaaaaakeep it simple");
    len = strlen(str1);
    if (!use_msvcrt)
        CHECK_SLACK(&str1[len], 50 - len);

    /*--------------------------------------------------*/
    /* TR example */

    strcpy(str1, "good");
    strcpy(str2, "bye");

    rc = strncat_s(str1, LEN, str2, LEN);
    ERR(EOK)
    EXPSTR(str1, "goodbye");
    len = strlen(str1);
    if (!use_msvcrt)
        CHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/
    /* TR example */

    strcpy(str1, "hello");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 6, "", 1);
    GCC_POP_WARN_DMAX
    ERR(EOK)
    EXPSTR(str1, "hello");
    len = strlen(str1);
    if (!use_msvcrt)
        CHECK_SLACK(&str1[len], 6 - len);

    /*--------------------------------------------------*/
    /* TR example */

    strcpy(str1, "hello");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 6, "X", 2);
    GCC_POP_WARN_DMAX
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(str1, 6);

    /*--------------------------------------------------*/
    /* TR example */

    strcpy(str1, "abc");

    GCC_PUSH_WARN_DMAX
    rc = strncat_s(str1, 7, "defghijklmn", 3);
    GCC_POP_WARN_DMAX
    ERR(EOK)
    EXPSTR(str1, "abcdef");
    len = strlen(str1);
    if (!use_msvcrt)
        CHECK_SLACK(&str1[len], 6 - len);

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strncat_s()); }
#endif
