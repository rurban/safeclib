/*------------------------------------------------------------------
 * safe_types.h - C99 std types & defs or Linux kernel equivalents
 *
 * March 2007, Bo Berry
 * Modified 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 *
 * Copyright (c) 2007-2012 by Cisco Systems, Inc
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

#ifndef __SAFE_TYPES_H__
#define __SAFE_TYPES_H__

/* Supported environments are:
 * - Linux kerenl
 * - C99 std. C library compiler
 * other environments require a custom header to be included, which can be
 * done automatically via the build system.
 */

#ifdef __KERNEL__
/* linux kernel environment */

/*#warning Compiling for Linux kernel*/
#include <linux/stddef.h>
#include <linux/types.h>
#include <linux/errno.h>
#define LACKS_ERRNO_T    1
#define LACKS_BOOLEAN_T  1

/* C99 standard c library compiler support */
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

/*#warning Compiling for standard C library*/
#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#define LACKS_BOOLEAN_T  1

#endif /* __KERNEL__ */

/*
 * Note: We only redefine types that we use in the library here, don't add
 * new types or defines if they are not used in the library.
 */

#ifdef LACKS_ERRNO_T
typedef int errno_t;
#endif

/*
 * TODO: eventually do a search and replace of the code and remove
 * references to boolean_t in favor of the C99 standard _Bool or bool
 */
#ifdef LACKS_BOOL
typedef unsigned char bool;
#endif

#ifdef LACKS_BOOLEAN_T
typedef bool boolean_t;
#endif

#ifdef LACKS_INT32_T
typedef int int32_t;
#endif

#ifdef LACKS_UINT8_T
typedef unsigned char uint8_t;
#endif

#ifdef LACKS_UINT16_T
typedef unsigned short uint16_t;
#endif

#ifdef LACKS_UINT32_T
typedef unsigned int uint32_t;
#endif

#ifdef LACKS_UINTPTR_T
#if POINTER_BIT == 64
typedef unsigned long long uintptr_t;
#else
typedef unsigned int uintptr_t;
#endif /* POINTER_BIT */
#endif /* LACKS_UINTPTR_T */

/*
 * The following definitions are not part of C99 so we always test for them.
 */

/* TODO: eventually do a search and replace for TRUE & FALSE and change to the
   C99 'true' & 'false' */
#ifndef TRUE
#  ifndef true
#    define TRUE   ( 1 )
#  else
#    define TRUE   ( true )
#  endif /* true */
#endif /* TRUE */

#ifndef FALSE
#  ifndef false
#    define FALSE  ( 0 )
#  else
#    define FALSE  ( false )
#  endif /* false */
#endif /* FALSE */

#ifndef NULL
#define NULL ((void *) 0)
#endif /* NULL */

#endif /* __SAFE_TYPES_H__ */
