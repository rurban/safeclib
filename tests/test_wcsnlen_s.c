/*------------------------------------------------------------------
 * test_wcsnlen_s
 *
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
    len = wcsnlen_s(NULL, max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = 0;
    len = wcsnlen_s(L"test", max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = RSIZE_MAX_WSTR+1;
    len = wcsnlen_s(L"test", max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
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
