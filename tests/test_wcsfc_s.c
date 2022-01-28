/*------------------------------------------------------------------
 * test_wcsfc_s
 * File 'wcsfc_s.c'
 * Lines executed:94.12% of 136
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <locale.h>

#define LEN (128)
int test_wcsfc_s(void);

#define PERL_TEST
/* Must have the same Unicode version 9.0, at least 5.26.
   Better 5.27.3 with Unicode 10, 5.30 with 12.1, 5.32, 5.36 with 14.0
   perl -MUnicode::UCD -e'print Unicode::UCD::UnicodeVersion()'
*/
#ifndef PERL
/*# define PERL "perl"*/
/*# define PERL "cperl5.30.0"*/
#define PERL "perl5.36.0"
#endif
#define TESTPL "test-fc.pl"

int test_wcsfc_s(void) {
    errno_t rc;
    wchar_t str[LEN];
    wchar_t str1[LEN];
    size_t len;
    rsize_t ind;
    int wc;
    int errs = 0;
    char *lc_cat, *lang;
#ifdef PERL_TEST
    FILE *pl;
    struct stat st;

    pl = fopen(TESTPL, "w");
#endif

    /*--------------------------------------------------*/

    wcscpy(str, L"untouched");
#ifndef HAVE_CT_BOS_OVR
    ind = 4;
    EXPECT_BOS("empty dest")
    rc = wcsfc_s(NULL, LEN, L"test", &ind);
    ERR(ESNULLP);
    INDCMP(!= 0)

    ind = 4;
    EXPECT_BOS("empty dest or dmax")
    rc = wcsfc_s(str, 0, L"test", &ind);
    ERR(ESZEROL)
    WEXPSTR(str, L"untouched")
    INDCMP(!= 0)

    ind = 4;
    EXPECT_BOS("dest overflow")
    rc = wcsfc_s(str, RSIZE_MAX_WSTR + 1, L"test", &ind);
    ERR(ESLEMAX)
    WEXPSTR(str, L"untouched")
    INDCMP(!= 0)

    if (_BOS_KNOWN(str)) {
        EXPECT_BOS("dest overflow")
        rc = wcsfc_s(str, LEN + 1, L"test", &ind);
        ERR(EOVERFLOW)
        WEXPSTR(str, L"") /* cleared */
    }

    ind = 4;
    EXPECT_BOS("empty src")
    rc = wcsfc_s(str, LEN, NULL, &ind);
    ERR(ESNULLP);
    INDCMP(!= 0)
    WEXPSTR(str, L"") /* cleared */

#endif

    /*--------------------------------------------------*/

    rc = wcsfc_s(str, LEN, (wchar_t *)L"Name", &ind);
    ERR(EOK);
    WEXPSTR(str, L"name");
    INDCMP(!= 4);
    WCHECK_SLACK(&str[4], LEN - 4);

    /*--------------------------------------------------*/

    rc = wcsfc_s(str, LEN, (wchar_t *)L"name", &ind);
    ERR(EOK)
    WEXPSTR(str, L"name");
    INDCMP(!= 4);
    WCHECK_SLACK(&str[4], LEN - 4);

    /*--------------------------------------------------*/

    rc = wcsfc_s(str, LEN, (wchar_t *)L"NOWISTHETIM3", NULL);
    ERR(EOK)
    WEXPSTR(str, L"nowisthetim3");
    len = wcslen(L"nowisthetim3");
    WCHECK_SLACK(&str[len], LEN - len);

    /*--------------------------------------------------*/

    /* towfc_s() */
    rc = wcsfc_s(str, LEN,
                 (wchar_t *)L"A"
                            L"\x1fb3",
                 &ind); /* casefold ᾳ => 2 */
    ERR(EOK);
    wcscpy(str1, L"a"
                 L"\x3b1"
                 L"\x3b9"); /* but decomp: 1fb3 => 3b1 345 */
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[3], LEN - 3);

    rc = wcsfc_s(str, LEN,
                 (wchar_t *)L"A"
                            L"\x1fb7",
                 &ind); /* casefold ᾷ => 3 */
    ERR(EOK);
    wcscpy(str1, L"a"
                 L"\x3b1"
                 L"\x342"
                 L"\x3b9");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[4], LEN - 4);

    rc =
        wcsfc_s(str, LEN, (wchar_t *)L"\x100", &ind); /* fc=>101, nfd=>61 304 */
    ERR(EOK);
    wcscpy(str1, L"\x61"
                 L"\x304");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, (wchar_t *)L"\x101", &ind); /* fc=101, nfd only */
    ERR(EOK);
    wcscpy(str1, L"\x61"
                 L"\x304"); /* A w/ MACRON */
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, (wchar_t *)L"\x115", &ind); /* fc=115, nfd=65 306 */
    ERR(EOK);
    wcscpy(str1, L"\x65"
                 L"\x306");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, (wchar_t *)L"\xdf", &ind); /* fc=73 73, !nfd */
    ERR(EOK);
    wcscpy(str1, L"\x73"
                 L"\x73");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, (wchar_t *)L"\x385", &ind); /* fc=385, nfd=a8 301 */
    ERR(EOK);
    wcscpy(str1, L"\xa8"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, (wchar_t *)L"\x386", &ind); /* fc=3ac, nfd=3b1 301 */
    ERR(EOK);
    wcscpy(str1, L"\x3b1"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, (wchar_t *)L"\x388",
                 &ind); /* c=0, fc=3ad, nfd=3b5 301 */
    ERR(EOK);
    wcscpy(str1, L"\x3b5"
                 L"\x301"); /* 3ad: 249|TBL(2) */
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, (wchar_t *)L"\x1f71", &ind); /* !fc, nfd=3b1 301 */
    ERR(EOK);
    wcscpy(str1, L"\x3b1"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc =
        wcsfc_s(str, LEN, (wchar_t *)L"\x1f82", &ind); /*  fc=1f02 389 => l=4 */
    ERR(EOK);
    wcscpy(str1, L"\x3b1"
                 L"\x313"
                 L"\x300"
                 L"\x3b9");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[4], LEN - 4);

    rc = wcsfc_s(str, LEN, L"\x1feb", &ind); /*  fc,=17eb nfd=3c5 301 */
    ERR(EOK);
    wcscpy(str1, L"\x3c5"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, L"\x1fee", &ind); /*  !fc, nfd=a8 301 */
    ERR(EOK);
    wcscpy(str1, L"\xa8"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    /*--------------------------------------------------*/

    /* reorder? Aᾷ  fc A=>a 345=>3b9! See [GH #25] */
    /*
      FC        \x{41}\x{3B1}\x{345}\x{342} = \x{61}\x{3B1}\x{3B9}\x{342}
      NFD FC                           Aᾷ  = \x{61}\x{3B1}\x{3B9}\x{342}
      NFD       \x{41}\x{3B1}\x{345}\x{342} = \x{41}\x{3B1}\x{342}\x{345}
      FC NFD                           Aᾷ  = \x{61}\x{3B1}\x{342}\x{3B9} */

    rc = wcsfc_s(str, LEN,
                 L"A"
                 L"\x3b1"
                 L"\x345"
                 L"\x342",
                 &ind);
    ERR(EOK);
    wcscpy(str1, L"a"
                 L"\x3b1"
                 L"\x3b9"
                 L"\x342"); /* no reordering of 3b9 */
    WEXPSTR(str, str1);

    /*rc = wcsnorm_reorder_s(str, LEN, L"a" L"\x3b1" L"\x3b9" L"\x342", 4);
    ERR(EOK);
    wcscpy(str1, L"a" L"\x3b1" L"\x3b9" L"\x342");
    WEXPSTR(str, str1);*/

    rc = wcsfc_s(str, LEN, L"\x1d5", &ind); /* NFD=U\x308\x304 */
    ERR(EOK);
    wcscpy(str1, L"u"
                 L"\x308"
                 L"\x304"); /* no reordering */
    WEXPSTR(str, str1);

    /* no ordering defined, both CC 230 */
    /*rc = wcsnorm_reorder_s(str, LEN, L"U" L"\x304" L"\x308", 3);
    wcscpy(str1, L"U" L"\x304" L"\x308");
    WEXPSTR(str, str1);*/

    /*--------------------------------------------------*/

    /* wcsfc_s() locale-independent special rules */
    rc = wcsfc_s(str, LEN,
                 L"\x3a3"
                 L" ",
                 &ind); /* final sigma */
    ERR(EOK);
    wcscpy(str1, L"\x3c2"
                 L" ");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN,
                 L"\x3a3"
                 L"s",
                 &ind); /* non-final sigma */
    ERR(EOK);
    wcscpy(str1, L"\x3c3"
                 L"s");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    /*--------------------------------------------------*/

    /* wcsfc_s() locale-dependent special rules */
    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    rc = wcsfc_s(str, LEN, L"\x130", &ind); /* I-dot İ */
    ERR(EOK);
    WEXPSTR(str, L"i\x307"); /* COMBINING DOT ABOVE */
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, L"I\x307", &ind);
    ERR(EOK);
    WEXPSTR(str, L"i\x307");
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, L"I", &ind);
    ERR(EOK);
    WEXPSTR(str, L"i");
    WCHECK_SLACK(&str[1], LEN - 1);

    rc = wcsfc_s(str, LEN, L"\xc0",
                 &ind); /* accented a. fc => \xe0, nfd => a\x300 */
    ERR(EOK);
    WEXPSTR(str, L"a\x300");
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsfc_s(str, LEN, L"\xcc",
                 &ind); /* accented I/J fc => \xec, nfd => i\x300 */
    ERR(EOK);
    WEXPSTR(str, L"i\x300");
    WCHECK_SLACK(&str[2], LEN - 2);

    SETLOCALE("tr_TR");
    SETLANG("tr");
    CHKLOCALE("tr") {
        /* pair I-Dot with i for Turkish and Azeri. */
        rc = wcsfc_s(str, LEN, L"\x130", &ind); /* I-dot İ */
        ERR(EOK);
        WEXPSTR(str, L"i"); /* i alone */
        WCHECK_SLACK(&str[1], LEN - 1);

        /* Remove dot_above in the sequence
           I + dot_above, which will turn into i */
        rc = wcsfc_s(str, LEN, L"I\x307", &ind); /* skip the I-dot */
        ERR(EOK);
        WEXPSTR(str, L"i"); /* ditto */
        WCHECK_SLACK(&str[1], LEN - 1);

        rc = wcsfc_s(str, LEN, L"A\x307", &ind); /* Not with A */
        ERR(EOK);
        wcscpy(str1, L"a"
                     L"\x307");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[2], LEN - 2);

        /* Unless an I is before a dot_above, it turns
           into a dotless i. */
        rc = wcsfc_s(str, LEN, L"I", &ind);
        ERR(EOK);
        WEXPSTR(str, L"\x131"); /* dotless ı */
        WCHECK_SLACK(&str[1], LEN - 1);

        rc = wcsfc_s(str, LEN,
                     L"I\x307"
                     L"I",
                     &ind);
        ERR(EOK);
        wcscpy(str1, L"i"
                     L"\x131");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[2], LEN - 2);
    }

    SETLOCALE("lt_LT"); /* leaks 91k on darwin */
    SETLANG("lt");
    CHKLOCALE("lt") {
        rc = wcsfc_s(str, LEN, L"\xcc", &ind); /* accented I/J */
        ERR(EOK);
        wcscpy(str1, L"\x69"
                     L"\x307"
                     L"\x300");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[3], LEN - 3);

        rc = wcsfc_s(str, LEN, L"\xcd", &ind); /* accented I/J */
        ERR(EOK);
        wcscpy(str1, L"\x69"
                     L"\x307"
                     L"\x301");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[3], LEN - 3);

        rc = wcsfc_s(str, LEN, L"\x128", &ind); /* accented I/J */
        ERR(EOK);
        wcscpy(str1, L"\x69"
                     L"\x307"
                     L"\x303");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[3], LEN - 3);

        rc = wcsfc_s(str, LEN,
                     L"\x49"
                     L"\x300",
                     &ind); /* accented I/J */
        ERR(EOK);
        wcscpy(str1, L"\x69"
                     L"\x307");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[2], LEN - 2);

        rc = wcsfc_s(str, LEN,
                     L"\x4A"
                     L"\x300",
                     &ind); /* accented I/J */
        ERR(EOK);
        wcscpy(str1, L"\x6a"
                     L"\x307");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[2], LEN - 2);

        rc = wcsfc_s(str, LEN,
                     L"\x49"
                     L"\x302",
                     &ind); /* not accented I/J */
        ERR(EOK);
        wcscpy(str1, L"\x69"
                     L"\x302");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[2], LEN - 2);

        rc = wcsfc_s(str, LEN,
                     L"\x12e"
                     L"\x301",
                     &ind); /* accented I/J */
        ERR(EOK);
        wcscpy(str1, L"\x12f"
                     L"\x307");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[2], LEN - 2);

        rc = wcsfc_s(str, LEN,
                     L"\x12e"
                     L"\x302",
                     &ind); /* not accented I/J */
        ERR(EOK);
        wcscpy(str1, L"\x12f"
                     L"\x302");
        WEXPSTR(str, str1);
        WCHECK_SLACK(&str[2], LEN - 2);
    }

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    /*--------------------------------------------------*/

    /* see if we can lower-case and decompose all */
