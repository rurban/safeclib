/*------------------------------------------------------------------
 * test_wcsnatcmp_s
 * File 'wcsnatcmp_s.c'
 * Lines executed:88.29% of 111
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>

#define LEN (128)

static wchar_t str1[LEN];
static wchar_t str2[LEN];
int test_wcsnatcmp_s(void);

int test_wcsnatcmp_s(void) {
    errno_t rc;
    int ind;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = wcsnatcmp_s(NULL, LEN, str2, LEN, &ind);
    ERR(ESNULLP)
    INDZERO()

    EXPECT_BOS("empty src")
    rc = wcsnatcmp_s(str1, LEN, NULL, LEN, &ind);
    ERR(ESNULLP)
    INDZERO()

    EXPECT_BOS("empty resultp")
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = wcsnatcmp_s(str1, 0, str2, LEN, &ind);
    ERR(ESZEROL)
    INDZERO()

    EXPECT_BOS("empty src or smax")
    rc = wcsnatcmp_s(str1, LEN, str2, 0, &ind);
    ERR(ESZEROL)
    INDZERO()

    EXPECT_BOS("dest overflow")
    rc = wcsnatcmp_s(str1, RSIZE_MAX_STR + 1, str2, LEN, &ind);
    ERR(ESLEMAX)
    INDZERO()

    EXPECT_BOS("src overflow")
    rc = wcsnatcmp_s(str1, LEN, str2, RSIZE_MAX_STR + 1, &ind);
    ERR(ESLEMAX)
    INDZERO()
#endif

    /*--------------------------------------------------*/

    str1[0] = L'\0';
    str2[0] = L'\0';

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    wcscpy(str2, L"keep it simple");

    rc = wcsnatcmp_s(str1, 5, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    wcscpy(str1, L"Keep it simple");
    wcscpy(str2, L"keep it simple");

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    wcscpy(str2, L"keeP it simple");

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");

    rc = wcsnatcmp_s(str1, LEN, str1, LEN, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simplified");
    wcscpy(str2, L"keep it simple");

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(<= 0)

    /*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    wcscpy(str2, L"keep it simplified");

    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(>= 0)

    /*--------------------------------------------------*/

#if defined(HAVE___BUILTIN_OBJECT_SIZE) && !defined(HAVE_CT_BOS_OVR)
    EXPECT_BOS("src overflow")
    rc = wcsnatcmp_s(str1, LEN, str2, LEN + 1, &ind);
    ERR(EOVERFLOW)
    INDZERO()
#endif

    /* hide the buffer sizes from the compiler: BOS_UNKNOWN paths */
    {
        static volatile size_t hsz = LEN * sizeof(wchar_t);
        wchar_t *hd = (wchar_t *)malloc(hsz);
        wchar_t *hs = (wchar_t *)malloc(hsz);

        if (!hd || !hs) {
            free(hd);
            free(hs);
            return errs + 1;
        }
        wcscpy(hd, L"abc");
        wcscpy(hs, L"abc");

        rc = wcsnatcmp_s(hd, RSIZE_MAX_STR + 1, hs, LEN, &ind);
        ERR(ESLEMAX)
        INDZERO()

        rc = wcsnatcmp_s(hd, LEN, hs, RSIZE_MAX_WSTR + 1, &ind);
        ERR(ESLEMAX)
        INDZERO()

        rc = wcsnatcmp_s(hd, LEN, hs, LEN, &ind);
        ERR(EOK)
        INDZERO()

        wcscpy(hs, L"abd");
        rc = wcsnatcmp_s(hd, LEN, hs, LEN, &ind);
        ERR(EOK)
        INDCMP(!= -1)

        free(hd);
        free(hs);
    }

    /*--------------------------------------------------*/
    /* natural order of digit runs */

    wcscpy(str1, L"a10");
    wcscpy(str2, L"a9");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = wcsnatcmp_s(str2, LEN, str1, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    wcscpy(str1, L"a12b");
    wcscpy(str2, L"a12b");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

    /* same magnitude: first different digit decides (bias) */
    wcscpy(str1, L"a19");
    wcscpy(str2, L"a18");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = wcsnatcmp_s(str2, LEN, str1, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    /* the bias is kept after a later digit differs the other way */
    wcscpy(str1, L"a21");
    wcscpy(str2, L"a18");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = wcsnatcmp_s(str2, LEN, str1, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    /* leading zeros: left-aligned (fractional) comparison */
    wcscpy(str1, L"a01");
    wcscpy(str2, L"a02");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = wcsnatcmp_s(str2, LEN, str1, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    wcscpy(str2, L"a01");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

    wcscpy(str1, L"a0");
    wcscpy(str2, L"a00");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = wcsnatcmp_s(str2, LEN, str1, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    /* leading spaces are skipped */
    wcscpy(str1, L"a  b");
    wcscpy(str2, L"a b");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

    wcscpy(str1, L"a b");
    wcscpy(str2, L"a  c");
    rc = wcsnatcmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    /* src is not terminated within smax */
    wcscpy(str1, L"abcdef");
    wcscpy(str2, L"abcdef");
    rc = wcsnatcmp_s(str1, LEN, str2, 3, &ind);
    ERR(ESUNTERM)

    /*--------------------------------------------------*/
    /* case folding */

    wcscpy(str1, L"Keep");
    wcscpy(str2, L"keeQ");
    rc = wcsnaticmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    wcscpy(str1, L"A10");
    wcscpy(str2, L"a9");
    rc = wcsnaticmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    wcscpy(str1, L"abcdef");
    wcscpy(str2, L"ABC");
    /* src is a prefix of dest */
    rc = wcsnaticmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    /* dest is a prefix of src */
    rc = wcsnaticmp_s(str2, LEN, str1, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    /* equal after folding, the fold mode smax is the folded length */
    wcscpy(str1, L"abc");
    wcscpy(str2, L"ABC");
    rc = wcsnaticmp_s(str1, 3, str2, 3, &ind);
    ERR(EOK)
    INDZERO()

    /* multi-char folds fit into the fold buffer */
    wcscpy(str1, L"\x1f82\x1f82");
    wcscpy(str2, L"a");
    rc = wcsnaticmp_s(str1, 2, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = wcsnaticmp_s(str2, LEN, str1, 2, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    return (errs);
}

int main(void) { return (test_wcsnatcmp_s()); }
