/*------------------------------------------------------------------
 * test_u8norm_s
 * File 'u8norm_s.c'
 * Lines executed:79.40% of 364
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <locale.h>

#define LEN (128)

int test_u8norm_s(void);

/*#define PERL_TEST*/
/* Must have the same Unicode version 10, since 5.27.3
   perl -MUnicode::UCD -e'print Unicode::UCD::UnicodeVersion()'
*/
#ifndef PERL
/*# define PERL "perl" */
/*# define PERL "cperl5.29.2" */
#define PERL "perl5.32"
#endif
#define TESTPL "test-norm.pl"

int test_u8norm_s(void) {
    errno_t rc;
    char8_t str[LEN];
    char8_t str1[LEN];
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
    rc = u8norm_s(NULL, LEN, "test", WCSNORM_NFD, NULL);
    ERR(ESNULLP);

    strcpy(str, "Aᾳ");
    EXPECT_BOS("empty src")
    rc = u8norm_s(str, LEN, NULL, WCSNORM_NFD, NULL);
    ERR(ESNULLP);
    EXPSTR(str, "\0");

    EXPECT_BOS("empty dest or dmax")
    rc = u8norm_s(str, 0, "test", WCSNORM_NFD, &ind);
    ERR(ESZEROL)
    INDZERO();
    EXPSTR(str, "\0");

    ind = 4;
    EXPECT_BOS("dest overflow")
    rc = u8norm_s(str, RSIZE_MAX_STR + 1, "test", WCSNORM_NFD, &ind);
    ERR(ESLEMAX);
    EXPSTR(str, "\0");
    INDCMP(!= 0)

    if (_BOS_KNOWN(str)) {
        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = u8norm_s(str, LEN + 1, "test", WCSNORM_NFD, &ind);
        ERR(EOVERFLOW);
        EXPSTR(str, "\0");
        INDCMP(!= 0)
    }

    if (_BOS_KNOWN(str)) {
        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = u8norm_decompose_s(str, LEN + 1, "test", &ind, false);
        ERR(EOVERFLOW);
        EXPSTR(str, "\0");
        INDCMP(!= 0)

        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = u8norm_reorder_s(str, LEN + 1, "test", ind);
        ERR(EOVERFLOW);
        EXPSTR(str, "\0");

        EXPECT_BOS("dest overflow")
        rc = u8norm_compose_s(str, LEN + 1, "test", &ind, false);
        ERR(EOVERFLOW);
        EXPSTR(str, "\0");
        INDCMP(!= 0)
    }
#endif

#define OVMAX_WC "����" // 11ffff
#define MAX_WC1 "􏿰"  // 10fff0

    rc = u8norm_s(str, LEN, OVMAX_WC, WCSNORM_NFD, NULL);
    ERR(ESLEMAX);
    EXPSTR(str, "\0");

    rc = u8norm_decompose_s(str, LEN, OVMAX_WC, NULL, false);
    ERR(ESLEMAX);
    EXPSTR(str, "\0");

    rc = u8norm_decompose_s(str, LEN, OVMAX_WC, NULL, true);
#ifdef HAVE_NORM_COMPAT
    ERR(ESLEMAX);
    EXPSTR(str, "\0");
#else
    if (rc == EOF) {
        ERR(EOF);
    } else {
        debug_printf("%u TODO !norm-compat rc=%d\n", __LINE__, rc);
    }
#endif

    rc = u8norm_s(str, 4, MAX_WC1, WCSNORM_NFD, NULL);
    ERR(ESLEMIN);
    EXPSTR(str, "\0");

    /*--------------------------------------------------*/

    rc = u8norm_s(str, LEN, "Café", WCSNORM_NFD, &ind);
    ERR(EOK);
    EXPSTR(str, "Café");
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    /*--------------------------------------------------*/

#ifndef __PGI
    rc = u8norm_s(str, LEN, "Café", WCSNORM_NFD, &ind);
    ERR(EOK)
    EXPSTR(str, "Café");
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);
#endif

    rc = u8norm_s(str, LEN, "Café", WCSNORM_NFC, &ind);
    ERR(EOK)
    EXPSTR(str, "Café");
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    /*--------------------------------------------------*/

    strcpy(str, "Aᾳ");
    rc = u8norm_s(str1, LEN, str, WCSNORM_NFC, NULL);
    ERR(EOK);

    strcpy(str, "Abcᾷ");
    rc = u8norm_s(str1, 6, str, WCSNORM_NFD, NULL);
    ERR(ESNOSPC);
    EXPSTR(str1, "\0");

