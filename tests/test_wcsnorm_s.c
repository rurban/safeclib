/*------------------------------------------------------------------
 * test_wcsnorm_s
 * File 'wcsnorm_s.c'
 * Lines executed:79.40% of 364
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

#if SIZEOF_WCHAR_T > 2
#define _dec_w16(src) *(src)
#else
EXTERN uint32_t _dec_w16(wchar_t *src);
#endif
int test_wcsnorm_s(void);

/*#define PERL_TEST*/
/* Must have the same Unicode version 10, since 5.27.3
   perl -MUnicode::UCD -e'print Unicode::UCD::UnicodeVersion()'
*/
#ifndef PERL
/*# define PERL "perl" */
/*# define PERL "cperl5.30.0" */
#define PERL "perl5.36.0"
#endif
#define TESTPL "test-norm.pl"

int test_wcsnorm_s(void) {
    errno_t rc;
    wchar_t str[LEN];
    wchar_t str1[LEN];
    rsize_t ind;
    size_t len;
    int errs = 0;
#ifdef PERL_TEST
    FILE *pl;
    struct stat st;

    pl = fopen(TESTPL, "w");
#endif

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = wcsnorm_s(NULL, LEN, L"test", WCSNORM_NFD, NULL);
    ERR(ESNULLP);

    wcscpy(str, L"A"
                L"\x1fb3");
    EXPECT_BOS("empty src")
    rc = wcsnorm_s(str, LEN, NULL, WCSNORM_NFD, NULL);
    ERR(ESNULLP);
    WEXPSTR(str, L"\0");

    EXPECT_BOS("empty dest or dmax")
    rc = wcsnorm_s(str, 0, L"test", WCSNORM_NFD, &ind);
    ERR(ESZEROL)
    INDZERO();
    WEXPSTR(str, L"\0");

    ind = 4;
    EXPECT_BOS("dest overflow")
    rc = wcsnorm_s(str, RSIZE_MAX_WSTR + 1, L"test", WCSNORM_NFD, &ind);
    ERR(ESLEMAX);
    WEXPSTR(str, L"\0");
    INDCMP(!= 0)

    if (_BOS_KNOWN(str)) {
        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = wcsnorm_s(str, LEN + 1, L"test", WCSNORM_NFD, &ind);
        ERR(EOVERFLOW);
        WEXPSTR(str, L"\0");
        INDCMP(!= 0)
    }

    if (_BOS_KNOWN(str)) {
        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = wcsnorm_decompose_s(str, LEN + 1, L"test", &ind, false);
        ERR(EOVERFLOW);
        WEXPSTR(str, L"\0");
        INDCMP(!= 0)

        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = wcsnorm_reorder_s(str, LEN + 1, L"test", ind);
        ERR(EOVERFLOW);
        WEXPSTR(str, L"\0");

        EXPECT_BOS("dest overflow")
        rc = wcsnorm_compose_s(str, LEN + 1, L"test", &ind, false);
        ERR(EOVERFLOW);
        WEXPSTR(str, L"\0");
        INDCMP(!= 0)
    }
#endif

#if SIZEOF_WCHAR_T >= 4
#define OVMAX_WC L"\x11ffff"
#define MAX_WC1 L"\x10fff0"
#else
    /* => capped at 0x10ffff */
#define OVMAX_WC                                                               \
    L"\xdbff"                                                                  \
    L"\xdfff"
#define MAX_WC1                                                                \
    L"\xdb3f"                                                                  \
    L"\xdff0"
#endif

    rc = wcsnorm_s(str, LEN, OVMAX_WC, WCSNORM_NFD, NULL);
#if SIZEOF_WCHAR_T >= 4
    ERR(ESLEMAX);
    WEXPSTR(str, L"\0");
#else
    ERR(EOK); /* cp collapses to 0x10ffff, decompose keeps it */
    wcscpy(str1, L"\xd876"
                 L"\xdfff");
    WEXPSTR(&str[1], &str1[1]);
    WCHECK_SLACK(&str[2], LEN - 2);
#endif

#if SIZEOF_WCHAR_T >= 4
    rc = wcsnorm_decompose_s(str, LEN, OVMAX_WC, NULL, false);
    ERR(ESLEMAX);
    WEXPSTR(str, L"\0");

    rc = wcsnorm_decompose_s(str, LEN, OVMAX_WC, NULL, true);
#ifdef HAVE_NORM_COMPAT
    ERR(ESLEMAX);
    WEXPSTR(str, L"\0");
#else
    if (rc == EOF) {
        ERR(EOF);
    } else {
        debug_printf("%u TODO !norm-compat rc=%d\n", __LINE__, rc);
    }
#endif
#endif

    rc = wcsnorm_s(str, 4, MAX_WC1, WCSNORM_NFD, NULL);
    ERR(ESLEMIN);
    WEXPSTR(str, L"\0");

    /*--------------------------------------------------*/

    rc = wcsnorm_s(str, LEN, L"Caf\xe9", WCSNORM_NFD, &ind);
    ERR(EOK);
    WEXPSTR(str, L"Cafe\x301");
    INDCMP(!= 5);
    WCHECK_SLACK(&str[5], LEN - 5);

    /*--------------------------------------------------*/

#ifndef __PGI
    rc = wcsnorm_s(str, LEN, L"Café", WCSNORM_NFD, &ind);
    ERR(EOK)
    WEXPSTR(str, L"Cafe\x301");
    INDCMP(!= 5);
    WCHECK_SLACK(&str[5], LEN - 5);
#endif

    rc = wcsnorm_s(str, LEN, L"Cafe\x301", WCSNORM_NFC, &ind);
    ERR(EOK)
    WEXPSTR(str, L"Caf\xe9");
    INDCMP(!= 4);
    WCHECK_SLACK(&str[4], LEN - 4);

    /*--------------------------------------------------*/

    wcscpy(str, L"A"
                L"\x1fb3");
    rc = wcsnorm_s(str1, LEN, str, WCSNORM_NFC, NULL);
    ERR(EOK);

    wcscpy(str, L"Abc"
                L"\x1fb7");
    rc = wcsnorm_s(str1, 6, str, WCSNORM_NFD, NULL);
    ERR(ESNOSPC);
    WEXPSTR(str1, L"\0");

#ifdef HAVE_NORM_COMPAT
    wcscpy(str, L"A"
                L"\x321d");
    rc = wcsnorm_s(str1, 18, str, WCSNORM_NFKC, NULL);
    ERR(ESLEMIN);
    WEXPSTR(str1, L"\0");
#endif

    /* echo "Aᾳ" | unorm -n nfd | iconv -t UTF-32LE | od -h */
    rc = wcsnorm_s(str, LEN,
                   L"A"
                   L"\x1fb3",
                   WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"A"
                 L"\x3b1"
                 L"\x345");
    WEXPSTR(str, str1);
    INDCMP(!= 3);
    WCHECK_SLACK(&str[3], LEN - 3);

    /* Aᾳ => A≈ᾳ */
    rc = wcsnorm_s(str, LEN,
                   L"A"
                   L"\x3b1"
                   L"\x345",
                   WCSNORM_NFC, &ind);
    ERR(EOK);
    wcscpy(str1, L"A"
                 L"\x1fb3");
    WEXPSTR(str, str1);
    INDCMP(!= 2);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN,
                   L"A"
                   L"\x1fb7",
                   WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"A"
                 L"\x3b1"
                 L"\x342"
                 L"\x345");
    WEXPSTR(str, str1);
    INDCMP(!= 4);
    WCHECK_SLACK(&str[4], LEN - 4);

    rc = wcsnorm_s(str, LEN,
                   L"A"
                   L"\x3b1"
                   L"\x342"
                   L"\x345",
                   WCSNORM_NFC, &ind);
    ERR(EOK);
    wcscpy(str1, L"A"
                 L"\x1fb7");
    WEXPSTR(str, str1);
    INDCMP(!= 2);
    WCHECK_SLACK(&str[2], LEN - 2);

    /* TBL(6) on windows, no _exc tbl with canon */
