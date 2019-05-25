/*------------------------------------------------------------------
 * test_vsnwprintf_s
 * File 'wchar/vsnwprintf_s.c'
 * Lines executed:77.55% of 49
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>
#include <stdarg.h>

#define LEN (128)

static wchar_t str1[LEN];
static wchar_t str2[LEN];
static inline int vtwprintf_s(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict fmt, ...) BOSW_CHK(dest)
    BOS_NULL(fmt);
int test_vsnwprintf_s(void);

static inline int vtwprintf_s(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vsnwprintf_s(dest, dmax, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vsnwprintf_s(void) {
    errno_t rc;
    size_t len2;
    size_t len3;
    int errs = 0;

    /*--------------------------------------------------*/

    /* not testable
      rc = vtwprintf_s(str1, LEN, L"%ls", NULL);
      NEGERR(ESNULLP)
    */

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = vtwprintf_s(NULL, LEN, L"%ls", str2);
    NEGERR(ESNULLP);

    EXPECT_BOS("empty fmt")
    rc = vtwprintf_s(str1, LEN, NULL);
    NEGERR(ESNULLP);

    EXPECT_BOS("empty dest or dmax")
    rc = vtwprintf_s(str1, 0, L"%ls", str2);
    NEGERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = vtwprintf_s(str1, (RSIZE_MAX_WSTR + 1), L"%ls", str2);
    NEGERR(ESLEMAX);
#endif
    /*--------------------------------------------------*/

    str2[0] = '\0';
    rc = vtwprintf_s(str1, LEN, L"%s %n", str2);
    NEGERR(EINVAL)

    rc = vtwprintf_s(str1, LEN, L"%s %%n", str2);
    ERR(3)

    rc = vtwprintf_s(str1, LEN, L"%%n");
    ERR(2);

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    len2 = wcslen(str2);

    rc = vtwprintf_s(str1, 50, L"%ls", str2);
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

    rc = vtwprintf_s(str1, 1, L"%ls", str2);
    NOERR() /* truncation */
    WEXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 2, L"%ls", str2);
    NOERR() /* truncation */
    WEXPSTR(str1, L"k");

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 20, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = vtwprintf_s(str1, LEN, L"%ls", str2);
    ERR(0)
    WEXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, LEN, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"qqweqq");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, LEN, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 12, L"%ls", str2);
    NOERR() /* truncation */
    WEXPSTR(str1, L"keep it sim")

    /*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 52, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"12345678901234567890");

    rc = vtwprintf_s(str1, 8, L"%ls", &str1[7]);
    NOERR() /* truncation, overlapping allowed */
    WEXPSTR(str1, L"8901234")

    /*--------------------------------------------------*/

    wcscpy(str1, L"123456789");

    rc = vtwprintf_s(str1, 9, L"%ls", &str1[8]);
    ERR(1) /* overlapping allowed */
    WEXPSTR(str1, L"9")

    /*--------------------------------------------------*/

    wcscpy(str2, L"123");
    wcscpy(str1, L"keep it simple");

    rc = vtwprintf_s(str2, 31, L"%ls", &str1[0]);
    NOERR()
    WEXPSTR(str2, L"keep it simple");

    /*--------------------------------------------------*/

    wcscpy(str2, L"1234");
    wcscpy(str1, L"56789");

    rc = vtwprintf_s(str2, 10, L"%ls", str1);
    NOERR();
    WEXPSTR(str2, L"56789");

    /*--------------------------------------------------*/

    /* crashes with g++ -std=c11 or darwin clang-mp-4.0 -std=c99 */
#if !defined(__cplusplus) && !defined(__APPLE__)

    /* XXX valgrind error in __vfprintf */
    rc = vtwprintf_s(str1, 10, L"%vls", str2);
#if defined(__GLIBC__) || defined(BSD_OR_WINDOWS_LIKE)
    /* they print unknown formats verbatim */
    NOERR();
#else /* musl and darwin disallow this */
    ERR(-1);
    /* darwin throws Illegal byte sequence */
    WEXPNULL(str1);
#endif

    {
        wchar_t *wstr3;
        *str1 = 0;
        /* not the fast stack-branch */
        wstr3 = (wchar_t *)malloc(513);
        rc = vtwprintf_s(wstr3, 513, L"%vls", str1);
#if defined(__GLIBC__) || defined(BSD_OR_WINDOWS_LIKE)
        /* they print unknown formats verbatim */
        NOERR();
#else /* musl and darwin disallow this */
        ERR(-1);
        WEXPNULL(str1);
#endif
        free(wstr3);
    }

#endif

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vsnwprintf_s()); }
