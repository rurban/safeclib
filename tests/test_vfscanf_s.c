/*------------------------------------------------------------------
 * test_vfscanf_s
 * File 'io/vfscanf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef HAVE_VFSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
#define TMP "tmpvfscanf"
static FILE *stream = NULL;
static void stuff_stream(const char *dest);
static int vtfscanf_s(FILE *restrict f, const char *restrict fmt, ...);
int test_vfscanf_s(void);

static void stuff_stream(const char *dest) {
    if (!stream)
        stream = fopen(TMP, "w+");
    else
        rewind(stream);
    fprintf(stream, "%s\n", dest);
    rewind(stream);
}

static int vtfscanf_s(FILE *restrict f, const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vfscanf_s(f, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vfscanf_s(void) {
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
    EXPECT_BOS("empty stream")
    GCC_DIAG_IGNORE(-Wformat-zero-length)
    rc = vtfscanf_s(NULL, "");
    GCC_DIAG_RESTORE
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);

    stuff_stream("1");
    EXPECT_BOS("empty fmt")
    rc = vtfscanf_s(stream, NULL);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    stuff_stream("      24");
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtfscanf_s(stream, "%s %%n", str2, LEN);
    GCC_DIAG_RESTORE
#ifdef BSD_LIKE
    if (rc != -1) { /* BSD's return -1 on %%n */
        printf("%s %u wrong fscanf(\"\",\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    }
#else
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    ERRNO(0);
#endif

    stuff_stream("      24");
    rc = vtfscanf_s(stream, " %d", &i1);
    ERR(1);
    ERRNO(0);
    if (i1 != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, i1);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    stuff_stream(str1);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtfscanf_s(stream, "%s", str2, LEN);
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

    stuff_stream("keep it simple");

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtfscanf_s(stream, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1);
    EXPSTR(str2, "keep");

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    stuff_stream(str1);

    rc = vtfscanf_s(stream, "%s", str2);
    NOERR();
    EXPSTR(str2, str1);

    /*--------------------------------------------------*/

    stuff_stream("      24");
    GCC_DIAG_IGNORE(-Wformat)
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtfscanf_s(stream, "%s %n", str2, LEN, &ind);
    GCC_DIAG_RESTORE
    ERREOF(EINVAL);

    /*--------------------------------------------------*/

    str1[0] = '\0';
    stuff_stream(str1);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtfscanf_s(stream, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1); /* unspecified behaviour */

    /*--------------------------------------------------*/

    fclose(stream);

    /*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vfscanf_s()); }
