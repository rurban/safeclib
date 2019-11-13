/*------------------------------------------------------------------
 * test_snwprintf_s
 * File 'snwprintf_s.c'
 * Lines executed:81.48% of 54
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
int test_snwprintf_s(void);

int test_snwprintf_s(void) {
    errno_t rc;
    int32_t ind;
    size_t len2;
    size_t len3;
#ifndef __APPLE__
    wchar_t *wstr3;
#endif
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("dest overflow")
    rc = snwprintf_s(str1, RSIZE_MAX_WSTR + 1, L"%ls", str2);
    NEGERR(ESLEMAX)

    EXPECT_BOS("empty fmt")
    rc = snwprintf_s(str1, LEN, NULL);
    NEGERR(ESNULLP)

    EXPECT_BOS("empty dest") EXPECT_BOS("empty dest or dmax")
    rc = snwprintf_s(NULL, 0, L"%ls", str2);
    NEGERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = snwprintf_s(str1, 0, L"%ls", str2);
    NEGERR(ESZEROL)

    /* only with c99 __VA_ARGS__ we can pass destbos */
#ifdef SAFECLIB_HAVE_C99
    if (_BOS_KNOWN(str1)) {
        EXPECT_BOS("dest overflow")
        rc = snwprintf_s(str1, LEN + 1, L"%ls", str2);
        NEGERR(EOVERFLOW);       /* dmax exceeds dest */
        WCHECK_SLACK(str1, LEN); /* cleared */
    }
#endif
#endif

    /*--------------------------------------------------*/

    str2[0] = '\0';
    rc = snwprintf_s(str1, LEN, L"%s %n", str2, &ind);
    NEGERR(EINVAL)

    rc = snwprintf_s(str1, LEN, L"%s %%n", str2);
    ERR(3)

    rc = snwprintf_s(str1, LEN, L"%%n");
    ERR(2);

    /*--------------------------------------------------*/

    /* TODO
    rc = snwprintf_s(str1, LEN, L"%s", NULL);
    NEGERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    len2 = wcslen(str2);

    rc = snwprintf_s(str1, 50, L"%ls", str2);
    ERR((errno_t)len2)
    len3 = wcslen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        size_t len1 = wcslen(str1);
        debug_printf("%s %u lengths wrong: %d  %d  %d\n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
#endif
        errs++;
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"123456");
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, 1, L"%ls", str2);
    NOERRNULL() /* no ENOSPC */
    /* but truncated, written only 1, the \0 */
    WEXPSTR(str1, L"");
    if ((ind = wmemcmp(str1, L"\00023456\000", 7))) {
        debug_printf("%s %u snwprintf_s truncation: %d L\"\\x%x%ls\"\n",
                     __FUNCTION__, __LINE__, ind, str1[0], &str1[1]);
        errs++;
    }

    /*--------------------------------------------------*/

    wcscpy(str1, L"123456");
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, 2, L"%ls", str2);
    NOERRNULL() /* no ENOSPC */
    /* but truncated, written only 2, k\0 */
    WEXPSTR(str1, L"k");
    if ((ind = wmemcmp(str1, L"k\0003456\000", 7))) {
        debug_printf("%s %u snwprintf_s truncation: %d L\"\\x%x%ls\"\n",
                     __FUNCTION__, __LINE__, ind, str1[0], &str1[1]);
        errs++;
    }

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, 1, L"%ls", str2);
    NOERR() /* truncation */
    WEXPNULL(str1)

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, 2, L"%ls", str2);
    NOERR() /* truncation */
    WEXPSTR(str1, L"k");

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, 20, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = snwprintf_s(str1, LEN, L"%ls", str2);
    ERR(0)
    WEXPNULL(str1)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, LEN, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"qqweqq");
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, LEN, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, 12, L"%ls", str2);
    NOERR() /* truncation */
    WEXPSTR(str1, L"keep it sim")

    /*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = snwprintf_s(str1, 52, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

    /*--------------------------------------------------*/

    wcscpy(str1, L"12345678901234567890");

    rc = snwprintf_s(str1, 8, L"%ls", &str1[7]);
    NOERR() /* truncation */
    WEXPSTR(str1, L"8901234")

    /*--------------------------------------------------*/

    wcscpy(str1, L"123456789");

    rc = snwprintf_s(str1, 9, L"%ls", &str1[8]);
    ERR(1) /* overlapping allowed */
    WEXPSTR(str1, L"9")

    /*--------------------------------------------------*/

    wcscpy(str2, L"123");
    wcscpy(str1, L"keep it simple");

    rc = snwprintf_s(str2, 31, L"%ls", &str1[0]);
    NOERR()
    WEXPSTR(str2, L"keep it simple");

    /*--------------------------------------------------*/

    wcscpy(str2, L"1234");
    wcscpy(str1, L"56789");

    rc = snwprintf_s(str2, 10, L"%ls", str1);
    NOERR()
    WEXPSTR(str2, L"56789")

    /*--------------------------------------------------*/

#ifndef __APPLE__
    /* This crashes on darwin in vswprintf_l() */
    rc = snwprintf_s(str1, 10, L"%vls", str2);
#if defined(__GLIBC__) || defined(BSD_OR_WINDOWS_LIKE)
    /* they print unknown formats verbatim */
    NOERR();
#else /* musl and darwin disallow this */
    ERR(-1);
    /* darwin throws Illegal byte sequence */
    WEXPNULL(str1);
#endif

    /* not the fast stack-branch */
    wstr3 = (wchar_t *)malloc(513 * sizeof(wchar_t));
    rc = snwprintf_s(wstr3, 513, L"%vls", str1);
#if defined(__GLIBC__) || defined(BSD_OR_WINDOWS_LIKE)
    /* they print unknown formats verbatim */
    NOERR();
    free(wstr3);
#else /* musl and darwin disallow this */
    ERR(-1);
    WEXPNULL(str1)
    /* musl crashes on free() */
#endif
#endif

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_snwprintf_s()); }
#endif
