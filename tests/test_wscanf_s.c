/*------------------------------------------------------------------
 * test_wscanf_s
 * File 'wchar/wscanf_s.c'
 * Lines executed:100.00% of 19
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef HAVE_WSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static wchar_t wstr1[LEN];
static wchar_t wstr2[LEN];
static char str3[LEN];
void stuff_stdin(const wchar_t *dest);
int test_wscanf_s(void);

#define TMP "tmpwscanf"
static FILE *stream = NULL;

void stuff_stdin(const wchar_t *dest) {
    stream = fopen(TMP, "w+");
    fwprintf(stream, L"%ls\n", dest);
    fclose(stream);
    stream = freopen(TMP, "r", stdin);
}

int test_wscanf_s(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int num = 0;
    int errs = 0;
    int have_wine = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    /* wine msvcrt doesn't check fmt==NULL */
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_FPRINTF_S))
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    rc = wscanf_s(NULL);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif
#else
    printf("Using wine\n");
    have_wine = 1;
    use_msvcrt = 1;
#endif

    /* TODO: should error */
#if 0
    rc = wscanf_s(L"%ls", NULL);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    wstr2[0] = '\0';
    stuff_stdin(L"      24");
    /* msvcrt doesn't check for %n neither */
    rc = wscanf_s(L"%ls %n", wstr2, LEN, &ind);
    if (!use_msvcrt) {
        ERREOF(EINVAL);
        WEXPSTR(wstr2, L"");
    }

    stuff_stdin(L"      24");
    rc = wscanf_s(L"%ls %%n", wstr2);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong fwscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    } else
#endif
        ERR(1);
    ERRNO_MSVC(0, have_wine ? 0 : EINVAL);

    stuff_stdin(L"      24");
    rc = wscanf_s(L"%ls %%n", wstr2, 6);
    ERR(1);
    ERRNO_MSVC(0, have_wine ? 0 : EINVAL);
    WEXPSTR(wstr2, L"24");

    stuff_stdin(L"      24");
    rc = wscanf_s(L"%s %%n", str3, 6);
    ERR(1);
    ERRNO_MSVC(0, have_wine ? 0 : EINVAL);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    EXPSTR(str3, "24");
#else
    EXPSTR(str3, "2");
#endif

    stuff_stdin(L"      24");
    rc = wscanf_s(L" %d", &num);
    ERR(1);
    ERRNO_MSVC(0, have_wine ? 0 : EINVAL);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"aaaaaaaaaa");
    len1 = wcslen(wstr1);
    stuff_stdin(wstr1);

    rc = wscanf_s(L"%ls", wstr2, LEN);
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
    stuff_stdin(wstr1);

    rc = wscanf_s(L"%ls", wstr2, LEN);
    ERR(1);
    WEXPSTR(wstr2, L"keep");

    /*--------------------------------------------------*/

    wstr1[0] = '\0';
    wstr2[0] = '\0';
    stuff_stdin(wstr1);

    rc = wscanf_s(L"%ls", wstr2, LEN);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERR(-1);
#else
    ERR(0);
#endif
    WEXPNULL(wstr2);

    /*--------------------------------------------------*/

    wstr1[0] = '\0';
    stuff_stdin(wstr1);

    rc = wscanf_s(L"%ls", wstr2, LEN);
#if !defined(HAVE_MINGW32) || defined(HAVE_MINGW64)
    ERR(-1);
#else
    ERR(0);
#endif
    WEXPNULL(wstr2);

    /*--------------------------------------------------*/

    wcscpy(wstr1, L"qqweqq");
    wcscpy(wstr2, L"keep it simple");
    stuff_stdin(wstr1);

    rc = wscanf_s(L"%ls", wstr2);
    NOERR()
    WEXPSTR(wstr2, wstr1);

    /*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    wcscpy(wstr1, L"12345678901234567890");

    rc = wscanf_s(L"%ls", &wstr1[7]);
    ERR(1);
    WEXPSTR(wstr1, L"123456712345678901234567890");

    wcscpy(wstr1, L"123456789");

    rc = wscanf_s(L"%ls", &wstr1[8]);
    ERR(1);
    WEXPSTR(wstr1, L"12345678123456789");
    */

    /*--------------------------------------------------*/

#ifdef __linux
    debug_printf("%s %u skip test reading from closed stream\n", __FUNCTION__,
                 __LINE__);
#else
    fclose(stream); /* crashes with linux glibc or musl: double free */

    wcscpy(wstr1, L"qqweqq");
    stuff_stdin(wstr1);
    /* would block reading */
    rc = wscanf_s(L"%ls", wstr2, LEN);
#endif

    /*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_wscanf_s()); }
