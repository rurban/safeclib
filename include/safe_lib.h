/*------------------------------------------------------------------
 * safe_lib.h -- Safe C Library
 *
 * September 2017, Reini Urban
 * Modified 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 *
 * Copyright (c) 2008-2013 by Cisco Systems, Inc
 * Copyright (c) 2017 by Reini Urban
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

#if defined _WIN32 && !defined(DISABLE_DLLIMPORT)
# if defined(EXPORT) && defined(__SAFECLIB_PRIVATE_H__)
#  define EXTERN extern __declspec(dllexport)
# else
#  define EXTERN extern __declspec(dllimport)
# endif
#else
# if defined(_WIN32) && defined(DISABLE_DLLIMPORT)
/* ignore the sec_api imports. with DISABLE_DLLIMPORT we want our own */
#  undef _SECIMP
#  define _SECIMP
# endif
# define EXTERN extern
#endif

#ifndef __KERNEL__
#include <time.h>
#if defined HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#endif /* __KERNEL__ */

#if defined __MINGW64_VERSION_MAJOR
#  define HAVE_MINGW64  /* mingw-w64 (either 32 or 64bit) */
#elif defined __MINGW32__
#  define HAVE_MINGW32  /* old mingw */
#endif

/* duplicate decls */
#ifndef __SAFE_STR_LIB_H__

EXTERN void
abort_handler_s(const char *restrict msg, void *restrict ptr, errno_t error)
#ifdef __GNUC__
__attribute__((noreturn))
#endif
;

EXTERN void
ignore_handler_s(const char *restrict msg, void *restrict ptr, errno_t error);

#endif

#define sl_default_handler ignore_handler_s

#ifndef TMP_MAX_S
# ifdef TMP_MAX
#  define TMP_MAX_S TMP_MAX
# else
#  define TMP_MAX_S 308915776
# endif
#endif
#ifndef L_tmpnam_s
# ifdef L_tmpnam
#  define L_tmpnam_s L_tmpnam
# else
#  define L_tmpnam_s 1024
# endif
#endif

/* not str/mem/wchar: io, os, misc */

/* This functions is defined in the C11 standard Annex K, but unsafe
   and excluded by default. In most libc's it is deprecated. */
#ifdef SAFECLIB_ENABLE_UNSAFE
EXTERN errno_t
tmpnam_s(char *filename_s, rsize_t maxsize);
#endif

#ifndef __KERNEL__
EXTERN errno_t
tmpfile_s(FILE * restrict * restrict streamptr);
#endif /* __KERNEL__ */

EXTERN char *
gets_s(char *dest, rsize_t dmax)
    BOS_CHK(dest);

/* Windows sec_api does without restrict */
#ifndef MINGW_HAS_SECURE_API
#ifndef __KERNEL__
EXTERN errno_t
fopen_s(FILE *restrict *restrict streamptr,
        const char *restrict filename,
        const char *restrict mode);

EXTERN errno_t
freopen_s(FILE *restrict *restrict newstreamptr,
          const char *restrict filename, const char *restrict mode,
          FILE *restrict stream);
#endif /* __KERNEL__ */

EXTERN errno_t
asctime_s(char *dest, rsize_t dmax, const struct tm *tm)
    BOS_CHK(dest);

EXTERN errno_t
ctime_s(char *dest, rsize_t dmax, const time_t *timer)
    BOS_CHK(dest);

/* Beware: This return errno_t on the MINGW64 windows sec_api,
   and switched its args:

   errno_t gmtime_s(struct tm *_Tm, const time_t *_Time);
   errno_t localtime_s(struct tm *_Tm,const time_t *_Time); */
#ifndef HAVE_MINGW64
EXTERN struct tm *
gmtime_s(const time_t *restrict timer, struct tm *restrict dest);

EXTERN struct tm *
localtime_s(const time_t *restrict timer, struct tm *restrict dest);
#endif

#endif /* MINGW_HAS_SECURE_API */

/* windows has also the wide and time64 variants, and _strtime_s */

EXTERN errno_t
getenv_s(size_t *restrict len,
         char *restrict dest, rsize_t dmax,
         const char *restrict name)
    BOS_CHK(dest);

EXTERN void *
bsearch_s(const void *key, const void *base,
          rsize_t nmemb, rsize_t size,
          int (*compar)(const void *k, const void *y, void *context),
          void *context)
    BOS_CHK2(base, nmemb*size);

#ifndef MINGW_HAS_SECURE_API
EXTERN errno_t
qsort_s(void *base, rsize_t nmemb, rsize_t size,
        int (*compar)(const void *x, const void *y, void *context),
        void *context)
    BOS_CHK2(base, nmemb*size);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_LIB_H__ */
