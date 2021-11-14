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
#ifndef NDEBUG
#define NDEBUG
#endif
#endif

#include "config.h"

#ifdef __KERNEL__
/* linux kernel environment */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ctype.h>

/* Needed since the switch to time64_t */
#if defined CONFIG_COMPAT_32BIT_TIME && defined _LINUX_TIME64_H && defined __VDSO_TIME32_H
#define time_t old_time32_t
#elif defined _LINUX_TIME64_H && !defined __VDSO_TIME32_H
#define time_t time64_t
#endif

#define RCNEGATE(x) (-(x))

#ifndef SAFECLIB_HAVE_C99
#define slprintf
#define sldebug_printf
#else
#define slprintf(...) printk(KERN_EMERG __VA_ARGS__)
#define slabort()
#ifdef DEBUG
#define sldebug_printf(...) printk(KERN_DEBUG __VA_ARGS__)
#endif
#endif

#else /* !__KERNEL__ */

#if defined(__CYGWIN__) && defined(__x86_64)
#define HAVE_CYGWIN64
#endif

#if defined(HAVE_SECURE_GETENV)
#ifdef _GNU_SOURCE
#define _GNU_SOURCE_WAS_DEFINED
#else
#define _GNU_SOURCE
#endif
#endif
#include <stdio.h>
#ifdef STDC_HEADERS
#include <ctype.h>
#include <stdlib.h>
#if defined(HAVE_SECURE_GETENV)
#ifndef _GNU_SOURCE_WAS_DEFINED
#undef _GNU_SOURCE
#else
#undef _GNU_SOURCE_WAS_DEFINED
#endif
#endif
#include <stddef.h>
#include <stdarg.h>
#else
/* newlib, cygwin64 has no STDC_HEADERS */
#ifdef HAVE_STDLIB_H
#ifdef HAVE_SECURE_GETENV
#define _GNU_SOURCE
#endif
#include <stdlib.h>
#else
/* cygwin64 */
#ifdef __GNUC__
void abort(void) __attribute__((noreturn));
#endif
#endif
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#endif
#ifdef HAVE_STRING_H
#if !defined STDC_HEADERS && defined HAVE_MEMORY_H
#include <memory.h>
#endif
#include <string.h>
#endif
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#if defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1)
#define WANT_C11
#endif

#ifndef HAVE_UINTPTR_T
typedef unsigned long uintptr_t;
#endif

/* mingw 3.4 */
#ifndef EOVERFLOW
#ifdef _WIN32
#define EOVERFLOW 132
#else
#define EOVERFLOW 75
#endif
#endif

#ifndef HAVE_STRNSTR
#define strnstr(a, b, c) strstr(a, b)
/* broken in cygwin/newlib until Aug 2017 */
#elif !defined HAVE_STRNSTR_OK
#define strnstr(a, b, c) strstr(a, b)
#endif

/* Only needed for the linux kernel */
#define EXPORT_SYMBOL(sym)
#define RCNEGATE(x) (x)

#define slabort() abort()
#ifndef SAFECLIB_HAVE_C99
#define slprintf printf
#define sldebug_printf printf
#else
#define slprintf(...) fprintf(stderr, __VA_ARGS__)
#ifdef DEBUG
#define sldebug_printf(...) printf(__VA_ARGS__)
#endif
#endif

#if __GNUC__ >= 3
#define _expect(expr, value) __builtin_expect((expr), (value))
#define INLINE static inline
#else
#define _expect(expr, value) (expr)
#define INLINE static
#endif
#ifndef likely
#define likely(expr) _expect((long)((expr) != 0), 1)
#define unlikely(expr) _expect((long)((expr) != 0), 0)
#endif

#endif /* __KERNEL__ */

/* for a proper asctime string with 26 digits, i.e. max 4 digit year.
   01.01.10000 00:00 */
#ifndef MAX_TIME_T_STR
#if SIZEOF_TIME_T < 8
#define MAX_TIME_T_STR (0x7fffffff)
#else
#define MAX_TIME_T_STR (313360441200L)
#endif
#endif

