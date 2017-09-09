/*------------------------------------------------------------------
 * test_vfwprintf_s
 * File 'wchar/vfwprintf_s.c'
 * Lines executed:80.00% of 15
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#define TMP   "tmpvfwp"
#define LEN   ( 128 )

static FILE* out;
static wchar_t   wstr[LEN];
static char      str[LEN];

int vtwprintf_s (FILE *restrict stream,
                const wchar_t *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vfwprintf_s(stream, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vfwprintf_s (void)
{
    errno_t rc;
    int32_t ind;
    int errs = 0;

    out = fopen(TMP, "w");

/*--------------------------------------------------*/

    rc = vtwprintf_s(out, NULL, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    wstr[0] = L'\0'; 
    rc = vtwprintf_s(out, L"%s%n\n", wstr, &ind);
    ERR(EINVAL)

    if (!out) {
        printf("Failed to open file %s for write: %s\n",
               TMP, strerror(errno));
        return errs;
    }

/*--------------------------------------------------*/

    rc = vtwprintf_s(out, L"%s%%n\n", wstr);
    ERR(3)

    rc = vtwprintf_s(out, L"%%n\n");
    ERR(3);

/*--------------------------------------------------*/

    /* TODO
    rc = vtwprintf_s(out, L"%s", NULL);
    ERR(ESNULLP)
    */

/*--------------------------------------------------*/

    wcscpy(wstr, L"12");
    strcpy(str, "34");

    rc = vtwprintf_s(out, L"%ls%s", wstr, str);
    ERR(4)

/*--------------------------------------------------*/
    fclose(out);
    unlink(TMP);

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_vfwprintf_s());
}
#endif
