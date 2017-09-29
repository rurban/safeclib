/*------------------------------------------------------------------
 * test_swprintf_s
 * File 'wchar/swprintf_s.c'
 * Lines executed:78.26% of 46
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

 
int test_swprintf_s (void)
{
    errno_t rc;
    size_t  len2;
    size_t  len3;
    int errs = 0;

/*--------------------------------------------------*/

    rc = swprintf_s(str1, RSIZE_MAX_STR+1, L"%ls", str2);
    NEGERR(ESLEMAX)

/*--------------------------------------------------*/

    rc = swprintf_s(str1, LEN, NULL, NULL);
    NEGERR(ESNULLP)

/*--------------------------------------------------*/

    rc = swprintf_s(NULL, 0, L"%ls", str2);
    NEGERR(ESNULLP)

/*--------------------------------------------------*/

    rc = swprintf_s(str1, 0, L"%ls", str2);
    NEGERR(ESZEROL)

/*--------------------------------------------------*/

    str2[0] = '\0';
    rc = swprintf_s(str1, LEN, L"%s %n", str2);
    NEGERR(EINVAL)

    rc = swprintf_s(str1, LEN, L"%s %%n", str2);
    ERR(3)

    rc = swprintf_s(str1, LEN, L"%%n");
    ERR(2);

/*--------------------------------------------------*/

    /* TODO
    rc = swprintf_s(str1, LEN, L"%s", NULL);
    NEGERR(ESNULLP)
    */

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    len2 = wcslen(str2);

    rc = swprintf_s(str1, 50, L"%ls", str2);
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

    rc = swprintf_s(str1, 1, L"%ls", str2);
    NEGERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = swprintf_s(str1, 2, L"%ls", str2);
    NEGERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = swprintf_s(str1, 20, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = swprintf_s(str1, LEN, L"%ls", str2);
    ERR(0)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = swprintf_s(str1, LEN, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqq");
    wcscpy(str2, L"keep it simple");

    rc = swprintf_s(str1, LEN, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = swprintf_s(str1, 12, L"%ls", str2);
    NEGERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = swprintf_s(str1, 52, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    wcscpy(str1, L"12345678901234567890");

    rc = swprintf_s(str1, 8, L"%ls", &str1[7]);
    if (rc == 0) {
        NEGERR(EOK); /* overlapping implementation defined */
        WEXPSTR(str1, L"8901234");
    } else {
        NEGERR(ESNOSPC);
        WEXPNULL(str1);
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"123456789");

    rc = swprintf_s(str1, 9, L"%ls", &str1[8]);
    ERR(1); /* overlapping allowed */
    WEXPSTR(str1, L"9")

/*--------------------------------------------------*/

    wcscpy(str2, L"123");
    wcscpy(str1, L"keep it simple");

    rc = swprintf_s(str2, 31, L"%ls", &str1[0]);
    NOERR()
    WEXPSTR(str2, L"keep it simple");

/*--------------------------------------------------*/

    wcscpy(str2, L"1234");
    wcscpy(str1, L"56789");

    rc = swprintf_s(str2, 10, L"%ls", str1);
    NOERR()
    WEXPSTR(str2, L"56789")

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_swprintf_s());
}
#endif
