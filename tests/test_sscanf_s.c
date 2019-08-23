/*------------------------------------------------------------------
 * test_sscanf_s
 * File 'io/sscanf_s.c'
 * Lines executed:100.00% of 23
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
    EXPECT_BOS("empty fmt")
    rc = sscanf_s(str1, NULL, NULL);
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
    rc = sscanf_s(str1, "%s %n", str2, LEN, &ind);
    ERREOF(EINVAL);

    rc = sscanf_s(str1, "%%n");
    ERR(0);
    ERRNO(0);

    rc = sscanf_s(str1, "%s %%n", str2);
    ERR(1);
    ERRNO(0);

    rc = sscanf_s(str1, "%s %%n", str2, 6);
    ERR(1);
    ERRNO(0);

    rc = sscanf_s(str1, "%s %%n", str3, 6);
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

    /* TODO
    rc = sscanf_s(str1, "%s", NULL);
    ERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    len1 = strlen(str1);

    rc = sscanf_s(str1, "%s", str2, LEN);
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

    rc = sscanf_s(str1, "%s", str2, LEN);
    ERR(1);
    EXPSTR(str1, "keep it simple")

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = sscanf_s(str1, "%s", str2, LEN);
    ERR(-1)
    EXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sscanf_s(str1, "%s", str2, LEN);
    ERR(-1)
    EXPSTR(str1, "")

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    rc = sscanf_s(str1, "%s", str2);
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
