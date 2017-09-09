/*------------------------------------------------------------------
 * test_wcsncpy_s
 * File 'wchar/wcsncpy_s.c'
 * Lines executed:96.49% of 57
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
static wchar_t   dest[LEN];

int test_wcsncpy_s (void)
{
    errno_t rc;
    rsize_t nlen;
    int ind;
    int errs = 0;

/*--------------------------------------------------*/

    nlen = 5;
    rc = wcsncpy_s(NULL, LEN, str2, nlen);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    nlen = 5;
    rc = wcsncpy_s(str1, 5, NULL, nlen);
    ERR(ESNULLP)
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }

/*--------------------------------------------------*/

    nlen = 5;
    rc = wcsncpy_s(str1, 0, str2, nlen);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = wcsncpy_s(str1, (RSIZE_MAX_STR+1), str2, nlen);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = L'\0';

    rc = wcsncpy_s(str1, 5, str2, 0);
    ERR(ESZEROL)
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }

/*--------------------------------------------------*/

#if 1
   wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
   wcscpy(str2, L"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

    rc = wcsncpy_s(str1, 5, str2, (RSIZE_MAX_STR+1));
    ERR(ESLEMAX)
#endif

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = L'\0';
    nlen = 5;

    rc = wcsncpy_s(&str1[0], LEN/2, &str2[0], nlen);
    ERR(EOK)
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 5;

    /* test overlap */
    rc = wcsncpy_s(str1, LEN, str1, nlen);
    ERR(ESOVRLP)
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 18;

    rc = wcsncpy_s(&str1[0], LEN, &str1[5], nlen);
    ERR(ESOVRLP)
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    str2[0] = L'\0';

    nlen = 10;
    rc = wcsncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    if (str1[0] != L'\0') {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = L'\0';
    wcscpy(str2, L"keep it simple");

    nlen = 20;
    rc = wcsncpy_s(str1, LEN, str2, nlen);
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

    nlen = 32;
    rc = wcsncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/
/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcsncpy_s(str1, 1, str2, nlen);
    ERR(ESNOSPC)
    if (*str1 != L'\0') {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcsncpy_s(str1, 2, str2, nlen);
    ERR(ESNOSPC)
    if (*str1 != L'\0') {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/
/* TR example */

    wcscpy(dest, L"                            ");
    wcscpy(str1, L"hello");

    rc = wcsncpy_s(dest, 6, str1, 100);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(dest, str1);
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/
/* TR example */

    wcscpy(dest, L"                            ");
    wcscpy(str2, L"goodbye");

    rc = wcsncpy_s(dest, 5, str2, 7);
    ERR(ESNOSPC)
/*--------------------------------------------------*/
/* TR example */

    wcscpy(dest, L"                            ");
    wcscpy(str2, L"goodbye");

    rc = wcsncpy_s(dest, 5, str2, 4);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(dest, L"good");
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(dest, L"                            ");
    wcscpy(str2, L"good");

    /*   strnlen("good") < 5   */
    rc = wcsncpy_s(dest, 5, str2, 8);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(dest, L"good");
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(str1, L"qq12345weqeqeqeq");
    wcscpy(str2, L"it");

    nlen = 10;
    rc = wcsncpy_s(str1, 10, str2, nlen);
    ERR(EOK)
    /* be sure the results are the same as wcscmp */
    ind = wcscmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u -%ls-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcsncpy_s());
}
#endif
