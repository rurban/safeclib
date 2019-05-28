/*------------------------------------------------------------------
 * test_wcrtomb_s
 * File 'wchar/wcrtomb_s.c'
 * Lines executed:95.00% of 20
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCRTOMB_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX (128)
#define LEN (128)

static char dest[LEN];
static wchar_t wc;
int test_wcrtomb_s(void);

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#define CLRPS memset(&ps, '\0', sizeof(mbstate_t))

int test_wcrtomb_s(void) {
    errno_t rc;
    size_t ind;
    const char *lang;
    const char *lc_cat;
    mbstate_t ps;
    int errs = 0;
#ifndef HAVE_CT_BOS_OVR
    int have_wine = 0;
#endif

    /*--------------------------------------------------*/

    memset(dest, '-', LEN);
    wc = L'a';
    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty retvalp") EXPECT_BOS("empty dest")
    rc = wcrtomb_s(NULL, NULL, LEN, wc, &ps);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);
    CLRPS;

    ind = 0;
    EXPECT_BOS("empty dest or dmax")
    rc = wcrtomb_s(&ind, dest, 0, wc, &ps);
    if (use_msvcrt && rc == 0 && ind == 0) {
        printf("wine: Unimplemented function msvcrt.dll.wcrtomb_s\n");
        have_wine = 1;
        return errs;
    }
    ERR_MSVC(ESZEROL, have_wine ? EINVAL : 0);
    CLRPS;

    EXPECT_BOS("empty dest")
    rc = wcrtomb_s(&ind, NULL, LEN, wc, &ps);
    ERR_MSVC(ESNULLP, EINVAL);
    CLRPS;

    EXPECT_BOS("empty ps")
    rc = wcrtomb_s(&ind, dest, LEN, wc, NULL);
    ERR_MSVC(ESNULLP, EINVAL);

    EXPECT_BOS("dest overflow")
    rc = wcrtomb_s(&ind, dest, RSIZE_MAX_STR + 1, wc, &ps);
    ERR_MSVC(ESLEMAX, 0);
    CLRPS;

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = wcrtomb_s(&ind, dest, LEN + 1, wc, &ps);
    ERR_MSVC(EOVERFLOW, 0);
    CLRPS;
#endif
#endif

    /*--------------------------------------------------*/

    rc = wcrtomb_s(&ind, dest, LEN, L'\0', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    CLRPS;

    memset(dest, ' ', LEN);
    rc = wcrtomb_s(&ind, dest, LEN, L'\a', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    if (!use_msvcrt) {
        CHECK_SLACK(&dest[1], LEN - 1);
    }
    CLRPS;

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    memset(dest, ' ', LEN);
    /* no-breaking space illegal in ASCII, but legal in C */
    rc = wcrtomb_s(&ind, dest, LEN, L'\xa0', &ps);
    if (rc == 0) { /* legal */
        ERR(EOK);
        INDCMP(!= 1);
        if ((unsigned char)dest[0] != 0xa0) {
            printf("%s %u  Error  ind=%d rc=%d %d\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, dest[0]);
            errs++;
        }
        if (!use_msvcrt) {
            CHECK_SLACK(&dest[1], LEN - 1);
            if (dest[1] != L'\0') {
                printf("%s %u  Error  ind=%d rc=%d %d\n", __FUNCTION__,
                       __LINE__, (int)ind, rc, dest[1]);
                errs++;
            }
        }
    } else {
        ERR(EILSEQ); /* or illegal */
        INDCMP(!= -1);
        CHECK_SLACK(dest, LEN);
        if (dest[0] != '\0') {
            printf("%s %u  Error  ind=%d rc=%d %d\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, dest[0]);
            errs++;
        }
    }
    CLRPS;

    rc = wcrtomb_s(&ind, dest, LEN, L'\x78', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    if (!use_msvcrt) {
        CHECK_SLACK(&dest[1], LEN - 1);
    }
    CLRPS;

    /* surrogates */
    memset(dest, ' ', LEN);
    rc = wcrtomb_s(&ind, dest, LEN, L'\xdf81', &ps);
    if (rc == 0) { /* well, musl on ASCII allows this */
        INDCMP(!= 1);
        CHECK_SLACK(&dest[1], LEN - 1);
    } else {
        ERR(EILSEQ);
        INDCMP(!= -1);
        if (!use_msvcrt) {
            CHECK_SLACK(dest, LEN);
        }
    }
    CLRPS;

    SETLOCALE_UTF8;
    SETLANG("default");
    REQLOCALE("UTF-8")

    /* overlarge utf-8 sequence */
    rc = wcrtomb_s(&ind, dest, 2, L'\x2219', &ps);
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(dest, 2);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, 3, L'\x2219', &ps);
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(dest, 3);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, 4, L'\x2219', &ps);
    ERR(EOK);
    INDCMP(!= 3);
    if (dest[0] != '\xe2') {
        printf("%s %u  Error  ind=%d rc=%d %x\n", __FUNCTION__, __LINE__,
               (int)ind, rc, dest[0]);
        errs++;
    }
    if (dest[1] != '\x88') {
        printf("%s %u  Error  ind=%d rc=%d %x\n", __FUNCTION__, __LINE__,
               (int)ind, rc, dest[1]);
        errs++;
    }
    if (dest[2] != '\x99') {
        printf("%s %u  Error  ind=%d rc=%d %x\n", __FUNCTION__, __LINE__,
               (int)ind, rc, dest[2]);
        errs++;
    }
    if (dest[3] != '\0') {
        printf("%s %u  Error  ind=%d rc=%d %x\n", __FUNCTION__, __LINE__,
               (int)ind, rc, dest[3]);
        errs++;
    }
    CLRPS;

#if 0
    /* illegal unicode. but some may allow it: 0xf0 0x9d 0x8c 0x81 */
    /* wc =  0x10000 + ((0xd834 & 0x3ff) << 10) + (0xdf01 & 0x3ff); */
    /* 32bit compilers reject the illegal wchar_t */
    /* error: overflow in implicit constant conversion [-Werror=overflow] */
#if SIZEOF_WCHAR_T > 2
    rc = wcrtomb_s(&ind, dest, LEN, 0xd834df01UL, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    CHECK_SLACK(dest, LEN);
#endif
#endif

    /*--------------------------------------------------*/

    return (errs);
}

#endif

int main(void) {
#ifdef HAVE_WCHAR_H
    return (test_wcrtomb_s());
#else
    return 0;
#endif
}
