/*------------------------------------------------------------------
 * test_strcmp_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include "config.h"

#define sgn(i) ((i)>0 ? 1 : ((i)<0 ? -1 : 0))

#if defined(__has_feature)
# if __has_feature(address_sanitizer)
/* asan wrongly intercepts those, and flats them to signum */
#define STDCMP()                                                 \
    std_ind = strcmp(str1, str2);                                \
    if (ind != std_ind) {                                        \
        printf("%s %u  ind=%d  asan strcmp()=%d  rc=%d \n",      \
               __FUNCTION__, __LINE__,  ind, std_ind, rc);       \
    }                                                            \
    if (sgn(ind) != std_ind) {                                   \
        printf("%s %u  sgn(ind)=%d  std_ind=%d  rc=%d \n",       \
               __FUNCTION__, __LINE__,  sgn(ind), std_ind, rc);  \
        errs++;                                                  \
    }
# endif
#endif
#if !defined(STDCMP)
#if defined(HAVE_STRCMP)
#define STDCMP()                                                 \
    std_ind = strcmp(str1, str2);                                \
    if (ind != std_ind) {                                        \
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",            \
               __FUNCTION__, __LINE__,  ind, std_ind, rc);       \
        errs++;                                                  \
    }
#else
#define STDCMP()  \
    debug_printf("%s %u  have no strcmp()\n", __FUNCTION__, __LINE__);
#endif
#endif


#define LEN   ( 128 )
#define SHORT_LEN  ( 5 )

static char   str1[LEN];
static char   str2[LEN];

int test_strcmp_s (void)
{
    errno_t rc;
    int ind;
    int std_ind;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strcmp_s(NULL, LEN, str2, &ind);
    ERR(ESNULLP)
    INDNULL()

/*--------------------------------------------------*/

    rc = strcmp_s(str1, LEN, NULL, &ind);
    ERR(ESNULLP)
    INDNULL()

/*--------------------------------------------------*/

    rc = strcmp_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = strcmp_s(str1, 0, str2, &ind);
    ERR(ESZEROL)
    INDNULL()

/*--------------------------------------------------*/

    rc = strcmp_s(str1, RSIZE_MAX_STR+1, str2, &ind);
    ERR(ESLEMAX)
    INDNULL()

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDNULL()

    STDCMP()

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep it simple");

    rc = strcmp_s(str1, 5, str2, &ind);
    ERR(EOK)
    INDNULL()

/*--------------------------------------------------*/

    /*   K - k ==  -32  */
    strcpy (str1, "Keep it simple");
    strcpy (str2, "keep it simple");

    rc = strcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDCMP(!= (-32))
    STDCMP()

/*--------------------------------------------------*/

    /*   p - P ==  32  */
    strcpy (str1, "keep it simple");
    strcpy (str2, "keeP it simple");

    rc = strcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDCMP(!= 32)
    STDCMP()

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");

    rc = strcmp_s(str1, LEN, str1, &ind);
    ERR(EOK)
    INDNULL()
    /* be sure the results are the same as strcmp. */
    std_ind = strcmp(str1, str1);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
               __FUNCTION__, __LINE__,  ind, std_ind, rc);
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simplified");
    strcpy (str2, "keep it simple");

    rc = strcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDCMP(<= 0)
    /* be sure the results are the same as strcmp */
    STDCMP()

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep it simplified");

    rc = strcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDCMP(>= 0)
    /* be sure the results are the same as strcmp */
    STDCMP()

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strcmp_s());
}
#endif
