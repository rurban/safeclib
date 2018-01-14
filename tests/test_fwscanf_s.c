/*------------------------------------------------------------------
 * test_fwscanf_s
 * File 'wchar/fwscanf_s.c'
 * Lines executed:86.96% of 23
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include "safe_lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define LEN   ( 128 )

static wchar_t   wstr1[LEN];
static wchar_t   wstr2[LEN];
static char      str3[LEN];
#define TMP   "tmpfwscanf"
static FILE* stream = NULL;

void stuff_stream(wchar_t *restrict dest)
{
    if (!stream)
        stream = fopen(TMP, "w+");
    else
        rewind(stream);
    fwprintf(stream, L"%ls\n", dest);
    rewind(stream);
}

int test_fwscanf_s (void)
{
    errno_t rc;
    int32_t  ind;
    size_t  len1;
    size_t  len2;
    size_t  len3;
    int errs = 0;
    int p[2];
    FILE *f;

/*--------------------------------------------------*/

/* This pipe does not work on windows */
#ifndef __MINGW32__
    pipe(p);
    f = fdopen(p[0], "rb");
    if (!f) {
        close(p[0]);
        close(p[1]);
        return 0;
    }

    write(p[1], L"a", sizeof(L"a"));
#else
    stuff_stream(L"a");
#endif
    wstr2[0] = '\0';
    rc = fwscanf_s(NULL, L"%ls", wstr2);
    ERREOF(ESNULLP);
    WEXPSTR(wstr2, L"");

    rc = fwscanf_s(f, NULL, NULL);
    ERREOF(ESNULLP);

    /* SEGV
      rc = fwscanf_s(f, L"%ls", NULL);
      ERREOF(ESNULLP);
    */

    /* inconsistent:
      rc = fwscanf_s(f, L"", NULL);
      ERR(-1); or 0
    */

/*--------------------------------------------------*/

#ifndef __MINGW32__
    write(p[1], L"      24", sizeof(L"      24"));
#else
    stuff_stream(L"      24");
#endif
    rc = fwscanf_s(f, L"%ls %n", wstr2, LEN, &ind);
#ifndef __MINGW32__
    ERREOF(EINVAL);
#else
    ERREOF(ESNULLP);
#endif

    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L"%ls %%n", wstr2);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong fwscanf(\"\",L\"%%n\"): %d\n",
                     __FUNCTION__, __LINE__, (int)rc);
    } else
#endif
    ERR(1);
    ERRNO(0);

    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L"%ls %%n", wstr2, 6);
    ERR(1);
    ERRNO(0);

    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L"%s %%n", str3, 6);
    ERR(1);
    ERRNO(0);
#ifndef __MINGW32__
    EXPSTR(str3, "24");
#else
    EXPSTR(str3, "2");
#endif

    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L" %d", &len1);
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
    stuff_stream(wstr1);

    rc = fwscanf_s(stream, L"%ls", wstr2, LEN);
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
    stuff_stream(wstr1);

    rc = fwscanf_s(stream, L"%ls", wstr2, LEN);
    ERR(1);
    WEXPSTR(wstr1, L"keep it simple")

/*--------------------------------------------------*/

    wstr1[0] = L'\0';
    stuff_stream(wstr1);

    rc = fwscanf_s(stream, L"%ls", wstr2, LEN);
    ERR(1)
    /*WEXPNULL(str2) TODO. Got "eep" */

/*--------------------------------------------------*/

    wcscpy(wstr1, L"qqweqq");
    stuff_stream(wstr1);

    rc = fwscanf_s(stream, L"%ls", wstr2);
    NOERR()
    WEXPSTR(wstr2, wstr1);

/*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    wcscpy(wstr1, L"12345678901234567890");

    rc = fwscanf_s(stream, L"%ls", &wstr1[7]);
    ERR(1);
    WEXPSTR(wstr1, L"123456712345678901234567890");

    wcscpy(wstr1, L"123456789");

    rc = fwscanf_s(stream, L"%ls", &wstr1[8]);
    ERR(1);
    WEXPSTR(wstr1, L"12345678123456789");
    */

/*--------------------------------------------------*/

    /* TODO: we want to test a vfwscanf error propagated through vfwscanf_s.
       The only error is EOF(stream), but this is blocking.
       Reading from a closed stream is platform dependent.
     */
    fclose(stream);
    close(p[1]);

#if 0
    wcscpy(wstr1, L"qqweqq");
    stuff_stream(wstr1);

    rc = fwscanf_s(f, L"%ls", wstr2, LEN);
#if defined(__GLIBC__)
    if (rc < 0) {
        ERR(-1);
        ERRNO(EBADF);
    } else { /* TODO: reads from closed pipe */
        ERR(14); /* older glibc upstream bug */
        ERRNO(0);
    }
#elif defined __CYGWIN__
    ERR(1);
#else
    ERR(-1);
#endif
    /*WEXPNULL(wstr2); TODO zero the output args */
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
    SKIP_APPVEYOR(str3); /* mingw really */
    return (test_fwscanf_s());
}
#endif
