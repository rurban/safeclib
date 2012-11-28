/*------------------------------------------------------------------
 * test_strnlen_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int test_strnlen_s (void)
{
    rsize_t len;
    rsize_t std_len;
    rsize_t max_len;

/*--------------------------------------------------*/

    max_len = 3;
    len = strnlen_s(NULL, max_len);
    if (len != 0) {
        printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  len);
    }

/*--------------------------------------------------*/

    max_len = 0;
    len = strnlen_s("test", max_len);
    if (len != 0) {
        printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  len);
    }

/*--------------------------------------------------*/

    max_len = RSIZE_MAX_STR+1;
    len = strnlen_s("test", max_len);
    if (len != 0) {
        printf("%s %u   Len=%u \n",
                     __FUNCTION__, __LINE__,  len);
    }

/*--------------------------------------------------*/

    std_len = strlen("");
    max_len = RSIZE_MAX_STR;

    len = strnlen_s ("", max_len);

    if (std_len != len) {
        printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  std_len, len);
    }

/*--------------------------------------------------*/

    std_len = strlen("t");
    max_len = RSIZE_MAX_STR;

    len = strnlen_s ("t", max_len);

    if (std_len != len) {
        printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  std_len, len);
    }

/*--------------------------------------------------*/

    std_len = strlen("to");
    max_len = RSIZE_MAX_STR;

    len = strnlen_s ("to", max_len);

    if (std_len != len) {
        printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  std_len, len);
    }

/*--------------------------------------------------*/

    std_len = strlen("testing");
    max_len = RSIZE_MAX_STR;

    len = strnlen_s ("testing", max_len);

    if (std_len != len) {
        printf("%s %u   std_len=%u  len=%u  \n",
                     __FUNCTION__, __LINE__,  std_len, len);
    }

/*--------------------------------------------------*/

    max_len = 1;
    len = strnlen_s ("testing", max_len);

    if (len != max_len) {
        printf("%s %u   len=%u  \n",
                     __FUNCTION__, __LINE__, len);
    }

/*--------------------------------------------------*/

    max_len = 2;
    len = strnlen_s ("testing", max_len);

    if (len != max_len) {
        printf("%s %u   len=%u  \n",
                     __FUNCTION__, __LINE__, len);
    }

/*--------------------------------------------------*/

    max_len = 3;
    len = strnlen_s ("testing", max_len);

    if (len != max_len) {
        printf("%s %u   len=%u  \n",
                     __FUNCTION__, __LINE__, len);
    }

/*--------------------------------------------------*/

    return (0);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strnlen_s());
}
#endif
