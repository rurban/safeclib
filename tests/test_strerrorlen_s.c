/*------------------------------------------------------------------
 * test_strerrorlen_s
 * File 'str/strerrorlen_s.c'
 * Lines executed:100.00% of 3
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <errno.h>

#ifndef ELAST
#define ELAST 100
#endif
int test_strerrorlen_s(void);

int test_strerrorlen_s(void) {
    size_t len;
    int i;
    const char *buf;
    int errs = 0;

    /*--------------------------------------------------*/

    for (i = 0; i <= ELAST; i++) {
        len = strerrorlen_s(i);
        if (!len) {
            if ((buf = strerror(i))) {
                debug_printf("%s %u  Error empty strerrorlen_s(%d) \"%s\" \n",
                             __FUNCTION__, __LINE__, i, buf);
                errs++;
            }
        } else {
            buf = strerror(i);
            if (strlen(buf) != len) {
                debug_printf(
                    "%s %u  Wrong strerrorlen_s(%d) => %d != %d, \"%s\" \n",
                    __FUNCTION__, __LINE__, i, (int)len, (int)strlen(buf), buf);
                errs++;
            }
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strerrorlen_s()); }
#endif
