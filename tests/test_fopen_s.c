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

#if defined(_WIN32) && defined(HAVE_FOPEN_S)
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

int test_fopen_s (void)
{
    errno_t rc;
    int errs = 0;
    FILE *tmp;

/*--------------------------------------------------*/

    rc = fopen_s(NULL, TMP, "r");
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
int main (void)
{
    return (test_fopen_s());
}
#endif
