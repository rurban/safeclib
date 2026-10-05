/*------------------------------------------------------------------
 * test_sscanf_s
 * File 'io/sscanf_s.c'
 * Lines executed:100.00% of 24
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>
#ifdef HAVE_STDDEF_H
#include <stddef.h> // for ptrdiff_t
#endif

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
int test_sscanf_s(void);

int test_sscanf_s(void) {
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
    rc = sscanf_s(str1, NULL, NULL);
    GCC_DIAG_RESTORE
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    str2[0] = '\0';
    EXPECT_BOS("empty buffer")
    rc = sscanf_s(NULL, "%s", str2);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    strcpy(str1, "      24");
    GCC_DIAG_IGNORE(-Wformat)
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s %n", str2, LEN, &ind);
    GCC_DIAG_RESTORE
    ERREOF(EINVAL);

    rc = sscanf_s(str1, "%%n");
    ERR(0);
    ERRNO(0);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s %%n", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1);
    ERRNO(0);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s %%n", str2, 6);
    GCC_DIAG_RESTORE
    ERR(1);
    ERRNO(0);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s %%n", str3, 6);
    GCC_DIAG_RESTORE
    ERR(1);
    ERRNO(0);
    EXPSTR(str3, "24");

    rc = sscanf_s(str1, "%%n");
    ERR(0);
    ERRNO(0);

    rc = sscanf_s(str1, " %d", &num);
    ERR(1);
    ERRNO(0);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/
    /* floating point / long long / long double / ptrdiff_t coverage,
       exercising --disable-float, --disable-float-exp,
       --disable-long-long, --disable-long-double and
       --disable-printf-ptrdiff */
    {
        float fval;
        double dval;
        long double ldval;
        long long llval;
#ifdef HAVE_STDDEF_H
        ptrdiff_t tval;
#endif

        /* %f: plain decimal, gated only by --disable-float */
        strcpy(str1, "3.5");
        fval = -1;
        rc = sscanf_s(str1, "%f", &fval);
#ifdef PRINTF_DISABLE_SUPPORT_FLOAT
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (fval != 3.5f) {
            debug_printf("%s %u wrong float arg: %f\n", __FUNCTION__, __LINE__,
                         (double)fval);
            errs++;
        }
#endif

        /* %lf: double, same gate as %f */
        strcpy(str1, "-2.25");
        dval = -1;
        rc = sscanf_s(str1, "%lf", &dval);
#ifdef PRINTF_DISABLE_SUPPORT_FLOAT
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (dval != -2.25) {
            debug_printf("%s %u wrong double arg: %f\n", __FUNCTION__, __LINE__,
                         dval);
            errs++;
        }
#endif

        /* %e: exponential notation, additionally gated by
           --disable-float-exp */
        strcpy(str1, "3.5e2");
        fval = -1;
        rc = sscanf_s(str1, "%e", &fval);
#if defined(PRINTF_DISABLE_SUPPORT_FLOAT) ||                                   \
    defined(PRINTF_DISABLE_SUPPORT_EXPONENTIAL)
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (fval != 350.0f) {
            debug_printf("%s %u wrong float arg: %f\n", __FUNCTION__, __LINE__,
                         (double)fval);
            errs++;
        }
#endif

        /* %g: adaptive exponential notation, same gate as %e */
        strcpy(str1, "3.5e2");
        fval = -1;
        rc = sscanf_s(str1, "%g", &fval);
#if defined(PRINTF_DISABLE_SUPPORT_FLOAT) ||                                   \
    defined(PRINTF_DISABLE_SUPPORT_EXPONENTIAL)
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (fval != 350.0f) {
            debug_printf("%s %u wrong float arg: %f\n", __FUNCTION__, __LINE__,
                         (double)fval);
            errs++;
        }
#endif

        /* %a: hex float notation, same gate as %e */
        strcpy(str1, "0x1.cp+1");
        fval = -1;
        rc = sscanf_s(str1, "%a", &fval);
#if defined(PRINTF_DISABLE_SUPPORT_FLOAT) ||                                   \
    defined(PRINTF_DISABLE_SUPPORT_EXPONENTIAL)
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (fval != 3.5f) {
            debug_printf("%s %u wrong float arg: %f\n", __FUNCTION__, __LINE__,
                         (double)fval);
            errs++;
        }
#endif

        /* %Lf: long double, gated by --disable-long-double */
        strcpy(str1, "1.125");
        ldval = -1;
        rc = sscanf_s(str1, "%Lf", &ldval);
#if defined(PRINTF_DISABLE_SUPPORT_FLOAT) ||                                   \
    defined(PRINTF_DISABLE_SUPPORT_LONG_DOUBLE)
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (ldval != 1.125L) {
            debug_printf("%s %u wrong long double arg: %Lf\n", __FUNCTION__,
                         __LINE__, ldval);
            errs++;
        }
#endif

        /* %lld: long long, gated by --disable-long-long */
        strcpy(str1, "123456789012");
        llval = -1;
        rc = sscanf_s(str1, "%lld", &llval);
#ifdef PRINTF_DISABLE_SUPPORT_LONG_LONG
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (llval != 123456789012LL) {
            debug_printf("%s %u wrong long long arg: %lld\n", __FUNCTION__,
                         __LINE__, llval);
            errs++;
        }
#endif

#ifdef HAVE_STDDEF_H
        /* %td: ptrdiff_t, gated by --disable-printf-ptrdiff */
        strcpy(str1, "42");
        tval = -1;
        rc = sscanf_s(str1, "%td", &tval);
#ifdef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
        ERR(-1);
        ERRNO(0);
#else
        ERR(1);
        ERRNO(0);
        if (tval != 42) {
            debug_printf("%s %u wrong ptrdiff_t arg: %td\n", __FUNCTION__,
                         __LINE__, (ptrdiff_t)tval);
            errs++;
        }
#endif
#endif
    }

    /*--------------------------------------------------*/

    /* TODO
    rc = sscanf_s(str1, "%s", NULL);
    ERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    len1 = strlen(str1);

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
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
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1);
    EXPSTR(str1, "keep it simple")

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(-1)
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(-1)
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    NOERR()
    EXPSTR(str1, str2);

    /*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    strcpy(str1, "12345678901234567890");

    rc = sscanf_s(str1, "%s", &str1[7]);
    ERR(1);
    EXPSTR(str1, "123456712345678901234567890");

    strcpy(str1, "123456789");

    rc = sscanf_s(str1, "%s", &str1[8]);
    ERR(1);
    EXPSTR(str1, "12345678123456789");
    */

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_sscanf_s()); }
