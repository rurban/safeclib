/*------------------------------------------------------------------
 * test_mbsrtowcs_s
 * File 'wchar/mbsrtowcs_s.c'
 * Lines executed:91.89% of 37
 * locale specific, sets it to C and UTF-8
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_MBSRTOWCS_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX (128)
#define LEN (128)

static wchar_t dest[LEN];
#ifndef HAVE_CT_BOS_OVR
static char src[LEN];
#endif

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#define CLRPS memset(&ps, '\0', sizeof(mbstate_t))
static volatile size_t v20 = 20;
static volatile size_t vrmw = RSIZE_MAX_WSTR + 1;

int test_mbsrtowcs_s(void);

int test_mbsrtowcs_s(void) {
    errno_t rc;
    size_t ind;
    /*uint32_t i;*/
    const char *cs;
    mbstate_t ps;
    const char *lang;
    const char *lc_cat;
    int errs = 0;

    /*--------------------------------------------------*/

    cs = "a";
#ifndef HAVE_CT_BOS_OVR
    print_msvcrt(use_msvcrt);
    EXPECT_BOS("empty retvalp") EXPECT_BOS("empty *srcp or len")
    rc = mbsrtowcs_s(NULL, NULL, LEN, &cs, 0, &ps);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);
    CLRPS;

    EXPECT_BOS("empty ps") EXPECT_BOS("empty *srcp or len")
    rc = mbsrtowcs_s(&ind, NULL, LEN, &cs, 0, NULL);
    ERR_MSVC(ESNULLP, EINVAL);

    EXPECT_BOS("empty srcp") EXPECT_BOS("empty *srcp or len")
    rc = mbsrtowcs_s(&ind, dest, LEN, NULL, 0, &ps);
    ERR_MSVC(ESNULLP, EINVAL);
    CLRPS;

    {
        const char **srcp = (const char **)(void *)&src;
        *src = 0;
        /*EXPECT_BOS_TODO("empty *srcp or len")*/
        rc = mbsrtowcs_s(&ind, dest, LEN, srcp, 1, &ps);
        ERR_MSVC(ESNULLP, EINVAL);
        CLRPS;
    }

    cs = "a";
    EXPECT_BOS("empty dmax")
    rc = mbsrtowcs_s(&ind, dest, 0, &cs, 3, &ps);
    ERR_MSVC(ESZEROL, EINVAL);
    CLRPS;

    EXPECT_BOS("dest overflow")
    rc = mbsrtowcs_s(&ind, dest, RSIZE_MAX_STR + 1, &cs, 3, &ps);
    ERR_MSVC(ESLEMAX, 0);
    CLRPS;

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = mbsrtowcs_s(&ind, dest, LEN + 1, &cs, 3, &ps);
    ERR(EOVERFLOW);
    CLRPS;
#endif

    {
        void *p1;
        cs = "abcdef";
        p1 = &cs;
        EXPECT_BOS_TODO("dest overlap")
        rc = mbsrtowcs_s(&ind, (wchar_t *)p1, 1, &cs, 1, &ps);
        ERR_MSVC(ESOVRLP, ERANGE);
        CLRPS;
    }
#endif

    /*--------------------------------------------------*/

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "abcdef", &cs), 3, &ps);
    ERR(EOK);
    INDCMP(!= 3);
    WCHECK_SLACK(&dest[3], LEN - 3);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "abcdef", &cs), 8, &ps);
    ERR(EOK);
    INDCMP(!= 6);
    WCHECK_SLACK(&dest[6], LEN - 6);
    CLRPS;

    rc = mbsrtowcs_s(&ind, NULL, LEN, (cs = "abcdef", &cs), 2, &ps);
    ERR(EOK);
#ifndef HAVE_MINGW32 /* ind=2 */
    INDCMP(!= 6);
