/*------------------------------------------------------------------
 * test_wctomb_s
 * File 'wchar/wctomb_s.c'
 * Lines executed:100.00% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static char      dest[LEN];
static wchar_t   src;

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif


int test_wctomb_s (void)
{
    errno_t rc;
    int ind;
    const char* lang;
    const char* lc_cat;
    int errs = 0;

/*--------------------------------------------------*/

    src = L'a';
    rc = wctomb_s(NULL, NULL, LEN, src);
    ERR(ESNULLP);

    rc = wctomb_s(&ind, dest, LEN, L'\0');
    ERR(EOK);
    INDCMP(!= 1);

    rc = wctomb_s(&ind, dest, 0, src);
    ERR(ESZEROL);

    rc = wctomb_s(&ind, dest, RSIZE_MAX_STR+1, src);
    ERR(ESLEMAX);

/*--------------------------------------------------*/

    rc = wctomb_s(&ind, dest, LEN, L'\a');
    ERR(EOK);
    INDCMP(!= 1);

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    /* no-breaking space illegal in ASCII, but legal in C */
    rc = wctomb_s(&ind, dest, LEN, L'\xa0');
    if (rc == 0) { /* legal */
      ERR(EOK);
      INDCMP(!= 1);
      if ((unsigned char)dest[0] != 0xa0) {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
      }
      if (dest[1] != L'\0') {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[1]);
        errs++;
      }
    } else {
      ERR(EILSEQ); /* or illegal */
      INDCMP(!= -1);
      if (dest[0] != '\0') {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
      }
    }

    rc = wctomb_s(&ind, dest, LEN, L'\x78');
    ERR(EOK);
    INDCMP(!= 1);

    /* surrogates */
    rc = wctomb_s(&ind, dest, LEN, L'\xdf81');
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

    /* overlarge utf-8 sequence */
    rc = wctomb_s(&ind, dest, 2, L'\x2219');
    ERR(ESNOSPC);
    rc = wctomb_s(&ind, dest, 3, L'\x2219');
    ERR(ESNOSPC);

    rc = wctomb_s(&ind, dest, 4, L'\x2219');
    ERR(EOK);
    INDCMP(!= 3);
    if (dest[0] != '\xe2') {
        printf("%s %u  Error  ind=%d rc=%d %x\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
    }
    if (dest[1] != '\x88') {
        printf("%s %u  Error  ind=%d rc=%d %x\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[1]);
        errs++;
    }
    if (dest[2] != '\x99') {
        printf("%s %u  Error  ind=%d rc=%d %x\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[2]);
        errs++;
    }
    if (dest[3] != '\0') {
        printf("%s %u  Error  ind=%d rc=%d %x\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[3]);
        errs++;
    }

    /* illegal unicode. but some may allow it: 0xf0 0x9d 0x8c 0x81 */
    /* 32bit: hex escape sequence out of range */
    rc = wctomb_s(&ind, dest, LEN, L'\xd834df01');
#if SIZEOF_WCHAR_T == 2
    ERR(0);
#else
    ERR(EILSEQ);
    INDCMP(!= -1);
#endif

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
    return (test_wctomb_s());
#else
    return 0;
#endif
}
#endif
