/*------------------------------------------------------------------
 * test_strcpy_s
 * File 'str/strcpy_s.c'
 * Lines executed:100.00% of 44
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_STRCPY_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];

int test_strcpy_s (void)
{
    errno_t rc;
    int32_t  ind;
    int len;
    int errs = 0;

/*--------------------------------------------------*/
    if (use_msvcrt)
        printf("Using msvcrt...\n");

    rc = strcpy_s(NULL, LEN, str2);
    if ( use_msvcrt && rc == ESNULLP ) {
        printf("safec.dll overriding msvcrt.dll\n");
        use_msvcrt = false;
    }
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    rc = strcpy_s(str1, 5, NULL);
    ERR_MSVC(ESNULLP, EINVAL);
    CHECK_SLACK(str1, 5);

/*--------------------------------------------------*/

    rc = strcpy_s(str1, 0, str2);
    ERR_MSVC(ESZEROL, EINVAL);

/*--------------------------------------------------*/

    rc = strcpy_s(str1, (RSIZE_MAX_STR+1), str2);
    ERR_MSVC(ESLEMAX, 0);

    rc = strcpy_s(str1, (size_t)-1L, str2);
    ERR_MSVC(ESLEMAX, 0);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';

    rc = strcpy_s(str1, LEN/2, str2);
    ERR(EOK)
    CHECK_SLACK(str1, LEN/2);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    len = strlen(str1);

    rc = strcpy_s(str1, LEN, str1);
    ERR(EOK);
    EXPSTR(str1, "aaaaa");

/*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");

    rc = strcpy_s(&str1[0], LEN, &str1[5]);
    ERR_MSVC(ESOVRLP, 0);
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN);
    } else { /* windows overlap with dest < src */
        EXPSTR(str1, "it simple");
    }

/*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");

    rc = strcpy_s(&str1[5], LEN-5, &str1[0]);
    /* Windows forbids overlap with dest > src */
    ERR_MSVC(ESOVRLP, ERANGE);
    CHECK_SLACK(&str1[5], LEN-5);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    len = strlen(str1);

    rc = strcpy_s(str2, LEN, str1);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str2[len], LEN-len);

/*--------------------------------------------------*/

    strcpy(&str1[0], "keep it simple");
    str2[0] = '\0';

    rc = strcpy_s(str1, LEN, str2);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(str1, LEN);

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(&str2[0], "keep it simple");
    len = strlen(str2);

    rc = strcpy_s(str1, LEN, str2);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[len], LEN-len);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");
    len = strlen(str2);

    rc = strcpy_s(str1, LEN, str2);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[len], LEN-len);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strcpy_s(str1, 1, str2);
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(str1, 1);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strcpy_s(str1, 2, str2);
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(str1, 2);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "it");

    rc = strcpy_s(str1, 3, str2);
    ERR(EOK)
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[2], 1);

/*--------------------------------------------------*/

    strcpy(str1, "qq12345weqeqeqeq");
    strcpy(str2, "it");

    rc = strcpy_s(str1, 10, str2);
    ERR(EOK);
    EXPSTR(str1, str2);
    CHECK_SLACK(&str1[2], 8);
    /*for (i=0; i<10; i++) {
        debug_printf("str1[%i] = %d \n", i, str1[i]);
    }*/
/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strcpy_s());
}
#endif
