/*------------------------------------------------------------------
 * test_vfprintf_s
 * File 'vfprintf_s.c'
 * Lines executed:100.00% of 22
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include "test_msvcrt.h"

#define TMP "tmpvfp"
#define LEN (128)

static FILE *out;
static char buf[LEN];
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

int errs = 0;
int win_wplus = 0; // if EINVAL with fopen w+

// a variant for debugging
static size_t cmp_and_reset(FILE *out, const char *s) {
    size_t pos = ftell(out);
    fflush(out);
    *buf = '\0';
    if (win_wplus) {
        fclose(out);
        out = fopen(TMP, "r");
        size_t nread = fread(buf, pos, 1, out);
        fclose(out);
        out = fopen(TMP, "w");
    } else {
        rewind(out);
        size_t nread = fread(buf, pos, 1, out);
    }
    EXPSTR(buf, s)
    if (!win_wplus) {
        rewind(out);
        size_t nread = ftruncate(fileno(out), 0L);
    }
    return pos;
}

int test_vfprintf_s(void) {
    errno_t rc;
    int32_t ind;
    long pos;

    // mingw (with some versions) disallows w+ (EINVAL)
    out = fopen(TMP, "w+");
    if (errno) {
        perror("fopen w+");
        fclose(out);
	out = fopen(TMP, "w");
	win_wplus = 1;
    }

#define CMP_AND_RESET(out, s)                                                  \
    pos = ftell(out);                                                          \
    fflush(out);                                                               \
    *buf = '\0';                                                               \
    if (win_wplus) {                                                           \
        fclose(out);                                                           \
        out = fopen(TMP, "r");                                                 \
        ind = fread(buf, pos, 1, out);                                         \
        fclose(out);                                                           \
        out = fopen(TMP, "w");                                                 \
    } else {                                                                   \
        rewind(out);                                                           \
        ind = fread(buf, pos, 1, out);                                         \
    }                                                                          \
    EXPSTR(buf, s)                                                             \
    if (!win_wplus) {                                                          \
        rewind(out);                                                           \
        ind = ftruncate(fileno(out), 0L);                                      \
    }

#ifdef HAVE_SYS_STAT_H
    {
        int fd = fileno(out);
        if (fd < 0) {
            perror("invalid " TMP);
            fclose(out);
            unlink(TMP);
            return 1;
        }
    }
#endif
    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    rc = vtfprintf_s(out, NULL);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    NEGERR_MSVC(ESNULLP, EOF);
    CMP_AND_RESET(out, "");

    rc = vtfprintf_s(NULL, "");
    NEGERR_MSVC(ESNULLP, EOF);
    CMP_AND_RESET(out, "");

    rc = vtfprintf_s(out, "", NULL);
    NEGERR(EOK)
    CMP_AND_RESET(out, "");

    rc = vtfprintf_s(NULL, "%s", NULL);
    NEGERR(ESNULLP);
    CMP_AND_RESET(out, "");

    /*--------------------------------------------------*/

    str1[0] = '\0';
    rc = vtfprintf_s(out, "%s%n\n", str1, &ind);
    NEGERR_MSVC(EINVAL, EOF);
    CMP_AND_RESET(out, "");

    /*--------------------------------------------------*/

    rc = vtfprintf_s(out, "%s%%n\n", str1);
    if (rc < 0) {
        perror("vtfprintf_s");
        fclose(out);
        unlink(TMP);
        return errs;
    }
    ERR(3)
    //rewind(out);
    CMP_AND_RESET(out, "%n\n"); //FIXME

    rc = vtfprintf_s(out, "%%n\n");
    ERR(3);
    CMP_AND_RESET(out, "%n\n");

    /*--------------------------------------------------*/

    rc = vtfprintf_s(out, "%s", NULL);
    NEGERR(ESNULLP)

    /*--------------------------------------------------*/

    strcpy(str1, "12");
    strcpy(str2, "34");

    rc = vtfprintf_s(out, "%s%s", str1, str2);
    ERR(4);
    CMP_AND_RESET(out, "1234");

    /*--------------------------------------------------*/

#if 1
    /* 0x7fffffff + 1 >INT_MAX */
    rc = printf_s("\n%2147483648d\n", INT_MAX);
    NEGERR(ESLEMAX);
    CMP_AND_RESET(out, "");
    //#if defined(__GLIBC__)
    //ERRNO(EOVERFLOW);
    //#endif
#endif

#if 0
    /* segfaults */
    rc = printf_s("%s\n", L'\xd834df01');
    ANYERR();
#endif

    fclose(out);
    out = fopen(TMP, "r");
    if (errno) {
        perror("fopen r");
        unlink(TMP);
        return errs + 1;
    }    
    /* print to not-writable stream */
    errno = 0;
    rc = vtfprintf_s(out, "%s", str1);
    ANYERR(); // Linux may produce EINVAL, most others EBADF
    if (errno == 0) {
        debug_printf("%s %u  Error errno=%d \n", __FUNCTION__,
                     __LINE__, (int)errno);
        errs++;
    }
    CMP_AND_RESET(out, "");
    if (pos) {
        debug_printf("%s %u  pos=%ld \n", __FUNCTION__, __LINE__, pos);
        errs++;
    }

    /*--------------------------------------------------*/

    fclose(out);

    /* print to closed stream: across libc's unportable, and not valgrind-safe */
    rc = vtfprintf_s(out, "%s", str1);
    ANYERR(); // Windows may produce EOF, most others EBADF
    // EBADF (linux, mingw) or EINVAL (bsd)
    if (errno == 0) {
        debug_printf("%s %u  Error errno=%d \n", __FUNCTION__,
                     __LINE__, (int)errno);
        errs++;
    }

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vfprintf_s()); }
