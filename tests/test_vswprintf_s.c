/*------------------------------------------------------------------
 * test_vswprintf_s
 * File 'wchar/vswprintf_s.c'
 * Lines executed:79.59% of 49
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>
#include <stdarg.h>

#ifdef HAVE_VSWPRINTF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

#undef ERRNO_MSVC
#define ERRNO_MSVC(rc, err)                                                    \
    if (!use_msvcrt) {                                                         \
        ERR(rc);                                                               \
        ERRNO(0);                                                              \
    } else {                                                                   \
        ERR(0);                                                                \
        ERRNO(err);                                                            \
    }

static wchar_t str1[LEN];
static wchar_t str2[LEN];
int vtwprintf_s(wchar_t *restrict dest, rsize_t dmax, const size_t destbos,
                const wchar_t *restrict fmt, ...);
int test_vswprintf_s(void);

int vtwprintf_s(wchar_t *restrict dest, rsize_t dmax, const size_t destbos,
                const wchar_t *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    if (use_msvcrt) {
        (void)destbos;
        rc = vswprintf_s(dest, dmax, fmt, ap);
    } else {
        rc = _vswprintf_s_chk(dest, dmax, destbos, fmt, ap);
    }
    va_end(ap);
    return rc;
}

int test_vswprintf_s(void) {
    errno_t rc;
    size_t len2;
    size_t len3;
#ifndef __APPLE__
    wchar_t *wstr3;
#endif
    int errs = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);

    rc = vtwprintf_s(NULL, LEN, 0, L"%ls", str2);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    ERRNO_MSVC(-ESNULLP, EINVAL);

    /* not testable */
    if (use_msvcrt) {
        rc = vtwprintf_s(str1, LEN, BOS(str1), L"%ls", NULL);
        ERR(0);
        ERRNO_MSVC(0, EINVAL);
    }

    rc = vtwprintf_s(str1, LEN, BOS(str1), NULL, NULL);
    ERRNO_MSVC(-ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    rc = vtwprintf_s(str1, 0, BOS(str1), L"%ls", str2);
    ERRNO_MSVC(-ESZEROL, EINVAL);

    /*--------------------------------------------------*/

    rc = vtwprintf_s(str1, (RSIZE_MAX_STR + 1), BOS(str1), L"%ls", str2);
    ERRNO_MSVC(-ESLEMAX, 0);

    if (_BOS_KNOWN(str1)) {
      rc = vtwprintf_s(str1, (LEN + 1), BOS(str1), L"%ls", str2);
      ERRNO_MSVC(-EOVERFLOW, 0);
    }

    /*--------------------------------------------------*/

    str2[0] = '\0';
    rc = vtwprintf_s(str1, LEN, BOS(str1), L"%s %n", str2);
    ERR(-EINVAL)

    rc = vtwprintf_s(str1, LEN, BOS(str1), L"%s %%n", str2);
    ERR(3)

    rc = vtwprintf_s(str1, LEN, BOS(str1), L"%%n");
    ERR(2);

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    len2 = wcslen(str2);

    rc = vtwprintf_s(str1, 50, BOS(str1), L"%ls", str2);
    ERR((errno_t)len2)
    len3 = wcslen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        size_t len1 = wcslen(str1);
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
#endif
        errs++;
    }

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 1, BOS(str1), L"%ls", str2);
    ERRNO_MSVC(-ESNOSPC, ERANGE);
    WEXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 2, BOS(str1), L"%ls", str2);
    ERRNO_MSVC(-ESNOSPC, ERANGE);
    WEXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 20, BOS(str1), L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = vtwprintf_s(str1, LEN, BOS(str1), L"%ls", str2);
    ERR(0)
    WEXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, LEN, BOS(str1), L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"qqweqq");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, LEN, BOS(str1), L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 12, BOS(str1), L"%ls", str2);
    ERRNO_MSVC(-ESNOSPC, ERANGE);
    WEXPNULL(str1)

    wcscpy(str1, L"1234");

    rc = vtwprintf_s(str1, 5, BOS(str1), L"%ls", str2);
    ERRNO_MSVC(-ESNOSPC, ERANGE);
    WEXPNULL(str1)

    /*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 52, BOS(str1), L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"12345678901234567890");

    rc = vtwprintf_s(str1, 8, BOS(str1), L"%ls", &str1[7]);
    /* overlapping implementation defined */
#if defined(__GLIBC__) || defined(_WIN32)
    ERR(-ESNOSPC);
    WEXPNULL(str1);
#else
    NOERR();
    /* WEXPSTR(str1, L"8901234"); or WEXPNULL(str1) */
#endif

    /*--------------------------------------------------*/

    wcscpy(str1, L"123456789");

    rc = vtwprintf_s(str1, 9, BOS(str1), L"%ls", &str1[8]);
    ERR(1) /* overlapping allowed */
    WEXPSTR(str1, L"9")

    /*--------------------------------------------------*/

    wcscpy(str2, L"123");
    wcscpy(str1, L"keep it simple");

    rc = vtwprintf_s(str2, 31, BOS(str1), L"%ls", &str1[0]);
    NOERR()
    WEXPSTR(str2, L"keep it simple");

    /*--------------------------------------------------*/

    wcscpy(str2, L"1234");
    wcscpy(str1, L"56789");

    rc = vtwprintf_s(str2, 10, BOS(str1), L"%ls", str1);
    NOERR();
    WEXPSTR(str2, L"56789");

    /*--------------------------------------------------*/

#ifndef __APPLE__
    rc = vtwprintf_s(str1, 10, BOS(str1), L"%vls", str2);
#if defined(__GLIBC__) || defined(BSD_OR_NEWLIB_LIKE) || defined(__MINGW32__)
    /* they print unknown formats verbatim */
    NOERR();
#else /* musl and darwin disallow this */
    ERR(-1);
    /* darwin throws Illegal byte sequence */
    WEXPNULL(str1);
#endif

    /* not the fast stack-branch */
    wstr3 = (wchar_t *)malloc(513 * sizeof(wchar_t));
    rc = vtwprintf_s(wstr3, 513, BOS(wstr3), L"%vls", str1);
#if defined(__GLIBC__) || defined(BSD_OR_NEWLIB_LIKE) || defined(__MINGW32__)
    /* they print unknown formats verbatim */
    NOERR();
#else /* musl and darwin disallow this */
    ERR(-1);
    WEXPNULL(str1)
    /* musl crashes on free() */
#endif
    free(wstr3);
#endif

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vswprintf_s()); }
