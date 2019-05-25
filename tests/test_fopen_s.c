/*------------------------------------------------------------------
 * test_fopen_s
 * File 'io/fopen_s.c'
 * Lines executed:100.00% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <unistd.h>

#define TMP "tmpfopen"

#ifdef HAVE_FOPEN_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"
int test_fopen_s(void);

int test_fopen_s(void) {
    errno_t rc;
    int errs = 0;
    FILE *tmp;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    rc = fopen_s(NULL, TMP, "r");
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    rc = fopen_s(&tmp, NULL, "r");
    ERR_MSVC(ESNULLP, EINVAL);

    rc = fopen_s(&tmp, TMP, NULL);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    rc = fopen_s(&tmp, TMP, "w");
    ERR(0);

    rc = fopen_s(&tmp, "<xx", "r");
    NOERR(); /* ENOENT or 0 */

    /*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_fopen_s()); }
#endif
