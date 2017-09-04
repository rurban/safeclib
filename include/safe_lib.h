/*------------------------------------------------------------------
 * safe_lib.h -- Safe C Library
 *
 * October 2008, Bo Berry
 * Modified 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 *
 * Copyright (c) 2008-2013 by Cisco Systems, Inc
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
#include "safe_types.h"
#include "safe_lib_errno.h"

#ifdef _WIN32
# if defined(EXPORT) && defined(__SAFECLIB_PRIVATE_H__)
#  define EXTERN extern __declspec(dllexport)
# else
#  define EXTERN extern __declspec(dllimport)
# endif
#else
# define EXTERN extern
#endif

/* C11 appendix K types - specific for bounds checking */
typedef size_t  rsize_t;

#ifndef RSIZE_MAX
# define RSIZE_MAX (~(rsize_t)0)  /* leave here for completeness */
#endif

typedef void (*constraint_handler_t) (const char *restrict /* msg */,
                                      void *restrict       /* ptr */,
                                      errno_t              /* error */);

EXTERN void
abort_handler_s(const char *restrict msg, void *restrict ptr, errno_t error);

EXTERN void
ignore_handler_s(const char *restrict msg, void *restrict ptr, errno_t error);

#define sl_default_handler ignore_handler_s

#ifndef __SAFE_MEM_LIB_H__
#include "safe_mem_lib.h"
#endif
#ifndef __SAFE_STR_LIB_H__
#include "safe_str_lib.h"
#endif

/* This functions is defined in the C11 standard Annex K, but unsafe
   and excluded by default. In most libc's it is deprecated. */
#ifdef SAFECLIB_ENABLE_UNSAFE
  
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

/* not str nor mem */

EXTERN errno_t
tmpnam_s(char *filename_s, rsize_t maxsize);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_LIB_H__ */
