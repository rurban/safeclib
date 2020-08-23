/*------------------------------------------------------------------
 * test_vswscanf_s
 * File 'wchar/vswscanf_s.c'
 * Lines executed:100.00% of 21
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#define LEN (128)

static wchar_t wstr1[LEN];
static wchar_t wstr2[LEN];
static char str3[LEN];
static int vtwscanf_s(wchar_t *restrict dest, const wchar_t *restrict fmt, ...);
int test_vswscanf_s(void);

static int vtwscanf_s(wchar_t *restrict dest, const wchar_t *restrict fmt,
                      ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vswscanf_s(dest, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vswscanf_s(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int num = 0;
    int errs = 0;

    /*--------------------------------------------------*/

    rc = vtwscanf_s(wstr1, NULL, NULL);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERREOF(ESNULLP);
#else
    ERRNO(0);
#endif

    /*--------------------------------------------------*/

    wstr2[0] = '\0';
    rc = vtwscanf_s(NULL, L"%ls", wstr2);
    ERREOF(ESNULLP);

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"      24");
    rc = vtwscanf_s(wstr1, L"%ls %n", wstr2, LEN, &ind);
    ERREOF(EINVAL);

    rc = vtwscanf_s(wstr1, L"%ls %%n", wstr2);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong vswscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    } else
#endif
        ERR(1);
    ERRNO(0);

    rc = vtwscanf_s(wstr1, L"%ls %%n", wstr2, 6);
    ERR(1);
    ERRNO(0);

    rc = vtwscanf_s(wstr1, L"%s %%n", str3, 6);
    ERR(1);
    ERRNO(0);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    EXPSTR(str3, "24");
#else
    EXPSTR(str3, "2");
#endif

    rc = vtwscanf_s(wstr1, L" %d", &num);
    ERR(1);
    ERRNO(0);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/

    /* TODO
    rc = vtwscanf_s(wstr1, L"%s", NULL);
    ERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"aaaaaaaaaa");
    len1 = wcslen(wstr1);

    rc = vtwscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR(1)
    len2 = wcslen(wstr2);
    len3 = wcslen(wstr1);
    if (len3 != len2) {
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
        errs++;
    }

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it simple");

    rc = vtwscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR(1);
    WEXPSTR(wstr1, L"keep it simple")

    /*--------------------------------------------------*/

    wstr1[0] = '\0';
    wstr2[0] = '\0';

    rc = vtwscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR(-1)
    WEXPNULL(wstr1)

    /*--------------------------------------------------*/

    wstr1[0] = '\0';
    wcscpy(wstr2, L"keep it simple");

    rc = vtwscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR(-1)
    WEXPSTR(wstr1, L"")

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"qqweqq");
    wcscpy(wstr2, L"keep it simple");

    rc = vtwscanf_s(wstr1, L"%ls", wstr2);
    NOERR()
    WEXPSTR(wstr1, wstr2);

    /*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    wcscpy(wstr1, L"12345678901234567890");

    rc = vtwscanf_s(wstr1, L"%ls", &wstr1[7]);
    ERR(1);
    WEXPSTR(wstr1, L"123456712345678901234567890");

    wcscpy(wstr1, L"123456789");

    rc = vtwscanf_s(wstr1, L"%ls", &wstr1[8]);
    ERR(1);
    WEXPSTR(wstr1, L"12345678123456789");
    */

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vswscanf_s()); }