#if SIZEOF_WCHAR_T == 2
    {
        wchar_t *sp = &str1[0];
        uint32_t cp = 0x1d1c0UL;    /* need surrogate pair for this */
        wcscpy(str1, L"\x0\x0\x0"); /* 0xd874 0xddc0 */
        _ENC_W16(sp, len, cp);
        rc = wcsnorm_s(str, LEN, str1, WCSNORM_NFD, &ind);
    }
    ERR(EOK);
    wcscpy(str1, L"\xd834\xddba\xd834\xdd65\xd834\xdd6f");
    WEXPSTR(str, str1);
    INDCMP(!= 6);
    WCHECK_SLACK(&str[6], LEN - 6);
#else
    /* MUSICAL SYMBOL FUSA BLACK */
    rc = wcsnorm_s(str, LEN, L"\x1d1c0", WCSNORM_NFD, &ind);
    ERR(EOK);
    /* MUSICAL SYMBOL MINIMA BLACK (U+1D1BC) MUSICAL SYMBOL COMBINING FLAG-2
     * (U+1D16F) */
    /* => MUSICAL SYMBOL SEMIBREVIS BLACK (U+1D1BA) MUSICAL SYMBOL COMBINING
     * STEM (U+1D165) + U+1D16F */
    wcscpy(str1, L"\x1d1ba\x1d165\x1d16f");
    WEXPSTR(str, str1);
    INDCMP(!= 3);
    WCHECK_SLACK(&str[3], LEN - 3);
