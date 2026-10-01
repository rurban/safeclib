/*------------------------------------------------------------------
 * test_vscanf_s
 * File 'io/vscanf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef HAVE_VSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
#define TMP "tmpvscanf"
static FILE *stream = NULL;
static void stuff_stdin(const char *dest);
static int vtscanf_s(const char *restrict fmt, ...);
int test_vscanf_s(void);

static void stuff_stdin(const char *dest) {
    stream = fopen(TMP, "w+");
    fprintf(stream, "%s\n", dest);
    fclose(stream);
    stream = freopen(TMP, "r", stdin);
}

static int vtscanf_s(const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vscanf_s(fmt, ap);
    va_end(ap);
    return rc;
}

int test_vscanf_s(void) {
    errno_t rc;
    int32_t ind;
    int i1;
    size_t len1;
    size_t len2;
    size_t len3;
    int errs = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtscanf_s(NULL, NULL);
    GCC_DIAG_RESTORE
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    stuff_stdin("      24");
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtscanf_s("%s %%n", str2, LEN);
    GCC_DIAG_RESTORE
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    ERRNO(0);

    stuff_stdin("      24");
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtscanf_s("%s %%n", str2, 6);
    GCC_DIAG_RESTORE
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    ERRNO(0);
    EXPSTR(str2, "24");

    stuff_stdin("      24");
    rc = vtscanf_s(" %d", &i1);
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    ERRNO(0);
    if (i1 != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, i1);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    len1 = strlen(str1);
    stuff_stdin(str1);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtscanf_s("%s", str2, LEN);
    GCC_DIAG_RESTORE
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1)
    len2 = strlen(str2);
    len3 = strlen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        len1 = strlen(str1);
#endif
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");
    stuff_stdin(str1);

    rc = vtscanf_s("%s", str2);
    NOERR()
    EXPSTR(str2, str1);

    /*--------------------------------------------------*/

    stuff_stdin("      24");
    GCC_DIAG_IGNORE(-Wformat)
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtscanf_s("%s %n", str2, LEN, &ind);
    GCC_DIAG_RESTORE
    ERREOF(EINVAL);

    /*--------------------------------------------------*/

    str1[0] = '\0';
    stuff_stdin(str1);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtscanf_s("%s", str2, LEN);
    GCC_DIAG_RESTORE
    if (rc != -1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(-1)

    /*--------------------------------------------------*/

#ifdef __linux
    debug_printf("%s %u skip test reading from closed stream\n", __FUNCTION__,
                 __LINE__);
#else
    fclose(stream);

    strcpy(str1, "qqweqq");
    stuff_stdin(str1);
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtscanf_s("%s", str2, LEN);
    GCC_DIAG_RESTORE
#endif

    /*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

#ifndef __KERNEL__
int main(void) { return (test_vscanf_s()); }
#endif
