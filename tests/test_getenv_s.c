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

#if defined(_WIN32) && defined(HAVE_GETENV_S)
# define USE_MSVCRT
#endif

#ifdef USE_MSVCRT
#define ERR_MSVC(n, winerr)                        \
    if (rc != (winerr)) {                          \
        debug_printf("%s %u  Error rc=%d \n",      \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#else
#define ERR_MSVC(n, winerr)                        \
    if (rc != (n)) {                               \
        debug_printf("%s %u  Error rc=%d \n",      \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#endif

#define LEN   ( 4090 )

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
    ERR_MSVC(ESNULLP, EINVAL);

    rc = getenv_s(&len, dest, LEN, NULL);
    ERR_MSVC(ESNULLP, 0);

/*--------------------------------------------------*/

    rc = getenv_s(&len, dest, 0, name);
    ERR_MSVC(ESZEROL, EINVAL);

/*--------------------------------------------------*/

    rc = getenv_s(&len, dest, RSIZE_MAX_STR+1, name);
    ERR_MSVC(ESLEMAX, 0);

/*--------------------------------------------------*/

    rc = getenv_s(&len, dest, 1, name);
    ERR_MSVC(ESNOSPC, 34);

/*--------------------------------------------------*/

    rc = getenv_s(&len, dest, LEN, name);
    ERR(EOK);
    str2 = getenv(name);
    EXPSTR(dest, str2);
    ind = strlen(str2);
#ifndef USE_MSVCRT    
    INDCMP(!= (int)len);
#endif

    rc = getenv_s(NULL, dest, LEN, name);
    ERR_MSVC(EOK, EINVAL);
    EXPSTR(dest, str2);

/*--------------------------------------------------*/

    rc = getenv_s(NULL, dest, LEN, "c#hewhc&wehc%erwhc$weh");
    ERR_MSVC(-1, EINVAL);
#ifndef USE_MSVCRT    
    EXPNULL(dest);
#endif

    rc = getenv_s(&len, dest, LEN, "c#hewhc&wehc%erwhc$weh");
    ERR_MSVC(-1, 0);
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
