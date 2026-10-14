/*------------------------------------------------------------------
 * test_u8fc_s
 * File 'extu8/u8fc_s.c'
 * Lines executed:90.48% of 42
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t dest[LEN];
static char8_t src[LEN];
/* minimal UTF-8 encoder for exhaustive codepoint sweeps */
static int enc(char8_t *o, unsigned cp) {
    if (cp < 0x80) {
        o[0] = (char8_t)cp;
        return 1;
    } else if (cp < 0x800) {
        o[0] = (char8_t)(0xC0 | (cp >> 6));
        o[1] = (char8_t)(0x80 | (cp & 0x3F));
        return 2;
    } else if (cp < 0x10000) {
        o[0] = (char8_t)(0xE0 | (cp >> 12));
        o[1] = (char8_t)(0x80 | ((cp >> 6) & 0x3F));
        o[2] = (char8_t)(0x80 | (cp & 0x3F));
        return 3;
    }
    o[0] = (char8_t)(0xF0 | (cp >> 18));
    o[1] = (char8_t)(0x80 | ((cp >> 12) & 0x3F));
    o[2] = (char8_t)(0x80 | ((cp >> 6) & 0x3F));
    o[3] = (char8_t)(0x80 | (cp & 0x3F));
    return 4;
}

int test_u8fc_s(void);

