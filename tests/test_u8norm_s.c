/*------------------------------------------------------------------
 * test_u8norm_s
 * File 'u8norm_s.c'
 * Lines executed:91.11% of 450
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

static size_t *volatile vnulls = NULL;
static volatile size_t vzero = 0;

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

/* Non-starter run "x" + n times U+0301 (COMBINING ACUTE ACCENT, cc 230) */
static size_t make_marks(char8_t *buf, size_t n) {
    size_t i;
    buf[0] = 'x';
    for (i = 0; i < n; i++) {
        buf[1 + 2 * i] = 0xCC;
        buf[2 + 2 * i] = 0x81;
    }
    buf[1 + 2 * n] = '\0';
    return 1 + 2 * n;
}

/* Branches of the decompose/reorder/compose steps which u8norm_s() with a
   sufficiently big dest never reaches. */
static int check_norm_edges(void) {
    errno_t rc;
    int errs = 0;
    rsize_t ind = 0;
    static char8_t buf[256];
    static char8_t src[128];
    static char8_t out[128];
    static char8_t big[300];
    static char8_t longsrc[201];
    size_t n;

    /*--------------------------------------------------*/
    /* decompose: dest == src */

    strcpy((char *)buf, "abc");
    rc = u8norm_decompose_s(buf, 16, buf, &ind, false);
    ERR(ESOVRLP)

    /* decompose: dest < src, both layouts. */
    {
        char8_t *d = buf;
        char8_t *s = buf + 64;

        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "ab\xC3\xA9");
        rc = u8norm_decompose_s(d, 32, s, &ind, false);
        ERR(EOK)
        EXPSTR((char *)d, "abe\xCC\x81");
        INDCMP(!= 5)

        /* cp > U+10FFFF */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "\xF4\x9F\xBF\xBF");
        rc = u8norm_decompose_s(d, 32, s, &ind, false);
        ERR(ESLEMAX)

        /* overlapping: dest runs into src */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)buf + 1, "abc");
        rc = u8norm_decompose_s(buf, 16, buf + 1, &ind, false);
        ERR(ESOVRLP)

        /* too small dmax for the decomposition, in the dest < src branch */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "abc");
        rc = u8norm_decompose_s(d, 5, s, &ind, false);
        ERR(ESNOSPC)

        /* hangul syllable needs 3 bytes of room, only 2 left */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "\xEA\xB0\x80\xEA\xB0\x80");
        rc = u8norm_decompose_s(d, 5, s, &ind, false);
        ERR(ESNOSPC)
    }

    /* decompose: dest > src */
    {
        char8_t *s = buf;
        char8_t *d = buf + 64;

        /* overlapping: src runs into dest */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)buf, "abc");
        rc = u8norm_decompose_s(buf + 1, 16, buf, &ind, false);
        ERR(ESOVRLP)

        /* too small dmax for the decomposition */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "abc");
        rc = u8norm_decompose_s(d, 5, s, &ind, false);
        ERR(ESNOSPC)

        /* hangul syllable needs 3 bytes of room, only 2 left */
        memset(buf, 'X', sizeof(buf));
        strcpy((char *)s, "\xEA\xB0\x80\xEA\xB0\x80");
        rc = u8norm_decompose_s(d, 5, s, &ind, false);
        ERR(ESNOSPC)
    }

    /* codepoints in planes without any decomposition (U+E0001) */
    rc = u8norm_s(out, 32, (const char8_t *)"\xF3\xA0\x80\x81", WCSNORM_NFD,
                  &ind);
    ERR(EOK)
    EXPSTR((char *)out, "\xF3\xA0\x80\x81");
    INDCMP(!= 4)

    /*--------------------------------------------------*/
    /* u8norm_s with a result longer than the stack scratch buffer */

    memset(longsrc, 'a', 200);
    longsrc[200] = '\0';
    rc = u8norm_s(big, sizeof(big), longsrc, WCSNORM_NFD, &ind);
    ERR(EOK)
    INDCMP(!= 200)
    EXPSTR((char *)big, (char *)longsrc);

    rc = u8norm_s(big, sizeof(big), longsrc, WCSNORM_NFC, &ind);
    ERR(EOK)
    INDCMP(!= 200)
    EXPSTR((char *)big, (char *)longsrc);

    /*--------------------------------------------------*/
    /* reorder */

    rc = u8norm_reorder_s(out, vzero, (const char8_t *)"", 0);
    ERR(ESNOSPC)

    /* more non-starters than the stack scratch area: heap extension */
    n = make_marks(src, 20);
    rc = u8norm_reorder_s(out, sizeof(out), src, n);
    ERR(EOK)
    EXPSTR((char *)out, (char *)src);

    /* too small dmax in the non-starter run */
    n = make_marks(src, 12);
    rc = u8norm_reorder_s(out, 10, src, n);
    ERR(ESNOSPC)

    /* too small dmax at a following starter */
    n = make_marks(src, 12);
    memcpy(&src[n], "bcd", 4);
    rc = u8norm_reorder_s(out, 27, src, n + 3);
    ERR(ESNOSPC)

    /* too small dmax for a plain starter run */
    rc = u8norm_reorder_s(out, 4, (const char8_t *)"abcdef", 6);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/
    /* compose */

    /* hangul: L + V, then LV + T */
    rc = u8norm_s(out, 32, (const char8_t *)"\xE1\x84\x80\xE1\x85\xA1",
                  WCSNORM_NFC, &ind);
    ERR(EOK)
    EXPSTR((char *)out, "\xEA\xB0\x80");
    INDCMP(!= 3)

    rc = u8norm_s(out, 32,
                  (const char8_t *)"\xE1\x84\x80\xE1\x85\xA1\xE1\x86\xA8",
                  WCSNORM_NFC, &ind);
    ERR(EOK)
    EXPSTR((char *)out, "\xEA\xB0\x81");
    INDCMP(!= 3)

    /* starters without any composition entry: sparse table rows/planes */
    rc = u8norm_s(out, 32, (const char8_t *)"\xE4\xB8\x80\xCC\x81", WCSNORM_NFC,
                  &ind);
    ERR(EOK)
    EXPSTR((char *)out, "\xE4\xB8\x80\xCC\x81");
    INDCMP(!= 5)

    rc = u8norm_s(out, 32, (const char8_t *)"\xF3\xA0\x80\x81\xCC\x81",
                  WCSNORM_NFC, &ind);
    ERR(EOK)
    EXPSTR((char *)out, "\xF3\xA0\x80\x81\xCC\x81");
    INDCMP(!= 6)

    /* starters beyond U+105D2 use the long composition list:
       KAITHI LETTER U+11099 + SIGN NUKTA U+110BA => U+1109A */
    rc = u8norm_s(out, 32, (const char8_t *)"\xF0\x91\x82\x99\xF0\x91\x82\xBA",
                  WCSNORM_NFC, &ind);
    ERR(EOK)
    EXPSTR((char *)out, "\xF0\x91\x82\x9A");
    INDCMP(!= 4)

    /* ... and a pair which doesn't compose */
    rc = u8norm_s(out, 32, (const char8_t *)"\xF0\x91\x82\x99\xCC\x81",
                  WCSNORM_NFC, &ind);
    ERR(EOK)
    EXPSTR((char *)out, "\xF0\x91\x82\x99\xCC\x81");
    INDCMP(!= 6)

    /* leading non-starter is copied as is */
    ind = 3;
    rc = u8norm_compose_s(out, sizeof(out), (const char8_t *)"\xCC\x81x", &ind,
                          false);
    ERR(EOK)
    EXPSTR((char *)out, "\xCC\x81x");
    INDCMP(!= 3)

    ind = 2;
    rc = u8norm_compose_s(out, 2, (const char8_t *)"\xCC\x81", &ind, false);
    ERR(ESNOSPC)

    /* blocked by the same combining class: x + U+0301 + U+0301 */
    n = make_marks(src, 2);
    ind = n;
    rc = u8norm_compose_s(out, sizeof(out), src, &ind, false);
    ERR(EOK)
    EXPSTR((char *)out, (char *)src);
    INDCMP(!= (int)n)

    /* FCC: a + U+0323 (cc 220) composes to U+1EA1, the following U+0301
       has no composition with it */
    ind = 5;
    rc = u8norm_compose_s(out, sizeof(out),
                          (const char8_t *)"a\xCC\xA3\xCC\x81", &ind, true);
    ERR(EOK)
    EXPSTR((char *)out, "\xE1\xBA\xA1\xCC\x81");

    /* more non-starters than the stack scratch area: heap extension */
    n = make_marks(src, 20);
    ind = n;
    rc = u8norm_compose_s(out, sizeof(out), src, &ind, false);
    ERR(EOK)
    EXPSTR((char *)out, (char *)src);
    INDCMP(!= (int)n)

    /* too small dmax at the starter, with a heap extended run */
    n = make_marks(src, 12);
    ind = n;
    rc = u8norm_compose_s(out, 1, src, &ind, false);
    ERR(ESNOSPC)

    /* too small dmax in the non-starter run */
    n = make_marks(src, 12);
    ind = n;
    rc = u8norm_compose_s(out, 10, src, &ind, false);
    ERR(ESNOSPC)

    /* too small dmax for a starter run */
    ind = 2;
    rc = u8norm_compose_s(out, 1, (const char8_t *)"ab", &ind, false);
    ERR(ESNOSPC)

    /* an embedded NUL within len is a codepoint 0 which never composes */
    memset(out, 'Q', sizeof(out));
    ind = 3;
    rc = u8norm_compose_s(out, sizeof(out),
                          (const char8_t *)"a\0"
                                           "b",
                          &ind, false);
    ERR(EOK)
    INDCMP(!= 3)
    if (out[0] != 'a' || out[1] != '\0' || out[2] != 'b' || out[3] != '\0') {
        debug_printf("%s %u  wrong result for an embedded NUL\n", __FUNCTION__,
                     __LINE__);
        errs++;
    }

    /*--------------------------------------------------*/
    /* sizes only known at runtime: dmax > RSIZE_MAX_STR */
    {
        static volatile size_t hsz = 16;
        char8_t *hp = (char8_t *)malloc(hsz);
        if (!hp)
            return errs + 1;
        ind = 1;
        rc = u8norm_reorder_s(hp, RSIZE_MAX_STR + 1, (const char8_t *)"a", 1);
        ERR(ESLEMAX)
        ind = 1;
        rc = u8norm_compose_s(hp, RSIZE_MAX_STR + 1, (const char8_t *)"a", &ind,
                              false);
        ERR(ESLEMAX)
        INDCMP(!= 0)
        free(hp);
    }

    /* lenp is NULL */
    rc =
        u8norm_compose_s(out, sizeof(out), (const char8_t *)"a", vnulls, false);
    ERR(ESNULLP)

    /* cp > 0x10ffff is an error, not a composite which drops the starter */
    ind = 5;
    rc = u8norm_compose_s(out, sizeof(out),
                          (const char8_t *)"a\xF4\x9F\xBF\xBF", &ind, false);
    ERR(ESLEMAX)

    return errs;
}

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
    rc = u8norm_s(NULL, LEN, (const char8_t *)("test"), WCSNORM_NFD, NULL);
    ERR(ESNULLP);

    strcpy((char *)str, "Aᾳ");
    EXPECT_BOS("empty src")
    rc = u8norm_s(str, LEN, (const char8_t *)(NULL), WCSNORM_NFD, NULL);
    ERR(ESNULLP);
    EXPSTR((char *)str, "\0");

    EXPECT_BOS("empty dest or dmax")
    rc = u8norm_s(str, 0, (const char8_t *)("test"), WCSNORM_NFD, &ind);
    ERR(ESZEROL)
    INDZERO();
    EXPSTR((char *)str, "\0");

    ind = 4;
    EXPECT_BOS("dest overflow")
    rc = u8norm_s(str, RSIZE_MAX_STR + 1, (const char8_t *)("test"),
                  WCSNORM_NFD, &ind);
    ERR(ESLEMAX);
    EXPSTR((char *)str, "\0");
    INDCMP(!= 0)

    if (_BOS_KNOWN(str)) {
        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = u8norm_s(str, LEN + 1, (const char8_t *)("test"), WCSNORM_NFD,
                      &ind);
        ERR(EOVERFLOW);
        EXPSTR((char *)str, "\0");
        INDCMP(!= 0)
    }

    if (_BOS_KNOWN(str)) {
        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = u8norm_decompose_s(str, LEN + 1, (const char8_t *)("test"), &ind,
                                false);
        ERR(EOVERFLOW);
        EXPSTR((char *)str, "\0");
        INDCMP(!= 0)

        ind = 4;
        EXPECT_BOS("dest overflow")
        rc = u8norm_reorder_s(str, LEN + 1, (const char8_t *)("test"), ind);
        ERR(EOVERFLOW);
        EXPSTR((char *)str, "\0");

        EXPECT_BOS("dest overflow")
        rc = u8norm_compose_s(str, LEN + 1, (const char8_t *)("test"), &ind,
                              false);
        ERR(EOVERFLOW);
        EXPSTR((char *)str, "\0");
        INDCMP(!= 0)
    }
