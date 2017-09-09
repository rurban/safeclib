/*------------------------------------------------------------------
 * test_wcstombs_s
 * File 'wchar/wcstombs_s.c'
 * Lines executed:100.00% of 25
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static char      dest[LEN];
static wchar_t   src[LEN];

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

int test_wcstombs_s (void)
{
    errno_t rc;
    size_t ind;
    const wchar_t *cs;
    const char* lang;
    const char* lc_cat;
    int errs = 0;

/*--------------------------------------------------*/

    cs = L"a";
    rc = wcstombs_s(NULL, NULL, LEN, cs, 0);
    ERR(ESNULLP);

    rc = wcstombs_s(&ind, dest, LEN, NULL, 0);
    ERR(ESNULLP);

    rc = wcstombs_s(&ind, dest, 0, cs, 0);
    ERR(ESZEROL);

    rc = wcstombs_s(&ind, NULL, 0, cs, 0);
    ERR(ESNOSPC);

    rc = wcstombs_s(&ind, dest, RSIZE_MAX_STR+1, cs, 0);
    ERR(ESLEMAX);

    cs = L"abcdef";
    rc = wcstombs_s(&ind, (char*)cs, LEN, cs, 3);
    ERR(ESOVRLP);

    dest[0] = 'a';
    rc = wcstombs_s(&ind, dest, LEN, (const wchar_t*)dest, 1);
    ERR(ESOVRLP);

/*--------------------------------------------------*/

    rc = wcstombs_s(&ind, dest, LEN, (cs=L"abcdef",cs), 3);
    ERR(EOK);
    INDCMP(!= 3);

    rc = wcstombs_s(&ind, dest, LEN, (cs=L"abcdef",cs), 8);
    ERR(EOK);
    INDCMP(!= 6);

    rc = wcstombs_s(&ind, NULL, LEN, (cs=L"abcdef",cs), 2);
    ERR(EOK);
    INDCMP(!= 6);

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    rc = wcstombs_s(&ind, dest, LEN, (cs=L"\x78",cs), 1);
    ERR(EOK);
    INDCMP(!= 1);

    src[0] = 0xdf81;
    src[1] = 0;
    cs = src;
    rc = wcstombs_s(&ind, dest, LEN, cs, LEN);
    if (rc == 0) { /* well, musl on ASCII allows this */
      INDCMP(!= 1);
    } else {
      ERR(EILSEQ);
      INDCMP(!= -1);
    }

    SETLOCALE_UTF8;
    SETLANG("UTF-8");
    if (!strstr(lang, "UTF-8")) {
        printf(__FILE__ ": cannot set UTF-8 locale for test"
               " (codeset=%s)\n", lang);
        return 0;
    }

    /* illegal sequences (locale independent) */

    rc = wcstombs_s(&ind, dest, LEN, (cs=L"0xdf79",cs), 1);
    ERR(EOK);
    INDCMP(!= 1);

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
    return (test_wcstombs_s());
#else
    return 0;
#endif    
}
#endif
