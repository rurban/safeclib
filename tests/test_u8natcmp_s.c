/*------------------------------------------------------------------
 * test_u8natcmp_s
 * File 'extu8/u8natcmp_s.c'
 * Lines executed:98.61% of 72
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char8_t str2[LEN];
int test_u8natcmp_s(void);
#define CMPSIGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)

int test_u8natcmp_s(void) {
    errno_t rc;
    int result;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8natcmp_s(NULL, LEN, str2, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = u8natcmp_s(str1, LEN, NULL, &result);
    ERR(ESNULLP)

    EXPECT_BOS("empty resultp")
    rc = u8natcmp_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8natcmp_s(str1, 0, str2, &result);
    ERR(ESZEROL)
#endif

    /*--------------------------------------------------*/

    /* digit runs compare numerically: file2 < file10 */
    strcpy((char *)str1, "file10");
    strcpy((char *)str2, "file2");

    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result <= 0) {
        debug_printf("%s %u  result=%d, expected >0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* equal strings */
    strcpy((char *)str1, "file2");
    strcpy((char *)str2, "file2");

    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* plain lexicographic difference still applies outside digit runs */
    strcpy((char *)str1, "abc");
    strcpy((char *)str2, "abd");

    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result >= 0) {
        debug_printf("%s %u  result=%d, expected <0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* u8natfccmp_s folds ASCII case */
    strcpy((char *)str1, "File2");
    strcpy((char *)str2, "file2");

    rc = u8natfccmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* natural order variants */

    /* digit run shorter: smaller */
    strcpy((char *)str1, "file2");
    strcpy((char *)str2, "file10");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != -1) {
        debug_printf("%s %u  \"file2\" <=> \"file10\" result=%d, expected sign -1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* digit run longer: greater */
    strcpy((char *)str1, "file10");
    strcpy((char *)str2, "file2");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != 1) {
        debug_printf("%s %u  \"file10\" <=> \"file2\" result=%d, expected sign 1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* same length run, greater value */
    strcpy((char *)str1, "file29");
    strcpy((char *)str2, "file12");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != 1) {
        debug_printf("%s %u  \"file29\" <=> \"file12\" result=%d, expected sign 1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* same length run, smaller value */
    strcpy((char *)str1, "file12");
    strcpy((char *)str2, "file29");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != -1) {
        debug_printf("%s %u  \"file12\" <=> \"file29\" result=%d, expected sign -1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* left aligned (leading zero) equal runs, then differ */
    strcpy((char *)str1, "a01b");
    strcpy((char *)str2, "a01c");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != -1) {
        debug_printf("%s %u  \"a01b\" <=> \"a01c\" result=%d, expected sign -1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* left aligned runs, smaller */
    strcpy((char *)str1, "a01");
    strcpy((char *)str2, "a02");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != -1) {
        debug_printf("%s %u  \"a01\" <=> \"a02\" result=%d, expected sign -1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* left aligned runs, greater */
    strcpy((char *)str1, "a02");
    strcpy((char *)str2, "a01");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != 1) {
        debug_printf("%s %u  \"a02\" <=> \"a01\" result=%d, expected sign 1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* left aligned runs, dest run ends first */
    strcpy((char *)str1, "a01");
    strcpy((char *)str2, "a012");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != -1) {
        debug_printf("%s %u  \"a01\" <=> \"a012\" result=%d, expected sign -1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* left aligned runs, src run ends first */
    strcpy((char *)str1, "a012");
    strcpy((char *)str2, "a01");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != 1) {
        debug_printf("%s %u  \"a012\" <=> \"a01\" result=%d, expected sign 1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* left aligned runs, equal */
    strcpy((char *)str1, "a01");
    strcpy((char *)str2, "a01");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != 0) {
        debug_printf("%s %u  \"a01\" <=> \"a01\" result=%d, expected sign 0\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* leading spaces in dest are skipped */
    strcpy((char *)str1, "a  b");
    strcpy((char *)str2, "a b");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != 0) {
        debug_printf("%s %u  \"a  b\" <=> \"a b\" result=%d, expected sign 0\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* leading spaces in src are skipped */
    strcpy((char *)str1, "a b");
    strcpy((char *)str2, "a  b");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != 0) {
        debug_printf("%s %u  \"a b\" <=> \"a  b\" result=%d, expected sign 0\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* plain greater */
    strcpy((char *)str1, "abd");
    strcpy((char *)str2, "abc");
    rc = u8natcmp_s(str1, LEN, str2, &result);
    ERR(EOK)
    if (CMPSIGN(result) != 1) {
        debug_printf("%s %u  \"abd\" <=> \"abc\" result=%d, expected sign 1\n",
                     __FUNCTION__, __LINE__, result);
        errs++;
    }

    /* dmax ends the comparison early */
    strcpy((char *)str1, "abcdef");
    strcpy((char *)str2, "abcxyz");
    rc = u8natcmp_s(str1, 3, str2, &result);
    ERR(EOK)
    if (result != 0) {
        debug_printf("%s %u  result=%d, expected 0\n", __FUNCTION__, __LINE__,
                     result);
        errs++;
    }

    /* unterminated src */
    {
        char8_t usrc[8];
        memset(usrc, 'a', sizeof(usrc));
        memset(str1, 'a', 40);
        str1[40] = '\0';
        rc = u8natcmp_s(str1, LEN, usrc, &result);
        ERR(ESUNTERM)
    }

    /* dmax > RSIZE_MAX_STR is only checked for a dest of unknown size.
       Hide the size from the compiler. */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        if (!hp)
            return errs + 1;
        strcpy((char *)hp, "abc");
        rc = u8natcmp_s(hp, RSIZE_MAX_STR + 1, str2, &result);
        ERR(ESLEMAX)
        free(hp);
    }

    return (errs);
}

int main(void) { return (test_u8natcmp_s()); }
