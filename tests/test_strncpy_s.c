/*------------------------------------------------------------------
 * test_strncpy_s
 * File 'str/strncpy_s.c'
 * Lines executed:95.31% of 64
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_STRNCPY_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
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

    print_msvcrt(use_msvcrt);
    rc = strncpy_s(NULL, LEN, str2, nlen);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    strcpy(str2, "b");

    rc = strncpy_s(NULL, LEN, str2, 5);
    ERR_MSVC(ESNULLP, EINVAL); /* and untouched */

    rc = strncpy_s(NULL, LEN, str2, 0);
    ERR_MSVC(ESNULLP, EINVAL); /* and untouched */

    rc = strncpy_s(str1, 0, str2, 0);
    ERR_MSVC(ESZEROL, EINVAL); /* and untouched */
    EXPSTR(str1, "aaaaa")

    strcpy(str1, "aaaaa");
    rc = strncpy_s(str1, LEN, str2, 0);
    ERR(EOK); /* and cleared */
    EXPSTR(str1, "")

    strcpy(str1, "aaaaa");
    rc = strncpy_s(str1, 1, str2, 0);
    ERR(EOK) /* and cleared */
    EXPSTR(str1, "")

    strcpy(str1, "aaaaa");
    rc = strncpy_s(str1, 5, NULL, 0);
    ERR(EOK); /* and cleared */
    EXPSTR(str1, "")

/*--------------------------------------------------*/

    strcpy(str1, "aaaaa");

    nlen = 5;
    rc = strncpy_s(str1, 5, NULL, nlen);
    ERR_MSVC(ESNULLP, EINVAL); /* and cleared */
    if (!use_msvcrt)
        CHECK_SLACK(str1, 5);
    
    strcpy(str1, "a");
    rc = strncpy_s(str1, 0, str2, 0);
    ERR_MSVC(ESZEROL, EINVAL); /* and untouched */
    EXPSTR(str1, "a")

/*--------------------------------------------------*/

    strcpy(str1, "aa");
    nlen = 5;
    rc = strncpy_s(str1, 0, str2, nlen);
    ERR_MSVC(ESZEROL, EINVAL); /* and untouched */
    EXPSTR(str1, "aa")

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    strcpy(str1, "aa");

# ifndef HAVE_ASAN /* With asan no BOS */
    if (_BOS_KNOWN(str1)) {
        rc = strncpy_s(str1, LEN+1, str2, nlen);
        if (!rc) {
            printf("Todo BOS overflow check\n");
        } else {
            ERR(ESLEMAX);     /* dmax exceeds dest */
            EXPSTR(str1, ""); /* cleared */
        }
    }
# endif

    strcpy(str1, "aa");
    rc = strncpy_s(str1, (RSIZE_MAX_STR+1), str2, nlen);
    ERR_MSVC(ESLEMAX, 0); /* different MAX */
    if (!use_msvcrt) {
        EXPSTR(str1, "aa");
    } else {
        EXPSTR(str1, "b");
    }

/*--------------------------------------------------*/

   strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
   strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

    rc = strncpy_s(str1, 5, str2, (RSIZE_MAX_STR+1));
    ERR_MSVC(ESLEMAX, ERANGE); /* and cleared */
    EXPSTR(str1, "");
#endif

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';
    nlen = 5;

    rc = strncpy_s(&str1[0], LEN/2, &str2[0], nlen);
    ERR(EOK); /* and cleared */
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN/2);
    } else {
        EXPSTR(str1, "");
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 5;

    /* test overlap */
    rc = strncpy_s(str1, LEN, str1, nlen);
    ERR_MSVC(ESOVRLP, 0); /* and cleared/mangled */
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN);
    } else {
        EXPSTR(str1, "aaaaa");
    }
/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 18;

    rc = strncpy_s(&str1[0], LEN, &str1[5], nlen);
    ERR_MSVC(ESOVRLP, 0); /* and cleared/mangled */
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN);
    } else {
        EXPSTR(str1, "aaaaaaaaaaaaaaaaaa");
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    str2[0] = '\0';

    nlen = 10;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK); /* and cleared */
    if (!use_msvcrt) {
        CHECK_SLACK(str1, LEN);
    } else {
       EXPSTR(str1, "")
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    nlen = 20;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    EXPSTR(str1, str2)
    if (!use_msvcrt) {
        CHECK_SLACK(&str1[14], LEN-14);
    }

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    nlen = 32;
    rc = strncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    EXPSTR(str1, str2)
    if (!use_msvcrt) {
        CHECK_SLACK(&str1[14], LEN-14);
    }

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strncpy_s(str1, 1, str2, nlen);
    ERR_MSVC(ESNOSPC, ERANGE);
    if (!use_msvcrt) {
        CHECK_SLACK(str1, 1);
    } else {
        EXPSTR(str1, "");
    }

/*--------------------------------------------------*/

    strcpy(str1, "qqweqeqeqeq");
    strcpy(str2, "keep it simple");

    rc = strncpy_s(str1, 2, str2, nlen);
    ERR_MSVC(ESNOSPC, ERANGE); /* and cleared */
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
    if (!use_msvcrt) {
        CHECK_SLACK(&str1[5], 1);
    }

/*--------------------------------------------------*/
/* TR example */

    strcpy(dest, "                            ");
    strcpy(str2, "goodbye");

    rc = strncpy_s(dest, 5, str2, 7);
    ERR_MSVC(ESNOSPC, ERANGE);
    if (!use_msvcrt) {
        CHECK_SLACK(dest, 5);
    }

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
