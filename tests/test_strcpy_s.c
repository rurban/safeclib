/*------------------------------------------------------------------
 * test_strcpy_s
 * File 'str/strcpy_s.c'
 * Lines executed:100.00% of 48
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#if defined(TEST_MSVCRT) && defined(HAVE_STRCPY_S)
#undef strcpy_s
EXTERN errno_t strcpy_s(char *restrict dest, rsize_t dmax,
                        const char *restrict src);
#endif

#ifdef HAVE_STRCPY_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX (128)
#define LEN (128)

static char str1[LEN];
static char str2[LEN];
int test_strcpy_s(void);

#ifndef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
/* internal constraint handler helpers, exported by libsafec */
EXTERN int handle_str_bos_overflow(const char *restrict msg,
                                   char *restrict dest, const rsize_t dmax);
EXTERN void handle_str_bos_chk_warn(const char *restrict func,
                                    char *restrict dest, const rsize_t dmax,
                                    const size_t destbos);
EXTERN void handle_str_src_bos_chk_warn(const char *restrict func,
                                        char *restrict dest, const rsize_t smax,
                                        const size_t srcbos,
                                        const char *srcname,
                                        const char *smaxname);

static int h1_calls, h2_calls;
static errno_t h1_err, h2_err;
static void *h1_ptr;
static char h1_msg[128];

static void test_handler1(const char *restrict msg, void *restrict ptr,
                          errno_t error) {
    h1_calls++;
    h1_err = error;
    h1_ptr = ptr;
    strncpy(h1_msg, msg, sizeof(h1_msg) - 1);
    h1_msg[sizeof(h1_msg) - 1] = '\0';
}
static void test_handler2(const char *restrict msg, void *restrict ptr,
                          errno_t error) {
    (void)msg;
    (void)ptr;
    h2_calls++;
    h2_err = error;
}
#endif

int test_strcpy_s(void) {
    errno_t rc;
    int len;
    int errs = 0;

    /*--------------------------------------------------*/

#if defined(TEST_MSVCRT) && defined(HAVE_STRNCPY_S)
    use_msvcrt = true;
#endif
    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strcpy_s(NULL, LEN, str2);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);

    EXPECT_BOS("empty dest or dmax")
    rc = strcpy_s(str1, 0, str2);
    ERR_MSVC(ESZEROL, EINVAL);

    /*--------------------------------------------------*/

#ifndef HAVE_ASAN /* With asan no BOS? */
    if (_BOS_KNOWN(str1)) {
        EXPECT_BOS("dest overflow")
        rc = strcpy_s(str1, LEN + 1, str2);
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
    rc = strcpy_s(str1, (RSIZE_MAX_STR + 1), str2);
    ERR_MSVC(ESLEMAX, 0);

    EXPECT_BOS("dest overflow")
    rc = strcpy_s(str1, (size_t)-1L, str2);
    ERR_MSVC(ESLEMAX, 0);

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);

    EXPECT_BOS("empty src")
    rc = strcpy_s(str1, 5, NULL);
    ERR_MSVC(ESNULLP, EINVAL);
    CHECK_SLACK(str1, 5);
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';

    rc = strcpy_s(str1, LEN / 2, str2);
    ERR(EOK)
    CHECK_SLACK(str1, LEN / 2);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    len = strlen(str1);

    GCC_PUSH_WARN_RESTRICT
    rc = strcpy_s(str1, LEN, str1);
    GCC_POP_WARN_RESTRICT
    ERR(EOK);
    EXPSTR(str1, "aaaaa");

    /*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");

    rc = strcpy_s(&str1[0], LEN, &str1[5]);
#ifdef HAVE_CT_BOS_OVR
    init_msvcrt(rc == ESOVRLP, &use_msvcrt);
#endif
    ERR_MSVC(ESOVRLP, 0);
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN);
    } else { /* windows overlap with dest < src */
        EXPSTR(str1, "it simple");
    }

    /*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");

    rc = strcpy_s(&str1[5], LEN - 5, &str1[0]);
    /* Windows forbids overlap with dest > src */
    ERR_MSVC(ESOVRLP, ERANGE);
    CHECK_SLACK(&str1[5], LEN - 5);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    len = strlen(str1);

    rc = strcpy_s(str2, LEN, str1);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str2[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");
    str2[0] = '\0';

    rc = strcpy_s(str1, LEN, str2);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(str1, LEN);

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(&str2[0], "keep it simple");
    len = strlen(str2);

    rc = strcpy_s(str1, LEN, str2);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    len = strlen(str2);

    rc = strcpy_s(str1, LEN, str2);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strcpy_s(str1, 1, str2);
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(str1, 1);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strcpy_s(str1, 2, str2);
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(str1, 2);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "it");

    rc = strcpy_s(str1, 3, str2);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[2], 1);

    /*--------------------------------------------------*/

    strcpy(str1, "qq12345weqeqeqeq");
    strcpy(str2, "it");

    rc = strcpy_s(str1, 10, str2);
    ERR(EOK);
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[2], 8);
    /*for (i=0; i<10; i++) {
        debug_printf("str1[%i] = %d \n", i, str1[i]);
    }*/
    /*--------------------------------------------------*/

