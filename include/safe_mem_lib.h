/*------------------------------------------------------------------
 * safe_mem_lib.h -- Safe C Library Memory APIs
 *
 * October 2008, Bo Berry
 * Modified 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 * Modified September 2017, Reini Urban
 *
 * Copyright (c) 2008-2012 by Cisco Systems, Inc.
 * Copyright (c) 2017-2018 by Reini Urban
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#ifndef __SAFE_MEM_LIB_H__
#define __SAFE_MEM_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "safe_config.h"
#include "safe_lib_errno.h"
#include "safe_types.h"
#include "safe_compile.h"

#ifndef SAFECLIB_DISABLE_WCHAR
#include <wchar.h>
#endif

#if defined _WIN32 && !defined(DISABLE_DLLIMPORT)
#if defined(EXPORT) && defined(__SAFECLIB_PRIVATE_H__)
#define EXTERN extern __declspec(dllexport)
#else
#define EXTERN extern __declspec(dllimport)
#endif
#else
#define EXTERN extern
#endif

#define RSIZE_MAX_MEM16 (RSIZE_MAX_MEM / 2)
#define RSIZE_MAX_MEM32 (RSIZE_MAX_MEM / 4)
#define RSIZE_MAX_WMEM (RSIZE_MAX_MEM / sizeof(wchar_t))

/* set memory constraint handler */
EXTERN constraint_handler_t
set_mem_constraint_handler_s(constraint_handler_t handler);

