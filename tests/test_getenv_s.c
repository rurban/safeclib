/*------------------------------------------------------------------
 * test_getenv_s
 * File 'os/getenv_s.c'
 * Lines executed:100.00% of 29
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <stdlib.h>

#ifdef HAVE_GETENV_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (4090)

static char dest[LEN];
int test_getenv_s(void);

int test_getenv_s(void) {
    errno_t rc;
    int errs = 0;
    int ind;
    size_t len;
    char *str2;
    const char *name = "PATH";

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty name")
    rc = getenv_s(&len, dest, LEN, NULL);
    if (use_msvcrt && rc == ESNULLP) {
        printf("safec.dll overriding msvcrt.dll\n");
        use_msvcrt = false;
    }
    ERR_MSVC(ESNULLP, 0);
#endif

    rc = getenv_s(&len, NULL, 0, name);
    ERR_MSVC(0, EINVAL);

    /*--------------------------------------------------*/

    rc = getenv_s(&len, dest, 0, name);
    ERR_MSVC(0, EINVAL);

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("dest overflow")
    rc = getenv_s(&len, dest, RSIZE_MAX_STR + 1, name);
    ERR_MSVC(ESLEMAX, 0);
#endif

    /*--------------------------------------------------*/

    rc = getenv_s(&len, dest, 1, name);
    ERR_MSVC(ESNOSPC, 34);
    if (len != 0) {
        printf("%s %u  Error len=%u rc=%d \n", __FUNCTION__, __LINE__,
               (unsigned)len, rc);
        errs++;
    }

    /*--------------------------------------------------*/

    rc = getenv_s(&len, dest, LEN, name);
    ERR(EOK);
    str2 = getenv(name);
    EXPSTR(dest, str2);
    ind = strlen(str2);
    if (!use_msvcrt) {
        INDCMP(!= (int)len);
    }

    rc = getenv_s(NULL, dest, LEN, name);
    ERR_MSVC(EOK, EINVAL);
    EXPSTR(dest, str2);

    /*--------------------------------------------------*/

    rc = getenv_s(NULL, dest, LEN, "c#hewhc&wehc%erwhc$weh");
    ERR_MSVC(-1, EINVAL);
    if (!use_msvcrt) {
        EXPNULL(dest);
    }

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
int main(void) { return (test_getenv_s()); }
#endif
