/*------------------------------------------------------------------
 * test_vfwprintf_s
 * File 'wchar/vfwprintf_s.c'
 * Lines executed:100.00% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>
#include <locale.h>

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

    /* write to a read-only stream: the underlying vfwprintf fails */
    fclose(out);
    out = fopen(TMP, "r");
    if (out) {
        rc = vtwprintf_s(out, L"%ls", wstr);
#ifndef _WIN32 /* the Windows CRT may buffer it */
        ANYERR();
#endif
        fclose(out);
    }

#if !defined(HAVE_MINGW32) && !defined(_WIN32)
    /* illegal multibyte sequence in a %s argument: EILSEQ */
    {
        char *oldloc = setlocale(LC_CTYPE, NULL);
        char saved[64];
        saved[0] = '\0';
        if (oldloc) {
            strncpy(saved, oldloc, sizeof(saved) - 1);
            saved[sizeof(saved) - 1] = '\0';
        }
        if (setlocale(LC_CTYPE, "C.UTF-8") ||
            setlocale(LC_CTYPE, "C.utf8") ||
            setlocale(LC_CTYPE, "en_US.UTF-8")) {
            out = fopen(TMP, "w");
            if (out) {
                errno = 0;
                rc = vtwprintf_s(out, L"%s", "\xff\xfe");
                ANYERR();
                ERRNO(EILSEQ);
                fclose(out);
            }
        }
        setlocale(LC_CTYPE, saved[0] ? saved : "C");
    }
#endif

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vfwprintf_s()); }
