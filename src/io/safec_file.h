#ifndef __SAFEC_FILE_H__
#define __SAFEC_FILE_H__

/* Pseudo-FILE used by the scanf_s family, adapted from musl's internal
 * FILE/shgetc machinery (MIT licensed). It wraps either
 *  - a real stdio FILE* (fscanf_s/vfscanf_s/scanf_s/vscanf_s,
 *    fwscanf_s/vfwscanf_s/wscanf_s/vwscanf_s): `f' is set, `buf' is a
 *    scratch refill buffer owned by the caller;
 *  - an in-memory C string (sscanf_s/vsscanf_s): `f' is NULL, `buf' is
 *    the source string itself (buf_size left 0, so the first shgetc()
 *    immediately pulls through `read' == safec_string_read);
 *  - an in-memory wide string (swscanf_s/vswscanf_s): `f' is NULL,
 *    `cookie' is walked directly as a wchar_t* by the wide scanner,
 *    which never touches `buf'/`rpos'/`rend' at all.
 */
typedef struct _SAFEC_FILE {
    FILE *f;
    unsigned char *buf;
    size_t buf_size;
    void *cookie;
    unsigned char *rpos, *rend;
    size_t (*read)(struct _SAFEC_FILE *, unsigned char *, size_t);
    volatile int lock;
    unsigned char *shend;
    off_t shlim, shcnt;
} _SAFEC_FILE;

/* Scan helper "stdio" functions for use by the scanf_s family. To
 * begin using these, shlim must first be called on the FILE to set a
 * field width limit, or 0 for no limit. After that, shgetc, shunget,
 * and shcnt are valid as long as no other stdio functions are called
 * on the stream. */

#define shcnt(sf) ((sf)->shcnt + ((sf)->rpos - (sf)->buf))
#define shlim(sf, lim) safec_shlim((sf), (lim))
#define shgetc(sf) (((sf)->rpos != (sf)->shend) ? *(sf)->rpos++ : safec_shgetc(sf))
#define shunget(sf) ((sf)->shlim >= 0 ? (void)(sf)->rpos-- : (void)0)

/* Ensures sf->rpos/rend reflect an empty window, forcing the next
 * shgetc() to refill through sf->read(). Safe to call unconditionally
 * (including repeatedly); mirrors musl's __toread(). */
void safec_toread(_SAFEC_FILE *sf);
void safec_shlim(_SAFEC_FILE *sf, off_t lim);
int safec_shgetc(_SAFEC_FILE *sf);

/* sf->read callback for a plain NUL-terminated char* source
 * (sscanf_s/vsscanf_s); sf->cookie is the remaining string. */
size_t safec_string_read(_SAFEC_FILE *sf, unsigned char *buf, size_t len);

/* sf->read callback for a real stdio stream (fscanf_s/vfscanf_s/
 * scanf_s/vscanf_s); sf->f is the stream, sf->buf/buf_size the
 * caller-owned scratch buffer to refill through fread(). */
size_t safec_stream_read(_SAFEC_FILE *sf, unsigned char *buf, size_t len);

/* chk_destsize: when nonzero, %c/%s/%[ (with a non-suppressed, non-%m
 * destination) consume an extra rsize_t destination-size argument
 * immediately after the destination pointer and enforce it, per the
 * Annex K two-argument convention -- raising ESNOSPC and returning
 * EOF instead of writing past it. Every scanf_s family caller passes
 * 1 (Annex K mandates the argument for every %c/%s/%[ conversion). */
int safec_vfscanf_s(_SAFEC_FILE *sf, const char *funcname, const char *fmt,
                    va_list ap, int chk_destsize);
int safec_vfwscanf_s(_SAFEC_FILE *sf, const char *funcname, const wchar_t *fmt,
                     va_list ap, int chk_destsize);

#endif // __SAFEC_FILE_H__
