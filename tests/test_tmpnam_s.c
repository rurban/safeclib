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

#define LEN   ( 128 )

static char   str1[LEN];
int test_tmpnam_s (void);

int test_tmpnam_s (void)
{
    errno_t rc;
    int len;
    int errs = 0;

/*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = tmpnam_s(NULL, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    EXPECT_BOS("empty dest")
    rc = tmpnam_s(NULL, 5);
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dmax")
    rc = tmpnam_s(str1, 0);
    ERR_MSVC(ESZEROL, ERANGE);

/*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = tmpnam_s(str1, RSIZE_MAX_STR+1);
    ERR_MSVC(ESLEMAX, 0);

# ifdef HAVE___BUILTIN_OBJECT_SIZE
    /* dest is now const so the literal check works */
    EXPECT_BOS("dest overflow")
    rc = tmpnam_s("aaa", 5);
    ERR(EOVERFLOW);
# endif
/*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = tmpnam_s(str1, L_tmpnam_s+1);
    if (!(rc == 0 || rc == ESLEMAX)) {
        debug_printf("%s %u \"%s\"  Error rc=%d \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
#endif

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

int main (void)
{
    return (test_tmpnam_s());
}
