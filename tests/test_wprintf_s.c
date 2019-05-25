/*------------------------------------------------------------------
 * test_wprintf_s
 * File 'wchar/wprintf_s.c'
 * Lines executed:81.25% of 16
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#ifdef HAVE_WPRINTF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static wchar_t wstr[LEN];
static char str[LEN];
int test_wprintf_s(void);

int test_wprintf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;
    int have_wine = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    /* wine msvcrt doesn't check fmt==NULL */
#if defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_WPRINTF_S)
    printf("Using wine\n");
    rc = 0;
    use_msvcrt = 1;
    have_wine = 1;
#elif !(defined(TEST_MSVCRT) && defined(HAVE_WPRINTF_S))
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    rc = wprintf_s(NULL);
    init_msvcrt(rc == -ESNULLP, &use_msvcrt);
    NEGERR_MSVC(ESNULLP, 0);
#endif
#else
    use_msvcrt = 1;
#endif

    /*--------------------------------------------------*/

    wstr[0] = L'\0';
    /* wine msvcrt doesn't check for %n neither */
    rc = wprintf_s(L"%s%n\n", wstr, &ind);
    if (!have_wine)
        NEGERR_MSVC(EINVAL, EOF);
    else
        ERR(1);

    /*--------------------------------------------------*/

    rc = wprintf_s(L"%s%%n\n", wstr);
    if (rc < 0) {
        printf("Failed to open stdout for write: %s\n", strerror(errno));
        return errs;
    }
    ERR(3)

    rc = wprintf_s(L"%%n\n");
    ERR(3);

    /*--------------------------------------------------*/

    /* TODO
    rc = wprintf_s(L"%s", NULL);
    NEGERR_MSVC(ESNULLP, EOF);
    */

    /*--------------------------------------------------*/

    wcscpy(wstr, L"12");
    strcpy(str, "34");

    rc = wprintf_s(L"%ls%s", wstr, str);
    if (!have_wine) {
        ERRWCHAR(4);
    } else {
        ERR(-1);
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_wprintf_s()); }
