/*------------------------------------------------------------------
 * test_vfwscanf_s
 * File 'wchar/vfwscanf_s.c'
 * Lines executed:85.71% of 21
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
#define TMP   "tmpvfwscanf"
static FILE* stream = NULL;

static void stuff_stream(wchar_t *restrict dest)
{
    if (!stream)
        stream = fopen(TMP, "w+");
    else
        rewind(stream);
    fwprintf(stream, L"%ls\n", dest);
    rewind(stream);
}

static int vtwscanf_s (FILE *stream, const wchar_t *restrict fmt, ...)
{
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vfwscanf_s(stream, fmt, ap);
    va_end(ap);
    return rc;
}

static int test_vfwscanf_s (void)
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

    pipe(p);
    f = fdopen(p[0], "rb");
    if (!f) {
        close(p[0]);
        close(p[1]);
        return 0;
    }

    write(p[1], L"a", sizeof(L"a"));
    wstr2[0] = '\0';
    rc = vtwscanf_s(NULL, L"%ls", wstr2, LEN);
    ERREOF(ESNULLP);
    WEXPSTR(wstr2, L"");

    rc = vfwscanf_s(f, NULL, NULL);
    ERREOF(ESNULLP);

    /* SEGV
      rc = vfwscanf_s(f, L"%ls", NULL);
      ERREOF(ESNULLP);
    */

/*--------------------------------------------------*/

    write(p[1], L"      24", sizeof(L"      24"));
    rc = vtwscanf_s(f, L"%ls %n", wstr2, LEN, &ind);
    ERREOF(EINVAL);

    stuff_stream(L"      24");
    rc = vtwscanf_s(stream, L"%ls %%n", wstr2, LEN);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong vfwscanf(\"\",L\"%%n\"): %d\n",
                     __FUNCTION__, __LINE__, (int)rc);
    } else
#endif
    ERR(1);
    ERRNO(0);

    stuff_stream(L"      24");
    rc = vtwscanf_s(stream, L"%ls %%n", wstr2, 6);
    ERR(1);
    ERRNO(0);

    stuff_stream(L"      24");
    rc = vtwscanf_s(stream, L"%s %%n", str3, 6);
    ERR(1);
    ERRNO(0);
    EXPSTR(str3, "24");

    stuff_stream(L"      24");
    rc = vtwscanf_s(stream, L" %d", &len1);
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

    rc = vtwscanf_s(stream, L"%ls", wstr2, LEN);
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

    rc = vtwscanf_s(stream, L"%ls", wstr2, LEN);
    ERR(1);
    WEXPSTR(wstr1, L"keep it simple")

/*--------------------------------------------------*/

    wstr1[0] = '\0';
    wstr2[0] = '\0';
    stuff_stream(wstr1);

    rc = vtwscanf_s(stream, L"%ls", wstr2, LEN);
    ERR(1)
    WEXPNULL(wstr1)

/*--------------------------------------------------*/

    wstr1[0] = '\0';
    wcscpy(wstr2, L"keep it simple");
    stuff_stream(wstr1);

    rc = vtwscanf_s(stream, L"%ls", wstr2, LEN);
    ERR(1)
    WEXPSTR(wstr1, L"")

/*--------------------------------------------------*/

    wcscpy(wstr1, L"qqweqq");
    wcscpy(wstr2, L"keep it simple");
    stuff_stream(wstr1);

    rc = vtwscanf_s(stream, L"%ls", wstr2);
    NOERR()
    WEXPSTR(wstr1, wstr2);

/*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    wcscpy(wstr1, L"12345678901234567890");

    rc = vtwscanf_s(stream, L"%ls", &wstr1[7]);
    ERR(1);
    WEXPSTR(wstr1, L"123456712345678901234567890");

    wcscpy(wstr1, L"123456789");

    rc = vtwscanf_s(stream, L"%ls", &wstr1[8]);
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
    rc = vtwscanf_s(f, L"%ls", wstr2, LEN);

#if defined(__GLIBC__)
    if (rc < 0) {
        ERR(-1);
        ERRNO(EBADF);
    } else {
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
    return (test_vfwscanf_s());
}
#endif
