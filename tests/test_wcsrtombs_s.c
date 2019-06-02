/*------------------------------------------------------------------
 * test_wcsrtombs_s
 * File 'wchar/wcsrtombs_s.c'
 * Lines executed:90.91% of 33
 *
 * wine tested with wine-2.0.4
 * msvcrt.dll 7.0.9200.16384 (win8 64bit default)
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#if defined(TEST_MSVCRT) && defined(HAVE_WCSRTOMBS_S)
#undef HAVE_CT_BOS_OVR
#endif

#ifdef HAVE_WCSRTOMBS_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX (128)
#define LEN (128)

static char dest[LEN];
static wchar_t src[LEN];
int test_wcsrtombs_s(void);

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#define CLRPS memset(&ps, '\0', sizeof(mbstate_t))

int test_wcsrtombs_s(void) {
    errno_t rc;
    size_t ind;
    /*uint32_t i;*/
    const wchar_t *cs;
    const char *lang;
    const char *lc_cat;
    mbstate_t ps;
    int errs = 0;
    int have_wine = 0;

    /*--------------------------------------------------*/
    cs = L"a";
#if defined(TEST_MSVCRT) && defined(HAVE_STRNCPY_S)
    use_msvcrt = true;
#endif
    print_msvcrt(use_msvcrt);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty retvalp")
    rc = wcsrtombs_s(NULL, dest, LEN, &cs, 1, &ps);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, 0);
    CLRPS;

    EXPECT_BOS("empty srcp")
    rc = wcsrtombs_s(&ind, dest, LEN, NULL, 1, &ps);
    ERR_MSVC(ESNULLP, EINVAL);
    CHECK_SLACK(dest, LEN);
    CLRPS;

    EXPECT_BOS("empty ps")
    rc = wcsrtombs_s(&ind, dest, LEN, &cs, 1, NULL);
    ERR_MSVC(ESNULLP, 0);
    CHECK_SLACK(dest, LEN);
    CLRPS;

    ind = 0;
    EXPECT_BOS("empty dmax")
    rc = wcsrtombs_s(&ind, dest, 0, &cs, 1, &ps);
    if (use_msvcrt && rc == 0 && ind == 0) { /* under wine it returns 0 */
        printf("Using wine\n");
        have_wine = 1;
    }
    ERR_MSVC(ESZEROL, have_wine ? 0 : EINVAL);
    CLRPS;

    {
        const wchar_t **srcp = (const wchar_t **)(void *)&src;
        src[0] = L'\0';
        EXPECT_BOS("empty *srcp or len")
        rc = wcsrtombs_s(&ind, dest, LEN, srcp, 0, &ps);
        ERR_MSVC(ESNULLP, have_wine ? EINVAL : 0);
        CHECK_SLACK(dest, LEN);
        CLRPS;
    }

    /*
    EXPECT_BOS("empty buf or bufsize")
    rc = wcsrtombs_s(&ind, dest, 0, &cs, 0, &ps);
    ERR_MSVC(ESNOSPC,ERANGE);
    CLRPS;*/

    cs = L"a";
#ifndef HAVE_ASAN
    if (_BOS_KNOWN(dest) && !use_msvcrt) {
        EXPECT_BOS("dest overflow")
        rc = wcsrtombs_s(&ind, dest, LEN + 1, &cs, 3, &ps);
        if (rc) /* TODO overflow */
            ERR(EOVERFLOW);
        CLRPS;
    }
#endif
    cs = L"abcdefghijklmno"; /* must be >= 4*3 */
    if (!use_msvcrt) {       /* segfaults */
        EXPECT_BOS("dest overflow")
        rc = wcsrtombs_s(&ind, dest, RSIZE_MAX_STR + 1, &cs, 3, &ps);
        if (!have_wine)
            ERR_MSVC(ESLEMAX, 0); /* under wine it returns 42, EILSEQ */
        CLRPS;
    }

    if (!use_msvcrt) { /* segfaults */
        GCC_PUSH_WARN_RESTRICT
        EXPECT_BOS("dest overlap")
        rc = wcsrtombs_s(&ind, (char *)&cs, 3, &cs, 3, &ps);
        GCC_POP_WARN_RESTRICT
        ERR_MSVC(ESOVRLP, have_wine ? EILSEQ : 0);
        CLRPS;
    }

    cs = L"abcdef";
    EXPECT_BOS("len overflow >dmax")
    rc = wcsrtombs_s(&ind, dest, 2, &cs, 3, &ps);
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(dest, 2);
    CLRPS;
