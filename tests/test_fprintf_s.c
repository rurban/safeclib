/*------------------------------------------------------------------
 * test_fprintf_s
 * File 'io/fprintf_s.c'
 * Lines executed:100.00% of 20
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#define TMP   "tmpfp"
#define LEN   ( 128 )

static FILE* out;
static char  str[LEN];

int test_fprintf_s (void)
{
    errno_t rc;
    int  ind;
    int  errs = 0;

    out = fopen(TMP, "w");

/*--------------------------------------------------*/

    rc = fprintf_s(NULL, "%s", NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = fprintf_s(out, NULL, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    str[0] = '\0';
    rc = fprintf_s(out, "%s %n", str, &ind);
    ERR(EINVAL)

    if (!out) {
        printf("Failed to open file %s for write: %s\n",
               TMP, strerror(errno));
        return errs;
    }

/*--------------------------------------------------*/
        
    rc = fprintf_s(out, "%s %%n", str);
    ERR(3)

    rc = fprintf_s(out, "%%n");
    ERR(2);

/*--------------------------------------------------*/

    /* TODO
    rc = fprintf_s(out, "%p", NULL);
    ERR(ESNULLP)
    */

/*--------------------------------------------------*/

    strcpy(str, "keep it simple");

    rc = fprintf_s(out, "%s", str);
    NOERRNULL()

/*--------------------------------------------------*/

    str[0] = '\0';

    rc = fprintf_s(out, "%s", str);
    ERR(0)

/*--------------------------------------------------*/

    strcpy(str, "keep it simple");

    rc = fprintf_s(out, "%s", str);
    NOERRNULL()

/*--------------------------------------------------*/

    fclose(out);

    /* print to closed stream: unportable, and not valgrind-safe */
#ifndef __GLIBC__
    rc = fprintf_s(out, "%s", str);
# if defined(__GLIBC__)
    if (rc < 0) {
        ERR(-1);
        ERRNO(EBADF);
    } else {
        ERR(14); /* older glibc upstream bug */
        ERRNO(0);
    }
# else
    /* musl returns 14, all other -1 */
    if (rc != 14 && rc != -1) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)rc);
        errs++;
    }
# endif
    /* musl throws no error */
#endif

    unlink(TMP);
        
    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_fprintf_s());
}
#endif
