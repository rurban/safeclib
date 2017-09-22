/*------------------------------------------------------------------
 * test_vswprintf_s
 * File 'wchar/vswprintf_s.c'
 * Lines executed:80.00% of 40
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>
#include <stdarg.h>

#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

int vtwprintf_s (wchar_t *restrict dest, rsize_t dmax,
                const wchar_t *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vswprintf_s(dest, dmax, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vswprintf_s (void)
{
    errno_t rc;
    size_t  len2;
    size_t  len3;
#ifndef __APPLE__
    wchar_t *wstr3;
#endif
    int errs = 0;

/*--------------------------------------------------*/

    /* not testable
      rc = vtwprintf_s(str1, LEN, L"%ls", NULL);
      ERR(ESNULLP)
    */

    rc = vtwprintf_s(NULL, LEN, L"%ls", str2);
    ERR(ESNULLP);

    rc = vtwprintf_s(str1, LEN, NULL, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = vtwprintf_s(str1, 0, L"%ls", str2);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = vtwprintf_s(str1, (RSIZE_MAX_STR+1), L"%ls", str2);
    ERR(ESLEMAX);

/*--------------------------------------------------*/

    str2[0] = '\0';
    rc = vtwprintf_s(str1, LEN, L"%s %n", str2);
    ERR(EINVAL)

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
        debug_printf("%s %u lengths wrong: %d  %d  %d \n",
                     __FUNCTION__, __LINE__, (int)len1, (int)len2, (int)len3);
#endif
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 1, L"%ls", str2);
    ERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 2, L"%ls", str2);
    ERR(ESNOSPC)
    WEXPNULL(str1)

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
    ERR(ESNOSPC)
    WEXPNULL(str1)

    wcscpy(str1, L"1234");

    rc = vtwprintf_s(str1, 5, L"%ls", str2);
    ERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 52, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    wcscpy(str1, L"12345678901234567890");

    rc = vtwprintf_s(str1, 8, L"%ls", &str1[7]);
    NOERR(); /* overlapping implementation defined */
    /* WEXPSTR(str1, L"8901234"); or WEXPNULL() */

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

#ifndef __APPLE__
    rc = vtwprintf_s(str1, 10, L"%vls", str2);
# if defined(__GLIBC__) || defined(BSD_OR_NEWLIB_LIKE)
    /* they print unknown formats verbatim */
    NOERR();
# else /* musl and darwin disallow this */
    ERR(-1);
    /* darwin throws Illegal byte sequence */
    WEXPNULL(str1);
# endif

    /* not the fast stack-branch */
    wstr3 = (wchar_t*)malloc(513);
    rc = vtwprintf_s(wstr3, 513, L"%vls", str1);
# if defined(__GLIBC__) || defined(BSD_OR_NEWLIB_LIKE)
    /* they print unknown formats verbatim */
    NOERR();
# else /* musl and darwin disallow this */
    ERR(-1);
    WEXPNULL(str1)
# endif
    free(wstr3);
#endif

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_vswprintf_s());
}
#endif