int test_u8fc_s(void) {
    errno_t rc;
    rsize_t len;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = u8fc_s(NULL, LEN, src, &len);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = u8fc_s(dest, 0, src, &len);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = u8fc_s(dest, RSIZE_MAX_STR + 1, src, &len);
    ERR(ESLEMAX)
#endif

    strcpy((char *)src, "hi");
#ifndef HAVE_CT_BOS_OVR
    rc = u8fc_s(dest, LEN, NULL, &len);
    ERR(ESNULLP)
#endif

    /*--------------------------------------------------*/

    /* simple ASCII fold via towlower */
    strcpy((char *)src, "Hello World");
    rc = u8fc_s(dest, LEN, src, &len);
    ERR(EOK)
    if (strcmp((char *)dest, "hello world") != 0) {
        debug_printf("%s %u  dest=%s, expected 'hello world'\n", __FUNCTION__,
                     __LINE__, dest);
        errs++;
    }
    if (len != strlen("hello world")) {
        debug_printf("%s %u  len=%zu, expected %zu\n", __FUNCTION__, __LINE__,
                     (size_t)len, strlen("hello world"));
        errs++;
    }

    /*--------------------------------------------------*/

    /* full fold-case multi-char expansion: U+00DF (sharp s) -> "ss" */
    strcpy((char *)src, "stra\xC3\x9F"
                        "e");
    rc = u8fc_s(dest, LEN, src, &len);
    ERR(EOK)
    if (strcmp((char *)dest, "strasse") != 0) {
        debug_printf("%s %u  dest=%s, expected strasse\n", __FUNCTION__,
                     __LINE__, dest);
        errs++;
    }

    /*--------------------------------------------------*/

    /* multibyte simple fold: U+00C9 -> U+00E9 */
    strcpy((char *)src, "CAF\xC3\x89");
    rc = u8fc_s(dest, LEN, src, &len);
    ERR(EOK)
    if (strcmp((char *)dest, "caf\xC3\xA9") != 0) {
        debug_printf("%s %u  dest=%s, expected caf+e-acute\n", __FUNCTION__,
                     __LINE__, dest);
        errs++;
    }

    /*--------------------------------------------------*/

    /* result must fit */
    strcpy((char *)src, "Hello World");
    rc = u8fc_s(dest, 3, src, &len);
    ERR(ESNOSPC)
    if (len != strlen("hello world")) {
        debug_printf("%s %u  len=%zu, expected %zu even on ESNOSPC\n",
                     __FUNCTION__, __LINE__, (size_t)len,
                     strlen("hello world"));
        errs++;
    }

    /*--------------------------------------------------*/

    /* exact full-fold results from the special-case tables */
    {
        static const struct {
            const char *in;
            const char *out;
        } tbl[] = {
            {"\xC4\xB0", "i\xCC\x87"},                 /* U+0130 */
            {"\xC5\x89", "\xCA\xBCn"},                 /* U+0149 */
            {"\xEF\xAC\x83", "ffi"},                   /* U+FB03 */
            {"\xEF\xAC\x84", "ffl"},                   /* U+FB04 */
            {"\xE1\xBF\xB7", "\xCF\x89\xCD\x82\xCE\xB9"}, /* U+1FF7 */
            {"\xF0\x9D\xBE\x95", "ss"},               /* U+1DF95 */
            {"\xC2\xB5", "\xCE\xBC"},                 /* U+00B5 */
            {"\xC5\xBF", "s"},                         /* U+017F */
            {"\xCD\x85", "\xCE\xB9"},                 /* U+0345 */
            {"\xCF\x82", "\xCF\x83"},                 /* U+03C2 */
            {"\xCF\x91", "\xCE\xB8"},                 /* U+03D1 */
            {"\xCF\xB1", "\xCF\x81"},                 /* U+03F1 */
            {"\xE1\xBA\x9E", "ss"},                   /* U+1E9E */
        };
        size_t i;
        for (i = 0; i < sizeof(tbl) / sizeof(tbl[0]); i++) {
            strcpy((char *)src, tbl[i].in);
            rc = u8fc_s(dest, LEN, src, &len);
            ERR(EOK)
            if (strcmp((char *)dest, tbl[i].out) != 0) {
                debug_printf("%s %u  entry %zu: fold mismatch\n", __FUNCTION__,
                             __LINE__, i);
                errs++;
            }
            if (len != strlen(tbl[i].out)) {
                debug_printf("%s %u  entry %zu: len %zu\n", __FUNCTION__,
                             __LINE__, i, (size_t)len);
                errs++;
            }
        }
    }

    /*--------------------------------------------------*/

    /* sweep all scalar values: fold succeeds and is idempotent */
    {
        static char8_t dest2[LEN];
        rsize_t len2;
        unsigned cp;
        for (cp = 1; cp <= 0x10FFFF; cp++) {
            if (cp >= 0xD800 && cp <= 0xDFFF)
                continue;
            src[enc(src, cp)] = 0;
            rc = u8fc_s(dest, LEN, src, &len);
            if (rc != EOK || len == 0 || len != strlen((char *)dest)) {
                debug_printf("%s %u  U+%04X rc=%d len=%zu\n", __FUNCTION__,
                             __LINE__, cp, (int)rc, (size_t)len);
                errs++;
                continue;
            }
            rc = u8fc_s(dest2, LEN, dest, &len2);
            if (rc != EOK || len2 != len || strcmp((char *)dest, (char *)dest2)) {
                debug_printf("%s %u  U+%04X not idempotent\n", __FUNCTION__,
                             __LINE__, cp);
                errs++;
            }
        }
    }

    /*--------------------------------------------------*/

    /* illegal UTF-8 in src */
    strcpy((char *)src, "a\xC0\x80");
    strcpy((char *)dest, "zzz");
    rc = u8fc_s(dest, LEN, src, &len);
    ERR(EILSEQ)
    if (dest[0] != '\0') {
        debug_printf("%s %u  dest not cleared on EILSEQ\n", __FUNCTION__,
                     __LINE__);
        errs++;
    }

    /*--------------------------------------------------*/

    /* unknown dest size: dmax > RSIZE_MAX_STR */
    {
        static volatile size_t usize = 32;
        char8_t *ubuf = (char8_t *)malloc(usize);
        if (ubuf) {
            strcpy((char *)src, "Ab");
            rc = u8fc_s(ubuf, RSIZE_MAX_STR + 1, src, &len);
            ERR(ESLEMAX)
            rc = u8fc_s(ubuf, usize, src, &len);
            ERR(EOK)
            EXPSTR((char *)ubuf, "ab")
            free(ubuf);
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8fc_s()); }
