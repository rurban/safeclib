/*------------------------------------------------------------------
 * test_tmpfile_s
 * File 'io/tmpfile_s.c'
 * Lines executed:58.82% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"

int test_tmpfile_s (void)
{
    errno_t rc;
    int errs = 0;
    FILE *tmp;

/*--------------------------------------------------*/

    rc = tmpfile_s(NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = tmpfile_s(&tmp);
    if (!(rc == 0 || rc == ESLEMAX)) {
        debug_printf("%s %u   Error rc=%d \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_tmpfile_s());
}
#endif