#endif
    CLRPS;

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    /* no-breaking space illegal in ASCII, but legal in C */
    rc = mbsrtowcs_s(&ind, dest, LEN,
                     (cs = "\xa0"
                           "abc",
                      &cs),
                     32, &ps);
    if (rc == 0) { /* legal */
        ERR(EOK);
        INDCMP(!= 4);
        WCHECK_SLACK(&dest[4], LEN - 4);
        /* musl on ASCII converts \xa0 to \xdfa0 */
        if ((int)dest[0] != 0xa0 && (int)dest[0] != 0xdfa0) {
            printf("%s %u  Error  ind=%d rc=%d 0x%lx\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, (long)dest[0]);
            errs++;
        }
        if (dest[1] != L'a') {
            printf("%s %u  Error  ind=%d rc=%d 0x%lx\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, (long)dest[1]);
            errs++;
        }
        if (cs) { /* needs to be at the end */
            printf("%s %u  Error  ind=%d rc=%d %s\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, cs);
            errs++;
        }
    } else {
        ERR(EILSEQ); /* or illegal */
        INDCMP(!= -1);
        WCHECK_SLACK(dest, LEN);
        if (dest[0] != L'\0') {
            printf("%s %u  Error  ind=%d rc=%d 0x%lx\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, (long)dest[0]);
            errs++;
        }
    }
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\x78", &cs), 1, &ps);
    ERR(EOK);
    INDCMP(!= 1);
    WCHECK_SLACK(&dest[1], LEN - 1);
    CLRPS;

    SETLOCALE_UTF8;
    SETLANG("default");
    REQLOCALE("UTF-8")

    /* illegal sequences (locale dependent) */

    /* illegal initial */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xc0", &cs), 1, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xc2", &cs), 1, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing nul */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xc0\x80", &cs), 2, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing slashes */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xc0\xaf", &cs), 2, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xe0\x80\xaf", &cs), 3, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xf0\x80\x80\xaf", &cs), 4, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xf8\x80\x80\x80\xaf", &cs), 5,
                     &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xfc\x80\x80\x80\x80\xaf", &cs), 6,
                     &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing U+0080 */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xe0\x82\x80", &cs), 3, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing U+07FF */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xe0\x9f\xbf", &cs), 3, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing U+0800 */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xf0\x80\xa0\x80", &cs), 4, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing U+FFFD */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xf0\x8f\xbf\xbd", &cs), 4, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* check enough space for src and conversion errors */

    rc = mbsrtowcs_s(&ind, dest, 6, (cs = "abcdef", &cs), 6, &ps);
    ERR_MSVC(ESNOSPC, ERANGE);
    WCHECK_SLACK(dest, 6);
    CLRPS;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("len overflow >dmax")
    rc = mbsrtowcs_s(&ind, dest, 3, (cs = "\xf0\x8f\xbf\xbd", &cs), 4, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;
#endif

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\x80\xbf\x80", &cs), 3, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xfc\x80\x80\x80\x80\x80", &cs), 6,
                     &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* dest NULL: illegal sequence is reported via errno */
    if (!use_msvcrt) {
        rc = mbsrtowcs_s(&ind, NULL, 0, (cs = "\x80\xbf\x80", &cs), 3, &ps);
        ERR(EILSEQ);
        INDCMP(!= -1);
        CLRPS;
    }

    /* len > dmax must not write past dmax. Hide the size from the compiler. */
    if (!use_msvcrt) {
        static volatile size_t hsz = 8 + 32;
        wchar_t *hp = (wchar_t *)malloc(hsz * sizeof(wchar_t));
        size_t i;

        if (!hp) {
            return errs + 1;
        }
        CLRPS;
        for (i = 0; i < hsz; i++) {
            hp[i] = L'x';
        }
        rc = mbsrtowcs_s(&ind, hp, 8, (cs = "abcdefghijklmnopqrst", &cs), v20,
                         &ps);
        ERR(ESNOSPC);
        for (i = 8; i < 8 + 32; i++) {
            if (hp[i] != L'x') {
                debug_printf("%s %u  overflow at %u\n", __FUNCTION__, __LINE__,
                             (unsigned)i);
                errs++;
                break;
            }
        }

        CLRPS;
        for (i = 0; i < hsz; i++) {
            hp[i] = L'x';
        }
        rc = mbsrtowcs_s(&ind, hp, 8, (cs = "abc", &cs), v20, &ps);
        ERR(EOK);
        INDCMP(!= 3);
        if (hp[0] != L'a' || hp[2] != L'c' || hp[3] != L'\0') {
            errs++;
        }
        for (i = 8; i < 8 + 32; i++) {
            if (hp[i] != L'x') {
                errs++;
                break;
            }
        }

        /* unknown dest size: dmax or len above RSIZE_MAX_WSTR */
        CLRPS;
        rc = mbsrtowcs_s(&ind, hp, RSIZE_MAX_WSTR + 1, (cs = "abc", &cs), 3,
                         &ps);
        ERR(ESLEMAX);
        rc = mbsrtowcs_s(&ind, hp, 8, (cs = "abc", &cs), vrmw,
                         &ps);
        ERR(ESLEMAX);
        free(hp);
    }

    /* more illegal, truncated and overlong sequences */
    {
        static const char *const bad[] = {
            "\xe2\x86",     /* truncated 3-byte */
            "\xe0\x80\x80", /* overlong 3-byte */
            "\x80",         /* lone continuation */
            "\xfe",
            "\xff",
            "ab\xc0\x80", /* valid chars first */
            "abc\xe2\x86",
            "a\xffz"};
        size_t i;
        for (i = 0; i < sizeof(bad) / sizeof(bad[0]); i++) {
            size_t n = strlen(bad[i]);
            wmemset(dest, L'x', LEN);
            CLRPS;
            rc = mbsrtowcs_s(&ind, dest, LEN, (cs = bad[i], &cs), n, &ps);
            ERR(EILSEQ);
            INDCMP(!= -1);
            WCHECK_SLACK(dest, LEN);

            /* only counting, no dest */
            if (!use_msvcrt) {
                CLRPS;
                rc = mbsrtowcs_s(&ind, NULL, 0, (cs = bad[i], &cs), n, &ps);
                ERR(EILSEQ);
                INDCMP(!= -1);
            }
        }
    }

    /* 5-byte sequence: beyond Unicode, rejected by strict libcs, but glibc
       decodes it as legacy U+200000 */
    wmemset(dest, L'x', LEN);
    CLRPS;
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs = "\xf8\x88\x80\x80\x80", &cs), 5,
                     &ps);
    if (rc == 0) {
        INDCMP(!= 1);
    } else {
        ERR(EILSEQ);
        INDCMP(!= -1);
        WCHECK_SLACK(dest, LEN);
    }
    CLRPS;

    /* len > dmax with an illegal sequence: EILSEQ, no write past dmax */
    if (!use_msvcrt) {
        static volatile size_t hsz = 8 + 32;
        wchar_t *hp = (wchar_t *)malloc(hsz * sizeof(wchar_t));
        size_t i;

        if (!hp) {
            return errs + 1;
        }
        CLRPS;
        for (i = 0; i < hsz; i++) {
            hp[i] = L'x';
        }
        rc = mbsrtowcs_s(&ind, hp, 8,
                         (cs = "abcdefghijklmnopq\xc0\x80rst", &cs), v20, &ps);
        ERR(EILSEQ);
        INDCMP(!= -1);
        for (i = 8; i < 8 + 32; i++) {
            if (hp[i] != L'x') {
                debug_printf("%s %u  overflow at %u\n", __FUNCTION__, __LINE__,
                             (unsigned)i);
                errs++;
                break;
            }
        }

        CLRPS;
        for (i = 0; i < hsz; i++) {
            hp[i] = L'x';
        }
        rc = mbsrtowcs_s(&ind, hp, 8, (cs = "ab\xff", &cs), v20, &ps);
        ERR(EILSEQ);
        INDCMP(!= -1);
        for (i = 8; i < 8 + 32; i++) {
            if (hp[i] != L'x') {
                errs++;
                break;
            }
        }
        free(hp);
    }

    /*--------------------------------------------------*/

    return (errs);
}

#endif

int main(void) {
#ifdef HAVE_WCHAR_H
    return (test_mbsrtowcs_s());
#else
    return 0;
#endif
}
