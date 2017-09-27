/*------------------------------------------------------------------
 * test_wcrtomb_s
 * File 'wchar/wcrtomb_s.c'
 * Lines executed:100.00% of 22
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static char      dest[LEN];
static wchar_t   wc;

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#define CLRPS \
  memset (&ps, '\0', sizeof (mbstate_t))

int test_wcrtomb_s (void)
{
    errno_t rc;
    size_t ind;
    const char* lang;
    const char* lc_cat;
    mbstate_t ps;
    int errs = 0;

/*--------------------------------------------------*/

    memset(dest, '-', LEN);
    wc = L'a';
    rc = wcrtomb_s(NULL, NULL, LEN, wc, &ps);
    ERR(ESNULLP);
    CLRPS;

    rc = wcrtomb_s(&ind, NULL, LEN, wc, NULL);
    ERR(ESNULLP);

    rc = wcrtomb_s(&ind, dest, LEN, L'\0', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, 0, wc, &ps);
    ERR(ESZEROL);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, RSIZE_MAX_STR+1, wc, &ps);
    ERR(ESLEMAX);
    CLRPS;

/*--------------------------------------------------*/

    memset(dest, ' ', LEN);
    rc = wcrtomb_s(&ind, dest, LEN, L'\a', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    CHECK_SLACK(&dest[1], LEN-1);
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
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
      }
      CHECK_SLACK(&dest[1], LEN-1);
      if (dest[1] != L'\0') {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[1]);
        errs++;
      }
    } else {
      ERR(EILSEQ); /* or illegal */
      INDCMP(!= -1);
      CHECK_SLACK(dest, LEN);
      if (dest[0] != '\0') {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
      }
    }
    CLRPS;

    rc = wcrtomb_s(&ind, dest, LEN, L'\x78', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    CHECK_SLACK(&dest[1], LEN-1);
    CLRPS;

    /* surrogates */
    memset(dest, ' ', LEN);
    rc = wcrtomb_s(&ind, dest, LEN, L'\xdf81', &ps);
    if (rc == 0) { /* well, musl on ASCII allows this */
      INDCMP(!= 1);
      CHECK_SLACK(&dest[1], LEN-1);
    } else {
      ERR(EILSEQ);
      INDCMP(!= -1);
      CHECK_SLACK(dest, LEN);
    }
    CLRPS;

    SETLOCALE_UTF8;
    SETLANG("default");
    CHKLOCALE("UTF-8");

    /* overlarge utf-8 sequence */
    rc = wcrtomb_s(&ind, dest, 2, L'\x2219', &ps);
    ERR(ESNOSPC);
    CHECK_SLACK(dest, 2);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, 3, L'\x2219', &ps);
    ERR(ESNOSPC);
    CHECK_SLACK(dest, 3);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, 4, L'\x2219', &ps);
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
    CLRPS;

    /* illegal unicode. but some may allow it: 0xf0 0x9d 0x8c 0x81 */
    /* 32bit: hex escape sequence out of range */
    rc = wcrtomb_s(&ind, dest, LEN, L'\xd834df01', &ps);
    /* 32bit compilers reject the illegal wchar_t */
#if SIZEOF_WCHAR_T == 2
    ERR(0);
#else
    ERR(EILSEQ);
    INDCMP(!= -1);
    CHECK_SLACK(dest, LEN);
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
    return (test_wcrtomb_s());
#else
    return 0;
#endif    
}
#endif