#endif

    /*--------------------------------------------------*/
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, LEN, (cs = L"abcdef", &cs), 3, &ps);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 3);
    } else {
        INDCMP(!= 4); /* !!! */
    }
    CHECK_SLACK(&dest[3], LEN - 3);
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, LEN, (cs = L"abcdef", &cs), 8, &ps);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 6);
    } else {
        INDCMP(!= 7);
    }
    CHECK_SLACK(&dest[6], LEN - 6);
    CLRPS;

    rc = wcsrtombs_s(&ind, NULL, LEN, (cs = L"abcdef", &cs), 2, &ps);
    ERR_MSVC(EOK, EINVAL);
    if (!use_msvcrt) {
#ifdef _WIN32
        /* older msvcrt's do 2 */
        /* "On the msvcrt with a NULL dest pointer, the retvalp length is
           limited by the srcp len. In other libc's len is ignored" */
        INDCMP(!= 2 && (int)ind != 6);
#else
        INDCMP(!= 6);
#endif
    } else if (have_wine) {
        INDCMP(!= 0);
    } else {
        INDCMP(!= -1);
    }
    CLRPS;

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    /* no-breaking space illegal in ASCII, but legal in C */
    cs = L"\x00a0abc";
    rc = wcsrtombs_s(&ind, dest, LEN, &cs, 32, &ps);
    if (rc == 0) { /* legal */
        ERR(EOK);
        if (!use_msvcrt) {
            INDCMP(!= 4);
        } else {
            INDCMP(!= 5);
        }
        if ((unsigned char)dest[0] != 0xa0) {
            printf("%s %u  Error  ind=%d rc=%d %d\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, dest[0]);
            errs++;
        }
        if (dest[1] != 'a') {
            printf("%s %u  Error  ind=%d rc=%d %d\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, dest[1]);
            errs++;
        }
        CHECK_SLACK(&dest[4], LEN - 4);
        if (cs && !have_wine) { /* needs to be at the end */
            printf("%s %u  Error  ind=%d rc=%d %p\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, (void *)cs);
            errs++;
        }
    } else {
        ERR(EILSEQ); /* or illegal */
        INDCMP(!= -1);
        if (dest[0] != '\0') {
            printf("%s %u  Error  ind=%d rc=%d %d\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, dest[0]);
            errs++;
        }
        CHECK_SLACK(&dest[0], LEN);
    }
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, LEN, (cs = L"\x78", &cs), 1, &ps);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 1);
    } else {
        INDCMP(!= 2);
    }
    CLRPS;

    src[0] = 0xdf81;
    src[1] = 0;
    cs = src;
    rc = wcsrtombs_s(&ind, dest, LEN, &cs, LEN, &ps);
    if (rc == 0) { /* well, musl on ASCII allows this */
        INDCMP(!= 1);
        CHECK_SLACK(&dest[1], LEN - 1);
    } else {
        ERR(EILSEQ);
        if (!have_wine) {
            INDCMP(!= -1);
        } else {
            INDCMP(!= 2);
        }
        CHECK_SLACK(&dest[0], LEN);
    }
    CLRPS;

    SETLOCALE_UTF8;
    SETLANG("default");
    REQLOCALE("UTF-8")

    /* illegal sequences (locale independent) */

    rc = wcsrtombs_s(&ind, dest, LEN, (cs = L"0xdf79", &cs), 1, &ps);
    ERR(EOK);
    INDCMP(!= 1);

    /*--------------------------------------------------*/

    return (errs);
}

#endif

int main(void) {
#ifdef HAVE_WCHAR_H
    return (test_wcsrtombs_s());
#else
    return 0;
#endif
}
