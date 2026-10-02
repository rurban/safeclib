/*------------------------------------------------------------------
 * test_u8tok_s
 * File 'extu8/u8tok_s.c'
 * Lines executed:62.37% of 93
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

int test_u8tok_s(void);

int test_u8tok_s(void) {
    char8_t *tok;
    char8_t *ptr;
    char8_t str1[LEN];
    rsize_t dmax;
    int errs = 0;

    /*--------------------------------------------------*/

    strcpy((char *)str1, "one,two;three");
    dmax = (rsize_t)strlen((char *)str1);

    tok = u8tok_s(str1, &dmax, (char8_t *)",;", &ptr);
    if (!tok || strcmp((char *)tok, "one")) {
        debug_printf("%s %u  tok=\"%s\", expected \"one\"\n", __FUNCTION__,
                     __LINE__, tok ? (char *)tok : "(null)");
        errs++;
    }

    tok = u8tok_s(NULL, &dmax, (char8_t *)",;", &ptr);
    if (!tok || strcmp((char *)tok, "two")) {
        debug_printf("%s %u  tok=\"%s\", expected \"two\"\n", __FUNCTION__,
                     __LINE__, tok ? (char *)tok : "(null)");
        errs++;
    }

    tok = u8tok_s(NULL, &dmax, (char8_t *)",;", &ptr);
    if (!tok || strcmp((char *)tok, "three")) {
        debug_printf("%s %u  tok=\"%s\", expected \"three\"\n", __FUNCTION__,
                     __LINE__, tok ? (char *)tok : "(null)");
        errs++;
    }

    tok = u8tok_s(NULL, &dmax, (char8_t *)",;", &ptr);
    if (tok != NULL) {
        debug_printf("%s %u  tok=\"%s\", expected NULL\n", __FUNCTION__,
                     __LINE__, (char *)tok);
        errs++;
    }

    /*--------------------------------------------------*/
    /* utf-8 payload around the (ASCII) delimiter */

    strcpy((char *)str1, "caf\xC3\xA9,th\xC3\xA9");
    dmax = (rsize_t)strlen((char *)str1);

    tok = u8tok_s(str1, &dmax, (char8_t *)",", &ptr);
    if (!tok || strcmp((char *)tok, "caf\xC3\xA9")) {
        debug_printf("%s %u  tok=\"%s\", expected \"caf\\xC3\\xA9\"\n",
                     __FUNCTION__, __LINE__, tok ? (char *)tok : "(null)");
        errs++;
    }

    tok = u8tok_s(NULL, &dmax, (char8_t *)",", &ptr);
    if (!tok || strcmp((char *)tok, "th\xC3\xA9")) {
        debug_printf("%s %u  tok=\"%s\", expected \"th\\xC3\\xA9\"\n",
                     __FUNCTION__, __LINE__, tok ? (char *)tok : "(null)");
        errs++;
    }

    /*--------------------------------------------------*/
    /* runtime-constraint violations */

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dmaxp")
    tok = u8tok_s(str1, NULL, (char8_t *)",", &ptr);
    if (tok != NULL || errno != ESNULLP) {
        debug_printf("%s %u  expected NULL/ESNULLP, got tok=%p errno=%d\n",
                     __FUNCTION__, __LINE__, (void *)tok, errno);
        errs++;
    }
#endif

    dmax = 0;
    EXPECT_BOS("empty *dmaxp")
    tok = u8tok_s(str1, &dmax, (char8_t *)",", &ptr);
    if (tok != NULL || errno != ESZEROL) {
        debug_printf("%s %u  expected NULL/ESZEROL, got tok=%p errno=%d\n",
                     __FUNCTION__, __LINE__, (void *)tok, errno);
        errs++;
    }

    dmax = 5;
    EXPECT_BOS("empty delim")
#ifndef HAVE_CT_BOS_OVR
    tok = u8tok_s(str1, &dmax, NULL, &ptr);
    if (tok != NULL || errno != ESNULLP) {
        debug_printf("%s %u  expected NULL/ESNULLP, got tok=%p errno=%d\n",
                     __FUNCTION__, __LINE__, (void *)tok, errno);
        errs++;
    }
#endif

    EXPECT_BOS("empty ptr")
#ifndef HAVE_CT_BOS_OVR
    tok = u8tok_s(str1, &dmax, (char8_t *)",", NULL);
    if (tok != NULL || errno != ESNULLP) {
        debug_printf("%s %u  expected NULL/ESNULLP, got tok=%p errno=%d\n",
                     __FUNCTION__, __LINE__, (void *)tok, errno);
        errs++;
    }
#endif

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8tok_s()); }
