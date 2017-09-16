/*------------------------------------------------------------------
 * safeclib_private.h - Internal library references
 *
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 *
 * Copyright (c) 2012, 2013 by Cisco Systems, Inc
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

#include "config.h"

#include <stdio.h>
#ifdef STDC_HEADERS
# include <ctype.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
#else
/* newlib, cygwin64 has no STDC_HEADERS */
# ifdef HAVE_STDLIB_H
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

/* mingw 3.4 */
#ifndef EOVERFLOW
#define EOVERFLOW 139
#endif

#ifndef HAVE_STRNSTR
#define strnstr(a,b,c) strstr(a,b)
#endif

/* for a proper asctime string with 26 digits, i.e. max 4 digit year.
   01.01.10000 00:00 */
#ifndef MAX_TIME_T_STR
# if SIZEOF_TIME_T < 8
#  define MAX_TIME_T_STR (0x7fffffff)
# else
#  define MAX_TIME_T_STR (313360441200L)
# endif
#endif

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

#include "safe_lib.h"
#include "safe_str_constraint.h"

/* platform quirks */

/* mingw32 3.15.2 */
#if defined(_WIN32) && defined(__MINGW32_MAJOR_VERSION) && \
    !defined(__STRICT_ANSI__) && !defined(__MINGW64_VERSION_MAJOR)
#define vswprintf(dest, dmax, fmt, ap) vswprintf(dest, fmt, ap)
#endif

/* mingw64 3.0.1
   has strtok_s, wcstok_s, and vsnprintf_s, which we patch in the tests. */

#endif /* __SAFECLIB_PRIVATE_H__ */
