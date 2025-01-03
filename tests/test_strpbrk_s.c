/*------------------------------------------------------------------
 * test_strpbrk_s
 * File 'extstr/strpbrk_s.c'
 * Lines executed:81.25% of 32
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)
#define SHORT_LEN (5)

static char str1[LEN];
static char str2[LEN];
int test_strpbrk_s(void);

#define EXPFIRST(x)                                                            \
    if (rc != x) {                                                             \
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",              \
                     __FUNCTION__, __LINE__, (void *)str1, (void *)first, rc); \
        errs++;                                                                \
    }
#define NOFIRST()                                                              \
    if (first) {                                                               \
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",              \
                     __FUNCTION__, __LINE__, (void *)str1, (void *)first, rc); \
        errs++;                                                                \
    }

int test_strpbrk_s(void) {
    errno_t rc;
    char *first;
    char *std_first;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strpbrk_s(NULL, LEN, str2, LEN, &first);
    ERR(ESNULLP)
    NOFIRST()

    EXPECT_BOS("empty firstp")
    rc = strpbrk_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = strpbrk_s(str1, LEN, NULL, LEN, &first);
    ERR(ESNULLP)
    NOFIRST()

    EXPECT_BOS("empty dest or dmax")
    rc = strpbrk_s(str1, 0, str2, LEN, &first);
    ERR(ESZEROL)
    NOFIRST()

    EXPECT_BOS("dest overflow")
    rc = strpbrk_s(str1, LEN + 1, str2, LEN, &first);
    ERR(EOVERFLOW)
    NOFIRST()

    EXPECT_BOS("slen exceeds src") EXPECT_BOS("src overflow or empty")
    rc = strpbrk_s(str1, LEN, str2, LEN + 1, &first);
    ERR(EOVERFLOW)
    NOFIRST()

    EXPECT_BOS("src overflow")
    rc = strpbrk_s(str1, LEN, str2, LEN + 1, &first);
    ERR(EOVERFLOW)
    NOFIRST()
#endif

    strcpy(str1, "keep it simple");
    strcpy(str2, "1234");
    rc = strpbrk_s(str1, LEN, str2, 3, &first);
    ERR(ESNOTFND)
    NOFIRST()

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    /* not to be found */
    rc = strpbrk_s(str1, LEN, str2, LEN, &first);
    EXPFIRST(ESNOTFND)
    std_first = strpbrk(str1, str2);
    if (first != std_first) {
        debug_printf("%s %u  first=%p  std_first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)first, (void *)std_first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "1234");

    rc = strpbrk_s(str1, 2, str2, LEN, &first);
    EXPFIRST(ESNOTFND)
    std_first = strpbrk(str1, str2);
    if (first != std_first) {
        debug_printf("%s %u  first=%p  std_first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)first, (void *)std_first, rc);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "ke");

    rc = strpbrk_s(str1, 2, str2, LEN, &first);
    EXPFIRST(EOK)
    std_first = strpbrk(str1, str2);
    if (first != std_first) {
        debug_printf("%s %u  first=%p  std_first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)first, (void *)std_first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simplez");
    strcpy(str2, "z");

    rc = strpbrk_s(str1, LEN, str2, LEN, &first);
    EXPFIRST(EOK)
    std_first = strpbrk(str1, str2);
    if (first != std_first) {
        debug_printf("%s %u  first=%p  std_first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)first, (void *)std_first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "123456");

    rc = strpbrk_s(str1, LEN, str2, 6, &first);
    ERR(ESNOTFND)
    if (first != 0) {
        debug_printf("%s %u  Error  first=%p  rc=%d \n", __FUNCTION__, __LINE__,
                     (void *)first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "");

    rc = strpbrk_s(str1, LEN, str2, 1, &first);
    ERR(ESNOTFND)
    NOFIRST()

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "k");

    rc = strpbrk_s(str1, LEN, str2, 1, &first);
    ERR(EOK)
    std_first = strpbrk(str1, str2);
    if (first != std_first) {
        debug_printf("%s %u  first=%p  std_first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)first, (void *)std_first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "ke");

    rc = strpbrk_s(str1, LEN, str2, 2, &first);
    ERR(EOK)
    std_first = strpbrk(str1, str2);
    if (first != std_first) {
        debug_printf("%s %u  first=%p  std_first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)first, (void *)std_first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "hip");

    rc = strpbrk_s(str1, LEN, str2, LEN, &first);
    ERR(EOK)
    std_first = strpbrk(str1, str2);
    if (first != std_first) {
        debug_printf("%s %u  first=%p  std_first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)first, (void *)std_first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simply");
    strcpy(str2, "123y");

    rc = strpbrk_s(str1, LEN, str2, LEN, &first);
    ERR(EOK)
    std_first = strpbrk(str1, str2);
    if (first != std_first) {
        debug_printf("%s %u  first=%p  std_first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)first, (void *)std_first, rc);
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strpbrk_s()); }
#endif
