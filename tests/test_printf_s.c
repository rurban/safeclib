/*------------------------------------------------------------------
 * test_printf_s
 * File 'printf_s.c'
 * Lines executed:81.25% of 16
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#ifdef HAVE_PRINTF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
int test_printf_s(void);

int test_printf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;

    /*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    rc = printf_s(NULL);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    NEGERR_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    rc = printf_s("");
    NEGERR(EOK)

    /* TODO
    rc = printf_s("%s", NULL);
    NEGERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    str1[0] = '\0';
    rc = printf_s("%s%n\n", str1, &ind);
    NEGERR(EINVAL)

    /*--------------------------------------------------*/

    rc = printf_s("%s%%n\n", str1);
    if (rc < 0) {
        printf("Failed to open stdout for write: %s\n", strerror(errno));
        return errs;
    }
    ERR(3)

    rc = printf_s("%%n\n");
    ERR(3);

    /*--------------------------------------------------*/

    strcpy(str1, "12");
    strcpy(str2, "34");

    rc = printf_s("%s%s", str1, str2);
    ERR(4)

    /*--------------------------------------------------*/

#if 0
    /* 0x7fffffff + 1 >INT_MAX */
    rc = printf_s("\n%2147483648d\n", INT_MAX);
    ANYERR();
#if defined(__GLIBC__)
    ERRNO(EOVERFLOW);
#endif

    /* this segfaults under darwin */
    rc = printf_s("%s\n", L'\xd834df01');
    ANYERR();

    rc = printf_s("%*9$d", 0);
    ANYERR();
#endif

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_printf_s()); }
