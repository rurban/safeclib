/*------------------------------------------------------------------
 * test_wcsicmp_s
 * File 'wcsicmp_s.c'
 * Lines executed:90.63% of 32
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define sgn(i) ((i)>0 ? 1 : ((i)<0 ? -1 : 0))

/* asan or some cross-compilers flat them to signum -1,0,1 only */
#define RELAXEDCMP()                                             \
    std_ind = wcscmp(str1, str2);                                \
    if (ind != std_ind) {                                        \
        printf("%s %u  ind=%d  relaxed wcscmp()=%d  rc=%d \n",   \
               __FUNCTION__, __LINE__,  ind, std_ind, rc);       \
      if (sgn(ind) != std_ind) {                                 \
        printf("%s %u  sgn(ind)=%d  std_ind=%d  rc=%d \n",       \
               __FUNCTION__, __LINE__,  sgn(ind), std_ind, rc);  \
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
    std_ind = wcscmp(str1, str2);                                \
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

int test_wcsicmp_s (void)
{
    errno_t rc;
    int ind;
    int std_ind;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcsicmp_s(NULL, LEN, str2, LEN, &ind);
    ERR(ESNULLP)
    INDZERO()

    rc = wcsicmp_s(str1, LEN, NULL, LEN, &ind);
    ERR(ESNULLP)
    INDZERO()

    rc = wcsicmp_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = wcsicmp_s(str1, 0, str2, LEN, &ind);
    ERR(ESZEROL)
    INDZERO()

    rc = wcsicmp_s(str1, LEN, str2, 0, &ind);
    ERR(ESZEROL)
    INDZERO()

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = wcsicmp_s(str1, RSIZE_MAX_STR+1, str2, LEN, &ind);
    ERR(ESLEMAX)
    INDZERO()

    rc = wcsicmp_s(str1, LEN, str2, RSIZE_MAX_STR+1, &ind);
    ERR(ESLEMAX)
    INDZERO()
#endif

/*--------------------------------------------------*/

    str1[0] = L'\0';
    str2[0] = L'\0';

    rc = wcsicmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

    STDCMP()

/*--------------------------------------------------*/

    wcscpy (str1, L"Keep it simple");
    wcscpy (str2, L"keep it simple");

    rc = wcsicmp_s(str1, 5, str2, LEN, &ind);
    ERR(ESNOSPC)
    INDZERO()

/*--------------------------------------------------*/

    /* fold-case both */
    wcscpy (str1, L"Simple A" L"\x100");
    wcscpy (str2, L"simple a" L"\x61" L"\x304");

    rc = wcsicmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDZERO()

/*--------------------------------------------------*/

    /*   p - x ==  -8  */
    wcscpy (str1, L"keep it simple");
    wcscpy (str2, L"keex it simple");

    rc = wcsicmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(!= -8)
    /* sgn with -m32 or cross on linux */
    RELAXEDCMP()

/*--------------------------------------------------*/

    wcscpy (str1, L"keep it simple");

    rc = wcsicmp_s(str1, LEN, str1, LEN, &ind);
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

    rc = wcsicmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(<= 0)
    /* sgn with -m32 or cross on linux */
    RELAXEDCMP()

/*--------------------------------------------------*/

    wcscpy (str1, L"keep it simple");
    wcscpy (str2, L"keep it simplified");

    rc = wcsicmp_s(str1, LEN, str2, LEN, &ind);
    ERR(EOK)
    INDCMP(>= 0)
    /* sgn with -m32 or cross on linux */
    RELAXEDCMP()

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcsicmp_s());
}
#endif
