/*------------------------------------------------------------------
 * test_wcscpy_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

int test_wcscpy_s (void)
{
    errno_t rc;
    uint32_t i;
    int     ind;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcscpy_s(NULL, LEN, str2);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    rc = wcscpy_s(str1, 5, NULL);
    ERR(ESNULLP)
#ifdef SAFE_LIB_STR_NULL_SLACK
    for (i=0; i<5; i++) {
        if (str1[i] != L'\0') {
            debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
            errs++;
        }
    }
#else
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
#endif

/*--------------------------------------------------*/

    rc = wcscpy_s(str1, 0, str2);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = wcscpy_s(str1, (RSIZE_MAX_STR+1), str2);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = L'\0';

    rc = wcscpy_s(str1, LEN/2, str2);
    ERR(EOK)
#ifdef SAFE_LIB_STR_NULL_SLACK
    for (i=0; i<LEN/2; i++) {
        if (str1[i] != L'\0') {
            debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
            errs++;
        }
    }
#else
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
#endif

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    rc = wcscpy_s(str1, LEN, str1);
    ERR(EOK)

/*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");

    rc = wcscpy_s(&str1[0], LEN, &str1[5]);
    ERR(ESOVRLP)
#ifdef SAFE_LIB_STR_NULL_SLACK
    for (i=0; i<LEN; i++) {
        if (str1[i] != L'\0') {
            debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
            errs++;
        }
    }
#else
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
#endif

    wcscpy(str1, L"keep it simple");
    rc = wcscpy_s(&str1[5], LEN, &str1[0]);
    ERR(ESOVRLP)

/*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    str2[0] = L'\0';

    rc = wcscpy_s(str1, LEN, str2);
    ERR(EOK)
    if (*str1 != L'\0') {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    str1[0] = L'\0';
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, LEN, str2);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, LEN, str2);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, 1, str2);
    ERR(ESNOSPC)
    if (*str1 != L'\0') {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcscpy_s(str1, 2, str2);
    ERR(ESNOSPC)
    if (*str1 != L'\0') {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"it");

    rc = wcscpy_s(str1, 3, str2);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"it");
    wcscpy(str2, L"qqweqeqeqeq");

    rc = wcscpy_s(str2, 3, str1);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"qq12345weqeqeqeq");
    wcscpy(str2, L"it");

    rc = wcscpy_s(str1, 10, str2);
    ERR(EOK)
    /* be sure the results are the same as wcscpy */
    ind = wcscmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
    for (i=0; i<10; i++) {
        debug_printf("str1[%i] = %d \n", i, str1[i]);
        /*errs++;*/
    }

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcscpy_s());
}
#endif
