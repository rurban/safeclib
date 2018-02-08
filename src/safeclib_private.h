/*------------------------------------------------------------------
 * safeclib_private.h - Internal library references
 *
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 * 2017 Reini Urban
 *
 * Copyright (c) 2012, 2013 by Cisco Systems, Inc
 * Copyright (c) 2017 Reini Urban
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

#ifndef __SAFECLIB_PRIVATE_H__
#define __SAFECLIB_PRIVATE_H__

#ifndef DEBUG
#define NDEBUG
#endif

#include "config.h"

#ifdef __KERNEL__
/* linux kernel environment */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ctype.h>

#define RCNEGATE(x)  ( -(x) )

#ifndef HAVE_C99
# define slprintf
# define sldebug_printf
#else
# define slprintf(...) printk(KERN_EMERG __VA_ARGS__)
# define slabort()
# ifdef DEBUG
#  define sldebug_printf(...) printk(KERN_DEBUG __VA_ARGS__)
# endif
#endif

#else  /* !__KERNEL__ */

#if defined(__CYGWIN__) && defined(__x86_64)
#define HAVE_CYGWIN64
#endif

#if defined(HAVE_SECURE_GETENV)
# ifdef _GNU_SOURCE
#  define _GNU_SOURCE_WAS_DEFINED
# else
#  define _GNU_SOURCE
# endif
#endif
#include <stdio.h>
#ifdef STDC_HEADERS
# include <ctype.h>
# include <stdlib.h>
# ifndef _GNU_SOURCE_WAS_DEFINED
#  undef _GNU_SOURCE
# else
#  undef _GNU_SOURCE_WAS_DEFINED
# endif
# include <stddef.h>
# include <stdarg.h>
#else
/* newlib, cygwin64 has no STDC_HEADERS */
# ifdef HAVE_STDLIB_H
#  ifdef HAVE_SECURE_GETENV
#   define _GNU_SOURCE
#  endif
#  include <stdlib.h>
# else
/* cygwin64 */
#  ifdef __GNUC__
void abort(void) __attribute__((noreturn));
#  endif
# endif
# ifdef HAVE_CTYPE_H
#  include <ctype.h>
# endif
# ifdef HAVE_MALLOC_H
#  include <malloc.h>
# endif
#endif
#ifdef HAVE_STRING_H
# if !defined STDC_HEADERS && defined HAVE_MEMORY_H
#  include <memory.h>
# endif
# include <string.h>
#endif
#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#if defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1)
# define WANT_C11
#endif

/* mingw 3.4 */
#ifndef EOVERFLOW
#define EOVERFLOW 139
#endif

#ifndef HAVE_STRNSTR
#define strnstr(a,b,c) strstr(a,b)
/* broken in cygwin/newlib until Aug 2017 */
#elif !defined HAVE_STRNSTR_OK
#define strnstr(a,b,c) strstr(a,b)
#endif

/* Only needed for the linux kernel */
#define EXPORT_SYMBOL(sym)
#define RCNEGATE(x)  (x)

#define slabort()       abort()
#ifndef HAVE_C99
# define slprintf	printf
# define sldebug_printf printf
#else
# define slprintf(...)  fprintf(stderr, __VA_ARGS__)
# ifdef DEBUG
#  define sldebug_printf(...) printf(__VA_ARGS__)
# endif
#endif

#if __GNUC__ >= 3
# define _expect(expr,value)        __builtin_expect((expr), (value))
# define INLINE                     static inline
#else
# define _expect(expr,value)        (expr)
# define INLINE                     static
#endif
#ifndef likely
#define likely(expr)   _expect ((long)((expr) != 0), 1)
#define unlikely(expr) _expect ((long)((expr) != 0), 0)
#endif

#endif /* __KERNEL__ */

/* for a proper asctime string with 26 digits, i.e. max 4 digit year.
   01.01.10000 00:00 */
#ifndef MAX_TIME_T_STR
# if SIZEOF_TIME_T < 8
#  define MAX_TIME_T_STR (0x7fffffff)
# else
#  define MAX_TIME_T_STR (313360441200L)
# endif
#endif

#define _UNICODE_MAX 0x10ffff

#ifndef sldebug_printf
# ifdef HAVE_C99
#  define sldebug_printf(...)
# else
#  define sldebug_printf printf
# endif
#endif

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include "safe_config.h"
#include "safe_lib_errno.h"
#include "safe_types.h"

#include "safe_str_lib.h"
#include "str/safe_str_constraint.h"
#include "safe_mem_lib.h"
#include "mem/safe_mem_constraint.h"
#include "safe_lib.h"

/* needed for -Wcast-align */
#if defined(__clang__) || defined(__clang) || \
       (defined( __GNUC__) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406)
