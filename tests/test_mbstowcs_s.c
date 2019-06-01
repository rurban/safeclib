/*------------------------------------------------------------------
 * test_mbstowcs_s
 * File 'wchar/mbstowcs_s.c'
 * Lines executed:91.43% of 35
 * locale specific, sets it to C and UTF-8
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_MBSTOWCS_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static wchar_t dest[LEN];
static char src[LEN];

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

int test_mbstowcs_s(void);

int test_mbstowcs_s(void) {
    errno_t rc;
    size_t ind;
    const char *cs;
    const char *lang;
    const char *lc_cat;
    int errs = 0;

    /*--------------------------------------------------*/

    cs = "a";
    strcpy(src, "abc");
    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty retvalp") EXPECT_BOS("empty src or len")
    rc = mbstowcs_s(NULL, NULL, LEN, cs, 0);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    EXPECT_BOS("empty src") EXPECT_BOS("empty src or len")
    rc = mbstowcs_s(&ind, dest, LEN, NULL, 0);
    ERR_MSVC(ESNULLP, EINVAL);

    src[0] = '\0';
    EXPECT_BOS("empty src or len")
    rc = mbstowcs_s(&ind, NULL, 0, cs, 0);
#ifdef BSD_LIKE
    if (rc != 2) { /* BSD's return 2 */
        printf("%s %u wrong mbstowcs(NULL,\"\\0\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    }
#else
    ERR(0);
#endif

    EXPECT_BOS("empty dmax")
    rc = mbstowcs_s(&ind, dest, 0, cs, 3);
    ERR_MSVC(ESZEROL, EINVAL);

    EXPECT_BOS("dest overflow")
    rc = mbstowcs_s(&ind, dest, RSIZE_MAX_STR + 1, cs, 3);
    ERR_MSVC(ESLEMAX, 0);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = mbstowcs_s(&ind, dest, LEN + 1, cs, 3);
    ERR(EOVERFLOW);
#endif

    dest[0] = L'a';
    GCC_PUSH_WARN_RESTRICT
    EXPECT_BOS("dest overlap")
    rc = mbstowcs_s(&ind, dest, LEN, (const char *)dest, 1);
    GCC_POP_WARN_RESTRICT
    ERR_MSVC(ESOVRLP, ERANGE);

    {
        void *p1;
        cs = "abcdef";
        GCC_PUSH_WARN_RESTRICT
        EXPECT_BOS("dest overlap")
        rc = mbstowcs_s(&ind, (wchar_t *)&p1, 1, (const char *)&p1, 1);
        GCC_POP_WARN_RESTRICT
        ERR_MSVC(ESOVRLP, ERANGE);
    }
#endif

    /*--------------------------------------------------*/

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "abcdef", cs), 3);
    ERR(EOK);
    INDCMP(!= 3);
    WCHECK_SLACK(&dest[3], LEN - 3);

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "abcdef", cs), 8);
    ERR(EOK);
    INDCMP(!= 6);
    WCHECK_SLACK(&dest[6], LEN - 6);

    rc = mbstowcs_s(&ind, NULL, LEN, (cs = "abcdef", cs), 2);
    ERR(EOK);
    INDCMP(!= 6);

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\x7f", cs), 1);
    ERR(EOK);
    INDCMP(!= 1);

    SETLOCALE_UTF8;
    SETLANG("default");
    REQLOCALE("UTF-8")

    rc = mbstowcs_s(&ind, dest, LEN,
                    (cs = "\xe2\x86\x92"
                          "abc",
                     cs),
                    32);
    {
        /* TODO: EILSEQ with cygwin64 */
#if defined(HAVE_CYGWIN64)
        int saveerrs = errs;
        const char *todo = "Todo";
#else
        const char *todo = "Error";
#endif
        if (rc != EOK) {
            debug_printf("%s %u  %s rc=%d ind=%d\n", __FUNCTION__, __LINE__,
                         todo, (int)rc, (int)ind);
            errs++;
        }
        if (ind != 4) {
            printf("%s %u  %s  ind=%d rc=%d \n", __FUNCTION__, __LINE__, todo,
                   (int)ind, rc);
            errs++;
        }
        /* WCHECK_SLACK(&dest[4], LEN-4); */
        if (dest[0] != 0x2192) {
            printf("%s %u  %s  ind=%d rc=%d %ld 0x%lx\n", __FUNCTION__,
                   __LINE__, todo, (int)ind, rc, (long)dest[0], (long)dest[0]);
            errs++;
        }
        if (dest[1] != 'a') {
            printf("%s %u  %s  ind=%d rc=%d 0x%lx\n", __FUNCTION__, __LINE__,
                   todo, (int)ind, rc, (long)dest[1]);
            errs++;
        }
#if defined(HAVE_CYGWIN64)
        if (errs)
            printf("TODO cygwin64 EILSEQ of \"\\xe2\\x86\\x92\"\n");
        errs = saveerrs;
#endif
    }

    /* illegal sequences (locale dependent) */
#ifdef HAVE_CYGWIN64
    return (errs);
#endif

    /* illegal initial */
    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xc0", cs), 1);
    /* TODO and this is legal on cygwin64 */
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xc2", cs), 1);
    ERR(EILSEQ);
    INDCMP(!= -1);

    /* aliasing nul */
    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xc0\x80", cs), 2);
    ERR(EILSEQ);
    INDCMP(!= -1);

    /* aliasing slashes */
    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xc0\xaf", cs), 2);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xe0\x80\xaf", cs), 3);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xf0\x80\x80\xaf", cs), 4);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xf8\x80\x80\x80\xaf", cs), 5);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xfc\x80\x80\x80\x80\xaf", cs), 6);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* aliasing U+0080 */
    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xe0\x82\x80", cs), 3);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* aliasing U+07FF */
    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xe0\x9f\xbf", cs), 3);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* aliasing U+0800 */
    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xf0\x80\xa0\x80", cs), 4);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* aliasing U+FFFD */
    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xf0\x8f\xbf\xbd", cs), 4);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* check enough space for src and conversion errors */

    rc = mbstowcs_s(&ind, dest, 6, (cs = "abcdef", cs), 6);
    ERR_MSVC(ESNOSPC, ERANGE);
    WCHECK_SLACK(dest, 6);

    rc = mbstowcs_s(&ind, dest, 3, (cs = "\xf0\x8f\xbf\xbd", cs), 4);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\x80\xbf\x80", cs), 3);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs = "\xfc\x80\x80\x80\x80\x80", cs), 6);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /*--------------------------------------------------*/

    return (errs);
}

#endif

int main(void) {
#ifdef HAVE_WCHAR_H
    return (test_mbstowcs_s());
#else
    return 0;
#endif
}
