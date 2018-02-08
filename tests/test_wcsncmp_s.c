/*------------------------------------------------------------------
 * test_wcsncmp_s
 * File 'wcsncmp_s.c'
 * Lines executed:100.00% of 27
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define sgn(i) ((i)>0 ? 1 : ((i)<0 ? -1 : 0))

/* asan or some cross-compilers flat them to signum -1,0,1 only */
#define RELAXEDCMP()                                             \
    std_ind = wcsncmp(str1, str2, len);                          \
    if (ind != std_ind) {                                        \
        printf("%s %u  ind=%d wcsncmp()=%d  rc=%d \n",  \
               __FUNCTION__, __LINE__,  ind, std_ind, rc);       \
    if (sgn(ind) != sgn(std_ind)) {                              \
        printf("%s %u  relaxed sgn(ind)=%d  sgn(std_ind)=%d  rc=%d \n",  \
               __FUNCTION__, __LINE__,  sgn(ind), sgn(std_ind), rc); \
        errs++;                                                  \
      }                                                          \
    }

#if defined(__has_feature)
# if __has_feature(address_sanitizer)
#   define STDCMP() RELAXEDCMP()
# endif
#endif

#if !defined(STDCMP)
#if defined(HAVE_STRCMP)
#define STDCMP()                                                 \
    std_ind = wcsncmp(str1, str2, len);                          \
    if (ind != std_ind) {                                        \
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",            \
               __FUNCTION__, __LINE__,  ind, std_ind, rc);       \
        errs++;                                                  \
    }
#else
#define STDCMP()  \
    debug_printf("%s %u  have no wcscmp()\n", __FUNCTION__, __LINE__);
#endif
#endif

#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

int test_wcsncmp_s (void)
{
    errno_t rc;
    int len = 1;
    int ind;
    int std_ind;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcsncmp_s(NULL, LEN, str2, LEN, 12, &ind);
    ERR(ESNULLP)
    INDZERO()

    rc = wcsncmp_s(str1, LEN, NULL, LEN, 12, &ind);
    ERR(ESNULLP)
    INDZERO()

    rc = wcsncmp_s(str1, LEN, str2, LEN, 12, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = wcsncmp_s(str1, 0, str2, LEN, 12, &ind);
    ERR(ESZEROL)
    INDZERO()

    rc = wcsncmp_s(str1, LEN, str2, 0, 12, &ind);
    ERR(ESZEROL)
    INDZERO()

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = wcsncmp_s(str1, RSIZE_MAX_STR+1, str2, LEN, 12, &ind);
    ERR(ESLEMAX)
    INDZERO()

    rc = wcsncmp_s(str1, LEN, str2, RSIZE_MAX_STR+1, 12, &ind);
    ERR(ESLEMAX)
    INDZERO()
#endif

/*--------------------------------------------------*/

    str1[0] = L'\0';
    str2[0] = L'\0';

    rc = wcsncmp_s(str1, LEN, str2, LEN, len, &ind);
    ERR(EOK)
    INDZERO()

    STDCMP()

/*--------------------------------------------------*/

    wcscpy (str1, L"keep it simple");
    wcscpy (str2, L"keep it simple");
    len = wcslen(str1);

    rc = wcsncmp_s(str1, 5, str2, LEN, len, &ind);
    ERR(EOK)
    INDZERO()

/*--------------------------------------------------*/

    /*   K - k ==  -32  */
    wcscpy (str1, L"Keep it simple");
    wcscpy (str2, L"keep it simple");

    rc = wcsncmp_s(str1, LEN, str2, LEN, len, &ind);
    ERR(EOK)
    INDCMP(!= (-32))
    /* sgn with -m32 or cross on linux */
    RELAXEDCMP()

/*--------------------------------------------------*/

    /*   p - P ==  32  */
    wcscpy (str1, L"keep it simple");
    wcscpy (str2, L"keeP it simple");

    rc = wcsncmp_s(str1, LEN, str2, LEN, len, &ind);
    ERR(EOK)
    INDCMP(!= 32)
    /* sgn with -m32 or cross on linux */
    RELAXEDCMP()

/*--------------------------------------------------*/

    wcscpy (str1, L"keep it simple");

    rc = wcsncmp_s(str1, LEN, str1, LEN, len, &ind);
    ERR(EOK)
    INDZERO()
    /* be sure the results are the same as wcscmp. */
    std_ind = wcscmp(str1, str1);
    if (ind != std_ind) {
        printf("%s %u  ind=%d  std_ind=%d  rc=%d \n",
               __FUNCTION__, __LINE__,  ind, std_ind, rc);
    }

/*--------------------------------------------------*/

    wcscpy (str1, L"keep it simplified");
    wcscpy (str2, L"keep it simple");

    rc = wcsncmp_s(str1, LEN, str2, LEN, len, &ind);
    ERR(EOK)
    INDCMP(<= 0)
    /* sgn with -m32 or cross on linux */
    RELAXEDCMP()

/*--------------------------------------------------*/

    wcscpy (str1, L"keep it simple");
    wcscpy (str2, L"keep it simplified");

    rc = wcsncmp_s(str1, LEN, str2, LEN, len, &ind);
    ERR(EOK)
    INDCMP(>= 0)
    /* sgn with -m32 or cross on linux */
    RELAXEDCMP()

/*--------------------------------------------------*/

    /* TODO */
    wcscpy (str1, L"keep it simple");
    wcscpy (str2, L"keep it simplified");
    len = 5;

    rc = wcsncmp_s(str1, LEN, str2, LEN, len, &ind);
    ERR(EOK)
    INDCMP(!= 0);
    RELAXEDCMP()

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcsncmp_s());
}
#endif
