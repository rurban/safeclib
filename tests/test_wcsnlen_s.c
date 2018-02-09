/*------------------------------------------------------------------
 * test_wcsnlen_s
 * File 'wchar/wcsnlen_s.c'
 * Lines executed:100.00% of 12
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int test_wcsnlen_s (void)
{
    rsize_t len;
    rsize_t std_len;
    rsize_t max_len;
    int errs = 0;

/*--------------------------------------------------*/

    max_len = 3;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    len = wcsnlen_s(NULL, max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = 0;
    EXPECT_BOS("empty dest or dmax") 
    len = wcsnlen_s(L"test", 0);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = RSIZE_MAX_WSTR+1;
    /*EXPECT_BOS("dest overflow") TODO? */
    len = wcsnlen_s(L"test", RSIZE_MAX_WSTR+1);
    /* They allow more */
# if !defined(MINGW_HAS_SECURE_API) && !defined(_WSTRING_S_DEFINED)
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
# else
    if (max_len < INT_MAX) {
        if (len != 4) {
            debug_printf("%s %u   Len=%u \n",
                         __FUNCTION__, __LINE__,  (unsigned)len);
            errs++;
        }
    } else {
        if (len != 0) {
            debug_printf("%s %u   Len=%u \n",
                         __FUNCTION__, __LINE__,  (unsigned)len);
            errs++;
        }
    }
# endif
#endif
/*--------------------------------------------------*/

    std_len = strlen("");
    max_len = RSIZE_MAX_WSTR;

    len = wcsnlen_s (L"", max_len);

    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  (unsigned)std_len, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    std_len = strlen("t");
    max_len = RSIZE_MAX_WSTR;

    len = wcsnlen_s (L"t", max_len);

    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  (unsigned)std_len, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    std_len = strlen("to");
    max_len = RSIZE_MAX_WSTR;

    len = wcsnlen_s (L"to", max_len);

    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  (unsigned)std_len, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    std_len = strlen("testing");
    max_len = RSIZE_MAX_WSTR;

    len = wcsnlen_s (L"testing", max_len);

    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  (unsigned)std_len, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = 1;
    len = wcsnlen_s (L"testing", max_len);

    if (len != max_len) {
        debug_printf("%s %u   len=%u  \n",
               __FUNCTION__, __LINE__, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = 2;
    len = wcsnlen_s (L"testing", max_len);

    if (len != max_len) {
        debug_printf("%s %u   len=%u  \n",
                     __FUNCTION__, __LINE__, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = 3;
    len = wcsnlen_s (L"testing", max_len);

    if (len != max_len) {
        debug_printf("%s %u   len=%u  \n",
                     __FUNCTION__, __LINE__, (unsigned)len);
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
    return (test_wcsnlen_s());
}
#endif
