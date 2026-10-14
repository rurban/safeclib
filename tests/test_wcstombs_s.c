/*------------------------------------------------------------------
 * test_wcstombs_s
 * File 'wchar/wcstombs_s.c'
 * Lines executed:93.75% of 32
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
static volatile size_t v20 = 20;
static char *volatile vnullc = NULL;
static volatile size_t vzero = 0;
static volatile size_t vrmw = RSIZE_MAX_WSTR + 1;

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

    /* len > dmax must not write past dmax. Hide the size from the compiler. */
    if (!use_msvcrt) {
        static volatile size_t hsz = 8 + 32;
        char *hp = (char *)malloc(hsz);
        size_t i;

        if (!hp) {
            return errs + 1;
        }
        memset(hp, 'x', hsz);
        rc = wcstombs_s(&ind, hp, 8, (cs = L"abcdefghijklmnopqrst", cs), v20);
        ERR(ESNOSPC);
        for (i = 8; i < 8 + 32; i++) {
            if (hp[i] != 'x') {
                debug_printf("%s %u  overflow at %u\n", __FUNCTION__, __LINE__,
                             (unsigned)i);
                errs++;
                break;
            }
        }

        memset(hp, 'x', hsz);
        rc = wcstombs_s(&ind, hp, 8, (cs = L"abc", cs), v20);
        ERR(EOK);
        INDCMP(!= 3);
        if (strcmp(hp, "abc") != 0) {
            errs++;
        }
        for (i = 8; i < 8 + 32; i++) {
            if (hp[i] != 'x') {
                errs++;
                break;
            }
        }

        /* unknown dest size: dmax or len above RSIZE_MAX_WSTR */
        rc = wcstombs_s(&ind, hp, RSIZE_MAX_WSTR + 1, (cs = L"abc", cs), 3);
        ERR(ESLEMAX);
        rc = wcstombs_s(&ind, hp, 8, (cs = L"abc", cs), vrmw);
        ERR(ESLEMAX);
        free(hp);
    }

    /* illegal wide chars in UTF-8: surrogates always, > 0x10ffff only on
       strict libcs (glibc encodes them as 4-6 byte legacy sequences) */
#if SIZEOF_WCHAR_T > 2
    {
        static const unsigned long bad[] = {0xdf81UL, 0xd800UL, 0xdfffUL,
                                            0x110000UL, 0x7fffffffUL};
        size_t i;
        for (i = 0; i < sizeof(bad) / sizeof(bad[0]); i++) {
            /* illegal after valid chars */
            src[0] = L'a';
            src[1] = L'b';
            src[2] = (wchar_t)bad[i];
            src[3] = L'c';
            src[4] = 0;
            memset(dest, 'x', LEN);
            rc = wcstombs_s(&ind, dest, LEN, src, 4);
            if (rc != 0 || bad[i] < 0x110000UL) {
                ERR(EILSEQ);
                INDCMP(!= -1);
                CHECK_SLACK(&dest[0], LEN);
            }

            /* illegal first */
            src[0] = (wchar_t)bad[i];
            src[1] = 0;
            memset(dest, 'x', LEN);
            rc = wcstombs_s(&ind, dest, LEN, src, LEN);
            if (rc != 0 || bad[i] < 0x110000UL) {
                ERR(EILSEQ);
                INDCMP(!= -1);
                CHECK_SLACK(&dest[0], LEN);
            }

            /* count only, no dest */
            rc = wcstombs_s(&ind, vnullc, vzero, src, 1);
            if (bad[i] < 0x110000UL) {
                ERR(EILSEQ);
                INDCMP(!= -1);
            }
        }
    }
#endif

    /* len > dmax with an illegal wide char: EILSEQ, no write past dmax */
#if SIZEOF_WCHAR_T > 2
    if (!use_msvcrt) {
        static volatile size_t hsz = 8 + 32;
        char *hp = (char *)malloc(hsz);
        size_t i;

        if (!hp) {
            return errs + 1;
        }
        memset(hp, 'x', hsz);
        rc = wcstombs_s(&ind, hp, 8,
                        (cs = L"abcdefghijklmnopq\xd800rst", cs), v20);
        ERR(EILSEQ);
        INDCMP(!= -1);
        for (i = 8; i < 8 + 32; i++) {
            if (hp[i] != 'x') {
                debug_printf("%s %u  overflow at %u\n", __FUNCTION__, __LINE__,
                             (unsigned)i);
                errs++;
                break;
            }
        }
        free(hp);
    }
#endif

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
