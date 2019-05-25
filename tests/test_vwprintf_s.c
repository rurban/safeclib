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

#ifdef HAVE_VWPRINTF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static wchar_t wstr[LEN];
static char str[LEN];
int vtwprintf_s(const wchar_t *restrict fmt, ...);
int test_vwprintf_s(void);

int vtwprintf_s(const wchar_t *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vwprintf_s(fmt, ap);
    va_end(ap);
    return rc;
}

int test_vwprintf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;
    int have_wine = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    /* wine msvcrt doesn't check fmt==NULL */
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_FPRINTF_S))
    rc = vtwprintf_s(NULL, NULL);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
#else
    printf("Using wine\n");
    rc = EOF;
    have_wine = 1;
    use_msvcrt = 1;
#endif
    NEGERR_MSVC(ESNULLP, EOF);

    /*--------------------------------------------------*/

    wstr[0] = L'\0';
    /* wine msvcrt doesn't check for %n neither */
    rc = vtwprintf_s(L"%s%n\n", wstr, &ind);
    if (!have_wine)
        NEGERR_MSVC(EINVAL, EOF);
    else
        ERR(1);

    /*--------------------------------------------------*/

    rc = vtwprintf_s(L"%s%%n\n", wstr);
    if (rc < 0) {
        printf("Failed to open stdout for write: %s\n", strerror(errno));
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
    if (!have_wine) {
        ERRWCHAR(4);
    } else {
        ERR(-1);
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vwprintf_s()); }
