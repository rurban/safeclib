/*------------------------------------------------------------------
 * test_gets_s
 * File 'io/gets_s.c'
 * Lines executed:70.73% of 41
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <stdlib.h>
#include <unistd.h>

#define TMP "tmpget"
#define LEN (128)

static FILE *out;
static char dest[LEN];
int test_gets_s(void);

int test_gets_s(void) {
    errno_t rc = 0;
    int errs = 0;
    char *sub;

    out = fopen(TMP, "w");
    fprintf(out, "1\n2\n");
    fprintf(out, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    fprintf(out, "1234\n");
    fprintf(out, "1234\004"); /* ^D  EOT */
    fclose(out);
    if (!freopen(TMP, "r", stdin)) {
        printf("freopen failed: %s\n", strerror(errno));
        return 0;
    }

    /*--------------------------------------------------*/

    /* tests reading from stdin */
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest") EXPECT_BOS("empty dest or dmax")
    sub = gets_s(NULL, 0);
    SUBNULL();
    ERRNO(ESNULLP);

    EXPECT_BOS("empty dest or dmax")
    sub = gets_s(dest, 0);
    SUBNULL();
    ERRNO(ESZEROL);

    EXPECT_BOS("dest overflow")
    sub = gets_s(dest, RSIZE_MAX_STR + 1);
    SUBNULL();
    ERRNO(ESLEMAX);
#endif

    /*--------------------------------------------------*/

    sub = gets_s(dest, LEN);
    SUBNN()
    else EXPSTR(dest, "1");
    ERRNO(0);

    sub = gets_s(dest, 10);
    SUBNN()
    else EXPSTR(dest, "2");
    ERRNO(0);

    sub = gets_s(dest, 5); /* but got more */
    SUBNULL();
    ERRNO(ESNOSPC);
    CHECK_SLACK(dest, 5);

    sub = gets_s(dest, LEN); /* drain the rest of xxx.. */
    SUBNN();
    ERRNO(0);

    sub = gets_s(dest, 5); /* got exactly 5: "1234\n" => "1234\0" */
    SUBNN()
    else EXPSTR(sub, "1234");
    ERRNO(0);

    sub = gets_s(dest, 5); /* edge-case len==dmax-1 */
    SUBNULL();
    ERRNO(ESNOSPC);
    CHECK_SLACK(dest, 5);

    sub = gets_s(dest, LEN); /* EOF */
    SUBNULL();
    ERRNO(0);

    fclose(stdin);

    sub = gets_s(dest, LEN); /* CLOSED */
    SUBNULL();
    /* musl does not return EBADF when fgets from the closed fd */
    if (errno && errno != EBADF) {
#ifndef HAVE_MINGW32
        ERRNO(EBADF);
#else
        ERRNO(ESNOSPC);
#endif
        CHECK_SLACK(dest, LEN);
    }

    unlink(TMP);

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_gets_s()); }
#endif
