/*------------------------------------------------------------------
 * test_vfprintf_s
 * File 'vfprintf_s.c'
 * Lines executed:83.33% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#ifdef HAVE_VFPRINTF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define TMP "tmpvfp"
#define LEN (128)

static FILE *out;
static char str1[LEN];
static char str2[LEN];
int vtfprintf_s(FILE *restrict stream, const char *restrict fmt, ...);
int test_vfprintf_s(void);

int vtfprintf_s(FILE *restrict stream, const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vfprintf_s(stream, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vfprintf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;

    out = fopen(TMP, "w");

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    rc = vtfprintf_s(out, NULL);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    NEGERR_MSVC(ESNULLP, EOF);

    rc = vtfprintf_s(NULL, "");
    NEGERR_MSVC(ESNULLP, EOF);

    rc = vtfprintf_s(out, "", NULL);
    NEGERR(EOK)

    /* TODO
    rc = vtfprintf_s(NULL, "%s", NULL);
    NEGERR(ESNULLP);
    */

    /*--------------------------------------------------*/

    str1[0] = '\0';
    rc = vtfprintf_s(out, "%s%n\n", str1, &ind);
    NEGERR_MSVC(EINVAL, EOF);

    /*--------------------------------------------------*/

    rc = vtfprintf_s(out, "%s%%n\n", str1);
    if (rc < 0) {
        printf("Failed to open stdout for write: %s\n", strerror(errno));
        return errs;
    }
    ERR(3)

    rc = vtfprintf_s(out, "%%n\n");
    ERR(3);

    /*--------------------------------------------------*/

    /* TODO
    rc = vtfprintf_s(out, "%s", NULL);
    NEGERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    strcpy(str1, "12");
    strcpy(str2, "34");

    rc = vtfprintf_s(out, "%s%s", str1, str2);
    ERR(4);

    /*--------------------------------------------------*/

#if 0
    /* 0x7fffffff + 1 >INT_MAX */
    rc = printf_s("\n%2147483648d\n", INT_MAX);
    ANYERR();
#if defined(__GLIBC__)
    ERRNO(EOVERFLOW);
#endif

    /* segfaults under darwin */
    rc = printf_s("%s\n", L'\xd834df01');
    ANYERR();
#endif

    /*--------------------------------------------------*/

    fclose(out);

    /* print to closed stream: unportable, and not valgrind-safe */
#ifndef __GLIBC__
    rc = vtfprintf_s(out, "%s", str1);
#if defined(__GLIBC__)
    if (rc < 0) {
        ERR(-1);
        ERRNO(EBADF);
    } else {
        /* older glibc upstream bug */
        NOERR();
        ERRNO(0);
    }
#elif defined BSD_ALL_LIKE
    ERR(-1);
#else
    /* musl throws no error */
#endif
#endif

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vfprintf_s()); }
