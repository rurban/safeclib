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

int errs = 0;
int win_wplus = 0; // if EINVAL with fopen w+
static FILE *out, *f;
static char str[LEN];
int test_fprintf_s(void);

// a variant for debugging
static size_t cmp_and_reset(FILE *out, const char *s) {
    size_t nread;
    size_t pos = ftell(out);
    fflush(out);
    *str = '\0';
    if (win_wplus) {
        fclose(out);
        out = fopen(TMP, "r");
        nread = fread(str, 1, pos, out);
        fclose(out);
        out = fopen(TMP, "w");
    } else {
        rewind(out);
        nread = fread(str, 1, pos, out);
    }
    EXPSTR(str, s)
    if (!win_wplus) {
        rewind(out);
#if defined(HAVE_FTRUNCATE) && defined(HAVE_FILENO)
        nread = ftruncate(fileno(out), 0L);
#endif
    }
    (void) nread;
    return pos;
}

int test_fprintf_s(void) {
    size_t nread;
    errno_t rc;
    int ind;

    // mingw (with some versions) disallows w+ (EINVAL)
    out = fopen(TMP, "w+");
    if (errno) {
        perror("fopen w+");
        fclose(out);
        out = fopen(TMP, "w");
        win_wplus = 1;
    }

    /*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

    rc = fprintf_s(NULL, "%s", (char*)NULL);
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

    rc = fprintf_s(out, "%s", (char*)NULL);
    NEGERR(ESNULLP)

    /*--------------------------------------------------*/

    strcpy(str, "keep it simple");

    rc = fprintf_s(out, "%s", str);
    NOERR()

    rc = fprintf_s(out, "%d", -10000);
    ERR(6)

    rc = fprintf_s(out, "%ld", -10000L);
    NOERR()

    rewind(out);
    nread = ftruncate(fileno(out), 0L);
    (void)nread;

    /*--------------------------------------------------*/

    str[0] = '\0';

    rc = fprintf_s(out, "%s", str);
    ERR(0)
    cmp_and_reset(out, "");

    /*--------------------------------------------------*/

    strcpy(str, "keep it simple");

    rc = fprintf_s(out, "%s", str);
    ERR(strlen(str))
    cmp_and_reset(out, "keep it simple");

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
