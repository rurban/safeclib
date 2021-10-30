/*------------------------------------------------------------------
 * test_stpcpy_s
 * File 'extstr/stpcpy_s.c'
 * Lines executed:83.53% of 85
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include "config.h"

#ifdef HAVE_STPCPY_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif

#define MAX (128)
#define LEN (128)

static char str1[LEN];
static char str2[LEN];
int test_stpcpy_s(void);

int test_stpcpy_s(void) {
    errno_t rc;
    int len;
    char* dest;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    dest = stpcpy_s(NULL, LEN, str2, &rc);
    ERR(ESNULLP);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);

    EXPECT_BOS("empty dest or dmax")
    dest = stpcpy_s(str1, 0, str2, &rc);
    ERR(ESZEROL);

    /*--------------------------------------------------*/

#ifndef HAVE_ASAN /* With asan no BOS? */
    if (_BOS_KNOWN(str1)) {
        EXPECT_BOS("dest overflow")
        dest = stpcpy_s(str1, LEN + 1, str2, &rc);
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
    dest = stpcpy_s(str1, (RSIZE_MAX_STR + 1), str2, &rc);
    ERR(ESLEMAX);

    EXPECT_BOS("dest overflow")
    dest = stpcpy_s(str1, (size_t)-1L, str2, &rc);
    ERR(ESLEMAX);

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);

    EXPECT_BOS("empty src")
    dest = stpcpy_s(str1, 5, NULL, &rc);
    ERR(ESNULLP);
    CHECK_SLACK(str1, 5);
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';

    dest = stpcpy_s(str1, LEN / 2, str2, &rc);
    ERR(EOK)
    CHECK_SLACK(str1, LEN / 2);

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    len = strlen(str1);

    /* test overlap */
    GCC_PUSH_WARN_RESTRICT
    dest = stpcpy_s(str1, LEN, str1, &rc);
    GCC_POP_WARN_RESTRICT
    ERR(EOK); // same => OK
    EXPSTR(str1, "aaaaa");

    /*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");

    dest = stpcpy_s(&str1[0], LEN, &str1[5], &rc);
    ERR(ESOVRLP);
    CHECK_SLACK(str1, LEN);

    /*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");

    dest = stpcpy_s(&str1[5], LEN - 5, &str1[0], &rc);
    ERR(ESOVRLP);
    CHECK_SLACK(&str1[5], LEN - 5);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    len = strlen(str1);

    dest = stpcpy_s(str2, LEN, str1, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str2[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");
    str2[0] = '\0';

    dest = stpcpy_s(str1, LEN, str2, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(str1, LEN);

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(&str2[0], "keep it simple");
    len = strlen(str2);

    dest = stpcpy_s(str1, LEN, str2, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    len = strlen(str2);

    dest = stpcpy_s(str1, LEN, str2, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[len], LEN - len);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    dest = stpcpy_s(str1, 1, str2, &rc);
    ERR(ESNOSPC);
    CHECK_SLACK(str1, 1);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    dest = stpcpy_s(str1, 2, str2, &rc);
    ERR(ESNOSPC);
    CHECK_SLACK(str1, 2);

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "it");

    dest = stpcpy_s(str1, 3, str2, &rc);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[2], 1);

    /*--------------------------------------------------*/

    strcpy(str1, "qq12345weqeqeqeq");
    strcpy(str2, "it");

    dest = stpcpy_s(str1, 10, str2, &rc);
    ERR(EOK);
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[2], 8);
    /*for (i=0; i<10; i++) {
        debug_printf("str1[%i] = %d \n", i, str1[i]);
    }*/
    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_stpcpy_s()); }
#endif