/* copy memory */
EXTERN errno_t _memcpy_s_chk(void *restrict dest, rsize_t dmax,
                             const void *restrict src, rsize_t slen,
                             const size_t destbos, const size_t srcbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2_BUTZERO(slen, dmax)
            BOS_ATTR(_BOS_KNOWN(dest) && _BOS_KNOWN(src) &&
                         ((dest > src && (char *)dest < (char *)src + slen) ||
                          (src > dest && (char *)src < (char *)dest + dmax)),
                     "dest overlaps with src");
#define memcpy_s(dest, dmax, src, slen)                                        \
    _memcpy_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* move memory, including overlapping memory */
EXTERN errno_t _memmove_s_chk(void *dest, rsize_t dmax, const void *src,
                              rsize_t slen, const size_t destbos,
                              const size_t srcbos) BOS_CHK_BUTZERO(dest, slen)
    BOS_OVR2_BUTZERO(src, slen) VAL_OVR2_BUTZERO(slen, dmax);
#define memmove_s(dest, dmax, src, slen)                                       \
    _memmove_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* set bytes. now __STDC_WANT_LIB_EXT1__ >= 1 apple/freebsd string.h compatible
 */
#if !defined(HAVE_MEMSET_S) ||                                                 \
    !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
#define memset_s(dest, dmax, value, n)                                         \
    _memset_s_chk(dest, dmax, value, n, BOS(dest))
#endif
EXTERN errno_t _memset_s_chk(void *dest, rsize_t dmax, int value, rsize_t n,
                             const size_t destbos) BOS_NULL(dest)
    BOS_CHK_BUTZERO(dest, n)
        BOS_ATTR(n && (_BOS_OVR(dest, n) || n > dmax), "n overflow >dest/dmax")
            VAL_OVR2(value, 255);

#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* set uint16_t */
EXTERN errno_t _memset16_s_chk(uint16_t *dest, rsize_t dmax, uint16_t value,
                               rsize_t n, const size_t destbos) BOS_NULL(dest)
    BOS_CHK_BUTZERO(dest, n)
        BOS_ATTR(n && (_BOS_OVR(dest, n * 2) || n * 2 > dmax),
                 "n*2 overflow >dest/dmax");
#define memset16_s(dest, dmax, value, n)                                       \
    _memset16_s_chk(dest, dmax, value, n, BOS(dest))

/* set uint32_t */
EXTERN errno_t _memset32_s_chk(uint32_t *dest, rsize_t dmax, uint32_t value,
                               rsize_t n, const size_t destbos) BOS_NULL(dest)
    BOS_CHK_BUTZERO(dest, n)
        BOS_ATTR(n && (_BOS_OVR(dest, n * 4) || n * 4 > dmax),
                 "n*4 overflow >dest/dmax");
#define memset32_s(dest, dmax, value, n)                                       \
    _memset32_s_chk(dest, dmax, value, n, BOS(dest))

/* compare memory */
EXTERN errno_t _memcmp_s_chk(const void *dest, rsize_t dmax, const void *src,
                             rsize_t slen, int *diff, const size_t destbos,
                             const size_t srcbos) BOS_CHK(dest)
    BOS_OVR2(src, slen) BOS_NULL(diff) BOS_ATTR(!slen, "empty slen")
        VAL_OVR2_BUTZERO(slen, dmax);
#define memcmp_s(dest, dmax, src, slen, diff)                                  \
    _memcmp_s_chk(dest, dmax, src, slen, diff, BOS(dest), BOS(src))

/* compare uint16_t memory */
EXTERN errno_t _memcmp16_s_chk(const uint16_t *dest, rsize_t dlen,
                               const uint16_t *src, rsize_t slen, int *diff,
                               const size_t destbos, const size_t srcbos)
    BOS_CHK2(dest, dlen * 2) BOS_CHK2(src, slen * 2) BOS_NULL(diff)
        VAL_OVR2_BUTZERO(slen, dlen);
#define memcmp16_s(dest, dlen, src, slen, diff)                                \
    _memcmp16_s_chk(dest, dlen, src, slen, diff, BOS(dest), BOS(src))

/* compare uint32_t memory */
EXTERN errno_t _memcmp32_s_chk(const uint32_t *dest, rsize_t dlen,
                               const uint32_t *src, rsize_t slen, int *diff,
                               const size_t destbos, const size_t srcbos)
    BOS_CHK2(dest, dlen * 4) BOS_CHK2(src, slen * 4) BOS_NULL(diff)
        VAL_OVR2_BUTZERO(slen, dlen);
#define memcmp32_s(dest, dlen, src, slen, diff)                                \
    _memcmp32_s_chk(dest, dlen, src, slen, diff, BOS(dest), BOS(src))

/* copy uint16_t memory */
EXTERN errno_t _memcpy16_s_chk(uint16_t *dest, rsize_t dmax,
                               const uint16_t *src, rsize_t slen,
                               const size_t destbos, const size_t srcbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2_BUTZERO(slen, dmax / 2);
#define memcpy16_s(dest, dmax, src, slen)                                      \
    _memcpy16_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* copy uint32_t memory */
EXTERN errno_t _memcpy32_s_chk(uint32_t *dest, rsize_t dmax,
                               const uint32_t *src, rsize_t slen,
                               const size_t destbos, const size_t srcbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2_BUTZERO(slen, dmax / 4);
#define memcpy32_s(dest, dmax, src, slen)                                      \
    _memcpy32_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* uint16_t move memory, including overlapping memory */
EXTERN errno_t _memmove16_s_chk(uint16_t *dest, rsize_t dmax,
                                const uint16_t *src, rsize_t slen,
                                const size_t destbos, const size_t srcbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2_BUTZERO(slen, dmax / 2);
#define memmove16_s(dest, dmax, src, slen)                                     \
    _memmove16_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* uint32_t move memory, including overlapping memory */
EXTERN errno_t _memmove32_s_chk(uint32_t *dest, rsize_t dmax,
                                const uint32_t *src, rsize_t slen,
                                const size_t destbos, const size_t srcbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2_BUTZERO(slen, dmax / 4);
#define memmove32_s(dest, dmax, src, slen)                                     \
    _memmove32_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* byte zero */
EXTERN errno_t _memzero_s_chk(void *dest, rsize_t dmax, const size_t destbos)
    BOS_CHK(dest);
#define memzero_s(dest, len) _memzero_s_chk(dest, len, BOS(dest))

/* uint16_t zero */
EXTERN errno_t _memzero16_s_chk(uint16_t *dest, rsize_t len,
                                const size_t destbos) BOS_CHK2(dest, len * 2);
#define memzero16_s(dest, len) _memzero16_s_chk(dest, len, BOS(dest))

/* uint32_t zero */
EXTERN errno_t _memzero32_s_chk(uint32_t *dest, rsize_t len,
                                const size_t destbos) BOS_CHK2(dest, len * 4);
#define memzero32_s(dest, len) _memzero32_s_chk(dest, len, BOS(dest))

/* find a byte */
EXTERN errno_t _memchr_s_chk(const void *restrict dest, rsize_t dmax,
                             const int ch, void **result, const size_t destbos)
    BOS_CHK(dest) VAL_OVR2(ch, 255) BOS_NULL(result);
#define memchr_s(dest, dmax, ch, result)                                       \
    _memchr_s_chk(dest, dmax, ch, result, BOS(dest))

EXTERN errno_t _memrchr_s_chk(const void *restrict dest, rsize_t dmax,
                              const int ch, void **result, const size_t destbos)
    BOS_CHK(dest) VAL_OVR2(ch, 255) BOS_NULL(result);
#define memrchr_s(dest, dmax, ch, result)                                      \
    _memrchr_s_chk(dest, dmax, ch, result, BOS(dest))

/* timing-safe byte sequence comparisons
   (already in OpenBSD, cygwin, newlib, FreeBSD) */
EXTERN int _timingsafe_bcmp_chk(const void *b1, const void *b2, size_t n,
                                const size_t destbos, const size_t srcbos)
    BOS_OVR2(b1, n) BOS_OVR2(b2, n);
#define timingsafe_bcmp(b1, b2, n)                                             \
    _timingsafe_bcmp_chk(b1, b2, n, BOS(b1), BOS(b2))

EXTERN int _timingsafe_memcmp_chk(const void *b1, const void *b2, size_t len,
                                  const size_t destbos, const size_t srcbos)
    BOS_OVR2(b1, len) BOS_OVR2(b2, len);
#define timingsafe_memcmp(b1, b2, len)                                         \
    _timingsafe_memcmp_chk(b1, b2, len, BOS(b1), BOS(b2))

/* copy string until character found (FreeBSD) */
EXTERN errno_t _memccpy_s_chk(void *dest, rsize_t dmax, const void *src, int c,
                              rsize_t n, const size_t destbos,
                              const size_t srcbos) BOS_CHK(dest)
    BOS_OVR2(src, n) VAL_OVR2(c, 255) VAL_OVR2_BUTZERO(n, dmax);
#define memccpy_s(dest, dmax, src, c, n)                                       \
    _memccpy_s_chk(dest, dmax, src, c, n, BOS(dest), BOS(src))

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

#ifndef SAFECLIB_DISABLE_WCHAR
/* wchar (of wchar_t size 2 or 4)
   disabled if your system has no wchar support.
 */

EXTERN errno_t _wmemcpy_s_chk(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict src, rsize_t smax,
                              const size_t destbos, const size_t srcbos)
    BOSW_CHK_BUTZERO(dest, smax) BOSW_OVR2_BUTZERO(src, smax);
#define wmemcpy_s(dest, dlen, src, count)                                      \
    _wmemcpy_s_chk(dest, dlen, src, count, BOS(dest), BOS(src))

EXTERN errno_t _wmemmove_s_chk(wchar_t *dest, rsize_t dmax, const wchar_t *src,
                               rsize_t smax, const size_t destbos,
                               const size_t srcbos) BOSW_CHK_BUTZERO(dest, smax)
    BOSW_OVR2_BUTZERO(src, smax);
#define wmemmove_s(dest, dlen, src, count)                                     \
    _wmemmove_s_chk(dest, dlen, src, count, BOS(dest), BOS(src))

#ifndef SAFECLIB_DISABLE_EXTENSIONS

EXTERN errno_t _wmemcmp_s_chk(const wchar_t *dest, rsize_t dmax,
                              const wchar_t *src, rsize_t slen, int *diff,
                              const size_t destbos, const size_t srcbos)
    BOSW_CHK(dest) BOSW_CHK2(src, slen) BOS_NULL(diff);
#define wmemcmp_s(dest, dlen, src, slen, diff)                                 \
    _wmemcmp_s_chk(dest, dlen, src, slen, diff, BOS(dest), BOS(src))

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

#endif /* SAFECLIB_DISABLE_WCHAR */

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_MEM_LIB_H__ */
