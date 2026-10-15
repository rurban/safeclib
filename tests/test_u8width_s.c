/*------------------------------------------------------------------
 * test_u8width_s
 * File 'extu8/u8width_s.c'
 * Lines executed:97.01% of 67
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
    len = u8width_s((const char8_t *)"test", 0);
    EXPLEN(0)
    /*--------------------------------------------------*/

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    strcpy((char *)str1, "test");
    EXPECT_BOS("str overflow")
    len = u8width_s(str1, LEN + 1);
    EXPLEN(4)
#endif

    strcpy((char *)str1, "test");
    max_len = RSIZE_MAX_STR + 1;
    EXPECT_BOS("str overflow")
    len = u8width_s(str1, max_len);
    EXPLEN(0)

    /* overflow: sizeof = 5 */
    /* PS: compile-time check once fixed by adding const'ness and all warnings
     */
    EXPECT_BOS("str overflow")
    len = u8width_s((const char8_t *)"test", 6);
    EXPLEN(4)
    EXPECT_BOS("str overflow")
    len = u8width_s((const char8_t *)"test", 7);
    EXPLEN(4)
    EXPECT_BOS("str overflow")
    len = u8width_s((const char8_t *)"test", 21);
    EXPLEN(4)

#endif

    /* no overflow: sizeof = 5 */
    len = u8width_s((const char8_t *)"test", 4);
    EXPLEN(4)
    /* no overflow: sizeof = 5 */
    len = u8width_s((const char8_t *)"test", 5);
    EXPLEN(4)

    // return errs;
    /*--------------------------------------------------*/

    std_len = 0;
    strcpy((char *)str1, "");
    max_len = LEN;

    len = u8width_s(str1, LEN);
    STDLEN()

    /*--------------------------------------------------*/

    std_len = 1;
    strcpy((char *)str1, "t");
    max_len = LEN;

    len = u8width_s(str1, max_len);
    STDLEN()

    len = u8width_s((const char8_t *)"t", 1); /* static string */
    STDLEN()

    /*--------------------------------------------------*/

    std_len = 2;
    strcpy((char *)str1, "to");
    max_len = LEN;

    len = u8width_s(str1, max_len);
    STDLEN()

    /*--------------------------------------------------*/

    std_len = strlen("testing");
    strcpy((char *)str1, "testing");
    max_len = LEN;

    len = u8width_s(str1, max_len);
    STDLEN()

    /*--------------------------------------------------*/

    max_len = 1;
    len = u8width_s((const char8_t *)"testing", 1);
    EXPLEN(max_len)

    max_len = 2;
    len = u8width_s((const char8_t *)"testing", 2);
    EXPLEN(max_len)

    max_len = 3;
    len = u8width_s((const char8_t *)"testing", 3);
    EXPLEN(max_len)

    /*--------------------------------------------------*/
    len = u8width_s((const char8_t *)"test", 5);
    EXPLEN(4)

    len = u8width_s((const char8_t *)"진서/真書", LEN);
    EXPLEN(5)
    len = u8width_s((const char8_t *)"암클", LEN);
    EXPLEN(2)
    len = u8width_s((const char8_t *)"Café", LEN); // TODO normalize
    EXPLEN(4)
    len = u8width_s((const char8_t *)"Café", LEN);
    EXPLEN(4)

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* extended grapheme cluster rules (TR29) */

    /* CR LF is one cluster (GB3) */
    len = u8width_s((const char8_t *)"\r\n", LEN);
    EXPLEN(1)

    /* break after control (GB4) */
    len = u8width_s((const char8_t *)"\na", LEN);
    EXPLEN(2)

    /* break before control (GB5) */
    len = u8width_s((const char8_t *)"a\n", LEN);
    EXPLEN(2)

    /* break around CR */
    len = u8width_s((const char8_t *)"a\rb", LEN);
    EXPLEN(3)

    /* L x L (GB6) */
    len = u8width_s((const char8_t *)"\xE1\x84\x80\xE1\x84\x80", LEN);
    EXPLEN(1)

    /* L x V (GB6) */
    len = u8width_s((const char8_t *)"\xE1\x84\x80\xE1\x85\xA1", LEN);
    EXPLEN(1)

    /* L x LV (GB6) */
    len = u8width_s((const char8_t *)"\xE1\x84\x80\xEA\xB0\x80", LEN);
    EXPLEN(1)

    /* L x LVT (GB6) */
    len = u8width_s((const char8_t *)"\xE1\x84\x80\xEA\xB0\x81", LEN);
    EXPLEN(1)

    /* LV x V (GB7) */
    len = u8width_s((const char8_t *)"\xEA\xB0\x80\xE1\x85\xA1", LEN);
    EXPLEN(1)

    /* V x T (GB7) */
    len = u8width_s((const char8_t *)"\xE1\x85\xA1\xE1\x86\xA8", LEN);
    EXPLEN(1)

    /* T x T (GB8) */
    len = u8width_s((const char8_t *)"\xE1\x86\xA8\xE1\x86\xA8", LEN);
    EXPLEN(1)

    /* LVT x T (GB8) */
    len = u8width_s((const char8_t *)"\xEA\xB0\x81\xE1\x86\xA8", LEN);
    EXPLEN(1)

    /* combining mark is Extend (GB9) */
    len = u8width_s((const char8_t *)"e\xCC\x81", LEN);
    EXPLEN(1)

    /* ZWJ (GB9) */
    len = u8width_s((const char8_t *)"a\xE2\x80\x8D", LEN);
    EXPLEN(1)

    /* SpacingMark (GB9a) */
    len = u8width_s((const char8_t *)"\xE0\xA4\x95\xE0\xA4\x83", LEN);
    EXPLEN(1)

    /* Prepend (GB9b) */
    len = u8width_s((const char8_t *)"\xD8\x80" "a", LEN);
    EXPLEN(1)

    /* clusters are counted separately */
    len = u8width_s((const char8_t *)"e\xCC\x81" "e\xCC\x81" "x", LEN);
    EXPLEN(3)


    return (errs);
}

int main(void) { return (test_u8width_s()); }