#endif

#define OVMAX_WC "\xf4\x9f\xbf\xbf" // 11ffff
#define MAX_WC1 "􏿰"              // 10fff0

    rc = u8norm_s(str, LEN, (const char8_t *)(OVMAX_WC), WCSNORM_NFD, NULL);
    ERR(ESLEMAX);
    EXPSTR((char *)str, "\0");

    rc = u8norm_decompose_s(str, LEN, (const char8_t *)(OVMAX_WC), NULL, false);
    ERR(ESLEMAX);
    EXPSTR((char *)str, "\0");

    rc = u8norm_decompose_s(str, LEN, (const char8_t *)(OVMAX_WC), NULL, true);
#ifdef HAVE_NORM_COMPAT
    ERR(ESLEMAX);
    EXPSTR((char *)str, "\0");
#else
    if (rc == EOF) {
        ERR(EOF);
    } else {
        debug_printf("%u TODO !norm-compat rc=%d\n", __LINE__, rc);
    }
#endif

    rc = u8norm_s(str, 4, (const char8_t *)(MAX_WC1), WCSNORM_NFD, NULL);
    ERR(ESLEMIN);
    EXPSTR((char *)str, "\0");

    /*--------------------------------------------------*/

    rc = u8norm_s(str, LEN, (const char8_t *)("Café"), WCSNORM_NFD, &ind);
    ERR(EOK);
    EXPSTR((char *)str, "Cafe\xcc\x81");
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    /*--------------------------------------------------*/

