/*------------------------------------------------------------------
 * test_wctomb_s
 * File 'wchar/wctomb_s.c'
 * Lines executed:94.74% of 19
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCTOMB_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX (128)
#define LEN (128)

static char dest[LEN];
static wchar_t src;
int test_wctomb_s(void);

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

int test_wctomb_s(void) {
    errno_t rc;
    int ind;
    const char *lang;
    const char *lc_cat;
    int errs = 0;

    /*--------------------------------------------------*/

    src = L'a';
    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty retvalp")
    rc = wctomb_s(NULL, dest, LEN, src);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, 0);

    EXPECT_BOS("dest overflow or empty")
    rc = wctomb_s(&ind, dest, 0, src);
    ERR_MSVC(ESZEROL, ERANGE);

    EXPECT_BOS("dest overflow or empty")
    rc = wctomb_s(&ind, dest, RSIZE_MAX_STR + 1, src);
    ERR_MSVC(ESLEMAX, 0);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow or empty")
    rc = wctomb_s(&ind, dest, LEN + 1, src);
    ERR(EOVERFLOW);
#endif
#endif

    /* check dmax zero, with !dest */
    rc = wctomb_s(&ind, NULL, 1, src);
    ERR_MSVC(ESNULLP, ERANGE);

    /*--------------------------------------------------*/

    rc = wctomb_s(&ind, dest, LEN, L'\0');
    ERR(EOK);
    INDCMP(!= 1);
    CHECK_SLACK(&dest[1], LEN - 1);

    /*--------------------------------------------------*/

    rc = wctomb_s(&ind, dest, LEN, L'\a');
    ERR(EOK);
    INDCMP(!= 1);
    CHECK_SLACK(&dest[1], LEN - 1);

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    /* no-breaking space illegal in ASCII, but legal in C */
    rc = wctomb_s(&ind, dest, LEN, L'\xa0');
    if (rc == 0) { /* legal */
        ERR(EOK);
        INDCMP(!= 1);
        if ((unsigned char)dest[0] != 0xa0) {
            printf("%s %u  Error  ind=%d rc=%d %d\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, dest[0]);
            errs++;
        }
        if (dest[1] != L'\0') {
            printf("%s %u  Error  ind=%d rc=%d %d\n", __FUNCTION__, __LINE__,
                   (int)ind, rc, dest[1]);
            errs++;
        }
        CHECK_SLACK(&dest[1], LEN - 1);
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

    rc = wctomb_s(&ind, dest, LEN, L'\x78');
    ERR(EOK);
    INDCMP(!= 1);
    CHECK_SLACK(&dest[1], LEN - 1);

    /* surrogates */
    rc = wctomb_s(&ind, dest, LEN, L'\xdf81');
    if (rc == 0) { /* well, musl on ASCII allows this */
        INDCMP(!= 1);
    } else {
        ERR(EILSEQ); /* FIXME NOSPC */
        INDCMP(!= -1);
    }

    SETLOCALE_UTF8;
    SETLANG("default");
    REQLOCALE("UTF-8");

    /* overlarge utf-8 sequence */
    rc = wctomb_s(&ind, dest, 2, L'\x2219');
    ERR_MSVC(ESNOSPC, EILSEQ);
    CHECK_SLACK(&dest[0], 2);

    rc = wctomb_s(&ind, dest, 3, L'\x2219');
    ERR_MSVC(ESNOSPC, EILSEQ);
    CHECK_SLACK(&dest[0], 3);

    rc = wctomb_s(&ind, dest, 4, L'\x2219');
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

    /* illegal unicode. but some may allow it: 0xf0 0x9d 0x8c 0x81 */
    /* 32bit: hex escape sequence out of range */
    rc = wctomb_s(&ind, dest, LEN, L'\xd834df01');
#if SIZEOF_WCHAR_T == 2
    ERR(0);
    CHECK_SLACK(&dest[ind], LEN - ind);
#else
    ERR(EILSEQ); /* FIXME NOSPC */
    INDCMP(!= -1);
    CHECK_SLACK(&dest[0], LEN);
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
    return (test_wctomb_s());
#else
    return 0;
#endif
}
#endif
