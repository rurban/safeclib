#ifndef __U8_PRIVATE_H__
#define __U8_PRIVATE_H__
#include <inttypes.h>

static int utf8_len(const char8_t ch);
static uint32_t dec_utf8(char8_t** strp);
#if 0
static int cp_len(const uint32_t cp);
static char8_t *enc_utf8(char8_t *dest, const uint32_t cp);
#endif

/* from https://rosettacode.org/wiki/UTF-8_encode_and_decode#C */
typedef struct {
    uint8_t mask; /* char data will be bitwise AND with this */
    uint8_t lead; /* start bytes of current char in utf-8 encoded character */
    uint32_t beg; /* beginning of codepoint range */
    uint32_t end; /* end of codepoint range */
    int bits_stored; /* number of bits from the codepoint that fits in char */
} _utf_t;

_utf_t *utf[] = {
    /*             mask                 lead                beg      end    bits */
    [0] = &(_utf_t){0x3f/*0b00111111*/, 0x80/*0b10000000*/, 0,       0,        6},
    [1] = &(_utf_t){0x7f/*0b01111111*/, 0x00/*0b00000000*/, 0000,    0177,     7},
    [2] = &(_utf_t){0x1f/*0b00011111*/, 0xc0/*0b11000000*/, 0200,    03777,    5},
    [3] = &(_utf_t){0x0f/*0b00001111*/, 0xe0/*0b11100000*/, 04000,   0177777,  4},
    [4] = &(_utf_t){0x07/*0b00000111*/, 0xf0/*0b11110000*/, 0200000, 04177777, 3},
    &(_utf_t){0},
};

#if 0
static int cp_len(const uint32_t cp) {
    int len = 0;
    for (_utf_t **u = utf; *u; ++u) {
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
    for (_utf_t **u = utf; *u; ++u) {
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

    if (bytes > 4) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "illegal UTF-8 character",
                                           NULL, EILSEQ);
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

#if 0
/* convert unicode codepoint to utf8 (to_utf8) */
static char8_t *enc_utf8(char8_t *dest, const uint32_t cp) {
    const int bytes = cp_len(cp);

    if (bytes > 4) {
        invoke_safe_str_constraint_handler("u8norm_s: "
                                           "illegal UTF-8 character",
                                           NULL, EILSEQ);
        return 0;
    } else {
        int shift = utf[0]->bits_stored * (bytes - 1);
        dest[0] = (cp >> shift & utf[bytes]->mask) | utf[bytes]->lead;
        shift -= utf[0]->bits_stored;
        for (int i = 1; i < bytes; ++i) {
            dest[i] = (cp >> shift & utf[0]->mask) | utf[0]->lead;
            shift -= utf[0]->bits_stored;
        }
        dest[bytes] = '\0';
        return dest;
    }
}
#endif

#endif
