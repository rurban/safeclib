/*------------------------------------------------------------------
 * test_strncpy_s
 * File 'str/strncpy_s.c'
 * Lines executed:100.00% of 55
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define HAVE_NATIVE defined(HAVE_STRNCPY_S)
#include "test_msvcrt.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];
static char   dest[LEN];

int test_strncpy_s (void)
{
    errno_t rc;
    rsize_t nlen;
    int32_t ind;
    int errs = 0;

/*--------------------------------------------------*/

    nlen = 5;
    rc = strncpy_s(NULL, LEN, str2, nlen);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    nlen = 5;
    rc = strncpy_s(str1, 5, NULL, nlen);
    ERR(ESNULLP);
    CHECK_SLACK(str1, 5);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';

    rc = strncpy_s(str1, 5, str2, 0);
#ifdef HAVE_C11
    ERR(EOK);
#else
    ERR(ESZEROL)
    CHECK_SLACK(str1, 5);
#endif

    nlen = 5;
    rc = strncpy_s(str1, 0, str2, nlen);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = strncpy_s(str1, (RSIZE_MAX_STR+1), str2, nlen);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

#if 1
   strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
   strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

    rc = strncpy_s(str1, 5, str2, (RSIZE_MAX_STR+1));
    ERR(ESLEMAX)
#endif

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';
    nlen = 5;

    rc = strncpy_s(&str1[0], LEN/2, &str2[0], nlen);
    ERR(EOK)
    CHECK_SLACK(str1, LEN/2);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 5;

    /* test overlap */
    rc = strncpy_s(str1, LEN, str1, nlen);
    ERR(ESOVRLP)
    CHECK_SLACK(str1, LEN);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 18;

    rc = strncpy_s(&str1[0], LEN, &str1[5], nlen);
    ERR(ESOVRLP)
    CHECK_SLACK(str1, LEN);

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    str2[0] = '\0';

    nlen = 10;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    CHECK_SLACK(str1, LEN);

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    nlen = 20;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    EXPSTR(str1, str2)
    CHECK_SLACK(&str1[14], LEN-14);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    nlen = 32;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    EXPSTR(str1, str2)
    CHECK_SLACK(&str1[14], LEN-14);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strncpy_s(str1, 1, str2, nlen);
    ERR(ESNOSPC)
    CHECK_SLACK(str1, 1);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strncpy_s(str1, 2, str2, nlen);
    ERR(ESNOSPC)
    if (*str1 != '\0') {
        debug_printf("%s %u -%s-  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  str1, rc );
        errs++;
    }
/*--------------------------------------------------*/
/* TR example */

    strcpy(dest, "                            ");
    strcpy(str1, "hello");

    rc = strncpy_s(dest, 6, str1, 100);
    ERR(EOK)
    EXPSTR(dest, str1)
    CHECK_SLACK(&str1[5], 1);

/*--------------------------------------------------*/
/* TR example */

    strcpy(dest, "                            ");
    strcpy(str2, "goodbye");

    rc = strncpy_s(dest, 5, str2, 7);
    ERR(ESNOSPC)
    CHECK_SLACK(dest, 5);

/*--------------------------------------------------*/
/* TR example */

    strcpy(dest, "                            ");
    strcpy(str2, "goodbye");

    rc = strncpy_s(dest, 5, str2, 4);
    ERR(EOK)
    EXPSTR(dest, "good")
    CHECK_SLACK(&dest[4], 5-4);

/*--------------------------------------------------*/

    strcpy(dest, "                            ");
    strcpy(str2, "good");

    /*   strnlen("good") < 5   */
    rc = strncpy_s(dest, 5, str2, 8);
    ERR(EOK)
    EXPSTR(dest, "good")
    CHECK_SLACK(&dest[4], 5-4);

/*--------------------------------------------------*/

    strcpy(str1, "qq12345weqeqeqeq");
    strcpy(str2, "it");

    nlen = 10;
    rc = strncpy_s(str1, 10, str2, nlen);
    ERR(EOK)
    EXPSTR(str1, str2)

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strncpy_s());
}
#endif
