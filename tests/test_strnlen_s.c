/*------------------------------------------------------------------
 * test_strnlen_s
 * File 'str/strnlen_s.c'
 * Lines executed:95.45% of 22
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#if defined(TEST_MSVCRT) && defined(HAVE_STRNLEN_S)
#undef strnlen_s
#endif

#define LEN (128)
static char str1[LEN];
int test_strnlen_s(void);

#define EXPLEN(n)                                                              \
    if (len != n) {                                                            \
        debug_printf("%s %u   Len=%u \n", __FUNCTION__, __LINE__,              \
                     (unsigned)len);                                           \
        errs++;                                                                \
    }
#define STDLEN()                                                               \
    if (std_len != len) {                                                      \
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__, \
                     (unsigned)std_len, (unsigned)len);                        \
        errs++;                                                                \
    }

int test_strnlen_s(void) {
    rsize_t len;
    rsize_t std_len;
    rsize_t max_len;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    max_len = 3;
    EXPECT_BOS("empty str")
    len = strnlen_s(NULL, max_len);
    EXPLEN(0)

    /*--------------------------------------------------*/

    max_len = 0;
    EXPECT_BOS("empty str or smax")
    len = strnlen_s("test", 0);
    EXPLEN(0)
    /*--------------------------------------------------*/

    max_len = RSIZE_MAX_STR + 1;
    EXPECT_BOS("str overflow")
    len = strnlen_s("test", RSIZE_MAX_STR + 1);
    EXPLEN(0)
#endif
    /*--------------------------------------------------*/

    *str1 = '\0';
    std_len = strlen(str1);
    max_len = LEN;

    len = strnlen_s(str1, max_len);
    STDLEN()
    /*--------------------------------------------------*/

    strcpy(str1, "t");
    std_len = strlen(str1);
    len = strnlen_s(str1, LEN);
    STDLEN()
    /*--------------------------------------------------*/

    strcpy(str1, "to");
    std_len = strlen(str1);

    len = strnlen_s(str1, LEN);
    STDLEN()
    /*--------------------------------------------------*/

    std_len = strlen("testing");
    max_len = std_len;

    len = strnlen_s("testing", max_len);
    STDLEN()
    /*--------------------------------------------------*/

    max_len = 1;
    len = strnlen_s("testing", max_len);
    EXPLEN(max_len)
    /*--------------------------------------------------*/

    max_len = 2;
    len = strnlen_s("testing", max_len);
    EXPLEN(max_len)
    /*--------------------------------------------------*/

    max_len = 3;
    len = strnlen_s("testing", max_len);
    EXPLEN(max_len)
    /*--------------------------------------------------*/

    max_len = 7;
    len = strnlen_s("testing", max_len);
    STDLEN()
    /*--------------------------------------------------*/

    max_len = 8;
    len = strnlen_s("testing", max_len);
    STDLEN()
    /*--------------------------------------------------*/

    max_len = 9;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("str overflow")
    len = strnlen_s("testing", 9);
#else
    len = strnlen_s("testing", max_len); /* depends on optimizer's var-tracking */
#endif
    STDLEN()

    /*--------------------------------------------------*/
    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strnlen_s()); }
#endif
