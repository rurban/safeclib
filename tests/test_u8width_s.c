/*------------------------------------------------------------------
 * test_u8width_s
 * File 'extu8/u8width_s.c'
 * Lines executed:94.12% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
int test_u8width_s(void);

#define EXPLEN(n)                                                              \
    if (len != n) {                                                            \
        debug_printf("%s %u   len=%u \n", __FUNCTION__, __LINE__,              \
                     (unsigned)len);                                           \
        errs++;                                                                \
    }
#define STDLEN()                                                               \
    if (std_len != len) {                                                      \
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__, \
                     (unsigned)std_len, (unsigned)len);                        \
        errs++;                                                                \
    }

int test_u8width_s(void) {
    rsize_t len;
    rsize_t std_len;
    rsize_t max_len;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    max_len = 3;
    EXPECT_BOS("empty str")
    len = u8width_s(NULL, max_len);
    EXPLEN(0)
    /*--------------------------------------------------*/

    max_len = 0;
    EXPECT_BOS("empty str or smax")
    len = u8width_s("test", 0);
    EXPLEN(0)
    /*--------------------------------------------------*/

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    strcpy(str1, "test");
    EXPECT_BOS("str overflow")
    len = u8width_s(str1, LEN + 1);
    EXPLEN(4)
#endif

    strcpy(str1, "test");
    max_len = RSIZE_MAX_STR + 1;
    EXPECT_BOS("str overflow")
    len = u8width_s(str1, max_len);
    EXPLEN(0)

    /* overflow: sizeof = 5 */
    /* PS: compile-time check once fixed by adding const'ness and all warnings
     */
    EXPECT_BOS("str overflow")
    len = u8width_s("test", 6);
    EXPLEN(4)
    EXPECT_BOS("str overflow")
    len = u8width_s("test", 7);
    EXPLEN(4)
    EXPECT_BOS("str overflow")
    len = u8width_s("test", 21);
    EXPLEN(4)

#endif

    /* no overflow: sizeof = 5 */
    len = u8width_s("test", 4);
    EXPLEN(4)
    /* no overflow: sizeof = 5 */
    len = u8width_s("test", 5);
    EXPLEN(4)

    //return errs;
    /*--------------------------------------------------*/

    std_len = 0;
    strcpy(str1, "");
    max_len = LEN;

    len = u8width_s(str1, LEN);
    STDLEN()

    /*--------------------------------------------------*/

    std_len = 1;
    strcpy(str1, "t");
    max_len = LEN;

    len = u8width_s(str1, max_len);
    STDLEN()

    len = u8width_s("t", 1); /* static string */
    STDLEN()

    /*--------------------------------------------------*/

    std_len = 2;
    strcpy(str1, "to");
    max_len = LEN;

    len = u8width_s(str1, max_len);
    STDLEN()

    /*--------------------------------------------------*/

    std_len = strlen("testing");
    strcpy(str1, "testing");
    max_len = LEN;

    len = u8width_s(str1, max_len);
    STDLEN()

    /*--------------------------------------------------*/

    max_len = 1;
    len = u8width_s("testing", 1);
    EXPLEN(max_len)

    max_len = 2;
    len = u8width_s("testing", 2);
    EXPLEN(max_len)

    max_len = 3;
    len = u8width_s("testing", 3);
    EXPLEN(max_len)

    /*--------------------------------------------------*/
    len = u8width_s("test", 5);
    EXPLEN(4)

    len = u8width_s("진서/真書", LEN);
    EXPLEN(5)
    len = u8width_s("암클", LEN);
    EXPLEN(2)
    len = u8width_s("Café", LEN); // TODO normalize
    EXPLEN(4)
    len = u8width_s("Café", LEN);
    EXPLEN(4)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8width_s()); }
