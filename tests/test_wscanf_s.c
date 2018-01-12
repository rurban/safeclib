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

#define LEN   ( 128 )

static wchar_t   wstr1[LEN];
static wchar_t   wstr2[LEN];
static char      str3[LEN];
#define TMP   "tmpwscanf"
static FILE* stream = NULL;

void stuff_stdin(wchar_t *restrict dest)
{
    stream = fopen(TMP, "w+");
    fwprintf(stream, L"%ls\n", dest);
    fclose(stream);
    freopen(TMP, "r", stdin);
}

int test_wscanf_s (void)
{
    errno_t rc;
    int32_t  ind;
    size_t  len1;
    size_t  len2;
    size_t  len3;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wscanf_s(NULL, NULL);
    ERREOF(ESNULLP);

    /* TODO: should error */
#if 0
    rc = wscanf_s(L"%ls", NULL);
    ERREOF(ESNULLP);
#endif

/*--------------------------------------------------*/

    stuff_stdin(L"      24");
    rc = wscanf_s(L"%ls %n", wstr2, LEN, &ind);
    ERREOF(EINVAL);

    stuff_stdin(L"      24");
    rc = wscanf_s(L"%ls %%n", wstr2);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong fwscanf(\"\",L\"%%n\"): %d\n",
                     __FUNCTION__, __LINE__, (int)rc);
    } else
#endif
    ERR(1);
    ERRNO(0);

    stuff_stdin(L"      24");
    rc = wscanf_s(L"%ls %%n", wstr2, 6);
    ERR(1);
    ERRNO(0);

    stuff_stdin(L"      24");
    rc = wscanf_s(L"%s %%n", str3, 6);
    ERR(1);
    ERRNO(0);
    EXPSTR(str3, "24");

    stuff_stdin(L"      24");
    rc = wscanf_s(L" %d", &len1);
    ERR(1);
    ERRNO(0);
    if ((int)len1 != 24) {
        debug_printf("%s %u wrong arg: %d\n",
                     __FUNCTION__, __LINE__, (int)len1);
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
#ifdef DEBUG
        size_t len1 = wcslen(wstr1);
#endif
        debug_printf("%s %u lengths wrong: %d  %d  %d \n",
                     __FUNCTION__, __LINE__, (int)len1, (int)len2, (int)len3);
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
    ERR(-1);
    WEXPNULL(wstr2);

/*--------------------------------------------------*/

    wstr1[0] = '\0';
    stuff_stdin(wstr1);

    rc = wscanf_s(L"%ls", wstr2, LEN);
    ERR(-1)
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
    debug_printf("%s %u skip test reading from closed stream\n",
                 __FUNCTION__, __LINE__);
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

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wscanf_s());
}
#endif