#ifndef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
    /* str constraint handler: set, thrd_set, invoke, bos helpers */
    {
        constraint_handler_t prev;
        char *volatile np = NULL;
        char *volatile vdest = str1;

        /* global handler: set returns the previous one */
        prev = set_str_constraint_handler_s(test_handler1);
        h1_calls = h2_calls = 0;
        rc = strcpy_s(np, LEN, str2);
        ERR_MSVC(ESNULLP, EINVAL);
        if (h1_calls != 1 || h1_err != ESNULLP) {
            debug_printf("%s %u  handler1 calls=%d err=%d\n", __FUNCTION__,
                         __LINE__, h1_calls, (int)h1_err);
            errs++;
        }
        prev = set_str_constraint_handler_s(test_handler2);
        if (prev != test_handler1) {
            debug_printf("%s %u  wrong prev handler\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = strcpy_s(np, LEN, str2);
        ERR_MSVC(ESNULLP, EINVAL);
        if (h1_calls != 1 || h2_calls != 1 || h2_err != ESNULLP) {
            debug_printf("%s %u  handler2 calls=%d err=%d\n", __FUNCTION__,
                         __LINE__, h2_calls, (int)h2_err);
            errs++;
        }
        /* NULL resets to the default handler (ignore_handler_s) */
        prev = set_str_constraint_handler_s(NULL);
        if (prev != test_handler2) {
            debug_printf("%s %u  wrong prev handler\n", __FUNCTION__, __LINE__);
            errs++;
        }
        prev = set_str_constraint_handler_s(test_handler1);
        if (prev != sl_default_handler) {
            debug_printf("%s %u  NULL did not set the default handler\n",
                         __FUNCTION__, __LINE__);
            errs++;
        }
        /* the internal bos helpers go through the current handler */
        h1_calls = 0;
        strcpy(str1, "abc");
        rc = handle_str_bos_overflow("overflow test", vdest, 2);
        ERR(EOVERFLOW);
        if (h1_calls != 1 || h1_err != EOVERFLOW || h1_ptr != (void *)str1 ||
            strcmp(h1_msg, "overflow test") != 0) {
            debug_printf("%s %u  bos_overflow handler calls=%d err=%d\n",
                         __FUNCTION__, __LINE__, h1_calls, (int)h1_err);
            errs++;
        }
        EXPSTR(str1, ""); /* cleared */

        h1_calls = 0;
        handle_str_bos_chk_warn("func", vdest, 10, 5);
        if (h1_calls != 1 || h1_err != ESLEWRNG || h1_ptr != (void *)str1 ||
            strcmp(h1_msg, "func: wrong dmax 10, dest has size 5") != 0) {
            debug_printf("%s %u  bos_chk_warn calls=%d err=%d msg=%s\n",
                         __FUNCTION__, __LINE__, h1_calls, (int)h1_err, h1_msg);
            errs++;
        }
        h1_calls = 0;
        handle_str_src_bos_chk_warn("func", vdest, 10, 5, "src", "slen");
        if (h1_calls != 1 || h1_err != ESLEWRNG || h1_ptr != (void *)str1 ||
            strcmp(h1_msg, "func: wrong slen 10, src has size 5") != 0) {
            debug_printf("%s %u  src_bos_chk_warn calls=%d err=%d msg=%s\n",
                         __FUNCTION__, __LINE__, h1_calls, (int)h1_err, h1_msg);
            errs++;
        }

        /* thread-local handler takes precedence over the global one */
        prev = thrd_set_str_constraint_handler_s(test_handler2);
        if (prev != NULL) {
            debug_printf("%s %u  thrd handler was set before\n", __FUNCTION__,
                         __LINE__);
            errs++;
        }
        h1_calls = h2_calls = 0;
        rc = strcpy_s(np, LEN, str2);
        ERR_MSVC(ESNULLP, EINVAL);
        if (h1_calls != 0 || h2_calls != 1 || h2_err != ESNULLP) {
            debug_printf("%s %u  thrd handler calls %d %d\n", __FUNCTION__,
                         __LINE__, h1_calls, h2_calls);
            errs++;
        }
        prev = thrd_set_str_constraint_handler_s(test_handler1);
        if (prev != test_handler2) {
            debug_printf("%s %u  wrong prev thrd handler\n", __FUNCTION__,
                         __LINE__);
            errs++;
        }
        /* NULL resets the thread handler to the default */
        prev = thrd_set_str_constraint_handler_s(NULL);
        prev = thrd_set_str_constraint_handler_s(test_handler1);
        if (prev != sl_default_handler) {
            debug_printf("%s %u  NULL did not set the default thrd handler\n",
                         __FUNCTION__, __LINE__);
            errs++;
        }
        /* restore defaults */
        thrd_set_str_constraint_handler_s(NULL);
        set_str_constraint_handler_s(NULL);
    }
#endif

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strcpy_s()); }
#endif
