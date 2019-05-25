/*------------------------------------------------------------------
 * test_strspn_s
 * File 'extstr/strspn_s.c'
 * Lines executed:89.47% of 38
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define COUNT(n)                                                               \
    if (count != (n)) {                                                        \
        debug_printf("%s %u  count=%d  rc=%d \n", __FUNCTION__, __LINE__,      \
                     (int)count, rc);                                          \
        errs++;                                                                \
    }
#define COUNTSTD(n)                                                            \
    if ((int)count != (n)) {                                                   \
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n", __FUNCTION__,  \
                     __LINE__, (int)count, std_count, rc);                     \
        errs++;                                                                \
    }

#define LEN (128)
#define SHORT_LEN (5)

static char str1[LEN];
static char str2[LEN];
int test_strspn_s(void);

int test_strspn_s(void) {
    errno_t rc = 0;
    rsize_t count = 0;
    int std_count = 0;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strspn_s(NULL, LEN, str2, LEN, &count);
    ERR(ESNULLP)
    COUNT(0)

    EXPECT_BOS("empty src")
    rc = strspn_s(str1, LEN, NULL, LEN, &count);
    ERR(ESNULLP)
    COUNT(0)

    EXPECT_BOS("empty countp")
    rc = strspn_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strspn_s(str1, 0, str2, LEN, &count);
    ERR(ESZEROL)
    COUNT(0)

    EXPECT_BOS("empty src or slen")
    rc = strspn_s(str1, LEN, str2, 0, &count);
    ERR(ESZEROL)
    COUNT(0)

    EXPECT_BOS("dest overflow")
    rc = strspn_s(str1, RSIZE_MAX_STR + 1, str2, LEN, &count);
    ERR(ESLEMAX)
    COUNT(0)

    EXPECT_BOS("src overflow")
    rc = strspn_s(str1, LEN, str2, RSIZE_MAX_STR + 1, &count);
    ERR(ESLEMAX)
    COUNT(0)

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strspn_s(str1, LEN + 1, str2, LEN, &count);
    ERR(EOVERFLOW)
    COUNT(0)

    EXPECT_BOS("src overflow")
    rc = strspn_s(str1, LEN, str2, LEN + 1, &count);
    ERR(EOVERFLOW)
    COUNT(0)
#endif
#endif
    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    COUNT(0)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strspn_s(str1, 1, str2, LEN, &count);
    ERR(EOK)
    COUNT(1)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strspn_s(str1, 2, str2, LEN, &count);
    ERR(EOK)
    COUNT(2)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strspn_s(str1, 3, str2, 12, &count);
    ERR(EOK)
    COUNT(3)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "k");

    rc = strspn_s(str1, LEN, str2, 1, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "ke");

    rc = strspn_s(str1, LEN, str2, 2, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "notincluded");

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "1234567890");

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strspn_s()); }
#endif
