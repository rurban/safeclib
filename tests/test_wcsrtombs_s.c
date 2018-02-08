/*------------------------------------------------------------------
 * test_wcsrtombs_s
 * File 'wchar/wcsrtombs_s.c'
 * Lines executed:100.00% of 32
 *
 * wine tested with wine-2.0.4
 * msvcrt.dll 7.0.9200.16384 (win8 64bit default)
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCSRTOMBS_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

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
    const char* lc_cat;
    mbstate_t ps;
    int errs = 0;
    int have_wine = 0;

/*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

    cs = L"a";
    rc = wcsrtombs_s(NULL, NULL, LEN, &cs, 0, &ps);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);
    CLRPS;

    rc = wcsrtombs_s(&ind, NULL, LEN, &cs, 0, NULL);
    ERR_MSVC(ESNULLP, EINVAL);

    rc = wcsrtombs_s(&ind, dest, LEN, NULL, 0, &ps);
    ERR_MSVC(ESNULLP, EINVAL);
    CHECK_SLACK(dest, LEN);
    CLRPS;

    ind = 0;
    rc = wcsrtombs_s(&ind, dest, 0, &cs, 0, &ps);
    if (use_msvcrt && rc == 0 && ind == 0) { /* under wine it returns 0 */
        printf("Using wine\n");
        have_wine = 1;
    }
    ERR_MSVC(ESZEROL, have_wine?0:EINVAL);
    CLRPS;

    src[0] = L'\0';
    rc = wcsrtombs_s(&ind, dest, LEN, (const wchar_t**)&src, 0, &ps);
    ERR_MSVC(ESNULLP, have_wine?EINVAL:0);
    CHECK_SLACK(dest, LEN);
    CLRPS;

    /*rc = wcsrtombs_s(&ind, dest, 0, &cs, 0, &ps);
    ERR_MSVC(ESNOSPC,ERANGE);
    CLRPS;*/

    cs = L"abcdef";
    rc = wcsrtombs_s(&ind, dest, 2, &cs, 3, &ps);
    ERR_MSVC(ESNOSPC,ERANGE);
    CHECK_SLACK(dest, 2);
    CLRPS;

#ifndef HAVE_CT_BOS_OVR
    rc = wcsrtombs_s(&ind, dest, RSIZE_MAX_STR+1, &cs, 3, &ps);
    if (!have_wine)
        ERR_MSVC(ESLEMAX, 0); /* under wine it returns 42, EILSEQ */
    CLRPS;

    rc = wcsrtombs_s(&ind, (char*)&cs, LEN, &cs, 3, &ps);
    ERR_MSVC(ESOVRLP, have_wine?EILSEQ:0);
    CLRPS;
#endif

    dest[0] = 'a';
    if (!use_msvcrt) { /* crashes with msvcrt in wctob/wcsrtombs */
        rc = wcsrtombs_s(&ind, dest, LEN, (const wchar_t**)&dest[0], 1, &ps);
        ERR(ESOVRLP);
        CLRPS;
    }

/*--------------------------------------------------*/

    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"abcdef",&cs), 3, &ps);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 3);
    } else {
        INDCMP(!= 4); /* !!! */
    }
    CHECK_SLACK(&dest[3], LEN-3);
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"abcdef",&cs), 8, &ps);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 6);
    } else {
        INDCMP(!= 7);
    }
    CHECK_SLACK(&dest[6], LEN-6);
    CLRPS;

    rc = wcsrtombs_s(&ind, NULL, LEN, (cs=L"abcdef",&cs), 2, &ps);
    ERR_MSVC(EOK, EINVAL);
    if (!use_msvcrt) {
        INDCMP(!= 6);
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
    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"\x00a0""abc",&cs), 32, &ps);
    if (rc == 0) { /* legal */
      ERR(EOK);
      if (!use_msvcrt) {
        INDCMP(!= 4);
      } else {
        INDCMP(!= 5);
      }
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
      CHECK_SLACK(&dest[4], LEN-4);
      if (cs && !have_wine) { /* needs to be at the end */
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
      CHECK_SLACK(&dest[0], LEN);
    }
    CLRPS;

    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"\x78",&cs), 1, &ps);
    ERR(EOK);
    if (!use_msvcrt) {
        INDCMP(!= 1);
    } else {
        INDCMP(!= 2);
    }
    CLRPS;

#ifndef HAVE_CT_BOS_OVR
    src[0] = 0xdf81;
    src[1] = 0;
    cs = src;
    rc = wcsrtombs_s(&ind, dest, LEN, &cs, LEN, &ps);
    if (rc == 0) { /* well, musl on ASCII allows this */
      INDCMP(!= 1);
      CHECK_SLACK(&dest[1], LEN-1);
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
#endif

    SETLOCALE_UTF8;
    SETLANG("default");
    REQLOCALE("UTF-8")

    /* illegal sequences (locale independent) */

    rc = wcsrtombs_s(&ind, dest, LEN, (cs=L"0xdf79",&cs), 1, &ps);
    ERR(EOK);
    INDCMP(!= 1);

/*--------------------------------------------------*/

    return (errs);
}

#endif

int main (void)
{
#ifdef HAVE_WCHAR_H
    return (test_wcsrtombs_s());
#else
    return 0;
#endif
}
