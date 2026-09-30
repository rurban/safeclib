#ifndef __U8_PRIVATE_H__
#define __U8_PRIVATE_H__
#include <inttypes.h>
#include <stdbool.h>

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#if __has_attribute(__unused__) || defined(__GNUC__)
#define U8_UNUSED __attribute__((__unused__))
#else
#define U8_UNUSED
#endif

static int utf8_len(const char8_t ch) U8_UNUSED;
static uint32_t dec_utf8(char8_t** strp) U8_UNUSED;
static int enc_utf8(char8_t *dest, const uint32_t cp) U8_UNUSED;
static int u8_seqlen(const char8_t *s, size_t n) U8_UNUSED;
static bool u8_is_valid(const char8_t *s, rsize_t smax) U8_UNUSED;

/* from https://rosettacode.org/wiki/UTF-8_encode_and_decode#C */
typedef struct {
    uint8_t mask; /* char data will be bitwise AND with this */
    uint8_t lead; /* start bytes of current char in utf-8 encoded character */
    uint32_t beg; /* beginning of codepoint range */
    uint32_t end; /* end of codepoint range */
    int bits_stored; /* number of bits from the codepoint that fits in char */
} _utf_t;

static const _utf_t *utf[] = {
    /*             mask                 lead                beg      end    bits */
    [0] = &(_utf_t){0x3f/*0b00111111*/, 0x80/*0b10000000*/, 0,       0,        6},
    [1] = &(_utf_t){0x7f/*0b01111111*/, 0x00/*0b00000000*/, 0000,    0177,     7},
    [2] = &(_utf_t){0x1f/*0b00011111*/, 0xc0/*0b11000000*/, 0200,    03777,    5},
    [3] = &(_utf_t){0x0f/*0b00001111*/, 0xe0/*0b11100000*/, 04000,   0177777,  4},
    [4] = &(_utf_t){0x07/*0b00000111*/, 0xf0/*0b11110000*/, 0200000, 04177777, 3},
    NULL,
};

#if 0
static int cp_len(const uint32_t cp) {
    int len = 0;
    for (_utf_t **u = (_utf_t **)utf; *u; ++u) {
        if ((cp >= (*u)->beg) && (cp <= (*u)->end)) {
            break;
        }
        ++len;
    }
#if 0 /* error must be handled in caller */
  if (len > 4) { /* Out of bounds */
    invoke_safe_str_constraint_handler("u8norm_s: "
                                       "illegal UTF-8 character",
                                       NULL, EILSEQ);
  }
#endif
    return len;
}
#endif

static int utf8_len(const char8_t ch) {
    int len = 0;
    for (_utf_t **u = (_utf_t **)utf; *u; ++u) {
        if ((ch & ~(*u)->mask) == (*u)->lead) {
            break;
        }
        ++len;
    }
#if 0 /* error must be handled in caller */
  if (len > 4) { /* Malformed leading byte */
    invoke_safe_str_constraint_handler("u8norm_s: "
                                       "illegal UTF-8 character",
                                       NULL, EILSEQ);
  }
#endif
    return len;
}

/* convert utf8 to unicode codepoint (to_cp) */
static uint32_t dec_utf8(char8_t** strp) {
    const char8_t *restrict str = *strp;
    int bytes = utf8_len(*str);
    int shift;
    uint32_t codep;

    if (bytes > 4 || bytes < 1) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "illegal UTF-8 character",
                                           NULL, EILSEQ);
        *strp = (char8_t *)(str + 1); /* skip the bad byte: guarantee
                                          forward progress for callers
                                          looping on dec_utf8 */
        return 0;
    }
    shift = utf[0]->bits_stored * (bytes - 1);
    codep = (*str++ & utf[bytes]->mask) << shift;
    for (int i = 1; i < bytes; ++i, ++str) {
        shift -= utf[0]->bits_stored;
        codep |= (*str & utf[0]->mask) << shift;
    }
    *strp = (char8_t*)str;
    return codep;
}