#ifdef HAVE_NORM_COMPAT
    strcpy(str, "A㈝");
    rc = u8norm_s(str1, 18, str, WCSNORM_NFKC, NULL);
    ERR(ESLEMIN);
    EXPSTR(str1, "\0");
#endif

    /* echo "Aᾳ" | unorm -n nfd | iconv -t UTF-32LE | od -h */
    rc = u8norm_s(str, LEN, "Aᾳ", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "Aᾳ");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    /* Aᾳ => A≈ᾳ */
    rc = u8norm_s(str, LEN, "Aᾳ", WCSNORM_NFC, &ind);
    ERR(EOK);
    strcpy(str1, "Aᾳ");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "Aᾷ", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "Aᾷ");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "Aᾷ", WCSNORM_NFC, &ind);
    ERR(EOK);
    strcpy(str1, "Aᾷ");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    /* MUSICAL SYMBOL FUSA BLACK \xf0\x9d\x87\x80 */
    rc = u8norm_s(str, LEN, "𝆺𝅥𝅯", WCSNORM_NFD, &ind);
    ERR(EOK);
    /* MUSICAL SYMBOL MINIMA BLACK (U+1D1BC) MUSICAL SYMBOL COMBINING FLAG-2
     * (U+1D16F) */
    /* => MUSICAL SYMBOL SEMIBREVIS BLACK (U+1D1BA) MUSICAL SYMBOL COMBINING
     * STEM (U+1D165) + U+1D16F */
    strcpy(str1, "𝆺𝅥𝅯");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    /*--------------------------------------------------*/

    /* reordering */
    rc = u8norm_s(str, LEN, "Aᾷ", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "Aᾷ"); /* do reorder */
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "Aᾷ", WCSNORM_FCD, &ind);
    ERR(EOK);
    strcpy(str1, "Aᾷ"); /* no reorder */
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "Aᾷ", WCSNORM_NFC, &ind);
    ERR(EOK);
    strcpy(str1, "Aᾷ"); /* nfc */
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "Aᾷ", WCSNORM_FCC, &ind);
    ERR(EOK);
    strcpy(str1, "Aᾷ"); /* the same in this case */
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    /*--------------------------------------------------*/

    rc = u8norm_s(str, LEN, "ā", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "ā");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "ā", WCSNORM_NFC, &ind);
    ERR(EOK);
    strcpy(str1, "ā");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "ĕ", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "ĕ");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "ß", WCSNORM_NFD, &ind); /* !nfd */
    ERR(EOK);
    strcpy(str1, "ß");
    EXPSTR(str, str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, ";", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, ";"); /* NFKC => 3b */
    len = strlen(str1);
    INDCMP(!= (int)len);
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "΅", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "΅");
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xce\xac", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "\xce\xb1"
                 "\xcc\x81");
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xce\xad", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "\xce\xb5"
                 "\xcc\x81");
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xe1\xbd\xb1", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "\xce\xb1"
                 "\xcc\x81");
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xe1\xbc\x82", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "\xce\xb1"
                 "\xcc\x93"
                 "\xcc\x80");
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xe1\xbf\xab", WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "\xce\xa5"
                 "\xcc\x81");
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "΅" /*\xe1\xbf\xae*/, WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy(str1, "΅" /*"\xa8" "\xcc\x81"*/);
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    /*--------------------------------------------------*/

    /* compat NFKD */
    /* echo "㈝" | unorm -n nfkd | iconv -t UTF-32LE | od -h */
    rc = u8norm_s(str, LEN, "㈝" /*\xe3\x88\x9d*/, WCSNORM_NFKD, &ind); /* TODO wchar2 */
