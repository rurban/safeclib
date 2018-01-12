/*------------------------------------------------------------------
 * test_getenv_s
 * File 'os/getenv_s.c'
 * Lines executed:100.00% of 32
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <stdlib.h>

#define LEN   ( 2048 )

static char   dest[LEN];

int test_getenv_s (void)
{
    errno_t rc;
    int errs = 0;
    int ind;
    size_t len;
    char *str2;
    char *name = "PATH";

/*--------------------------------------------------*/

    rc = getenv_s(&len, NULL, LEN, name);
    ERR(ESNULLP);

    rc = getenv_s(&len, dest, LEN, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = getenv_s(&len, dest, 0, name);
    ERR(ESZEROL);

/*--------------------------------------------------*/

    rc = getenv_s(&len, dest, RSIZE_MAX_STR+1, name);
    ERR(ESLEMAX);

/*--------------------------------------------------*/

    rc = getenv_s(&len, dest, 1, name);
    ERR(ESNOSPC);

/*--------------------------------------------------*/

    rc = getenv_s(&len, dest, LEN, name);
    ERR(EOK);
    str2 = getenv(name);
    EXPSTR(dest, str2);
    ind = strlen(str2);
    INDCMP(!= (int)len);

    rc = getenv_s(NULL, dest, LEN, name);
    ERR(EOK);
    EXPSTR(dest, str2);

/*--------------------------------------------------*/

    rc = getenv_s(NULL, dest, LEN, "c#hewhc&wehc%erwhc$weh");
    ERR(-1);
    EXPNULL(dest);

    rc = getenv_s(&len, dest, LEN, "c#hewhc&wehc%erwhc$weh");
    ERR(-1);
    ind = len;
    INDCMP(!= 0);
    EXPNULL(dest);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_getenv_s());
}
#endif
