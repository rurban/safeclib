/*------------------------------------------------------------------
 * test_wcstombs_s
 * File 'wchar/wcstombs_s.c'
 * Lines executed:90.62% of 32
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCSTOMBS_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX (128)
#define LEN (128)

static char dest[LEN];
static wchar_t src[LEN];
int test_wcstombs_s(void);

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

int test_wcstombs_s(void) {
    errno_t rc;
    size_t ind = 0;
    const wchar_t *cs;
    const char *lang;
    const char *lc_cat;
    int errs = 0;
#ifndef HAVE_CT_BOS_OVR
    int have_wine = 0;
#endif

    /*--------------------------------------------------*/

    cs = L"a";
    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty retvalp") EXPECT_BOS("empty dest")
    rc = wcstombs_s(NULL, NULL, LEN, cs, 1);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    EXPECT_BOS("empty dest or dmax")
    rc = wcstombs_s(&ind, dest, 0, cs, 1);
    if (use_msvcrt && rc == 0 && ind == 0) {
        printf("Using wine\n");
        have_wine = 1;
    }
    ERR_MSVC(ESZEROL, have_wine ? 0 : EINVAL);

    EXPECT_BOS("empty dest") EXPECT_BOS("empty dest or dmax")
    rc = wcstombs_s(&ind, NULL, 0, cs, 1);
    ERR_MSVC(ESNOSPC, 0);

    EXPECT_BOS("dest overflow")
    rc = wcstombs_s(&ind, dest, RSIZE_MAX_STR + 1, cs, 1);
    ERR_MSVC(ESLEMAX, 0);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = wcstombs_s(&ind, dest, LEN + 1, cs, 1);
    ERR(EOVERFLOW);
#endif
#endif

    {
        const wchar_t *srcp = (const wchar_t *)(void *)dest;
        strcpy(dest, "abcdef");
        rc = wcstombs_s(&ind, dest, LEN, srcp, 3);
        ERR_MSVC(ESOVRLP, EILSEQ);

        dest[0] = 'a';
        dest[1] = '\0';
        rc = wcstombs_s(&ind, dest, LEN, srcp, 1);
        ERR_MSVC(ESOVRLP, 0);
    }

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty src") EXPECT_BOS("empty src or len")
    rc = wcstombs_s(&ind, dest, LEN, NULL, 0);
    ERR_MSVC(ESNULLP, have_wine ? EINVAL : 0);
    CHECK_SLACK(dest, LEN);
#endif

    /*--------------------------------------------------*/

    memset(dest, 'x', LEN);
    rc = wcstombs_s(&ind, dest, LEN, (cs = L"abcdef", cs), 3);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 3);
    } else {
        INDCMP(!= 4); /* !!! */
    }
    CHECK_SLACK(&dest[3], LEN - 3);

    rc = wcstombs_s(&ind, dest, LEN, (cs = L"abcdef", cs), 6);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 6);
    } else {
        INDCMP(!= 7);
    }
    CHECK_SLACK(&dest[6], LEN - 6);

#ifndef HAVE_CT_BOS_OVR
    memset(dest, 'x', LEN);
    EXPECT_BOS("empty dest")
    rc = wcstombs_s(&ind, NULL, LEN, (cs = L"abcdef", cs), 2);
    ERR_MSVC(EOK, EINVAL);
    if (!use_msvcrt) {
        INDCMP(!= 6);
    } else if (use_msvcrt) {
        INDCMP(!= 0);
    }
#endif

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    memset(dest, 'x', LEN);
    rc = wcstombs_s(&ind, dest, LEN, (cs = L"\x78", cs), 1);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 1);
    } else {
        INDCMP(!= 2);
    }
    CHECK_SLACK(&dest[1], LEN - 1);

    src[0] = 0xdf81;
    src[1] = 0;
    cs = src;
    memset(dest, 'x', LEN);
    rc = wcstombs_s(&ind, dest, LEN, cs, LEN);
    if (rc == 0) { /* well, musl on ASCII allows this */
        INDCMP(!= 1);
        CHECK_SLACK(&dest[1], LEN - 1);
    } else {
        ERR(EILSEQ);
        if (!use_msvcrt) {
            INDCMP(!= -1);
        } else {
            INDCMP(!= 0);
        }
        CHECK_SLACK(&dest[0], LEN);
    }

    SETLOCALE_UTF8;
    SETLANG("default");
    REQLOCALE("UTF-8")

    /* illegal sequences (locale independent) */

    memset(dest, 'x', LEN);
    rc = wcstombs_s(&ind, dest, LEN, (cs = L"0xdf79", cs), 1);
    ERR(EOK);
    INDCMP(!= 1);
    CHECK_SLACK(&dest[1], LEN - 1);

    /*--------------------------------------------------*/

    return (errs);
}

#endif

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) {
#ifdef HAVE_WCHAR_H
    return (test_wcstombs_s());
#else
    return 0;
#endif
}
#endif
