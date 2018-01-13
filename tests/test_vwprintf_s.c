/*------------------------------------------------------------------
 * test_vwprintf_s
 * File 'wchar/vwprintf_s.c'
 * Lines executed:80.00% of 15
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#define LEN   ( 128 )

static wchar_t   wstr[LEN];
static char      str[LEN];

int vtwprintf_s (const wchar_t *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vwprintf_s(fmt, ap);
    va_end(ap);
    return rc;
}

int test_vwprintf_s (void)
{
    errno_t rc;
    int32_t ind;
    int errs = 0;

/*--------------------------------------------------*/

    rc = vtwprintf_s(NULL, NULL);
    NEGERR(ESNULLP)

/*--------------------------------------------------*/

    wstr[0] = L'\0';
    rc = vtwprintf_s(L"%s%n\n", wstr, &ind);
    NEGERR(EINVAL)

/*--------------------------------------------------*/

    rc = vtwprintf_s(L"%s%%n\n", wstr);
    if (rc < 0) {
        printf("Failed to open stdout for write: %s\n",
               strerror(errno));
        return errs;
    }
    ERR(3)

    rc = vtwprintf_s(L"%%n\n");
    ERR(3);

/*--------------------------------------------------*/

    /* TODO
    rc = vtwprintf_s(L"%s", NULL);
    NEGERR(ESNULLP)
    */

/*--------------------------------------------------*/

    wcscpy(wstr, L"12");
    strcpy(str, "34");

    rc = vtwprintf_s(L"%ls%s", wstr, str);
    ERRWCHAR(4)

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_vwprintf_s());
}
#endif
