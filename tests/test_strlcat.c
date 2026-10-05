/*------------------------------------------------------------------
 * test_strlcat
 * File 'extstr/strlcat.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (16)

int test_strlcat(void);

int test_strlcat(void) {
    char dest[LEN];
    size_t rc;
    int errs = 0;

    /*--------------------------------------------------*/
    /* plain append, dest large enough */

    strcpy(dest, "hi ");
    rc = strlcat(dest, "there", sizeof(dest));
    ERR(8)
    EXPSTR(dest, "hi there")

    /*--------------------------------------------------*/
    /* append to an empty dest */

    dest[0] = '\0';
    rc = strlcat(dest, "hello", sizeof(dest));
    ERR(5)
    EXPSTR(dest, "hello")

    /*--------------------------------------------------*/
    /* appending an empty src is a no-op */

    strcpy(dest, "unchanged");
    rc = strlcat(dest, "", sizeof(dest));
    ERR(9)
    EXPSTR(dest, "unchanged")

    /*--------------------------------------------------*/
    /* exact fit, no truncation */

    strcpy(dest, "0123456789");                /* len 10 */
    rc = strlcat(dest, "abcde", sizeof(dest)); /* 10+5=15 chars + NUL */
    ERR(15)
    EXPSTR(dest, "0123456789abcde")

    /*--------------------------------------------------*/
    /* truncation: result would not fit; return value is the
       untruncated total length, dest is always NUL-terminated */

    strcpy(dest, "0123456789"); /* len 10 */
    rc = strlcat(dest, "this part gets truncated", sizeof(dest));
    ERR(34)
    EXPSTR(dest, "0123456789this ")
    if (rc < sizeof(dest)) {
        debug_printf("%s %u  Error: rc=%d should be >= dsize=%d on "
                     "truncation \n",
                     __FUNCTION__, __LINE__, (int)rc, (int)sizeof(dest));
        errs++;
    }

    /*--------------------------------------------------*/
    /* dest already full (no NUL room left): n == 0 case,
       returns dlen + strlen(src), dest left untouched */

    strcpy(dest, "0123456789abcde"); /* 15 chars, fills LEN-1 */
    rc = strlcat(dest, "more", sizeof(dest));
    ERR(19)
    EXPSTR(dest, "0123456789abcde")

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strlcat()); }
#endif
