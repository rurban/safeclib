/*------------------------------------------------------------------
 * test_freopen_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <unistd.h>

#define TMP "tmpfreopen"

int test_freopen_s (void)
{
    errno_t rc;
    int errs = 0;
    FILE *tmp, *new;
    FILE *file = stdin;

/*--------------------------------------------------*/

    rc = freopen_s(NULL, TMP, "r", file);
    ERR(ESNULLP);

    rc = freopen_s(&tmp, TMP, NULL, file);
    ERR(ESNULLP);

    rc = freopen_s(&tmp, TMP, "r", NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = freopen_s(&tmp, TMP, "r", file);
    ERR(ENOENT)
    if (errno)
        ERRNO(ENOENT);

/*--------------------------------------------------*/

    /* TODO: fails with asan on some glibc systems (not repro) in strlen.
       glibc bug. */
#ifndef HAVE_ASAN
    file = stdin;
    rc = freopen_s(&tmp, NULL, "rb", file);
    if (rc == 0) { 
        ERR(0); /* EINVAL or EFAULT */
        if (errno)
            ERRNO(EINVAL);
    }
#endif

/*--------------------------------------------------*/

    tmp = fopen(TMP, "w");
    if (tmp == NULL) {
        puts ("fopen failed: %m");
        return errs+1;
    }

    fputs("Hello 1\n", tmp);
    rc = freopen_s(&new, TMP, "a+", tmp);
    ERR(0);
    fputs("Hello 2\n", new);
    fclose(new);

/*--------------------------------------------------*/
    
    unlink(TMP);

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_freopen_s());
}
#endif
