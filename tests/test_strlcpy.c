/*------------------------------------------------------------------
 * test_strlcpy
 * File 'extstr/strlcpy.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (16)

int test_strlcpy(void);

int test_strlcpy(void) {
    char dest[LEN];
    size_t rc;
    int errs = 0;

    /*--------------------------------------------------*/
    /* plain copy, dest large enough */

    memset(dest, 'x', sizeof(dest));
    rc = strlcpy(dest, "hello", sizeof(dest));
    ERR(5)
    EXPSTR(dest, "hello")

    /*--------------------------------------------------*/
    /* empty src */

    memset(dest, 'x', sizeof(dest));
    rc = strlcpy(dest, "", sizeof(dest));
    ERR(0)
    EXPSTR(dest, "")

    /*--------------------------------------------------*/
    /* exact fit, no truncation */

    memset(dest, 'x', sizeof(dest));
    rc = strlcpy(dest, "0123456789abcde", sizeof(dest)); /* 15 chars + NUL */
    ERR(15)
    EXPSTR(dest, "0123456789abcde")

    /*--------------------------------------------------*/
    /* truncation: src longer than dsize; return value is the
       untruncated strlen(src), always NUL-terminated */

    memset(dest, 'x', sizeof(dest));
    rc = strlcpy(dest, "this string is way too long for dest", sizeof(dest));
    ERR(36)
    EXPSTR(dest, "this string is ")
    if (rc < sizeof(dest)) {
        debug_printf("%s %u  Error: rc=%d should be >= dsize=%d on "
                     "truncation \n",
                     __FUNCTION__, __LINE__, (int)rc, (int)sizeof(dest));
        errs++;
    }

    /*--------------------------------------------------*/
    /* dsize == 0: dest is untouched, no write happens */

    dest[0] = 'A';
    rc = strlcpy(dest, "ignored", 0);
    ERR(7)
    if (dest[0] != 'A') {
        debug_printf("%s %u  Error: strlcpy wrote to dest with dsize=0 \n",
                     __FUNCTION__, __LINE__);
        errs++;
    }

    /*--------------------------------------------------*/
    /* dsize == 1: only the NUL fits */

    dest[0] = 'A';
    rc = strlcpy(dest, "ignored", 1);
    ERR(7)
    EXPSTR(dest, "")

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strlcpy()); }
#endif
