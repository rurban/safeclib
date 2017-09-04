/*------------------------------------------------------------------
 * test_wcsrtombs_s
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

#define CLRPS \
  memset (&ps, '\0', sizeof (mbstate_t))

int test_wcsrtombs_s (void)
{
    errno_t rc;
    size_t ind;
    /*uint32_t i;*/
    const wchar_t *cs;
    const char* lang;
    mbstate_t ps;
    int errs = 0;

/*--------------------------------------------------*/

    cs = L"a";
    rc = wcsrtombs_s(NULL, NULL, LEN, &cs, 0, &ps);
    ERR(ESNULLP);
    CLRPS;

    rc = wcsrtombs_s(&ind, NULL, LEN, &cs, 0, NULL);
    ERR(ESNULLP);

    rc = wcsrtombs_s(&ind, dest, LEN, NULL, 0, &ps);
    ERR(ESNULLP);
    CLRPS;

    src[0] = L'\0';
    rc = wcsrtombs_s(&ind, NULL, LEN, (const wchar_t**)&src, 0, &ps);
    ERR(ESNULLP);
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, 0, &cs, 0, &ps);
    ERR(ESZEROL);
    CLRPS;

    /*rc = wcsrtombs_s(&ind, dest, 0, &cs, 0, &ps);
    ERR(ESNOSPC);
    CLRPS;*/

    cs = L"abcdef";
    rc = wcsrtombs_s(&ind, dest, 2, &cs, 3, &ps);
    ERR(ESNOSPC);
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, RSIZE_MAX_STR+1, &cs, 0, &ps);
    ERR(ESLEMAX);
    CLRPS;

    rc = wcsrtombs_s(&ind, (char*)&cs, LEN, &cs, 3, &ps);
    ERR(ESOVRLP);
    CLRPS;

    dest[0] = 'a';
    rc = wcsrtombs_s(&ind, dest, LEN, (const wchar_t**)&dest[0], 1, &ps);
    ERR(ESOVRLP);
    CLRPS;

/*--------------------------------------------------*/

    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"abcdef",&cs), 3, &ps);
    ERR(EOK);
    INDCMP(!= 3);
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"abcdef",&cs), 8, &ps);
    ERR(EOK);
    INDCMP(!= 6);
    CLRPS;

    rc = wcsrtombs_s(&ind, NULL, LEN, (cs=L"abcdef",&cs), 2, &ps);
    ERR(EOK);
    INDCMP(!= 6);
    CLRPS;

    setlocale(LC_CTYPE, "C");
#ifdef HAVE_LANGINFO_H
    lang = nl_langinfo(CODESET);
#else
    lang = "C";
#endif
    /* not a big problem if this fails */
    if ( !strcmp(lang, "C") ||
         !strcmp(lang, "ASCII") ||
         !strcmp(lang, "ANSI_X3.4-1968") ||
         !strcmp(lang, "US-ASCII") )
        ; /* all fine */
    else /* dont inspect the values */
        printf(__FILE__ ": cannot set C locale for test"
                   " (codeset=%s)\n", lang);

    /* no-breaking space illegal in ASCII, but legal in C */
    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"\x00a0""abc",&cs), 32, &ps);
    if (rc == 0) { /* legal */
      ERR(EOK);
      ERR(EOK);
      INDCMP(!= 4);
      if ((unsigned char)dest[0] != 0xa0) {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
      }
      if (dest[1] != 'a') {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[1]);
        errs++;
      }
      if (cs) { /* needs to be at the end */
        printf("%s %u  Error  ind=%d rc=%d %p\n",
               __FUNCTION__, __LINE__, (int)ind, rc, cs);
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
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"\x78",&cs), 1, &ps);
    ERR(EOK);
    INDCMP(!= 1);
    CLRPS;

    src[0] = 0xdf81;
    src[1] = 0;
    cs = src;
    rc = wcsrtombs_s(&ind, dest, LEN, &cs, LEN, &ps);
    if (rc == 0) { /* well, musl on ASCII allows this */
      INDCMP(!= 1);
    } else {
      ERR(EILSEQ);
      INDCMP(!= -1);
    }
    CLRPS;

    setlocale(LC_CTYPE, "en_US.UTF-8") ||
	setlocale(LC_CTYPE, "en_GB.UTF-8") ||
	setlocale(LC_CTYPE, "en.UTF-8") ||
	setlocale(LC_CTYPE, "POSIX.UTF-8") ||
	setlocale(LC_CTYPE, "C.UTF-8") ||
	setlocale(LC_CTYPE, "UTF-8") ||
	setlocale(LC_CTYPE, "");
#ifdef HAVE_LANGINFO_H
    lang = nl_langinfo(CODESET);
#else
    lang = "UTF-8";
#endif
    if (!strstr(lang, "UTF-8")) {
        printf(__FILE__ ": cannot set UTF-8 locale for test"
               " (codeset=%s)\n", lang);
        return 0;
    }

    /* illegal sequences (locale independent) */

    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"0xdf79",&cs), 1, &ps);
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
    return (test_wcsrtombs_s());
#else
    return 0;
#endif    
}
#endif
