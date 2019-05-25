/*------------------------------------------------------------------
 * test_freopen_s
 * File 'io/freopen_s.c'
 * Lines executed:100.00% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <unistd.h>

#define TMP "tmpfreopen"

#ifdef HAVE_FREOPEN_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"
int test_freopen_s(void);

int test_freopen_s(void) {
    errno_t rc;
    int errs = 0;
    FILE *tmp, *newf;
    FILE *file = stdin;

    /*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

    rc = freopen_s(NULL, TMP, "r", file);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    rc = freopen_s(&tmp, TMP, NULL, file);
    ERR_MSVC(ESNULLP, EINVAL);

    rc = freopen_s(&tmp, TMP, "r", NULL);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    rc = freopen_s(&tmp, TMP, "r", file);
    ERR(ENOENT)
    if (errno)
        ERRNO(ENOENT);

        /*--------------------------------------------------*/

        /* TODO: fails with asan and valgrind on some glibc systems (not repro)
           in strlen or __open_nocancel. glibc bug. */
#ifndef __GLIBC__
#ifndef HAVE_ASAN
    file = stdin;
    rc = freopen_s(&tmp, NULL, "rb", file);
    if (rc == 0) {
        ERR(0); /* EINVAL or EFAULT */
        if (errno)
            ERRNO(EINVAL);
    }
#endif
#endif

    /*--------------------------------------------------*/

    tmp = fopen(TMP, "w");
    if (tmp == NULL) {
        puts("fopen failed: %m");
        return errs + 1;
    }

    fputs("Hello 1\n", tmp);
    rc = freopen_s(&newf, TMP, "a+", tmp);
    ERR(0);
    fputs("Hello 2\n", newf);
    fclose(newf);

    /*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_freopen_s()); }
