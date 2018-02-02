/*------------------------------------------------------------------
 * test_sprintf_s
 * File 'str/sprintf_s.c'
 * Lines executed:100.00% of 36
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_SPRINTF_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];

int test_sprintf_s (void)
{
    errno_t rc;
    int  ind;
    int  len2;
    int  len3;
    int  errs = 0;

/*--------------------------------------------------*/

    str2[0] = '\0';
    print_msvcrt(use_msvcrt);

    /* older windows sprintf_s segv here */
    rc = sprintf_s(NULL, LEN, "%s", str2);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERR(-1);
    ERRNO_MSVC(ESNULLP,EINVAL);

/*--------------------------------------------------*/
    rc = sprintf_s(str1, RSIZE_MAX_STR+1, "%s", str2);
    if (!use_msvcrt) {
        ERR(-1);
    } else {
        ERR(0);
    }
    ERRNO_MSVC(ESLEMAX, 0);

/*--------------------------------------------------*/

    rc = sprintf_s(str1, LEN, NULL, NULL);
    ERR(-1);
    ERRNO_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    rc = sprintf_s(str1, 0, "%s", str2);
    ERR(-1);
    ERRNO_MSVC(ESZEROL, EINVAL);

/*--------------------------------------------------*/

    rc = sprintf_s(str1, LEN, "%s %n", str2, &ind);
    ERR(-1);
    ERRNO(EINVAL);

    rc = sprintf_s(str1, LEN, "%s %%n", str2);
    ERR(3)

    rc = sprintf_s(str1, LEN, "%%n");
    ERR(2);

/*--------------------------------------------------*/

    /* TODO not yet implemented
    rc = sprintf_s(str1, LEN, "%p", NULL);
    ERR(-1);
    ERRNO_MSVC(ESNULLP, EINVAL);
    */

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 1, "%s", str2);
    ERR(-1);
    ERRNO_MSVC(ESNOSPC, ERANGE);
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 2, "%s", str2);
    ERR(-1);
    ERRNO_MSVC(ESNOSPC, ERANGE);
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    len2 = strlen(str2);

    rc = sprintf_s(str1, 50, "%s", str2);
    ERR(len2)
    len3 = strlen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        int len1 = strlen(str1);
        debug_printf("%s %u lengths wrong: %d  %d  %d \n",
                     __FUNCTION__, __LINE__, len1, len2, len3);
#endif
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 1, "%s", str2);
    ERR(-1);
    ERRNO_MSVC(ESNOSPC, ERANGE);
    EXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 2, "%s", str2);
    ERR(-1);
    ERRNO_MSVC(ESNOSPC, ERANGE);
    EXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 20, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = sprintf_s(str1, LEN, "%s", str2);
    ERR(0)
    EXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, LEN, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, str2)

/*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, LEN, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, "keep it simple")

/*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 12, "%s", str2);
    ERR(-1);
    ERRNO_MSVC(ESNOSPC, ERANGE);

/*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = sprintf_s(str1, 52, "%s", str2);
    NOERRNULL()
    EXPSTR(str1, "keep it simple")

/*--------------------------------------------------*/

    strcpy(str1, "12345678901234567890");

    rc = sprintf_s(str1, 8, "%s", &str1[7]);
    ERR(-1);
    ERRNO_MSVC(ESNOSPC, ERANGE);
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(str1, "123456789");

    rc = sprintf_s(str1, 9, "%s", &str1[8]);
    ERR(1) /* overlapping allowed */
    EXPSTR(str1, "9")

/*--------------------------------------------------*/

    strcpy(str2, "123");
    strcpy(str1, "keep it simple");

    rc = sprintf_s(str2, 31, "%s", &str1[0]);
    NOERRNULL()
    EXPSTR(str2, "keep it simple")

/*--------------------------------------------------*/

    strcpy(str2, "1234");
    strcpy(str1, "56789");

    rc = sprintf_s(str2, 10, "%s", str1);
    NOERRNULL()
    EXPSTR(str2, "56789")

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_sprintf_s());
}
#endif
