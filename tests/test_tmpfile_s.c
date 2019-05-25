/*------------------------------------------------------------------
 * test_tmpfile_s
 * File 'io/tmpfile_s.c'
 * Lines executed:58.82% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"

#ifdef HAVE_TMPFILE_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"
int test_tmpfile_s(void);

int test_tmpfile_s(void) {
    errno_t rc;
    int errs = 0;
    FILE *tmp;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);

    rc = tmpfile_s(NULL);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EACCES);

    /*--------------------------------------------------*/

    rc = tmpfile_s(&tmp);
    if (!(rc == 0 || rc == EACCES || rc == ESLEMAX)) {
        debug_printf("%s %u   Error rc=%d \n", __FUNCTION__, __LINE__, rc);
        errs++;
    }

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_tmpfile_s()); }
#endif
