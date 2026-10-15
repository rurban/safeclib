/*------------------------------------------------------------------
 * test_fwprintf_s
 * File 'wchar/fwprintf_s.c'
 * Lines executed:100.00% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>
#include <locale.h>

#define TMP "tmpfwp"
#define LEN (128)

static FILE *out;
static wchar_t wstr[LEN];
static char str[LEN];
int test_fwprintf_s(void);

int test_fwprintf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;
    int have_wine = 0;

    out = fopen(TMP, "w");

    /*--------------------------------------------------*/

    /* wine msvcrt doesn't check fmt==NULL */
#if !(defined(_WINE_MSVCRT) && defined(TEST_MSVCRT) && defined(HAVE_FWPRINTF_S))
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    rc = fwprintf_s(out, NULL);
    NEGERR(ESNULLP);
#endif
#else
    have_wine = 1;
#endif

    /*--------------------------------------------------*/

    wstr[0] = L'\0';
    rc = fwprintf_s(out, L"%s%n\n", wstr, &ind);
    NEGERR(EINVAL)

    if (!out) {
        printf("Failed to open file %s for write: %s\n", TMP, strerror(errno));
        return errs;
    }

    /*--------------------------------------------------*/

    rc = fwprintf_s(out, L"%s%%n\n", wstr);
    ERR(3)

    rc = fwprintf_s(out, L"%%n\n");
    ERR(3);

    /*--------------------------------------------------*/

    /* TODO
    rc = fwprintf_s(out, L"%s", NULL);
    NEGERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    wcscpy(wstr, L"12");
    strcpy(str, "34");

    rc = fwprintf_s(out, L"%ls%s", wstr, str);
    if (!have_wine) {
        ERRWCHAR(4);
    } else {
        ERR(-1);
    }

    /*--------------------------------------------------*/

    /* write to a read-only stream: the underlying vfwprintf fails */
    fclose(out);
    out = fopen(TMP, "r");
    if (out) {
        rc = fwprintf_s(out, L"%ls", wstr);
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
        if (setlocale(LC_CTYPE, "C.UTF-8") || setlocale(LC_CTYPE, "C.utf8") ||
            setlocale(LC_CTYPE, "en_US.UTF-8")) {
            out = fopen(TMP, "w");
            if (out) {
                errno = 0;
                rc = fwprintf_s(out, L"%s", "\xff\xfe");
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

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_fwprintf_s()); }
#endif
