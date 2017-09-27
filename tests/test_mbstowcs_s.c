/*------------------------------------------------------------------
 * test_mbstowcs_s
 * File 'wchar/mbstowcs_s.c'
 * Lines executed:100.00% of 32
 * locale specific, sets it to C and UTF-8
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

static wchar_t   dest[LEN];
static char      src[LEN];

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

int test_mbstowcs_s (void)
{
    errno_t rc;
    size_t ind;
    const char *cs;
    const char *lang;
    const char *lc_cat;
    int errs = 0;
#if defined(__CYGWIN__) && defined(__x86_64)
    int saveerrs;
#endif

/*--------------------------------------------------*/

    cs = "a";
    rc = mbstowcs_s(NULL, NULL, LEN, cs, 0);
    ERR(ESNULLP);

    rc = mbstowcs_s(&ind, dest, LEN, NULL, 0);
    ERR(ESNULLP);

    src[0] = '\0';
    rc = mbstowcs_s(&ind, NULL, 0, (const char*)src, 0);
#ifdef BSD_LIKE
    if (rc != 2) { /* BSD's return 2 */
        printf("%s %u wrong mbstowcs(NULL,\"\\0\"): %d\n",
                     __FUNCTION__, __LINE__, (int)rc);
    }
#else
    ERR(0);
#endif

    rc = mbstowcs_s(&ind, dest, 0, cs, 0);
    ERR(ESZEROL);

    rc = mbstowcs_s(&ind, dest, RSIZE_MAX_STR+1, cs, 0);
    ERR(ESLEMAX);

    cs = "abcdef";
    rc = mbstowcs_s(&ind, (wchar_t*)cs, LEN, cs, 3);
    ERR(ESOVRLP);

    dest[0] = L'a';
    rc = mbstowcs_s(&ind, dest, LEN, (const char*)dest, 1);
    ERR(ESOVRLP);

/*--------------------------------------------------*/

    rc = mbstowcs_s(&ind, dest, LEN, (cs="abcdef",cs), 3);
    ERR(EOK);
    INDCMP(!= 3);
    WCHECK_SLACK(&dest[3], LEN-3);

    rc = mbstowcs_s(&ind, dest, LEN, (cs="abcdef",cs), 8);
    ERR(EOK);
    INDCMP(!= 6);
    WCHECK_SLACK(&dest[6], LEN-6);

    rc = mbstowcs_s(&ind, NULL, LEN, (cs="abcdef",cs), 2);
    ERR(EOK);
    INDCMP(!= 6);

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\x7f",cs), 1);
    ERR(EOK);
    INDCMP(!= 1);

    SETLOCALE_UTF8;
    SETLANG("default");
    if (strcmp(lang, "UTF-8")) {
        printf(__FILE__ ": cannot set UTF-8 locale for test"
               " (category=%s, codeset=%s)\n", lc_cat, lang);
        return 0;
    }

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xe2\x86\x92" "abc",cs), 32);
    /* TODO: EILSEQ with cygwin64 */
#if defined(__CYGWIN__) && defined(__x86_64)
    saveerrs = errs;
#endif
    ERR(EOK);
    INDCMP(!= 4);
    WCHECK_SLACK(&dest[4], LEN-4);
    if (dest[0] != 0x2192) {
        printf("%s %u  Error  ind=%d rc=%d %ld 0x%lx\n",
               __FUNCTION__, __LINE__, (int)ind, rc, (long)dest[0], (long)dest[0]);
        errs++;
    }
    if (dest[1] != 'a') {
        printf("%s %u  Error  ind=%d rc=%d 0x%lx\n",
               __FUNCTION__, __LINE__, (int)ind, rc, (long)dest[1]);
        errs++;
    }
#if defined(__CYGWIN__) && defined(__x86_64)
    if (errs)
        printf("TODO cygwin64 EILSEQ of \"\\xe2\\x86\\x92\"\n");
    errs = saveerrs;
#endif

    /* illegal sequences (locale dependent) */
    
    /* illegal initial */
    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xc0",cs), 1);
    /* TODO and this is legal on cygwin64 */
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xc2",cs), 1);
    ERR(EILSEQ);
    INDCMP(!= -1);

    /* aliasing nul */
    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xc0\x80",cs), 2);
    ERR(EILSEQ);
    INDCMP(!= -1);

    /* aliasing slashes */
    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xc0\xaf",cs), 2);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xe0\x80\xaf",cs), 3);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xf0\x80\x80\xaf",cs), 4);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xf8\x80\x80\x80\xaf",cs), 5);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xfc\x80\x80\x80\x80\xaf",cs), 6);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* aliasing U+0080 */
    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xe0\x82\x80",cs), 3);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* aliasing U+07FF */
    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xe0\x9f\xbf",cs), 3);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* aliasing U+0800 */
    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xf0\x80\xa0\x80",cs), 4);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
      
    /* aliasing U+FFFD */
    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xf0\x8f\xbf\xbd",cs), 4);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    /* check enough space for src and conversion errors */

    rc = mbstowcs_s(&ind, dest, 6, (cs="abcdef",cs), 6);
    ERR(ESNOSPC);
    WCHECK_SLACK(dest, 6);
    
    rc = mbstowcs_s(&ind, dest, 3, (cs="\xf0\x8f\xbf\xbd",cs), 4);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\x80\xbf\x80",cs), 3);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

    rc = mbstowcs_s(&ind, dest, LEN, (cs="\xfc\x80\x80\x80\x80\x80",cs), 6);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

/*--------------------------------------------------*/
    
    return (errs);
}

#endif

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
#ifdef HAVE_WCHAR_H
    return (test_mbstowcs_s());
#else
    return 0;
#endif    
}
#endif
