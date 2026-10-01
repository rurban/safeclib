/*------------------------------------------------------------------
 * test_vsscanf_s
 * File 'io/vsscanf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#ifdef HAVE_SSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
static char str3[LEN];
static int vtsscanf_s(const char *restrict buffer, const char *restrict fmt,
                      ...);
int test_vsscanf_s(void);

static int vtsscanf_s(const char *restrict buffer, const char *restrict fmt,
                      ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vsscanf_s(buffer, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vsscanf_s(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int num = 0;
    int errs = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    EXPECT_BOS("empty fmt")
    rc = vtsscanf_s(str1, NULL, NULL);
    GCC_DIAG_RESTORE
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    str2[0] = '\0';
    EXPECT_BOS("empty buffer")
    rc = vtsscanf_s(NULL, "%s", str2);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "      24");
    GCC_DIAG_IGNORE(-Wformat)
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtsscanf_s(str1, "%s %n", str2, LEN, &ind);
    GCC_DIAG_RESTORE
    ERREOF(EINVAL);

    rc = vtsscanf_s(str1, "%%n");
    ERR(0);
    ERRNO(0);

    rc = vtsscanf_s(str1, "%s %%n", str2);
    ERR(1);
    ERRNO(0);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtsscanf_s(str1, "%s %%n", str2, 6);
    GCC_DIAG_RESTORE
    ERR(1);
    ERRNO(0);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtsscanf_s(str1, "%s %%n", str3, 6);
    GCC_DIAG_RESTORE
    ERR(1);
    ERRNO(0);
    EXPSTR(str3, "24");

    rc = vtsscanf_s(str1, "%%n");
    ERR(0);
    ERRNO(0);

    rc = vtsscanf_s(str1, " %d", &num);
    ERR(1);
    ERRNO(0);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    len1 = strlen(str1);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtsscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
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

    strcpy(str1, "keep it simple");

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtsscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1);
    EXPSTR(str1, "keep it simple")

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtsscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(-1)
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtsscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(-1)
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    rc = vtsscanf_s(str1, "%s", str2);
    NOERR()
    EXPSTR(str1, str2);

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vsscanf_s()); }
