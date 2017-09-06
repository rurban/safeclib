/*------------------------------------------------------------------
 * test_fopen_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <unistd.h>

#define TMP "tmpfopen"

int test_fopen_s (void)
{
    errno_t rc;
    int errs = 0;
    FILE *tmp;

/*--------------------------------------------------*/

    rc = fopen_s(NULL, TMP, "r");
    ERR(ESNULLP);

    rc = fopen_s(&tmp, NULL, "r");
    ERR(ESNULLP);

    rc = fopen_s(&tmp, TMP, NULL);
    ERR(ESNULLP);

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
int main (void)
{
    return (test_fopen_s());
}
#endif