#ifndef __PGI
    rc = u8norm_s(str, LEN, (const char8_t *)("Café"), WCSNORM_NFD, &ind);
    ERR(EOK)
    EXPSTR((char *)str, "Cafe\xcc\x81");
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));
#endif

    rc = u8norm_s(str, LEN, (const char8_t *)("Café"), WCSNORM_NFC, &ind);
    ERR(EOK)
    EXPSTR((char *)str, "Café");
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    /*--------------------------------------------------*/

    strcpy((char *)str, "Aᾳ");
    rc = u8norm_s(str1, LEN, (const char8_t *)(str), WCSNORM_NFC, NULL);
    ERR(EOK);

    strcpy((char *)str, "Abcᾷ");
    rc = u8norm_s(str1, 6, (const char8_t *)(str), WCSNORM_NFD, NULL);
    ERR(ESNOSPC);
    EXPSTR((char *)str1, "\0");

#ifdef HAVE_NORM_COMPAT
    strcpy((char *)str, "A㈝");
    rc = u8norm_s(str1, 18, (const char8_t *)(str), WCSNORM_NFKC, NULL);
    ERR(ESLEMIN);
    EXPSTR((char *)str1, "\0");
#endif

    /* echo "Aᾳ" | unorm -n nfd | iconv -t UTF-32LE | od -h */
    rc = u8norm_s(str, LEN, (const char8_t *)("Aᾳ"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "A\xce\xb1\xcd\x85");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    /* Aᾳ => A≈ᾳ */
    rc = u8norm_s(str, LEN, (const char8_t *)("Aᾳ"), WCSNORM_NFC, &ind);
    ERR(EOK);
    strcpy((char *)str1, "Aᾳ");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("Aᾷ"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "A\xce\xb1\xcd\x82\xcd\x85");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("Aᾷ"), WCSNORM_NFC, &ind);
    ERR(EOK);
    strcpy((char *)str1, "Aᾷ");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    /* MUSICAL SYMBOL FUSA BLACK \xf0\x9d\x87\x80 */
    rc = u8norm_s(str, LEN, (const char8_t *)("𝆺𝅥𝅯"), WCSNORM_NFD, &ind);
    ERR(EOK);
    /* MUSICAL SYMBOL MINIMA BLACK (U+1D1BC) MUSICAL SYMBOL COMBINING FLAG-2
     * (U+1D16F) */
    /* => MUSICAL SYMBOL SEMIBREVIS BLACK (U+1D1BA) MUSICAL SYMBOL COMBINING
     * STEM (U+1D165) + U+1D16F */
    strcpy((char *)str1, "𝆺𝅥𝅯");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    /*--------------------------------------------------*/

    /* reordering */
    rc = u8norm_s(str, LEN, (const char8_t *)("Aᾷ"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "A\xce\xb1\xcd\x82\xcd\x85"); /* do reorder */
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("Aᾷ"), WCSNORM_FCD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "A\xce\xb1\xcd\x82\xcd\x85"); /* no reorder */
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("Aᾷ"), WCSNORM_NFC, &ind);
    ERR(EOK);
    strcpy((char *)str1, "Aᾷ"); /* nfc */
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("Aᾷ"), WCSNORM_FCC, &ind);
    ERR(EOK);
    strcpy((char *)str1, "Aᾷ"); /* the same in this case */
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    /*--------------------------------------------------*/

    rc = u8norm_s(str, LEN, (const char8_t *)("ā"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "a\xcc\x84");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("ā"), WCSNORM_NFC, &ind);
    ERR(EOK);
    strcpy((char *)str1, "ā");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("ĕ"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "e\xcc\x86");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("ß"), WCSNORM_NFD,
                  &ind); /* !nfd */
    ERR(EOK);
    strcpy((char *)str1, "ß");
    EXPSTR((char *)str, (char *)str1);
    INDCMP(!= (int)strlen((char *)str));
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)(";"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, ";"); /* NFKC => 3b */
    len = strlen((char *)str1);
    INDCMP(!= (int)len);
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("΅"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xc2\xa8\xcc\x81");
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xce\xac"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xce\xb1"
                         "\xcc\x81");
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xce\xad"), WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xce\xb5"
                         "\xcc\x81");
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xe1\xbd\xb1"), WCSNORM_NFD,
                  &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xce\xb1"
                         "\xcc\x81");
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xe1\xbc\x82"), WCSNORM_NFD,
                  &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xce\xb1"
                         "\xcc\x93"
                         "\xcc\x80");
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xe1\xbf\xab"), WCSNORM_NFD,
                  &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xce\xa5"
                         "\xcc\x81");
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("΅" /*\xe1\xbf\xae*/),
                  WCSNORM_NFD, &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xc2\xa8\xcc\x81");
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    /*--------------------------------------------------*/

    /* compat NFKD */
    /* echo "㈝" | unorm -n nfkd | iconv -t UTF-32LE | od -h */
    rc = u8norm_s(str, LEN, (const char8_t *)("㈝" /*\xe3\x88\x9d*/),
                  WCSNORM_NFKD, &ind); /* TODO wchar2 */
