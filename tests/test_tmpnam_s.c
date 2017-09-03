/*------------------------------------------------------------------
 * test_tmpnam_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static char   str1[LEN];

int test_tmpnam_s (void)
{
    errno_t rc;
    uint32_t i;
    int len;
    int errs = 0;

/*--------------------------------------------------*/

    rc = tmpnam_s(NULL, LEN);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = tmpnam_s(NULL, 5);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = tmpnam_s(str1, 0);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = tmpnam_s(str1, RSIZE_MAX_STR+1);
    ERR(ESLEMAX)

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
    ERR(ESNOSPC)
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
#ifdef SAFECLIB_STR_NULL_SLACK
    for (i=len; i<LEN/2; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u   Error rc=%d \n",
                     __FUNCTION__, __LINE__,  rc );
            errs++;
            break;
        }
    }
#else
    if (str1[len] != '\0') {
        debug_printf("%s %u   Error rc=%d \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
#endif

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
