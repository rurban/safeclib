/*------------------------------------------------------------------
 * test_fprintf_s
 * File 'io/fprintf_s.c'
 * Lines executed:85.71% of 21
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#ifdef HAVE_FPRINTF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define TMP "tmpfp"
#define LEN (128)

static FILE *out;
static char str[LEN];
int test_fprintf_s(void);

int test_fprintf_s(void) {
    errno_t rc;
    int ind;
    int errs = 0;

    out = fopen(TMP, "w");

    /*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

    rc = fprintf_s(NULL, "%s", NULL);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    NEGERR_MSVC(ESNULLP, EOF);

    /*--------------------------------------------------*/

    /* wine msvcrt doesn't check fmt==NULL */
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_FPRINTF_S))
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    rc = fprintf_s(out, NULL);
    NEGERR_MSVC(ESNULLP, EOF);
#endif
#endif

    /*--------------------------------------------------*/

    str[0] = '\0';
    rc = fprintf_s(out, "%s %n", str, &ind);
    NEGERR_MSVC(EINVAL, EOF);

    if (!out) {
        printf("Failed to open file %s for write: %s\n", TMP, strerror(errno));
        return errs;
    }

    /*--------------------------------------------------*/

    rc = fprintf_s(out, "%s %%n", str);
    ERR(3)

    rc = fprintf_s(out, "%%n");
    ERR(2);

    /*--------------------------------------------------*/

    rc = fprintf_s(out, "%s", NULL);
    NEGERR(ESNULLP)

    /*--------------------------------------------------*/

    strcpy(str, "keep it simple");

    rc = fprintf_s(out, "%s", str);
    NOERR()

    rc = fprintf_s(out, "%d", -10000);
    ERR(6)

    rc = fprintf_s(out, "%ld", -10000L);
    NOERR()

    /*--------------------------------------------------*/

    str[0] = '\0';

    rc = fprintf_s(out, "%s", str);
    ERR(0)

    /*--------------------------------------------------*/

    strcpy(str, "keep it simple");

    rc = fprintf_s(out, "%s", str);
    ERR(strlen(str))

    /*--------------------------------------------------*/

    fclose(out);

    /* print to closed stream: unportable, and not valgrind-safe */
#ifndef __GLIBC__
    rc = fprintf_s(out, "%s", str);
#if defined(__GLIBC__)
    if (rc < 0) {
        ERR(-1);
        ERRNO(EBADF);
    } else {
        ERR(14); /* older glibc upstream bug */
        ERRNO(0);
    }
#else
    /* musl returns 14, all other -1 */
    if (rc != 14 && rc != -1) {
        debug_printf("%s %u  Error rc=%d \n", __FUNCTION__, __LINE__, (int)rc);
        errs++;
    }
#endif
    /* musl throws no error */
#endif

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_fprintf_s()); }