#endif

    /*--------------------------------------------------*/

    /* reordering */
    rc = wcsnorm_s(str, LEN,
                   L"A"
                   L"\x3b1"
                   L"\x345"
                   L"\x342",
                   WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"A"
                 L"\x3b1"
                 L"\x342"
                 L"\x345"); /* do reorder */
    WEXPSTR(str, str1);
    INDCMP(!= 4);
    WCHECK_SLACK(&str[4], LEN - 4);

    rc = wcsnorm_s(str, LEN,
                   L"A"
                   L"\x3b1"
                   L"\x345"
                   L"\x342",
                   WCSNORM_FCD, &ind);
    ERR(EOK);
    wcscpy(str1, L"A"
                 L"\x3b1"
                 L"\x345"
                 L"\x342"); /* no reorder */
    WEXPSTR(str, str1);
    INDCMP(!= 4);
    WCHECK_SLACK(&str[4], LEN - 4);

    rc = wcsnorm_s(str, LEN,
                   L"A"
                   L"\x3b1"
                   L"\x345"
                   L"\x342",
                   WCSNORM_NFC, &ind);
    ERR(EOK);
    wcscpy(str1, L"A"
                 L"\x1fb7"); /* nfc */
    WEXPSTR(str, str1);
    INDCMP(!= 2);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN,
                   L"A"
                   L"\x3b1"
                   L"\x345"
                   L"\x342",
                   WCSNORM_FCC, &ind);
    ERR(EOK);
    wcscpy(str1, L"A"
                 L"\x1fb7"); /* the same in this case */
    WEXPSTR(str, str1);
    INDCMP(!= 2);
    WCHECK_SLACK(&str[2], LEN - 2);

    /*--------------------------------------------------*/

    rc = wcsnorm_s(str, LEN, L"\x101", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x61"
                 L"\x304");
    WEXPSTR(str, str1);
    INDCMP(!= 2);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN,
                   L"\x61"
                   L"\x304",
                   WCSNORM_NFC, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x101");
    WEXPSTR(str, str1);
    INDCMP(!= 1);
    WCHECK_SLACK(&str[1], LEN - 1);

    rc = wcsnorm_s(str, LEN, L"\x115", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x65"
                 L"\x306");
    WEXPSTR(str, str1);
    INDCMP(!= 2);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN, L"\xdf", WCSNORM_NFD, &ind); /* !nfd */
    ERR(EOK);
    wcscpy(str1, L"\xdf");
    WEXPSTR(str, str1);
    INDCMP(!= 1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN, L"\x37e", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x37e"); /* NFKC => 3b */
    len = wcslen(str1);
    INDCMP(!= (int)len);
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[len], LEN - len);

    rc = wcsnorm_s(str, LEN, L"\x385", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\xa8"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN, L"\x3ac", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x3b1"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN, L"\x3ad", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x3b5"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN, L"\x1f71", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x3b1"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN, L"\x1f02", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x3b1"
                 L"\x313"
                 L"\x300");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[3], LEN - 3);

    rc = wcsnorm_s(str, LEN, L"\x1feb", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x3a5"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    rc = wcsnorm_s(str, LEN, L"\x1fee", WCSNORM_NFD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\xa8"
                 L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN - 2);

    /*--------------------------------------------------*/

    /* compat NFKD */
    /* echo "㈝" | unorm -n nfkd | iconv -t UTF-32LE | od -h */
    rc = wcsnorm_s(str, LEN, L"\x321d", WCSNORM_NFKD, &ind); /* TODO wchar2 */
#ifdef HAVE_NORM_COMPAT
    ERR(EOK);
    wcscpy(str1, L"(\x110b\x1169\x110c\x1165\x11ab)");
    len = wcslen(str1);
    INDCMP_(!= len);
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[len], LEN - len);
#else
    if (rc == -1) {
        ERR(-1);
    } else {
        debug_printf("%u TODO !norm-compat rc=%d\n", __LINE__, rc);
    }
#endif

