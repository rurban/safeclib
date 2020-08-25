/*------------------------------------------------------------------
 * safe_lib.h -- Safe C Library
 *
 * Modified 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 * September 2017, Reini Urban
 *
 * Copyright (c) 2008-2013 by Cisco Systems, Inc
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

#ifndef __SAFE_LIB_H__
#define __SAFE_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "safe_config.h"
#include "safe_lib_errno.h"
#include "safe_types.h"
#include "safe_compile.h"

#if defined _WIN32 && !defined(DISABLE_DLLIMPORT)
#if defined(EXPORT) && defined(__SAFECLIB_PRIVATE_H__)
#define EXTERN extern __declspec(dllexport)
#else
#define EXTERN extern __declspec(dllimport)
#endif
#else
#if defined(_WIN32) && defined(DISABLE_DLLIMPORT)
/* ignore the sec_api imports. with DISABLE_DLLIMPORT we want our own */
#undef _SECIMP
#define _SECIMP
#endif
#define EXTERN extern
#endif

#ifndef __KERNEL__
#include <time.h>
#if defined HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#endif /* __KERNEL__ */

#if defined __MINGW64_VERSION_MAJOR
#define HAVE_MINGW64 /* mingw-w64 (either 32 or 64bit) */
#elif defined __MINGW32__
#define HAVE_MINGW32 /* old mingw */
#endif

/* duplicate decls */
#ifndef __SAFE_STR_LIB_H__

EXTERN void abort_handler_s(const char *restrict msg, void *restrict ptr,
                            errno_t error)
#ifdef __GNUC__
    __attribute__((noreturn))
#endif
    ;

EXTERN void ignore_handler_s(const char *restrict msg, void *restrict ptr,
                             errno_t error);

#endif

#define sl_default_handler ignore_handler_s

#ifndef TMP_MAX_S
#ifdef TMP_MAX
#define TMP_MAX_S TMP_MAX
#else
#define TMP_MAX_S 308915776
#endif
#endif
#ifndef L_tmpnam_s
#ifdef L_tmpnam
#define L_tmpnam_s L_tmpnam
#else
#define L_tmpnam_s 1024
#endif
#endif

/* not str/mem/wchar: io, os, misc */

/* tmpnam_s is defined in the C11 standard Annex K, but unsafe
   and excluded by default. In most libc's it is deprecated. */
#ifdef SAFECLIB_ENABLE_UNSAFE
EXTERN errno_t _tmpnam_s_chk(const char *dest, rsize_t dmax,
                             const size_t destbos) BOS_OVR2Z(dest, dmax);
#define tmpnam_s(dest, dmax) _tmpnam_s_chk(dest, dmax, BOS(dest))
#endif

#ifndef __KERNEL__
EXTERN errno_t tmpfile_s(FILE *restrict *restrict streamptr);
#endif /* __KERNEL__ */

EXTERN char *_gets_s_chk(char *dest, rsize_t dmax, const size_t destbos)
    BOS_CHK(dest);
#define gets_s(dest, dmax) _gets_s_chk(dest, dmax, BOS(dest))

/* Windows sec_api does without restrict */
#ifndef MINGW_HAS_SECURE_API
#ifndef __KERNEL__
EXTERN errno_t fopen_s(FILE *restrict *restrict streamptr,
                       const char *restrict filename,
                       const char *restrict mode);

EXTERN errno_t freopen_s(FILE *restrict *restrict newstreamptr,
                         const char *restrict filename,
                         const char *restrict mode, FILE *restrict stream);
#endif /* __KERNEL__ */
#define asctime_s(dest, dmax, tm) _asctime_s_chk(dest, dmax, tm, BOS(dest))
#define ctime_s(dest, dmax, timer) _ctime_s_chk(dest, dmax, timer, BOS(dest))
#endif

EXTERN errno_t _asctime_s_chk(char *dest, rsize_t dmax, const struct tm *tm,
                              const size_t destbos) BOS_CHK(dest)
    BOS_ATTR(dmax < 26, "dmax underflow") BOS_NULL(tm);

/* TODO time64_t
   https://www.gnu.org/software/libc/manual/html_node/64_002dbit-time-symbol-handling.html */
EXTERN errno_t _ctime_s_chk(char *dest, rsize_t dmax, const time_t *timer,
                            const size_t destbos) BOS_CHK(dest)
    BOS_ATTR(dmax < 26, "dmax underflow") BOS_NULL(timer);

#ifndef MINGW_HAS_SECURE_API
/* Beware: These return errno_t on the MINGW64 windows sec_api,
   and switched its args:

   errno_t gmtime_s(struct tm *_Tm, const time_t *_Time);
   errno_t localtime_s(struct tm *_Tm,const time_t *_Time); */
#ifndef HAVE_MINGW64
EXTERN struct tm *gmtime_s(const time_t *restrict timer,
                           struct tm *restrict dest) BOS_NULL(timer)
    BOS_NULL(dest);

EXTERN struct tm *localtime_s(const time_t *restrict timer,
                              struct tm *restrict dest) BOS_NULL(timer)
    BOS_NULL(dest);
#endif

#endif /* MINGW_HAS_SECURE_API */

/* windows has also the wide and time64 variants, and _strtime_s */

EXTERN errno_t _getenv_s_chk(size_t *restrict len, char *restrict dest,
                             rsize_t dmax, const char *restrict name,
                             const size_t destbos) BOS_CHK(dest)
    BOS_ATTR(_BOS_NULL(name), "empty name");
#define getenv_s(len, dest, dmax, name)                                        \
    _getenv_s_chk(len, dest, dmax, name, BOS(dest))

EXTERN void *
_bsearch_s_chk(const void *key, const void *base, rsize_t nmemb, rsize_t size,
               int (*compar)(const void *k, const void *y, void *context),
               void *context, const size_t basebos)
    BOS_ATTR(nmemb && (_BOS_NULL(key) || _BOS_NULL(base) ||
                       _BOS_ZERO(base, nmemb *size)),
             "empty buf or bufsize") BOS_OVR2_BUTNULL(base, nmemb *size)
        BOS_ATTR(nmemb && !compar, "empty compar");
#define bsearch_s(key, base, nmemb, size, compar, context)                     \
    _bsearch_s_chk(key, base, nmemb, size, compar, context, BOS(base))

EXTERN errno_t _qsort_s_chk(void *base, rsize_t nmemb, rsize_t size,
                            int (*compar)(const void *x, const void *y,
                                          void *context),
                            void *context, const size_t basebos)
    BOS_ATTR(nmemb && (_BOS_NULL(base) || _BOS_ZERO(base, nmemb *size)),
             "empty buf or bufsize") BOS_OVR2_BUTNULL(base, nmemb *size)
        BOS_ATTR(nmemb && !compar, "empty compar");
#ifndef MINGW_HAS_SECURE_API
#define qsort_s(base, nmemb, size, compar, context)                            \
    _qsort_s_chk(base, nmemb, size, compar, context, BOS(base))
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_LIB_H__ */
