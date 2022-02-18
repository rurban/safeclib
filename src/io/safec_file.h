#ifndef __SAFEC_FILE_H__
#define __SAFEC_FILE_H__

//#include "stdio_impl.h"

// from musl, MIT Licensed

// struct _SAFEC_FILE;
typedef struct _SAFEC_FILE {
    FILE *f;
    unsigned char *buf;
    size_t buf_size;
    void *cookie;
    unsigned char *rpos, *rend;
    int (*close)(struct _SAFEC_FILE *);
    unsigned char *wend, *wpos;
    unsigned char *mustbezero_1;
    unsigned char *wbase;
    size_t (*read)(struct _SAFEC_FILE *, unsigned char *, size_t);
    size_t (*write)(struct _SAFEC_FILE *, const unsigned char *, size_t);
    off_t (*seek)(struct _SAFEC_FILE *, off_t, int);
    volatile int lock;
    unsigned char *shend;
    off_t shlim, shcnt;
} _SAFEC_FILE;

/* Scan helper "stdio" functions for use by scanf-family and strto*-family
 * functions. These accept either a valid stdio FILE, or a minimal pseudo
 * FILE whose buffer pointers point into a null-terminated string. In the
 * latter case, the sh_fromstring macro should be used to setup the FILE;
 * the rest of the structure can be left uninitialized.
 *
 * To begin using these functions, shlim must first be called on the FILE
 * to set a field width limit, or 0 for no limit. For string pseudo-FILEs,
 * a nonzero limit is not valid and produces undefined behavior. After that,
 * shgetc, shunget, and shcnt are valid as long as no other stdio functions
 * are called on the stream.
 *
 * When used with a real FILE object, shunget has only one byte of pushback
 * available. Further shunget (up to a limit of the stdio UNGET buffer size)
 * will adjust the position but will not restore the data to be read again.
 * This functionality is needed for the wcsto*-family functions, where it's
 * okay because the FILE will be discarded immediately anyway. When used
 * with string pseudo-FILEs, shunget has unlimited pushback, back to the
 * beginning of the string. */

#define shcnt(sf) ((sf)->shcnt + ((sf)->rpos - (sf)->buf))
#define shlim(sf, lim) /*__shlim((sf), (lim))*/
#define shgetc(sf) (((sf)->rpos != (sf)->shend) ? *(sf)->rpos++ : getc((sf)->f))
#define shunget(sf) ((sf)->shlim >= 0 ? (void)(sf)->rpos-- : (void)0)

#define shgetwc(sf)                                                            \
    ((sf)->rpos != (sf)->rend && *(sf)->rpos < 128 ? *(sf)->rpos++             \
                                                   : getwc(sf->f))

#define shungetwc(c, sf)                                                       \
    ((sf)->rend && (c) < 128 ? *--(sf)->rpos : ungetwc((c), (sf->f)))

#define sh_fromstring(sf, s)                                                   \
    ((sf)->buf = (sf)->rpos = (void *)(s), (sf)->rend = (void *)-1)

size_t safec_string_read(_SAFEC_FILE *sf, unsigned char *buf, size_t len);
size_t safec_wstring_read(_SAFEC_FILE *sf, unsigned char *buf, size_t len);

int safec_vfscanf_s(_SAFEC_FILE *sf, const char *funcname, const char *fmt,
                    va_list ap);
int safec_vfwscanf_s(_SAFEC_FILE *sf, const char *funcname, const wchar_t *fmt,
                     va_list ap);

#endif // __SAFEC_FILE_H__
