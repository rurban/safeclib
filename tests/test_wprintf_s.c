/*------------------------------------------------------------------
 * test_wprintf_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#define LEN   ( 128 )

static wchar_t   wstr[LEN];
static char      str[LEN];
 
int test_wprintf_s (void)
{
    errno_t rc;
    int32_t ind;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wprintf_s(NULL, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    wstr[0] = L'\0';
    rc = wprintf_s(L"%s%n\n", wstr, &ind);
    ERR(EINVAL)

/*--------------------------------------------------*/

    rc = wprintf_s(L"%s%%n\n", wstr);
    if (rc < 0) {
        printf("Failed to open stdout for write: %s\n",
               strerror(errno));
        return errs;
    }
    ERR(3)

    rc = wprintf_s(L"%%n\n");
    ERR(3);

/*--------------------------------------------------*/

    /* TODO
    rc = wprintf_s(L"%s", NULL);
    ERR(-ESNULLP)
    */

/*--------------------------------------------------*/

    wcscpy(wstr, L"12");
    strcpy(str, "34");

    rc = wprintf_s(L"%ls%s", wstr, str);
    ERR(4)

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wprintf_s());
}
#endif