#ifdef HAVE_NORM_COMPAT
    ERR(EOK);
    strcpy((char *)str1,
           "(\xe1\x84\x8b\xe1\x85\xa9\xe1\x84\x8c\xe1\x85\xa5\xe1\x86\xab)");
    len = strlen((char *)str1);
    INDCMP_(!= len);
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));
#else
    if (rc == -1) {
        ERR(-1);
    } else {
        debug_printf("%u TODO !norm-compat rc=%d\n", __LINE__, rc);
    }
#endif

#ifdef HAVE_NORM_COMPAT
    rc = u8norm_s(str, LEN, (const char8_t *)("\xcd\xbe"), WCSNORM_NFKC, &ind);
    ERR(EOK);
    strcpy((char *)str1, "\x3b");
    len = strlen((char *)str1);
    INDCMP_(!= len);
    EXPSTR((char *)str, (char *)str1);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    /* (오전) */
    rc = u8norm_s(str, LEN, (const char8_t *)("\xe3\x88\x9d"), WCSNORM_NFKC,
                  &ind);
    ERR(EOK);
    strcpy((char *)str1, "(\xec\x98\xa4\xec\xa0\x84)");
    EXPSTR((char *)str, (char *)str1);
    len = strlen((char *)str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xef\xb7\xbb"), WCSNORM_NFKD,
                  &ind);
    ERR(EOK);
    strcpy((char *)str1,
           "\xd8\xac\xd9\x84\x20\xd8\xac\xd9\x84\xd8\xa7\xd9\x84\xd9\x87");
    EXPSTR((char *)str, (char *)str1);
    len = strlen((char *)str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xe2\x84\x83"), WCSNORM_NFKD,
                  &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xc2\xb0"
                         "\x43");
    EXPSTR((char *)str, (char *)str1);
    len = strlen((char *)str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xe2\x85\x90"), WCSNORM_NFKD,
                  &ind);
    ERR(EOK);
    strcpy((char *)str1, "\x31"
                         "\xe2\x81\x84"
                         "\x37");
    EXPSTR((char *)str, (char *)str1);
    len = strlen((char *)str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

    rc = u8norm_s(str, LEN, (const char8_t *)("\xe3\x8e\x82"), WCSNORM_NFKD,
                  &ind);
    ERR(EOK);
    strcpy((char *)str1, "\xce\xbc"
                         "\x41");
    EXPSTR((char *)str, (char *)str1);
    len = strlen((char *)str1);
    INDCMP_(!= len);
    CHECK_SLACK(&str[strlen((char *)str)], LEN - strlen((char *)str));

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
            // WPRINTLS(src);
            debug_printf("%s => %s", src, str);
            // WPRINTLSN(str);
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

    errs += check_norm_edges();

    return (errs);
}

int main(void) { return test_u8norm_s(); }
