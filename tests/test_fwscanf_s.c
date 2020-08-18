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

/*
#ifndef _WIN32
#define USE_PIPE
#endif
*/

#ifdef HAVE_FWSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static wchar_t wstr1[LEN];
static wchar_t wstr2[LEN];
static char str3[LEN];
#define TMP "tmpfwscanf"
static FILE *stream = NULL;
void win_stuff_stream(const wchar_t *dest);
int test_fwscanf_s(void);

void win_stuff_stream(const wchar_t *dest) {
    if (!stream)
        stream = fopen(TMP, "w+");
    else
        rewind(stream);
    fwprintf(stream, L"%ls\n", dest);
    rewind(stream);
}

#ifndef USE_PIPE
#define stuff_stream(s)                                                        \
    wcscpy(wstr1, s);                                                          \
    len1 = wcslen(s);                                                          \
    win_stuff_stream(s);
#else
#define stuff_stream(s)                                                        \
    wcscpy(wstr1, s);                                                          \
    len1 = wcslen(s);                                                          \
    write(p[1], (s), sizeof(s) - 1);                                           \
    write(p[1], L"\n", sizeof(L"\n") - 1);
#endif

int test_fwscanf_s(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int num = 0;
    int errs = 0;
#ifdef USE_PIPE
    int p[2];
#endif

    /*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

/* This pipe does not work on windows */
#ifdef USE_PIPE
    if (pipe(p)) {
        printf("Failed to create pipe()\n");
        return 1;
    }
    stream = fdopen(p[0], "rb");
    if (!stream) {
        printf("Failed to fdopen pipe[0]\n");
        close(p[0]);
        close(p[1]);
        return 1;
    }
#endif
    stuff_stream(L"a");
    wstr2[0] = '\0';
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty stream")
    rc = fwscanf_s(NULL, L"%ls", wstr2);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERR(EOF);
    ERRNO_MSVC(ESNULLP, EINVAL);
    WEXPSTR(wstr2, L"");

    EXPECT_BOS("empty fmt")
    rc = fwscanf_s(stream, NULL, NULL);
    ERREOF(ESNULLP);
#endif
    /* SEGV
      rc = fwscanf_s(stream, L"%ls", NULL);
      ERREOF(ESNULLP);
    */

    /* inconsistent:
      rc = fwscanf_s(stream, L"", NULL);
      ERR(-1); or 0
    */

    /*--------------------------------------------------*/

    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L"%ls %n", wstr2, LEN, &ind);
    ERREOF(EINVAL);

    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L"%ls %%n", wstr2);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong fwscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    } else
#endif
        ERR(1);
    ERRNO(0);

    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L"%ls %%n", wstr2, LEN);
    ERR(1);
    ERRNO(0);

    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L"%s %%n", str3, LEN);
    ERR(1);
    ERRNO(0);
#if !defined(_WIN32) || defined(HAVE_MINGW64)
    EXPSTR(str3, "24");
#else
    EXPSTR(str3, "2");
#endif
    stuff_stream(L"      24");
    rc = fwscanf_s(stream, L" %d", &num);
    ERR(1);
    ERRNO(0);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/

    stuff_stream(L"aaaaaaaaaa");

    rc = fwscanf_s(stream, L"%ls", wstr2, LEN);
    ERR(1)
    len2 = wcslen(wstr2);
    len3 = wcslen(wstr1);
    if (len3 != len2) {
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
        errs++;
    }

    /*--------------------------------------------------*/

    stuff_stream(L"keep it simple");

    rc = fwscanf_s(stream, L"%ls", wstr2, LEN);
    ERR(1);
    WEXPSTR(wstr1, L"keep it simple")

    /*--------------------------------------------------*/

    stuff_stream(L"qqweqq");

    rc = fwscanf_s(stream, L"%ls", wstr2);
    NOERR()
    WEXPSTR(wstr2, wstr1);

    /*--------------------------------------------------*/

    wstr1[0] = L'\0';
    stuff_stream(L"");

    rc = fwscanf_s(stream, L"%ls", wstr2, LEN);
    ERR(1);
    /*WEXPNULL(str2) TODO. Got "eep" */

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
#ifdef USE_PIPE
    close(p[1]);
#endif

#if 0
    wcscpy(wstr1, L"qqweqq");
    stuff_stream(L"qqweqq");

    rc = fwscanf_s(stream, L"%ls", wstr2, LEN);
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
int main(void) { return (test_fwscanf_s()); }
#endif