#define _UNICODE_MAX 0x10ffff

#ifndef sldebug_printf
#ifdef SAFECLIB_HAVE_C99
#define sldebug_printf(...)
#else
#define sldebug_printf printf
#endif
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
#if defined(__clang__) || defined(__clang) ||                                  \
    (defined(__GNUC__) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406)
#define GCC_DIAG_PRAGMA(x) _Pragma(#x)
/* clang has "clang diagnostic" pragmas, but also understands gcc. */
#define GCC_DIAG_IGNORE(x)                                                     \
    _Pragma("GCC diagnostic push") GCC_DIAG_PRAGMA(GCC diagnostic ignored #x)
#define GCC_DIAG_RESTORE _Pragma("GCC diagnostic pop")
#else
#define GCC_DIAG_IGNORE(w)
#define GCC_DIAG_RESTORE
#endif

#if defined(DEBUG) && defined(SAFECLIB_HAVE_C99) && defined(__KERNEL__)
#define debug_printf(...) printk(KERN_DEBUG __VA_ARGS__)
#elif defined(SAFECLIB_HAVE_C99)
#define debug_printf(...) fprintf(STDERR, __VA_ARGS__)
#else
#define debug_printf printf
#endif

/* TODO: do we need the builtin's? rather just use __bnd... which is defined on
   CHKP and MPX. requires -fcheck-pointer-bounds -mmpx */
#if defined(HAVE___BUILTIN___BND_CHK_PTR_BOUNDS) && defined(__CHKP__) &&       \
    defined(__MPX__)
#define BND_CHK_PTR_BOUNDS(dest, count)                                        \
    __builtin___bnd_chk_ptr_bounds(dest, count)
#elif defined(HAVE___BND_CHK_PTR_BOUNDS) && defined(__CHKP__) &&               \
    defined(__MPX__)
#define BND_CHK_PTR_BOUNDS(dest, count) __bnd_chk_ptr_bounds(dest, count)
#else
#define BND_CHK_PTR_BOUNDS(dest, count)
#endif
#ifdef HAVE_ERROR_DMAX
#define RETURN_ESLEWRNG return (RCNEGATE(ESLEWRNG))
#else
#define RETURN_ESLEWRNG
#endif

#ifdef HAVE_ERROR_DMAX
#define RETURN_ESLEWRNG return (RCNEGATE(ESLEWRNG))
#else
#define RETURN_ESLEWRNG
#endif

#define CHK_DEST_NULL(func)                                                    \
    if (unlikely(dest == NULL)) {                                              \
        invoke_safe_str_constraint_handler(func ": dest is null",              \
                                           (void *)dest, ESNULLP);             \
        return RCNEGATE(ESNULLP);                                              \
    }
#define CHK_DMAX_ZERO(func)                                                    \
    if (unlikely(dmax == 0)) {                                                 \
        invoke_safe_str_constraint_handler(func ": dmax is 0", (void *)dest,   \
                                           ESZEROL);                           \
        return RCNEGATE(ESZEROL);                                              \
    }
#define CHK_DEST_MEM_NULL(func)                                                \
    if (unlikely(dest == NULL)) {                                              \
        invoke_safe_mem_constraint_handler(func ": dest is null",              \
                                           (void *)dest, ESNULLP);             \
        return RCNEGATE(ESNULLP);                                              \
    }
#define CHK_DMAX_MEM_ZERO(func)                                                \
    if (unlikely(dmax == 0)) {                                                 \
        invoke_safe_mem_constraint_handler(func ": dmax is 0", (void *)dest,   \
                                           ESZEROL);                           \
        return RCNEGATE(ESZEROL);                                              \
    }
#define CHK_DMAX_MAX(func, max)                                                \
    if (unlikely(dmax > (max))) {                                              \
        invoke_safe_str_constraint_handler(func ": dmax exceeds max",          \
                                           (void *)dest, ESLEMAX);             \
        return RCNEGATE(ESLEMAX);                                              \
    }
#define CHK_DMAX_MEM_MAX(func, max)                                            \
    if (unlikely(dmax > (max))) {                                              \
        invoke_safe_mem_constraint_handler(func ": dmax exceeds max",          \
                                           (void *)dest, ESLEMAX);             \
        return RCNEGATE(ESLEMAX);                                              \
    }
/* for known dest size, we should have already errored at compile-time before.
   anyway, for known dest size check overflows in detail. does CLEAR */
#if defined(HAVE_WARN_DMAX)
#define CHK_DEST_OVR_CLEAR(func, destbos)                                      \
    if (unlikely(dmax != destbos)) {                                           \
        if (unlikely(dmax > destbos)) {                                        \
            if (dmax > RSIZE_MAX_STR) {                                        \
                handle_error(dest, destbos, func ": dmax exceeds max",         \
                             ESLEMAX);                                         \
                return RCNEGATE(ESLEMAX);                                      \
            } else {                                                           \
                return handle_str_bos_overload(func ": dmax exceeds dest",     \
                                               (char *)dest, destbos);         \
            }                                                                  \
        }                                                                      \
        handle_str_bos_chk_warn(func, (char *)dest, dmax, destbos);            \
        RETURN_ESLEWRNG;                                                       \
    }
#define CHK_DEST_OVR(func, destbos)                                            \
    if (unlikely(dmax != destbos)) {                                           \
        if (unlikely(dmax > destbos)) {                                        \
            if (dmax > RSIZE_MAX_STR) {                                        \
                invoke_safe_str_constraint_handler(func ": dmax exceeds max",  \
                                                   (void *)dest, ESLEMAX);     \
                return RCNEGATE(ESLEMAX);                                      \
            } else {                                                           \
                invoke_safe_str_constraint_handler(func ": dmax exceeds dest", \
                                                   (void *)dest, EOVERFLOW);   \
                return RCNEGATE(EOVERFLOW);                                    \
            }                                                                  \
        }                                                                      \
        handle_str_bos_chk_warn(func, (char *)dest, dmax, destbos);            \
        RETURN_ESLEWRNG;                                                       \
    }
#define CHK_DESTW_OVR(func, destsz, destbos)                                   \
    if (unlikely(destsz != destbos)) {                                         \
        if (unlikely(destsz > destbos)) {                                      \
            if (dmax > RSIZE_MAX_WSTR) {                                       \
                invoke_safe_str_constraint_handler(func ": dmax exceeds max",  \
                                                   (void *)dest, ESLEMAX);     \
                return RCNEGATE(ESLEMAX);                                      \
            } else {                                                           \
                invoke_safe_str_constraint_handler(func ": dmax exceeds dest", \
                                                   (void *)dest, EOVERFLOW);   \
                return RCNEGATE(EOVERFLOW);                                    \
            }                                                                  \
        }                                                                      \
        handle_str_bos_chk_warn(func, (char *)dest, dmax,                      \
                                destbos / sizeof(wchar_t));                    \
        RETURN_ESLEWRNG;                                                       \
    }
#define CHK_DESTW_OVR_CLEAR(func, destsz, destbos)                             \
    if (unlikely(destsz != destbos)) {                                         \
        if (unlikely(destsz > destbos)) {                                      \
            if (dmax > RSIZE_MAX_WSTR) {                                       \
                handle_werror(dest, destbos / sizeof(wchar_t),                 \
                              func ": dmax exceeds max", ESLEMAX);             \
                return RCNEGATE(ESLEMAX);                                      \
            } else {                                                           \
                handle_werror(dest, destbos / sizeof(wchar_t),                 \
                              func ": dmax exceeds dest", EOVERFLOW);          \
                return RCNEGATE(EOVERFLOW);                                    \
            }                                                                  \
        }                                                                      \
        handle_str_bos_chk_warn(func, (char *)dest, dmax,                      \
                                destbos / sizeof(wchar_t));                    \
        RETURN_ESLEWRNG;                                                       \
    }
#define CHK_DEST_OVR_BOOL(func, destbos)                                       \
    if (unlikely(dmax != destbos)) {                                           \
        if (unlikely(dmax > destbos)) {                                        \
            if (dmax > RSIZE_MAX_STR) {                                        \
                invoke_safe_str_constraint_handler(func ": dmax exceeds max",  \
                                                   (void *)dest, ESLEMAX);     \
            } else {                                                           \
                invoke_safe_str_constraint_handler(func ": dmax exceeds dest", \
                                                   (void *)dest, EOVERFLOW);   \
            }                                                                  \
            return false;                                                      \
        }                                                                      \
        handle_str_bos_chk_warn(func, (char *)dest, dmax, destbos);            \
    }
/* does no clear */
#define CHK_DEST_MEM_OVR(func, destbos)                                        \
    if (unlikely(dmax != destbos)) {                                           \
        if (unlikely(dmax > destbos)) {                                        \
            if (dmax > RSIZE_MAX_MEM) {                                        \
                invoke_safe_mem_constraint_handler(func ": dmax exceeds max",  \
                                                   (void *)dest, ESLEMAX);     \
                return RCNEGATE(ESLEMAX);                                      \
            } else {                                                           \
                invoke_safe_mem_constraint_handler(func ": dmax exceeds dest", \
                                                   (void *)dest, EOVERFLOW);   \
                return RCNEGATE(EOVERFLOW);                                    \
            }                                                                  \
        }                                                                      \
        handle_mem_bos_chk_warn(func, (void *)dest, dmax, destbos);            \
        RETURN_ESLEWRNG;                                                       \
    }
#else /* WARN_DMAX */
#define CHK_DEST_OVR_CLEAR(func, destbos)                                      \
    if (unlikely(dmax > destbos)) {                                            \
        if (dmax > RSIZE_MAX_STR) {                                            \
            handle_error(dest, destbos, func ": dmax exceeds max", ESLEMAX);   \
            return RCNEGATE(ESLEMAX);                                          \
        } else {                                                               \
            return handle_str_bos_overload(func ": dmax exceeds dest",         \
                                           (char *)dest, destbos);             \
        }                                                                      \
    }
#define CHK_DEST_OVR(func, destbos)                                            \
    if (unlikely(dmax > destbos)) {                                            \
        if (dmax > RSIZE_MAX_STR) {                                            \
            invoke_safe_str_constraint_handler(func ": dmax exceeds max",      \
                                               (void *)dest, ESLEMAX);         \
            return RCNEGATE(ESLEMAX);                                          \
        } else {                                                               \
            invoke_safe_str_constraint_handler(func ": dmax exceeds dest",     \
                                               (void *)dest, EOVERFLOW);       \
            return RCNEGATE(EOVERFLOW);                                        \
        }                                                                      \
    }
#define CHK_DESTW_OVR(func, destsz, destbos)                                   \
    if (unlikely(destsz > destbos)) {                                          \
        if (dmax > RSIZE_MAX_WSTR) {                                           \
            invoke_safe_str_constraint_handler(func ": dmax exceeds max",      \
                                               (void *)dest, ESLEMAX);         \
            return RCNEGATE(ESLEMAX);                                          \
        } else {                                                               \
            invoke_safe_str_constraint_handler(func ": dmax exceeds dest",     \
                                               (void *)dest, EOVERFLOW);       \
            return RCNEGATE(EOVERFLOW);                                        \
        }                                                                      \
    }
#define CHK_DESTW_OVR_CLEAR(func, destsz, destbos)                             \
    if (unlikely(destsz > destbos)) {                                          \
        if (dmax > RSIZE_MAX_WSTR) {                                           \
            handle_werror(dest, destbos / sizeof(wchar_t),                     \
                          func ": dmax exceeds max", ESLEMAX);                 \
            return RCNEGATE(ESLEMAX);                                          \
        } else {                                                               \
            handle_werror(dest, destbos / sizeof(wchar_t),                     \
                          func ": dmax exceeds dest", EOVERFLOW);              \
            return RCNEGATE(EOVERFLOW);                                        \
        }                                                                      \
    }
#define CHK_DEST_OVR_BOOL(func, destbos)                                       \
    if (unlikely(dmax > destbos)) {                                            \
        if (dmax > RSIZE_MAX_STR) {                                            \
            invoke_safe_str_constraint_handler(func ": dmax exceeds max",      \
                                               (void *)dest, ESLEMAX);         \
        } else {                                                               \
            invoke_safe_str_constraint_handler(func ": dmax exceeds dest",     \
                                               (void *)dest, EOVERFLOW);       \
        }                                                                      \
        return false;                                                          \
    }
#define CHK_DEST_MEM_OVR(func, destbos)                                        \
    if (unlikely(dmax > destbos)) {                                            \
        if (dmax > RSIZE_MAX_MEM) {                                            \
            invoke_safe_mem_constraint_handler(func ": dmax exceeds max",      \
                                               (void *)dest, ESLEMAX);         \
            return RCNEGATE(ESLEMAX);                                          \
        } else {                                                               \
            invoke_safe_mem_constraint_handler(func ": dmax exceeds dest",     \
                                               (void *)dest, EOVERFLOW);       \
            return RCNEGATE(EOVERFLOW);                                        \
        }                                                                      \
    }
#endif
#define CHK_SRC_NULL(func, src)                                                \
    if (unlikely(src == NULL)) {                                               \
        invoke_safe_str_constraint_handler(func ": " _XSTR(src) " is null",    \
                                           (void *)src, ESNULLP);              \
        return RCNEGATE(ESNULLP);                                              \
    }
#define CHK_SRC_NULL_CLEAR(func, src)                                          \
    if (unlikely(src == NULL)) {                                               \
        handle_error(dest, _BOS_KNOWN(dest) ? BOS(dest) : dmax,                \
                     func ": " _XSTR(src) " is null", ESNULLP);                \
        return RCNEGATE(ESNULLP);                                              \
    }
#define CHK_SRCW_NULL_CLEAR(func, src)                                         \
    if (unlikely(src == NULL)) {                                               \
        handle_werror(dest,                                                    \
                      _BOS_KNOWN(dest) ? BOS(dest) / sizeof(wchar_t) : dmax,   \
                      func ": " _XSTR(src) " is null", ESNULLP);               \
        return RCNEGATE(ESNULLP);                                              \
    }
#if defined(HAVE_WARN_DMAX) && defined(HAVE_ERROR_DMAX)
#define CHK_SRC_OVR_CLEAR(func, src, slen, MAX)                                \
    if (_BOS_KNOWN(src)) {                                                     \
        if (unlikely(_BOS_OVR_N(src, slen))) {                                 \
            return handle_str_bos_overload(                                    \
                func, (char *)dest, _BOS_KNOWN(dest) ? BOS(dest) : dmax);      \
        } else if (_BOS_CHK_N(src, slen)) {                                    \
            handle_str_src_bos_chk_warn(func, (char *)dest, slen, BOS(src),    \
                                        _XSTR(src), _XSTR(slen));              \
            return RCNEGATE(ESLEWRNG);                                         \
        }                                                                      \
        BND_CHK_PTR_BOUNDS(src, slen);                                         \
    }
#elif defined(HAVE_WARN_DMAX)
#define CHK_SRC_OVR_CLEAR(func, src, slen, MAX)                                \
    if (_BOS_KNOWN(src)) {                                                     \
        if (unlikely(_BOS_OVR_N(src, slen))) {                                 \
            return handle_str_bos_overload(                                    \
                func, (char *)dest, _BOS_KNOWN(dest) ? BOS(dest) : dmax);      \
        } else if (_BOS_CHK_N(src, slen)) {                                    \
            handle_str_src_bos_chk_warn(func, (char *)dest, slen, BOS(src),    \
                                        _XSTR(src), _XSTR(slen));              \
        }                                                                      \
        BND_CHK_PTR_BOUNDS(src, slen);                                         \
    }
#else
#define CHK_SRC_OVR_CLEAR(func, src, slen, MAX)                                \
    if (_BOS_KNOWN(src)) {                                                     \
        if (unlikely(_BOS_OVR_N(src, slen))) {                                 \
            return handle_str_bos_overload(                                    \
                func, (char *)dest, _BOS_KNOWN(dest) ? BOS(dest) : dmax);      \
        }                                                                      \
        BND_CHK_PTR_BOUNDS(src, slen);                                         \
    }
#endif
#define CHK_SLEN_MAX_CLEAR(func, slen, max)                                    \
    if (unlikely(slen > (max))) {                                              \
        handle_error(dest,                                                     \
                     _BOS_KNOWN(dest) ? BOS(dest) : strnlen_s(dest, dmax),     \
                     func ": " _XSTR(slen) " exceeds max", ESLEMAX);           \
        return RCNEGATE(ESLEMAX);                                              \
    }
#define CHK_SLEN_MAX_NOSPC_CLEAR(func, slen, max)                              \
    if (unlikely(slen > dmax)) {                                               \
        errno_t error = slen > max ? ESLEMAX : ESNOSPC;                        \
        handle_error(dest,                                                     \
                     _BOS_KNOWN(dest) ? BOS(dest) : strnlen_s(dest, dmax),     \
                     func ": " _XSTR(slen) " exceeds max", error);             \
        return RCNEGATE(error);                                                \
    }
#define CHK_SRC_MEM_NULL_CLEAR(func, src)                                      \
    if (unlikely(src == NULL)) {                                               \
        handle_mem_error(dest, _BOS_KNOWN(dest) ? BOS(dest) : dmax,            \
                         func ": " _XSTR(src) " is null", ESNULLP);            \
        return RCNEGATE(ESNULLP);                                              \
    }
#define CHK_SLEN_MEM_MAX_NOSPC_CLEAR(func, slen, max)                          \
    if (unlikely(slen > dmax)) {                                               \
        errno_t error = slen > max ? ESLEMAX : ESNOSPC;                        \
        handle_mem_error((void *)dest, _BOS_KNOWN(dest) ? BOS(dest) : dmax,    \
                         func ": " _XSTR(slen) " exceeds max", error);         \
        return RCNEGATE(error);                                                \
    }
#define CHK_DEST_DMAX_BOOL(func, max)                                          \
    if (unlikely(dest == NULL)) {                                              \
        invoke_safe_str_constraint_handler(func ": dest is null", NULL,        \
                                           ESNULLP);                           \
        return false;                                                          \
    }                                                                          \
    if (unlikely(dmax == 0)) {                                                 \
        invoke_safe_str_constraint_handler(func ": dmax is 0", (void *)dest,   \
                                           ESZEROL);                           \
        return false;                                                          \
    }                                                                          \
    if (destbos == BOS_UNKNOWN) {                                              \
        if (unlikely(dmax > (max))) {                                          \
            invoke_safe_str_constraint_handler(func ": dmax exceeds max",      \
                                               (void *)dest, ESLEMAX);         \
            return false;                                                      \
        }                                                                      \
        BND_CHK_PTR_BOUNDS(dest, dmax);                                        \
    } else {                                                                   \
        CHK_DEST_OVR_BOOL(func, destbos)                                       \
    }

/* Comparing pointers from two separately allocated objects is forbidden
   as per 6.5.8 C11 except when using (in)equality. GH #51 */
#define CHK_OVRLP(dp, dlen, sp, slen)                                          \
    (((uintptr_t)dp >= (uintptr_t)sp) &&                                       \
     ((uintptr_t)dp < (uintptr_t)(sp + slen))) ||                              \
        (((uintptr_t)dp < (uintptr_t)sp) &&                                    \
         ((uintptr_t)sp < (uintptr_t)(dp + dlen)))
/* but allow dp==sp */
#define CHK_OVRLP_BUTSAME(dp, dlen, sp, slen)                                  \
    (((uintptr_t)dp > (uintptr_t)sp) &&                                        \
     ((uintptr_t)dp < (uintptr_t)(sp + slen))) ||                              \
        (((uintptr_t)dp < (uintptr_t)sp) &&                                    \
         ((uintptr_t)sp < (uintptr_t)(dp + dlen)))

/* platform quirks */
#ifndef SAFECLIB_DISABLE_WCHAR

/* mingw32 3.15.2 */
#if defined(_WIN32) && defined(__MINGW32_MAJOR_VERSION) &&                     \
    !defined(__STRICT_ANSI__) && !defined(__MINGW64_VERSION_MAJOR)
#define vswprintf(dest, dmax, fmt, ap) vswprintf(dest, fmt, ap)
#endif

#define wcstombs_s(retvalp, dest, dmax, src, len)                              \
    _wcstombs_s_chk(retvalp, dest, dmax, src, len, BOS(dest))

/* mingw64 3.0.1
   has strtok_s, wcstok_s, and vsnprintf_s, which we patch in the tests. */
#undef _ENC_W16

#if SIZEOF_WCHAR_T > 2

#undef _ENC_W16
#define _dec_w16(src) *(src)
#define _ENC_W16(dest, dmax, cp)                                               \
    *(dest)++ = (cp);                                                          \
    (dmax)--

#else
/* windows, cygwin + 32bit aix, solaris */
/* convert surrogate pair to unicode codepoint */
EXTERN uint32_t _dec_w16(wchar_t *src);

/* convert unicode codepoint to surrogate pair, advancing dest */
#define _ENC_W16(dest, dmax, cp)                                               \
    if (unlikely((cp) < 0x10000)) {                                            \
        *(dest)++ = (cp);                                                      \
        (dmax)--;                                                              \
    } else {                                                                   \
        *dest++ = ((cp) >> 10) + 0xd7c0;                                       \
        *dest++ = ((cp)&0x3ff) + 0xdc00;                                       \
        (dmax)--;                                                              \
        (dmax)--;                                                              \
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

// internal helpers for the *printf_s functions:

// output function type
typedef int (*out_fct_type)(char character, void *buffer, size_t idx,
                            size_t maxlen);
// wrapper (used as buffer) for output function type
typedef struct {
    int (*fct)(char character, void *arg);
    void *arg;
} out_fct_wrap_type;

// internal buffer output
static inline int safec_out_buffer(char character, void *buffer, size_t idx,
                                    size_t maxlen)
{
    if (idx < maxlen) {
        ((char *)buffer)[idx] = character;
        return 1;
    } else {
        invoke_safe_str_constraint_handler("vsnprintf_s: exceeds dmax",
                                           (char*)buffer, ESNOSPC);
        return -(ESNOSPC);
    }
}

// internal putchar wrapper
static inline int safec_out_char(char character, void *buffer, size_t idx,
                                  size_t maxlen)
{
    (void)buffer;
    (void)idx;
    (void)maxlen;
    if (character) {
#ifndef __KERNEL__
        return putchar(character);
#else
        int rc = 0;
        rc = slprintf("%c", character);
        return rc;
#endif
    }
    else
        return 0;
}

#ifndef __KERNEL__
// special-case of safec_out_fct for fprintf_s
static inline int safec_out_fchar(char character, void *wrap, size_t idx,
                             size_t maxlen) {
    (void)idx;
    (void)maxlen;
    //((out_fct_wrap_type *)wrap)->fct(character, ((out_fct_wrap_type *)wrap)->arg);
    return fputc(character, (FILE*)((out_fct_wrap_type *)wrap)->arg);
}
#endif

// internal output function wrapper
static inline int safec_out_fct(char character, void *wrap, size_t idx,
                                 size_t maxlen) {
    (void)idx;
    (void)maxlen;
    // wrap is the output fct pointer
    return ((out_fct_wrap_type *)wrap)->fct(character, ((out_fct_wrap_type *)wrap)->arg);
}

// mingw has a _vsnprintf_s. we use our own.
int safec_vsnprintf_s(out_fct_type out, const char *funcname, char *buffer,
                      const size_t bufsize, const char *format, va_list va);

#endif /* __SAFECLIB_PRIVATE_H__ */
