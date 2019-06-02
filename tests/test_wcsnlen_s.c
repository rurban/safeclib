/*------------------------------------------------------------------
 * test_wcsnlen_s
 * File 'wchar/wcsnlen_s.c'
 * Lines executed:94.12% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

static wchar_t str1[LEN];
int test_wcsnlen_s(void);

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

int test_wcsnlen_s(void) {
    rsize_t len;
    rsize_t std_len;
    rsize_t max_len;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    max_len = 3;
    EXPECT_BOS("empty str")
    len = wcsnlen_s(NULL, max_len);
    EXPLEN(0)
    /*--------------------------------------------------*/

    max_len = 0;
    EXPECT_BOS("empty str or smax")
    len = wcsnlen_s(L"test", 0);
    EXPLEN(0)
    /*--------------------------------------------------*/

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    wcscpy(str1, L"test");
    EXPECT_BOS("str overflow")
    len = wcsnlen_s(str1, LEN + 1);
    EXPLEN(4)
#endif

    wcscpy(str1, L"test");
    max_len = RSIZE_MAX_WSTR + 1;
    EXPECT_BOS("str overflow")
    len = wcsnlen_s(str1, RSIZE_MAX_WSTR + 1);
    /* They allow more */
#if !defined(MINGW_HAS_SECURE_API) && !defined(_WSTRING_S_DEFINED)
    EXPLEN(0)
#elif !(defined(TEST_MSVCRT) && defined(HAVE_WCSNLEN_S))
    EXPLEN(0)
#else
    if (max_len < INT_MAX) {
        EXPLEN(4)
    } else {
        EXPLEN(0)
    }
#endif

    /* overflow: sizeof = 5 */
    /* PS: compile-time check once fixed by adding const'ness and all warnings
     */
    EXPECT_BOS("str overflow")
    len = wcsnlen_s(L"test", 6);
    EXPLEN(4)
    EXPECT_BOS("str overflow")
    len = wcsnlen_s(L"test", 7);
    EXPLEN(4)
    EXPECT_BOS("str overflow")
    len = wcsnlen_s(L"test", 21);
    EXPLEN(4)

#endif

    /* no overflow: sizeof = 5 */
    len = wcsnlen_s(L"test", 4);
    EXPLEN(4)

    /* no overflow: sizeof = 5 */
    len = wcsnlen_s(L"test", 5);
    EXPLEN(4)

    //return errs;
    /*--------------------------------------------------*/

    std_len = 0;
    wcscpy(str1, L"");
    max_len = LEN;

    len = wcsnlen_s(str1, LEN);
    STDLEN()

    /*--------------------------------------------------*/

    std_len = 1;
    wcscpy(str1, L"t");
    max_len = LEN;

    len = wcsnlen_s(str1, max_len);
    STDLEN()

    len = wcsnlen_s(L"t", 1); /* static string */
    STDLEN()

    /*--------------------------------------------------*/

    std_len = 2;
    wcscpy(str1, L"to");
    max_len = LEN;

    len = wcsnlen_s(str1, max_len);
    STDLEN()

    /*--------------------------------------------------*/

    std_len = strlen("testing");
    wcscpy(str1, L"testing");
    max_len = LEN;

    len = wcsnlen_s(str1, max_len);
    STDLEN()

    /*--------------------------------------------------*/

    max_len = 1;
    len = wcsnlen_s(L"testing", 1);
    EXPLEN(max_len)

    /*--------------------------------------------------*/

    max_len = 2;
    len = wcsnlen_s(L"testing", 2);
    EXPLEN(max_len)

    /*--------------------------------------------------*/

    max_len = 3;
    len = wcsnlen_s(L"testing", 3);
    EXPLEN(max_len)

    /*--------------------------------------------------*/
    len = wcsnlen_s(L"test", 5);
    EXPLEN(4)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_wcsnlen_s()); }
