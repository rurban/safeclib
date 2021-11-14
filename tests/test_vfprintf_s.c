/*------------------------------------------------------------------
 * test_vfprintf_s
 * File 'vfprintf_s.c'
 * Lines executed:83.33% of 18
 *
 *------------------------------------------------------------------
 */

//#define _POSIX_C_SOURCE 199309
#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

//#ifdef HAVE_VFPRINTF_S
//#define HAVE_NATIVE 1
//#else
//#define HAVE_NATIVE 0
//#endif
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

#define CMP_AND_RESET(out, s)                                                  \
    pos = ftell(out);                                                          \
    fflush(out);							       \
    rewind(out);							       \
    *buf = '\0';                                                               \
    ind = fread(buf, pos, 1, out);                                             \
    EXPSTR(buf, s)							       \
    rewind(out);							       \
    ind = ftruncate(fileno(out), 0L);

int test_vfprintf_s(void) {
    errno_t rc;
    int32_t ind;
    long pos;
    int errs = 0;

    out = fopen(TMP, "w+");
    if (errno) {
        perror("fopen");
        return 1;
    }
#ifdef HAVE_SYS_STAT_H
    {
        int fd = fileno(out);
        if (fd < 0) {
            perror("invalid " TMP);
            return 1;
        }
    }
#endif
    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    rc = vtfprintf_s(out, NULL);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    NEGERR_MSVC(ESNULLP, EOF);
    CMP_AND_RESET(out, "")

    rc = vtfprintf_s(NULL, "");
    NEGERR_MSVC(ESNULLP, EOF);
    CMP_AND_RESET(out, "")

    rc = vtfprintf_s(out, "", NULL);
    NEGERR(EOK)
    CMP_AND_RESET(out, "")

    rc = vtfprintf_s(NULL, "%s", NULL);
    NEGERR(ESNULLP);
    CMP_AND_RESET(out, "")

    /*--------------------------------------------------*/

    str1[0] = '\0';
    rc = vtfprintf_s(out, "%s%n\n", str1, &ind);
    NEGERR_MSVC(EINVAL, EOF);
    CMP_AND_RESET(out, "")

    /*--------------------------------------------------*/

    rc = vtfprintf_s(out, "%s%%n\n", str1);
    if (rc < 0) {
        perror("vtfprintf_s");
        return errs;
    }
    ERR(3)
    //rewind(out);
    CMP_AND_RESET(out, "%n\n") //FIXME

    rc = vtfprintf_s(out, "%%n\n");
    ERR(3);
    CMP_AND_RESET(out, "%n\n")

    /*--------------------------------------------------*/

    rc = vtfprintf_s(out, "%s", NULL);
    NEGERR(ESNULLP)

    /*--------------------------------------------------*/

    strcpy(str1, "12");
    strcpy(str2, "34");

    rc = vtfprintf_s(out, "%s%s", str1, str2);
    ERR(4);
    CMP_AND_RESET(out, "1234")

    /*--------------------------------------------------*/

#if 1
    /* 0x7fffffff + 1 >INT_MAX */
    rc = printf_s("\n%2147483648d\n", INT_MAX);
    NEGERR(ESLEMAX);
    CMP_AND_RESET(out, "")
    //#if defined(__GLIBC__)
    //ERRNO(EOVERFLOW);
    //#endif
#endif

#if 0
    /* segfaults */
    rc = printf_s("%s\n", L'\xd834df01');
    ANYERR();
#endif

#if defined HAVE_SYS_STAT_H && 0
    {
        int fd = fileno(out);
        if (fd >= 0) {
            struct stat st;
            fstat(fd, &st);
            if (!S_ISREG(st.st_mode))
                return 1;
            /* print to not-writable stream */
#ifndef _WIN32
            if (fchmod(fd, st.st_mode & ~(S_IWUSR|S_IWGRP|S_IWOTH)) == 0)
#else
            if (chmod(TMP, st.st_mode & ~(S_IWUSR|S_IWGRP|S_IWOTH)) == 0)
#endif
	    {
                struct stat st1;
                fstat(fd, &st1);
                if (st1.st_mode < st.st_mode) {
                    rc = vtfprintf_s(out, "%s", str1);
                    ANYERR(); // Linux produces EINVAL, other EBADF
                    if (errno >= 0) {
                        debug_printf("%s %u  Error errno=%d \n", __FUNCTION__,
                                     __LINE__, (int)errno);
                        errs++;
                    }
                    CMP_AND_RESET(out, "")
		    if (pos) {
			debug_printf("%s %u  pos=%ld \n", __FUNCTION__, __LINE__, pos);
			errs++;
		    }
                } else {
		    debug_printf("fchmod failed to remove write perms\n");
		}
            }
#ifndef _WIN32
            fchmod(fd, st.st_mode); // restore wr perms
#else
            chmod(TMP, st.st_mode);
#endif
            rewind(out);
        }
    }
#endif

    /*--------------------------------------------------*/

    fclose(out);

    /* print to closed stream: across libc's unportable, and not valgrind-safe */
    rc = vtfprintf_s(out, "%s", str1);
    ERR(-EBADF);
    ERRNO(EBADF);

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vfprintf_s()); }
