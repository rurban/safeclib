/*------------------------------------------------------------------
 * test_tmpnam_s
 * File 'io/tmpnam_s.c'
 * Lines executed:100.00% of 19
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"

#ifdef HAVE_TMPNAM_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static char   str1[LEN];

int test_tmpnam_s (void)
{
    errno_t rc;
    int len, ind;
    int errs = 0;

/*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    rc = tmpnam_s(NULL, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    rc = tmpnam_s(NULL, 5);
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    rc = tmpnam_s(str1, 0);
    ERR_MSVC(ESZEROL, ERANGE);

/*--------------------------------------------------*/

    rc = tmpnam_s(str1, RSIZE_MAX_STR+1);
    ERR_MSVC(ESLEMAX, 0);

/*--------------------------------------------------*/

    rc = tmpnam_s(str1, L_tmpnam_s+1);
    if (!(rc == 0 || rc == ESLEMAX)) {
        debug_printf("%s %u \"%s\"  Error rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");

    rc = tmpnam_s(str1, 2);
    ERR_MSVC(ESNOSPC, ERANGE);
    if (*str1 != '\0') {
        debug_printf("%s %u \"%s\"  Error rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaa");

    rc = tmpnam_s(str1, LEN/2);
    if (!(rc == 0 || rc == ESLEMAX)) {
        debug_printf("%s %u   Error rc=%d \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
    len = strlen(str1);
    CHECK_SLACK(&str1[len], LEN/2-len);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaa");

    rc = tmpnam_s(str1, LEN);
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
    return (test_tmpnam_s());
}
#endif