#ifdef HAVE_NORM_COMPAT
    ERR(EOK);
    strcpy(str1, "(\xe1\x84\x8b\xe1\x85\xa9\xe1\x84\x8c\xe1\x85\xa5\xe1\x86\xab)");
    len = strlen(str1);
    INDCMP_(!= len);
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);
#else
    if (rc == -1) {
        ERR(-1);
    } else {
        debug_printf("%u TODO !norm-compat rc=%d\n", __LINE__, rc);
    }
#endif

#ifdef HAVE_NORM_COMPAT
    rc = u8norm_s(str, LEN, "\xcd\xbe", WCSNORM_NFKC, &ind);
    ERR(EOK);
    strcpy(str1, "\x3b");
    len = strlen(str1);
    INDCMP_(!= len);
    EXPSTR(str, str1);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    /* (오전) */
    rc = u8norm_s(str, LEN, "\xe3\x88\x9d", WCSNORM_NFKC, &ind);
    ERR(EOK);
    strcpy(str1, "(\xec\x98\xa4\xec\xa0\x84)");
    EXPSTR(str, str1);
    len = strlen(str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xef\xb7\xbb", WCSNORM_NFKD, &ind);
    ERR(EOK);
    strcpy(str1, "\xd8\xac\xd9\x84\x20\xd8\xac\xd9\x84\xd8\xa7\xd9\x84\xd9\x87");
    EXPSTR(str, str1);
    len = strlen(str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xe2\x84\x83", WCSNORM_NFKD, &ind);
    ERR(EOK);
    strcpy(str1, "\xc2\xb0"
                 "\x43");
    EXPSTR(str, str1);
    len = strlen(str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xe2\x85\x90", WCSNORM_NFKD, &ind);
    ERR(EOK);
    strcpy(str1, "\x31"
                 "\xe2\x81\x84"
                 "\x37");
    EXPSTR(str, str1);
    len = strlen(str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

    rc = u8norm_s(str, LEN, "\xe3\x8e\x82", WCSNORM_NFKD, &ind);
    ERR(EOK);
    strcpy(str1, "\xce\xbc" "\x41");
    EXPSTR(str, str1);
    len = strlen(str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str) + 1], LEN - strlen((char *)str) - 1);

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
        static char8_t src[5];
        uint32_t cp = (uint32_t)ind;
        int enclen;
        if (ind == 0xd800) {
            ind = 0xdfff;
            continue;
        }
        /* encode cp as utf-8 into src */
        if (cp < 0x80) {
            src[0] = (char8_t)cp;
            enclen = 1;
        } else if (cp < 0x800) {
            src[0] = (char8_t)(0xc0 | (cp >> 6));
            src[1] = (char8_t)(0x80 | (cp & 0x3f));
            enclen = 2;
        } else if (cp < 0x10000) {
            src[0] = (char8_t)(0xe0 | (cp >> 12));
            src[1] = (char8_t)(0x80 | ((cp >> 6) & 0x3f));
            src[2] = (char8_t)(0x80 | (cp & 0x3f));
            enclen = 3;
        } else {
            src[0] = (char8_t)(0xf0 | (cp >> 18));
            src[1] = (char8_t)(0x80 | ((cp >> 12) & 0x3f));
            src[2] = (char8_t)(0x80 | ((cp >> 6) & 0x3f));
            src[3] = (char8_t)(0x80 | (cp & 0x3f));
            enclen = 4;
        }
        src[enclen] = 0;
        rc = u8norm_s(str, 20, src, WCSNORM_NFD, &len);
        if (rc || len < 1) {
            debug_printf("%s %u  Error %d U+%04X len=%ld ", __FUNCTION__,
                         __LINE__, (int)rc, (int)ind, (long)len);
            //WPRINTLS(src);
            debug_printf("%s => %s", src, str);
            //WPRINTLSN(str);
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
            rc = u8norm_s(str, LEN, src, WCSNORM_NFKD, &len);
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

int main(void) { return test_u8norm_s(); }