/* convert unicode codepoint to utf8, writing 1-4 bytes to dest
   (not NUL-terminated). returns the number of bytes written, or 0
   for an illegal codepoint (surrogate half or > 0x10ffff). */
static int enc_utf8(char8_t *dest, const uint32_t cp) {
    if (cp < 0x80) {
        dest[0] = (char8_t)cp;
        return 1;
    } else if (cp < 0x800) {
        dest[0] = (char8_t)(0xc0 | (cp >> 6));
        dest[1] = (char8_t)(0x80 | (cp & 0x3f));
        return 2;
    } else if (cp >= 0xd800 && cp <= 0xdfff) {
        invoke_safe_str_constraint_handler("u8: illegal UTF-8 codepoint "
                                           "(surrogate half)",
                                           NULL, EILSEQ);
        return 0;
    } else if (cp < 0x10000) {
        dest[0] = (char8_t)(0xe0 | (cp >> 12));
        dest[1] = (char8_t)(0x80 | ((cp >> 6) & 0x3f));
        dest[2] = (char8_t)(0x80 | (cp & 0x3f));
        return 3;
    } else if (cp <= 0x10ffff) {
        dest[0] = (char8_t)(0xf0 | (cp >> 18));
        dest[1] = (char8_t)(0x80 | ((cp >> 12) & 0x3f));
        dest[2] = (char8_t)(0x80 | ((cp >> 6) & 0x3f));
        dest[3] = (char8_t)(0x80 | (cp & 0x3f));
        return 4;
    } else {
        invoke_safe_str_constraint_handler("u8: illegal UTF-8 codepoint "
                                           "(> U+10FFFF)",
                                           NULL, EILSEQ);
        return 0;
    }
}

/* validates and measures the well-formed UTF-8 sequence starting at s,
   of which at most n bytes are available. Rejects truncated sequences,
   invalid continuation bytes, overlong encodings, encoded surrogate
   halves and codepoints beyond U+10FFFF (RFC 3629). Returns the sequence
   length in bytes (1-4), or 0 if s does not start a well-formed
   sequence. */
static int u8_seqlen(const char8_t *s, size_t n) {
    const uint8_t c0 = (uint8_t)s[0];
    size_t len;
    uint32_t cp, min_cp;

    if (c0 < 0x80) {
        return 1;
    } else if ((c0 & 0xe0) == 0xc0) {
        len = 2;
        cp = c0 & 0x1f;
        min_cp = 0x80;
    } else if ((c0 & 0xf0) == 0xe0) {
        len = 3;
        cp = c0 & 0x0f;
        min_cp = 0x800;
    } else if ((c0 & 0xf8) == 0xf0) {
        len = 4;
        cp = c0 & 0x07;
        min_cp = 0x10000;
    } else {
        return 0; /* stray continuation byte or 0xf8-0xff */
    }

    if (n < len)
        return 0; /* truncated multi-byte sequence */

    for (size_t i = 1; i < len; i++) {
        const uint8_t ci = (uint8_t)s[i];
        if ((ci & 0xc0) != 0x80)
            return 0; /* missing/invalid continuation byte */
        cp = (cp << 6) | (ci & 0x3f);
    }
    if (cp < min_cp)
        return 0; /* overlong encoding */
    if (cp >= 0xd800 && cp <= 0xdfff)
        return 0; /* encoded surrogate half */
    if (cp > 0x10ffff)
        return 0; /* beyond Unicode range */

    return (int)len;
}

/* validates a NUL-terminated utf-8 string, not reading past smax bytes.
   returns true if str is well-formed UTF-8, false on the first illegal
   or truncated sequence. */
static bool u8_is_valid(const char8_t *s, rsize_t smax) {
    while (smax && *s) {
        const int len = u8_seqlen(s, smax);
        if (!len)
            return false;
        s += len;
        smax -= (rsize_t)len;
    }
    return true;
}

#endif
