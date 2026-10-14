/*------------------------------------------------------------------
 * test_tmpfile_s
 * File 'io/tmpfile_s.c'
 * Lines executed:58.82% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#ifdef __linux__
#include <sys/resource.h>
#endif

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
#ifdef HAVE_TMPFILE_S
    ERR_MSVC(ESNULLP, EINVAL);
#else
    ERR_MSVC(ESNULLP, EACCES);
#endif

    /*--------------------------------------------------*/

    rc = tmpfile_s(&tmp);
    if (!(rc == 0 ||
#if !defined(HAVE_TMPFILE_S) && defined(_WIN32)
          // old mingw
          rc == EACCES ||
#endif
          rc == ESLEMAX)) {
        debug_printf("%s %u   Error rc=%d \n", __FUNCTION__, __LINE__, rc);
        errs++;
    }

    if (rc == 0)
        fclose(tmp);

    /*--------------------------------------------------*/

#ifdef __linux__
    /* exhaust the fd limit: tmpfile() fails with EMFILE */
    {
        struct rlimit old, lim;
        FILE *files[64];
        int n = 0;
        if (getrlimit(RLIMIT_NOFILE, &old) == 0) {
            lim = old;
            lim.rlim_cur = 16;
            if (setrlimit(RLIMIT_NOFILE, &lim) == 0) {
                do {
                    tmp = (FILE *)1;
                    rc = tmpfile_s(&tmp);
                    if (rc == 0 && n < 64)
                        files[n++] = tmp;
                } while (rc == 0 && n < 64);
                if (rc != EMFILE || tmp != NULL) {
                    debug_printf("%s %u   Error rc=%d \n", __FUNCTION__,
                                 __LINE__, rc);
                    errs++;
                }
                while (n > 0)
                    fclose(files[--n]);
                setrlimit(RLIMIT_NOFILE, &old);
            }
        }
    }
#endif

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_tmpfile_s()); }
#endif
