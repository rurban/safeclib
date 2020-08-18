/*------------------------------------------------------------------
 * test_swscanf_s
 * File 'wchar/swscanf_s.c'
 * Lines executed:100.00% of 23
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#ifdef HAVE_SWSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static wchar_t wstr1[LEN];
static wchar_t wstr2[LEN];
static char str3[LEN];

int main(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int num = 0;
    int errs = 0;
    int have_wine = 0;

    /*--------------------------------------------------*/

#ifdef __MINGW32__
    wcscpy(wstr1, L" ");
#endif
    print_msvcrt(use_msvcrt);

    /* wine msvcrt doesn't check fmt==NULL */
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_SWSCANF_S))
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    rc = swscanf_s(wstr1, NULL, NULL);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif
#else
    printf("Using wine\n");
    use_msvcrt = 1;
    have_wine = 1;
#endif

    /*--------------------------------------------------*/

    wstr2[0] = '\0';
    /* msvcrt doesn't check src==NULL neither */
#if !(defined(TEST_MSVCRT) && defined(HAVE_SWSCANF_S))
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty src")
    rc = swscanf_s(NULL, L"%ls", wstr2);
    ERREOF_MSVC(ESNULLP, EINVAL);
    WEXPSTR(wstr2, L"");
#endif
#endif

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"      24");
    /* msvcrt doesn't check for %n neither */
    rc = swscanf_s(wstr1, L"%ls %n", wstr2, LEN, &ind);
#ifdef HAVE_CT_BOS_OVR
    init_msvcrt(errno != EINVAL, &use_msvcrt);
#endif
    if (!use_msvcrt) {
        ERREOF(EINVAL);
        WEXPSTR(wstr2, L"");
    } else { /* msvcrt is pretty broken */
        ERR(1);
        ERRNO(0);
        WEXPSTR(wstr2, L"24");
    }

    rc = swscanf_s(wstr1, L"%%n");
#ifdef BSD_OR_NEWLIB_LIKE
    if (rc != 0) { /* BSD + cygwin they all return -1 on %%n */
        printf("%s %u wrong vswscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    } else
#endif
        ERR(0); /* e.g. cygwin32 vswscanf() returns -1 */
    ERRNO(0);

    rc = swscanf_s(wstr1, L"%ls %%n", wstr2);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong vswscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    } else
#endif
        ERR(1);
    ERRNO(0);
    WEXPSTR(wstr2, L"24");

    rc = swscanf_s(wstr1, L"%ls %%n", wstr2, 6);
    ERR(1);
    ERRNO(0);
    WEXPSTR(wstr2, L"24");

    rc = swscanf_s(wstr1, L"%s %%n", str3, 6);
    ERR(1);
    ERRNO(0);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    EXPSTR(str3, "24");
#else
    EXPSTR(str3, "2");
#endif

    rc = swscanf_s(wstr1, L" %d", &num);
    ERR(1);
    ERRNO(0);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/

    /* TODO
    rc = swscanf_s(wstr1, L"%s", NULL);
    ERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"aaaaaaaaaa");
    len1 = wcslen(wstr1);

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR(1)
    len2 = wcslen(wstr2);
    len3 = wcslen(wstr1);
    if (len3 != len2) {
#ifdef DEBUG
        len1 = wcslen(wstr1);
#endif
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
        errs++;
    }

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it simple");

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR(1);
    WEXPSTR(wstr1, L"keep it simple")

    /*--------------------------------------------------*/

    wstr1[0] = '\0';
    wstr2[0] = '\0';

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR_MSVC(-1, have_wine ? -1 : 0);
    WEXPNULL(wstr1);

    /*--------------------------------------------------*/

    wstr1[0] = '\0';
    wcscpy(wstr2, L"keep it simple");

    rc = swscanf_s(wstr1, L"%ls", wstr2, LEN);
    ERR_MSVC(-1, have_wine ? -1 : 0);
    WEXPSTR(wstr1, L"");

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"qqweqq");
    wcscpy(wstr2, L"keep it simple");

    rc = swscanf_s(wstr1, L"%ls", wstr2);
    NOERR()
    WEXPSTR(wstr1, wstr2);

    /*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    wcscpy(wstr1, L"12345678901234567890");

    rc = swscanf_s(wstr1, L"%ls", &wstr1[7]);
    ERR(1);
    WEXPSTR(wstr1, L"123456712345678901234567890");

    wcscpy(wstr1, L"123456789");

    rc = swscanf_s(wstr1, L"%ls", &wstr1[8]);
    ERR(1);
    WEXPSTR(wstr1, L"12345678123456789");
    */

    /*--------------------------------------------------*/

    return (errs);
}
