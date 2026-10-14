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

#define TOKERR(e)                                                              \
    if (tok != NULL || errno != (e)) {                                         \
        debug_printf("%s %u  expected NULL/%d, got tok=%p errno=%d\n",         \
                     __FUNCTION__, __LINE__, (int)(e), (void *)tok, errno);    \
        errs++;                                                                \
    }

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

    /*--------------------------------------------------*/
    /* tokenizer context is NULL */

    ptr = NULL;
    dmax = 5;
    errno = 0;
    tok = u8tok_s(NULL, &dmax, (char8_t *)",", &ptr);
    TOKERR(ESNULLP)

    /* *dmaxp > RSIZE_MAX_STR: with dest == NULL the size is not known */
    strcpy((char *)str1, "a,b");
    ptr = str1;
    dmax = RSIZE_MAX_STR + 1;
    tok = u8tok_s(NULL, &dmax, (char8_t *)",", &ptr);
    TOKERR(ESLEMAX)

    /* *dmaxp > size of the known dest */
#ifndef HAVE_CT_BOS_OVR
    dmax = LEN + 1;
    EXPECT_BOS("dmax exceeds dest")
    tok = u8tok_s(str1, &dmax, (char8_t *)",", &ptr);
    TOKERR(EOVERFLOW)
#endif

    /*--------------------------------------------------*/
    /* leading delimiters are skipped */

    strcpy((char *)str1, ",;one,");
    dmax = (rsize_t)strlen((char *)str1);
    errno = 0;
    tok = u8tok_s(str1, &dmax, (char8_t *)",;", &ptr);
    if (!tok || strcmp((char *)tok, "one") || errno != 0) {
        debug_printf("%s %u  tok=\"%s\", expected \"one\"\n", __FUNCTION__,
                     __LINE__, tok ? (char *)tok : "(null)");
        errs++;
    }

    /* only delimiters: no token, not an error */
    strcpy((char *)str1, ",;,;");
    dmax = (rsize_t)strlen((char *)str1);
    errno = 0;
    tok = u8tok_s(str1, &dmax, (char8_t *)",;", &ptr);
    if (tok != NULL || errno != 0 || dmax != 0) {
        debug_printf("%s %u  tok=%p errno=%d dmax=%u, expected NULL 0 0\n",
                     __FUNCTION__, __LINE__, (void *)tok, errno,
                     (unsigned)dmax);
        errs++;
    }

    /*--------------------------------------------------*/
    /* unterminated dest: dmax ends while skipping delimiters */

    strcpy((char *)str1, ",,,,,,");
    dmax = 2;
    tok = u8tok_s(str1, &dmax, (char8_t *)",", &ptr);
    TOKERR(ESUNTERM)

    /* unterminated dest: dmax ends inside the token */

    strcpy((char *)str1, "abcdef,");
    dmax = 3;
    tok = u8tok_s(str1, &dmax, (char8_t *)",", &ptr);
    TOKERR(ESUNTERM)

    /*--------------------------------------------------*/
    /* unterminated (too long) delim: found while looking for the token
       start. The same check in the token end scan is not reachable, since
       the start scan has already scanned the whole delim for the first
       byte of the token. */

    {
        char8_t delim[24];
        memset(delim, 'x', 20);
        delim[20] = '\0';

        strcpy((char *)str1, "abc");
        dmax = (rsize_t)strlen((char *)str1);
        tok = u8tok_s(str1, &dmax, delim, &ptr);
        TOKERR(ESUNTERM)

        /* a leading delimiter byte matched early does not hide it */
        memset(delim, 'x', 20);
        delim[0] = ',';
        delim[20] = '\0';
        strcpy((char *)str1, ",abc");
        dmax = (rsize_t)strlen((char *)str1);
        tok = u8tok_s(str1, &dmax, delim, &ptr);
        TOKERR(ESUNTERM)
    }

    return (errs);
}

int main(void) { return (test_u8tok_s()); }