#ifdef HAVE_NORM_COMPAT
    rc = wcsnorm_s(str, LEN, L"\x37e", WCSNORM_NFKC, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x3b");
    len = wcslen(str1);
    INDCMP_(!= len);
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[len], LEN - len);

    /* (오전) */
    rc = wcsnorm_s(str, LEN, L"\x321d", WCSNORM_NFKC, &ind);
    ERR(EOK);
    wcscpy(str1, L"(\xc624\xc804)");
    WEXPSTR(str, str1);
    len = wcslen(str1);
    INDCMP_(!= len);
    WCHECK_SLACK(&str[ind], LEN - ind);

    rc = wcsnorm_s(str, LEN, L"\xfdfb", WCSNORM_NFKD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x062c\x0644\x0020\x062c\x0644\x0627\x0644\x0647");
    WEXPSTR(str, str1);
    len = wcslen(str1);
    INDCMP_(!= len);
    WCHECK_SLACK(&str[ind], LEN - ind);

    rc = wcsnorm_s(str, LEN, L"\x2103", WCSNORM_NFKD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\xb0"
                 L"\x43");
    WEXPSTR(str, str1);
    len = wcslen(str1);
    INDCMP_(!= len);
    WCHECK_SLACK(&str[ind], LEN - ind);

    rc = wcsnorm_s(str, LEN, L"\x2150", WCSNORM_NFKD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x31"
                 L"\x2044"
                 L"\x37");
    WEXPSTR(str, str1);
    len = wcslen(str1);
    INDCMP_(!= len);
    WCHECK_SLACK(&str[ind], LEN - ind);

    rc = wcsnorm_s(str, LEN, L"\x3382", WCSNORM_NFKD, &ind);
    ERR(EOK);
    wcscpy(str1, L"\x03bc"
                 L"\x41");
    WEXPSTR(str, str1);
    len = wcslen(str1);
    INDCMP_(!= len);
    WCHECK_SLACK(&str[ind], LEN - ind);

#endif

    /*--------------------------------------------------*/

    /* see if we can lower-case and decompose all */
#ifdef PERL_TEST
    fprintf_s(pl,
              "use v5.27.2;\nno warnings;\nuse Unicode::Normalize;\nmy $err;\n"
              "sub wstr ($) {\n"
              "  join('',map{sprintf'\\x{%%X}',$_} unpack 'W*',shift);\n"
              "}\n"
              "sub chknfd {\n"
              "  my ($ch, $got) = @_;\n"
              "  my $nfd = NFD($ch);\n"
              "  if ($nfd ne $got) {\n"
              "    printf \"Error NFD \\\\x{%%X} = %%s; got: %%s\\n\",\n"
              "         unpack('W*',$ch), wstr $nfd, wstr $got;\n"
              "    1\n"
              "  }\n"
              "}\n");
#ifdef HAVE_NORM_COMPAT
    fprintf_s(pl, "sub chknfkd {\n"
                  "  my ($ch, $got) = @_;\n"
                  "  my $nfd = NFKD($ch);\n"
                  "  if ($nfd ne $got) {\n"
                  "    printf \"Error NFKD \\\\x{%%X} = %%s; got: %%s\\n\",\n"
                  "         unpack('W*',$ch), wstr $nfd, wstr $got;\n"
                  "    1\n"
                  "  }\n"
                  "}\n");
#endif
#endif
    for (ind = 0xc0; ind < 0x02fa20; ind++) {
        static wchar_t src[5];
        wchar_t *dest = &src[0];
        rsize_t dmax = 5;
        if (ind == 0xd800) {
            ind = 0xdfff;
            continue;
        }
        _ENC_W16(dest, dmax, ind);
        *dest = 0;
        rc = wcsnorm_s(str, 10, src, WCSNORM_NFD, &len);
        if (rc || len < 1) {
            debug_printf("%s %u  Error %d U+%04X len=%ld ", __FUNCTION__,
                         __LINE__, (int)rc, (int)ind, (long)len);
            WPRINTLS(src);
            debug_printf(" => ");
            WPRINTLSN(str);
            errs++;
        }
#ifdef PERL_TEST
        {
            size_t i;
            /* cross-check with perl */
            fprintf_s(pl, "$err += chknfd (\"\\N{U+%04X}\",\"\\N{U+%04X}", ind,
                      str[0]);
            for (i = 1; i < len; i++) {
                fprintf_s(pl, "\\N{U+%04X}", str[i]);
            }
            fprintf_s(pl, "\");\n");

#ifdef HAVE_NORM_COMPAT
            rc = wcsnorm_s(str, LEN, src, WCSNORM_NFKD, &len);
            fprintf_s(pl, "$err += chknfkd(\"\\N{U+%04X}\",\"\\N{U+%04X}", ind,
                      str[0]);
            for (i = 1; i < len; i++) {
                fprintf_s(pl, "\\N{U+%04X}", str[i]);
            }
            fprintf_s(pl, "\");\n");
#endif
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
            system("perl " TESTPL) || printf("perl " TESTPL " failed\n");
        }
    }
#ifndef DEBUG
    unlink(TESTPL);
#endif

#endif /* PERL_TEST */

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return test_wcsnorm_s(); }
