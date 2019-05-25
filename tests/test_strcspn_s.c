/*------------------------------------------------------------------
 * test_strcspn_s
 * File 'extstr/strcspn_s.c'
 * Lines executed:90.32% of 31
 *
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
int test_strcspn_s(void);

int test_strcspn_s(void) {
    errno_t rc;
    rsize_t count;
    int std_count;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strcspn_s(NULL, LEN, str2, LEN, &count);
    ERR(ESNULLP)
    COUNT(0)

    EXPECT_BOS("empty src")
    rc = strcspn_s(str1, LEN, NULL, LEN, &count);
    ERR(ESNULLP)
    COUNT(0)

    EXPECT_BOS("empty countp")
    rc = strcspn_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strcspn_s(str1, 0, str2, LEN, &count);
    ERR(ESZEROL)
    COUNT(0)

    rc = strcspn_s(str1, LEN, str2, 0, &count);
    ERR(ESZEROL)
    COUNT(0)

    EXPECT_BOS("dest overflow")
    rc = strcspn_s(str1, RSIZE_MAX_STR + 1, str2, LEN, &count);
    ERR(ESLEMAX)
    COUNT(0)

    EXPECT_BOS("src overflow")
    rc = strcspn_s(str1, LEN, str2, RSIZE_MAX_STR + 1, &count);
    ERR(ESLEMAX)
    COUNT(0)
#endif

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    COUNT(0)
    std_count = strcspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "KEEP");

    rc = strcspn_s(str1, 1, str2, LEN, &count);
    ERR(EOK)
    COUNT(1)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "KEEP");

    rc = strcspn_s(str1, 2, str2, LEN, &count);
    ERR(EOK)
    COUNT(2)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "KEEP");

    rc = strcspn_s(str1, 3, str2, LEN, &count);
    ERR(EOK)
    COUNT(3)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "KEEP");

    rc = strcspn_s(str1, 6, str2, LEN, &count);
    ERR(EOK)
    COUNT(6)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "ABCDEF");

    rc = strcspn_s(str1, 6, str2, 2, &count);
    ERR(EOK)
    COUNT(6)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strcspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "notincluded");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strcspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "1234567890");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strcspn(str1, str2);
    COUNTSTD(std_count)

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strcspn_s()); }
#endif