#  define GCC_DIAG_PRAGMA(x) _Pragma (#x)
/* clang has "clang diagnostic" pragmas, but also understands gcc. */
#  define GCC_DIAG_IGNORE(x) _Pragma("GCC diagnostic push") \
                             GCC_DIAG_PRAGMA(GCC diagnostic ignored #x)
#  define GCC_DIAG_RESTORE   _Pragma("GCC diagnostic pop")
#else
#  define GCC_DIAG_IGNORE(w)
#  define GCC_DIAG_RESTORE
#endif

#define CHK_DEST_NULL(func)                                             \
    if (unlikely(dest == NULL)) {                                       \
        invoke_safe_str_constraint_handler(func ": dest is null", dest, ESNULLP); \
        return RCNEGATE(ESNULLP);                                       \
    }
#define CHK_DMAX_ZERO(func)                                             \
    if (unlikely(dmax == 0)) {                                          \
        invoke_safe_str_constraint_handler(func ": dmax is 0", dest, ESZEROL); \
        return RCNEGATE(ESZEROL);                                       \
    }
#define CHK_DMAX_MAX(func, max)                                         \
    if (unlikely(dmax > (max))) {                                       \
        invoke_safe_str_constraint_handler(func ": dmax exceeds max", dest, ESLEMAX); \
        return RCNEGATE(ESLEMAX);                                       \
    }
#define CHK_SRC_NULL(func, src)                                         \
    if (unlikely(src == NULL)) {                                        \
        invoke_safe_str_constraint_handler(func ": src is null", (char*)src, ESNULLP); \
        return RCNEGATE(ESNULLP);                                       \
    }
#define CHK_SRC_NULL_CLEAR(func, src)                                   \
    if (unlikely(src == NULL)) {                                        \
        handle_error(dest, strnlen_s(dest, dmax), func ": src is null", ESNULLP); \
        return RCNEGATE(ESNULLP); \
    }
#define CHK_SRCW_NULL_CLEAR(func, src)                                   \
    if (unlikely(src == NULL)) {                                        \
        handle_werror(dest, wcsnlen_s(dest, dmax), func ": src is null", ESNULLP); \
        return RCNEGATE(ESNULLP); \
    }
#define CHK_SLEN_MAX_CLEAR(func, max)                                   \
    if (unlikely(slen > RSIZE_MAX_STR)) {                               \
        handle_error(dest, strnlen_s(dest, dmax), func ": slen exceeds max", ESLEMAX); \
        return RCNEGATE(ESLEMAX);                                       \
    }

/* platform quirks */
#ifndef SAFECLIB_DISABLE_WCHAR

/* mingw32 3.15.2 */
#if defined(_WIN32) && defined(__MINGW32_MAJOR_VERSION) && \
    !defined(__STRICT_ANSI__) && !defined(__MINGW64_VERSION_MAJOR)
#define vswprintf(dest, dmax, fmt, ap) vswprintf(dest, fmt, ap)
#endif

/* mingw64 3.0.1
   has strtok_s, wcstok_s, and vsnprintf_s, which we patch in the tests. */

#if SIZEOF_WCHAR_T > 2

#define _dec_w16(src) *(src)
#define _ENC_W16(dest,dmax,cp) *(dest)++ = (cp); (dmax)--

#else
/* windows, cygwin + 32bit aix, solaris */
/* convert surrogate pair to unicode codepoint */
EXTERN uint32_t _dec_w16(wchar_t *src);

/* convert unicode codepoint to surrogate pair, advancing dest */
#define _ENC_W16(dest,dmax,cp)                  \
    if (unlikely((cp) < 0x10000)) {             \
        *(dest)++ = (cp); (dmax)--;             \
    } else {                                    \
        *dest++ = ((cp) >> 10) + 0xd7c0;        \
        *dest++ = ((cp) & 0x3ff) + 0xdc00;      \
        (dmax)--; (dmax)--;                     \
    }
#endif

/* is start of a surrogate pair? */
#define _IS_W16(cp) ((cp) >= 0xd800 && (cp) < 0xdc00)

EXTERN errno_t _towfc_single(wchar_t *restrict dest, const uint32_t src);
EXPORT uint32_t _towcase(uint32_t wc, int lower);
EXPORT uint32_t _towupper(uint32_t wc);
#ifndef HAVE_TOWLOWER
EXTERN wint_t towlower(wint_t wc);
#endif
/* from wcsnorm_s.c */
EXTERN int _decomp_s(wchar_t *restrict dest, rsize_t dmax, const uint32_t cp,
                     const bool iscompat);

#endif /* SAFECLIB_DISABLE_WCHAR */

#endif /* __SAFECLIB_PRIVATE_H__ */
