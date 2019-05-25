/*------------------------------------------------------------------
 * test_vfwprintf_s
 * File 'wchar/vfwprintf_s.c'
 * Lines executed:83.33% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#ifdef HAVE_VFWPRINTF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define TMP "tmpvfwp"
#define LEN (128)

static FILE *out;
static wchar_t wstr[LEN];
static char str[LEN];
int vtwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...);
int test_vfwprintf_s(void);

int vtwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vfwprintf_s(stream, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vfwprintf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;

    out = fopen(TMP, "w");

    /*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

    rc = vtwprintf_s(out, NULL);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    NEGERR_MSVC(ESNULLP, EOF);

    rc = vtwprintf_s(NULL, L"");
    NEGERR_MSVC(ESNULLP, EOF);

    rc = vtwprintf_s(out, L"");
    NEGERR(EOK)

    /* TODO
    rc = vtwprintf_s(out, L"%s", NULL);
    NEGERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    wstr[0] = L'\0';
    rc = vtwprintf_s(out, L"%s%n\n", wstr, &ind);
    NEGERR(EINVAL)

    if (!out) {
        printf("Failed to open file %s for write: %s\n", TMP, strerror(errno));
        return errs;
    }

    /*--------------------------------------------------*/

    rc = vtwprintf_s(out, L"%s%%n\n", wstr);
    ERR(3)

    rc = vtwprintf_s(out, L"%%n\n");
    ERR(3);

    /*--------------------------------------------------*/

    wcscpy(wstr, L"12");
    strcpy(str, "34");

    rc = vtwprintf_s(out, L"%ls%s", wstr, str);
    ERRWCHAR(4)

    /*--------------------------------------------------*/
    fclose(out);
    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vfwprintf_s()); }
