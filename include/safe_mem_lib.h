/*------------------------------------------------------------------
 * safe_mem_lib.h -- Safe C Library Memory APIs
 *
 * October 2008, Bo Berry
 * Modified 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 * Modified September 2017, Reini Urban
 *
 * Copyright (c) 2008-2012 by Cisco Systems, Inc.
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

#ifndef __SAFE_MEM_LIB_H__
#define __SAFE_MEM_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "safe_config.h"
#include "safe_lib_errno.h"
#include "safe_types.h"

#ifndef SAFECLIB_DISABLE_WCHAR
#include <wchar.h>
#endif

#if defined _WIN32 && !defined(DISABLE_DLLIMPORT)
# if defined(EXPORT) && defined(__SAFECLIB_PRIVATE_H__)
#  define EXTERN extern __declspec(dllexport)
# else
#  define EXTERN extern __declspec(dllimport)
# endif
#else
# define EXTERN extern
#endif

#define RSIZE_MAX_MEM16    ( RSIZE_MAX_MEM/2 )
#define RSIZE_MAX_MEM32    ( RSIZE_MAX_MEM/4 )
#define RSIZE_MAX_WMEM     ( RSIZE_MAX_MEM/sizeof(wchar_t) )

/* set memory constraint handler */
EXTERN constraint_handler_t
set_mem_constraint_handler_s(constraint_handler_t handler);


/* copy memory */
EXTERN errno_t
memcpy_s(void *restrict dest, rsize_t dmax,
         const void *restrict src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* move memory, including overlapping memory */
EXTERN errno_t
memmove_s(void *dest, rsize_t  dmax,
          const void *src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* set bytes. now __STDC_WANT_LIB_EXT1__ >= 1 compatible */
#if !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
EXTERN errno_t
memset_s(void *dest, rsize_t dmax, int value, rsize_t n)
    BOS_CHK(dest) BOS_OVR2(dest, n) VAL_OVR2(value, 255) VAL_OVR2(n, dmax);
#endif

#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* compare memory */
EXTERN errno_t
memcmp_s(const void *dest, rsize_t dmax,
         const void *src, rsize_t slen, int *diff)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* compare uint16_t memory */
EXTERN errno_t
memcmp16_s(const uint16_t *dest, rsize_t dmax,
           const uint16_t *src, rsize_t slen, int *diff)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* compare uint32_t memory */
EXTERN errno_t
memcmp32_s(const uint32_t *dest, rsize_t dmax,
           const uint32_t *src, rsize_t slen, int *diff)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* copy uint16_t memory */
EXTERN errno_t
memcpy16_s(uint16_t *dest, rsize_t dmax,
           const uint16_t *src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* copy uint32_t memory */
EXTERN errno_t
memcpy32_s(uint32_t *dest, rsize_t dmax,
           const uint32_t *src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* uint16_t move memory, including overlapping memory */
EXTERN errno_t
memmove16_s(uint16_t *dest, rsize_t dmax,
            const uint16_t *src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* uint32_t move memory, including overlapping memory */
EXTERN errno_t
memmove32_s(uint32_t *dest, rsize_t dmax,
            const uint32_t *src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* set uint16_t */
EXTERN errno_t
memset16_s(uint16_t *dest, rsize_t dmax, uint16_t value, rsize_t n)
    BOS_CHK(dest) BOS_OVR2(dest, n/2) VAL_OVR2(n, dmax/2);

/* set uint32_t */
EXTERN errno_t
memset32_s(uint32_t *dest, rsize_t dmax, uint32_t value, rsize_t n)
    BOS_CHK(dest) BOS_OVR2(dest, n/4) VAL_OVR2(n, dmax/4);

/* byte zero */
EXTERN errno_t
memzero_s(void *dest, rsize_t dmax)
    BOS_CHK(dest);

/* uint16_t zero */
EXTERN errno_t
memzero16_s(uint16_t *dest, rsize_t dmax)
    BOS_CHK(dest);

/* uint32_t zero */
EXTERN errno_t
memzero32_s(uint32_t *dest, rsize_t dmax)
    BOS_CHK(dest);

/* find a byte */
EXTERN errno_t
memchr_s(const void *restrict dest, rsize_t dlen,
         const int ch, void **result)
    BOS_CHK2(dest, dlen) VAL_OVR2(ch, 255);

EXTERN errno_t
memrchr_s(const void *restrict dest, rsize_t dlen,
          const int ch, void **result)
    BOS_CHK2(dest, dlen) VAL_OVR2(ch, 255);

/* timing-safe byte sequence comparisons
   (already in OpenBSD, cygwin, newlib) */
#if !(defined(__NEWLIB__) || defined(__OpenBSD__))
EXTERN int
timingsafe_bcmp(const void *b1, const void *b2, size_t n)
    BOS_OVR2(b1, n) BOS_OVR2(b2, n);

EXTERN int
timingsafe_memcmp(const void *b1, const void *b2, size_t len)
    BOS_OVR2(b1, len) BOS_OVR2(b2, len);
#endif

/* copy string until character found (FreeBSD) */
EXTERN errno_t
memccpy_s(void *dest, rsize_t dmax, const void *src, int c, rsize_t n)
    BOS_CHK(dest) BOS_OVR2(src, n) VAL_OVR2(c, 255);

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

#ifndef SAFECLIB_DISABLE_WCHAR
/* wchar (of wchar_t size 2 or 4)
   disabled if your system has no wchar support.
 */

EXTERN errno_t
wmemcpy_s(wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t smax)
    BOSW_CHK(dest) BOSW_OVR2(src, smax);

EXTERN errno_t
wmemmove_s(wchar_t *dest, rsize_t dmax,
           const wchar_t *src, rsize_t smax)
    BOSW_CHK(dest) BOSW_OVR2(src, smax);

#ifndef SAFECLIB_DISABLE_EXTENSIONS

EXTERN errno_t
wmemcmp_s(const wchar_t *dest, rsize_t dmax,
          const wchar_t *src, rsize_t slen, int *diff)
    BOSW_CHK(dest) BOSW_OVR2(src, slen);

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

#endif /* SAFECLIB_DISABLE_WCHAR */

#ifdef __cplusplus
}
#endif

#endif  /* __SAFE_MEM_LIB_H__ */
