/*------------------------------------------------------------------
 * test_strnlen_s
 * File 'str/strnlen_s.c'
 * Lines executed:89.47% of 19
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )
static char   str1[LEN];

int test_strnlen_s (void)
{
    rsize_t len;
    rsize_t std_len;
    rsize_t max_len;
    int errs = 0;

/*--------------------------------------------------*/

    max_len = 3;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty str")
    len = strnlen_s(NULL, max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
#endif
/*--------------------------------------------------*/

    max_len = 0;
    len = strnlen_s("test", max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    max_len = RSIZE_MAX_STR+1;
    len = strnlen_s("test", max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  (unsigned)len);
        errs++;
    }
#endif
/*--------------------------------------------------*/

    *str1 = '\0';
    std_len = strlen(str1);
    max_len = LEN;

    len = strnlen_s (str1, max_len);

    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  (unsigned)std_len, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "t");
    std_len = strlen(str1);

    len = strnlen_s (str1, LEN);

    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  (unsigned)std_len, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "to");
    std_len = strlen(str1);

    len = strnlen_s (str1, LEN);

    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  (unsigned)std_len, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    std_len = strlen("testing");
    max_len = std_len;

    len = strnlen_s ("testing", max_len);

    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  (unsigned)std_len, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = 1;
    len = strnlen_s ("testing", max_len);

    if (len != max_len) {
        debug_printf("%s %u   len=%u  \n",
               __FUNCTION__, __LINE__, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = 2;
    len = strnlen_s ("testing", max_len);

    if (len != max_len) {
        debug_printf("%s %u   len=%u  \n",
                     __FUNCTION__, __LINE__, (unsigned)len);
        errs++;
    }
/*--------------------------------------------------*/

    max_len = 3;
    len = strnlen_s ("testing", max_len);

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
    return (test_strnlen_s());
}
#endif