#ifdef PERL_TEST
    fprintf_s(pl,
              "use v5.34;\nno warnings;\nuse Unicode::Normalize;\nmy $err;"
              "sub wstr ($) {\n"
              "  join('',map{sprintf'\\x{%%X}',$_} unpack 'W*',shift);\n"
              "}\n"
              "sub chk {\n"
              "  my ($ch, $got) = @_;\n"
              "  my $fc = fc($ch);\n"
              "  my $nfd = NFD($fc);\n"
              "  if ($nfd ne $got) {\n"
              "    print \"Error \" if $fc ne $got;\n"
              "    printf \"NFD \\\\x{%%X} = %%s; fc = %%s; got: %%s\\n\",\n"
              "         unpack('W*',$ch), wstr $nfd, wstr $fc, wstr $got;\n"
              "    1\n"
              "  }\n"
              "}\n");
#endif
    for (wc = 0xc0; wc < 0x02fa20; wc++) {
        static wchar_t src[5];
        wchar_t *dest = &src[0];
        rsize_t dmax = 5;
        int c;
        if (wc == 0xd800) {
            wc = 0xdfff;
            continue;
        }
        _ENC_W16(dest, dmax, wc);
        *dest = 0;
        c = iswfc(wc);
        /*debug_printf("%s %u  U+%04X\n", __FUNCTION__, __LINE__, (int)ind);*/
        rc = wcsfc_s(str, 5, src, &len);
        /* c might be 1 or 0, but still expand to 2. eg U+C0 */
        /* 1fb0-1fff is special, expands via nfd to more */
        if (c >= 2 && c != (int)len && ((wc < 0x1f80) || (wc > 0x1fff))) {
            debug_printf("%s %u  %d U+%04X *lenp %d != iswfc %d  ",
                         __FUNCTION__, __LINE__, (int)rc, (int)wc, (int)len,
                         (int)c);
            WPRINTLSN(str);
        }
        if (!c) {
            ERR(EOK);
        } else if (rc) { /* 409 is not found, a iswfc() mismatch */
            debug_printf("%s %u  Error %d U+%04X  ", __FUNCTION__, __LINE__,
                         (int)rc, (int)wc);
            WPRINTLS(src);
            debug_printf(" => ");
            WPRINTLSN(str);
            errs++;
        }
#ifdef PERL_TEST
        {
            int i;
            len = wcslen(str);
            /* cross-check with perl */
            fprintf_s(pl, "$err += chk(\"\\N{U+%04X}\",\"\\N{U+%04X}", wc,
                      str[0]);
            for (i = 1; i < (int)len; i++) {
                fprintf_s(pl, "\\N{U+%04X}", str[i]);
            }
            fprintf_s(pl, "\");\n");
        }
#endif
    }

    /*--------------------------------------------------*/

#ifdef PERL_TEST
    fprintf_s(pl, "exit $err;\n");
#ifdef BSD_ALL_LIKE
    fstat(pl->_file, &st);
    fclose(pl);
#elif defined __GLIBC__
    fstat(pl->_fileno, &st);
    fclose(pl);
#else
    fclose(pl);
    stat(TESTPL, &st);
#endif
    if (st.st_size) {
        printf("Cross check with " PERL ":\n");
        fflush(stdout);
        if (system(PERL " " TESTPL) < 0) {
            printf("Redo with perl (probably wrong Unicode version):\n");
            fflush(stdout);
            if (!system("perl " TESTPL))
                printf("perl " TESTPL " failed\n");
        }
    }
#ifndef DEBUG
    unlink(TESTPL);
#endif

#endif /* PERL_TEST */

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return test_wcsfc_s(); }
