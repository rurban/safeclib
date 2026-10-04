/*------------------------------------------------------------------
 * safe_str_lib.h -- Safe C Library String APIs
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
 * August 2020, Reini Urban
 *
 * Copyright (c) 2008-2011, 2013 by Cisco Systems, Inc.
 * Copyright (c) 2017-2018, 2020 by Reini Urban
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

#ifndef __SAFE_STR_LIB_H__
#define __SAFE_STR_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "safe_config.h"
#include "safe_lib_errno.h"
#include "safe_types.h"
#include "safe_compile.h"

#ifndef __KERNEL__
#include <stdarg.h>
#ifndef SAFECLIB_DISABLE_IO
#include <time.h>
#if defined HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#endif
#endif /* __KERNEL__ */
#ifndef SAFECLIB_DISABLE_WCHAR
#include <wchar.h>
#endif

/* The mingw-w64 UCRT stdio.h and wchar.h (included above) define these
   static inline, with different semantics. Use ours under safec_ names. */
#if defined(__MINGW32__) && defined(_UCRT)
#define sscanf_s safec_sscanf_s
#define fscanf_s safec_fscanf_s
#define scanf_s safec_scanf_s
#define vscanf_s safec_vscanf_s
#define vfscanf_s safec_vfscanf_s
#define vsscanf_s safec_vsscanf_s
#define printf_s safec_printf_s
#define fprintf_s safec_fprintf_s
#define vprintf_s safec_vprintf_s
#define vfprintf_s safec_vfprintf_s
#ifndef SAFECLIB_DISABLE_WCHAR
#define wprintf_s safec_wprintf_s
#define vwprintf_s safec_vwprintf_s
#define fwprintf_s safec_fwprintf_s
#define vfwprintf_s safec_vfwprintf_s
#define swscanf_s safec_swscanf_s
#define vswscanf_s safec_vswscanf_s
#define wscanf_s safec_wscanf_s
#define vwscanf_s safec_vwscanf_s
#define fwscanf_s safec_fwscanf_s
#define vfwscanf_s safec_vfwscanf_s
#endif
#endif

#if defined _WIN32 && !defined(DISABLE_DLLIMPORT)
#undef EXTERN
#if defined(EXPORT) && defined(__SAFECLIB_PRIVATE_H__)
#define EXTERN extern __declspec(dllexport)
#else
#define EXTERN extern __declspec(dllimport)
#endif
#else
#define EXTERN extern
#endif

/**
 * With this UCD version we generated our tables.
 * Must be the same as TOWCTRANS_UNICODE_VERSION and
 * the version used for src/extwchar/unwifcan.h.
 */
#define SAFECLIB_UNICODE_VERSION 18

/**
 * The shortest string is a null string!!
 */
#define RSIZE_MIN_STR (1)

/** wide chars */
#ifndef SAFECLIB_DISABLE_WCHAR
#define RSIZE_MAX_WSTR (RSIZE_MAX_STR / sizeof(wchar_t))
#else
#define RSIZE_MAX_WSTR (RSIZE_MAX_STR / 2)
#endif

#if !defined SAFECLIB_DISABLE_WCHAR || defined SAFECLIB_ENABLE_U8
#define UNICODE_VERSION_MAJOR 13
#endif

/** The makeup of a password */
#define SAFE_STR_MIN_LOWERCASE (2)
#define SAFE_STR_MIN_UPPERCASE (2)
#define SAFE_STR_MIN_NUMBERS (1)
#define SAFE_STR_MIN_SPECIALS (1)

#define SAFE_STR_PASSWORD_MIN_LENGTH (6)
#define SAFE_STR_PASSWORD_MAX_LENGTH (32)

EXTERN void abort_handler_s(const char *restrict msg, void *restrict ptr,
                            errno_t error)
#if defined(__GNUC__) && !defined(__KERNEL__)
    __attribute__((noreturn))
#endif
    ;

EXTERN void ignore_handler_s(const char *restrict msg, void *restrict ptr,
                             errno_t error);

#define sl_default_handler ignore_handler_s

/* set string constraint handler */
EXTERN constraint_handler_t
set_str_constraint_handler_s(constraint_handler_t handler);
/* set thread-local constraint handler, overriding the global variant */
EXTERN constraint_handler_t
thrd_set_str_constraint_handler_s(constraint_handler_t handler);

/* string concatenate */
#if !defined(TEST_MSVCRT)
EXTERN errno_t _strcat_s_chk(char *restrict dest, rsize_t dmax,
                             const char *restrict src, size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define strcat_s(dest, dmax, src) _strcat_s_chk(dest, dmax, src, BOS(dest))

/* string copy */
EXTERN errno_t _strcpy_s_chk(char *restrict dest, rsize_t dmax,
                             const char *restrict src, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
/* With a compile-time known src length, copy inline if the ranges don't
   overlap. */
#if defined(HAVE___BUILTIN_STRLEN) && defined(HAVE___BUILTIN_MEMMOVE) &&       \
    (!defined(SAFECLIB_STR_NULL_SLACK) || defined(HAVE___BUILTIN_MEMSET))
#ifdef SAFECLIB_STR_NULL_SLACK
#define _strcpy_s_copy(dest, dmax, src)                                        \
    (_BOS_MEMCPY(dest, __builtin_strlen(src) + 1, src,                         \
                 __builtin_strlen(src) + 1, __builtin_strlen(src)),            \
     __builtin_memset((char *)(dest) + __builtin_strlen(src), 0,               \
                      (size_t)(dmax) - __builtin_strlen(src)))
#else
#define _strcpy_s_copy(dest, dmax, src)                                        \
    _BOS_MEMCPY(dest, __builtin_strlen(src) + 1, src,                          \
                __builtin_strlen(src) + 1, __builtin_strlen(src) + 1)
#endif
#define strcpy_s(dest, dmax, src)                                              \
    _BOS_UCHK_IF(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) &&             \
                     __builtin_strlen(src) < (size_t)(dmax),                   \
                 _BOS_DISJOINT(dest, __builtin_strlen(src) + 1, src,           \
                               __builtin_strlen(src) + 1),                     \
                 (_strcpy_s_copy(dest, dmax, src), EOK),                       \
                 _strcpy_s_chk(dest, dmax, src, BOS(dest)))
#else
#define strcpy_s(dest, dmax, src) _strcpy_s_chk(dest, dmax, src, BOS(dest))
#endif
#endif

/* fitted string concatenate */
EXTERN errno_t _strncat_s_chk(char *restrict dest, rsize_t dmax,
                              const char *restrict src, rsize_t slen,
                              size_t destbos, const size_t srcbos)
    BOS_ATTR((slen || dest || dmax) &&
                 (_BOS_NULL(dest) || _BOS_ZERO(dest, dmax)),
             "empty dest or dmax")
        BOS_ATTR((slen || dest || dmax) && _BOS_OVR(dest, dmax),
                 "dest overflow") BOS_OVR2_BUTZERO(src, slen);
#define strncat_s(dest, dmax, src, slen)                                       \
    _strncat_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* fitted string copy */
EXTERN errno_t _strncpy_s_chk(char *restrict dest, rsize_t dmax,
                              const char *restrict src, rsize_t slen,
                              const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2_BUTZERO(src, slen);
#define strncpy_s(dest, dmax, src, slen)                                       \
    _strncpy_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* string length */
EXTERN rsize_t _strnlen_s_chk(const char *str, rsize_t smax, size_t strbos)
    BOS_CHK2(str, smax);
EXTERN rsize_t _strnlen_s_uchk(const char *str, rsize_t smax);
#ifdef HAVE___BUILTIN_STRNLEN
#define strnlen_s(str, smax)                                                   \
    _BOS_UCHK(_BOS_DMAX_OK(str, smax, 1) && (size_t)(smax) <= RSIZE_MAX_STR,   \
              __builtin_strnlen(str, smax),                                    \
              _strnlen_s_chk(str, smax, BOS(str)))
#else
#define strnlen_s(str, smax) _BOS_UCHK_STR(strnlen_s, str, smax)
#endif

/* string tokenizer */
EXTERN char *_strtok_s_chk(char *restrict dest, rsize_t *restrict dmaxp,
                           const char *restrict delim, char **restrict ptr,
                           const size_t destbos) BOS_OVR2_BUTNULL(dest, *dmaxp)
    BOS_ATTR(dest &&_BOS_NULL(dmaxp), "empty dmax") BOS_NULL(delim)
        BOS_NULL(ptr);
#if !(defined(_WIN32) && defined(HAVE_STRTOK_S))
/* they use:
char * strtok_s(char *_Str,const char *_Delim,char **_Context); */
#define strtok_s(dest, dmaxp, delim, ptr)                                      \
    _strtok_s_chk(dest, dmaxp, delim, ptr, BOS(dest))
#endif

/* secure stdio */

/* safe sprintf_s */
/* now __STDC_WANT_LIB_EXT1__ >= 1 compatible */
#if defined(SAFECLIB_HAVE_C99) && !defined(FOR_DOXYGEN)
EXTERN int _sprintf_s_chk(char *restrict dest, const rsize_t dmax,
                          const size_t destbos, const char *restrict fmt, ...)
    __attribute_format__(printf, 4, 5) BOS_CHK(dest) BOS_FMT(fmt);
#define sprintf_s(dest, dmax, ...)                                             \
    _sprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)
#else
EXTERN int sprintf_s(char *restrict dest, rsize_t dmax,
                     const char *restrict fmt, ...)
    __attribute_format__(printf, 3, 4) BOS_CHK(dest) BOS_FMT(fmt);
#endif

EXTERN int _vsprintf_s_chk(char *restrict dest, rsize_t dmax,
                           const size_t destbos, const char *restrict fmt,
                           va_list ap) BOS_CHK(dest) BOS_FMT(fmt);
#define vsprintf_s(dest, dmax, fmt, ap)                                        \
    _vsprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)

/* truncating, no ESNOSPC */
#if defined(SAFECLIB_HAVE_C99) && !defined(TEST_MSVCRT) && !defined(FOR_DOXYGEN)
EXTERN int _snprintf_s_chk(char *restrict dest, rsize_t dmax,
                           const size_t destbos, const char *restrict fmt, ...)
    __attribute_format__(printf, 4, 5) BOS_CHK(dest) BOS_FMT(fmt);
#define snprintf_s(dest, dmax, ...)                                            \
    _snprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)
#else
EXTERN int snprintf_s(char *restrict dest, rsize_t dmax,
                      const char *restrict fmt, ...)
    __attribute_format__(printf, 3, 4) BOS_CHK(dest) BOS_FMT(fmt);
#endif

EXTERN int _vsnprintf_s_chk(char *restrict dest, rsize_t dmax,
                            const size_t destbos, const char *restrict fmt,
                            va_list ap) BOS_CHK(dest) BOS_FMT(fmt);
#if !(defined(_WIN32) && defined(HAVE_VSNPRINTF_S))
/* they use:
int vsnprintf_s(char *_DstBuf, size_t _DstSize, size_t _MaxCount,
                const char *_Format, va_list _ArgList); */
#define vsnprintf_s(dest, dmax, fmt, ap)                                       \
    _vsnprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)
#endif

/* Note: there is no __vsscanf_chk yet. Unchecked */
EXTERN int sscanf_s(const char *restrict buffer, const char *restrict fmt, ...)
    __attribute_format__(scanf, 2, 3) BOS_NULL(buffer) BOS_FMT(fmt);

#if !defined(__KERNEL__) && !defined(SAFECLIB_DISABLE_IO)
EXTERN int fscanf_s(FILE *restrict stream, const char *restrict fmt, ...)
    __attribute_format__(scanf, 2, 3) BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int scanf_s(const char *restrict fmt, ...)
    __attribute_format__(scanf, 1, 2) BOS_FMT(fmt);

EXTERN int vscanf_s(const char *restrict fmt, va_list ap) BOS_FMT(fmt);

#if !defined(__KERNEL__) && !defined(SAFECLIB_DISABLE_IO)
EXTERN int vfscanf_s(FILE *restrict stream, const char *restrict fmt,
                     va_list ap) BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int vsscanf_s(const char *restrict dest, const char *restrict fmt,
                     va_list ap) BOS_NULL(dest) BOS_FMT(fmt);

EXTERN int printf_s(const char *restrict fmt, ...)
    __attribute_format__(printf, 1, 2) BOS_FMT(fmt);

#if !defined(__KERNEL__) && !defined(SAFECLIB_DISABLE_IO)
EXTERN int fprintf_s(FILE *restrict stream, const char *restrict fmt, ...)
    __attribute_format__(printf, 2, 3) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int vprintf_s(const char *restrict fmt, va_list ap) BOS_FMT(fmt);

#if !defined(__KERNEL__) && !defined(SAFECLIB_DISABLE_IO)
EXTERN int vfprintf_s(FILE *restrict stream, const char *restrict fmt,
                      va_list arg) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN errno_t _strerror_s_chk(char *dest, rsize_t dmax, errno_t errnum,
                               const size_t destbos) BOS_CHK(dest);
EXTERN errno_t _strerror_s_uchk(char *dest, rsize_t dmax, errno_t errnum);
#define strerror_s(dest, dmax, errnum)                                         \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1),                                     \
              _strerror_s_uchk(dest, dmax, errnum),                            \
              _strerror_s_chk(dest, dmax, errnum, BOS(dest)))

EXTERN size_t strerrorlen_s(errno_t errnum);

/* for the other safe IO funcs see safe_lib.h */

#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* improved strcpy */
EXTERN char *_stpcpy_s_chk(char *restrict dest, rsize_t dmax,
                           const char *restrict src, errno_t *restrict errp,
                           const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(errp);
#define stpcpy_s(dest, dmax, src, errp)                                        \
    _stpcpy_s_chk(dest, dmax, src, errp, BOS(dest), BOS(src))

EXTERN char *_stpncpy_s_chk(char *restrict dest, rsize_t dmax,
                            const char *restrict src, rsize_t slen,
                            errno_t *restrict errp, const size_t destbos,
                            const size_t srcbos) BOS_CHK(dest)
    BOS_CHK2(src, slen) BOS_NULL(errp);
#define stpncpy_s(dest, dmax, src, slen, errp)                                 \
    _stpncpy_s_chk(dest, dmax, src, slen, errp, BOS(dest), BOS(src))

/* string compare */
EXTERN errno_t _strcmp_s_chk(const char *dest, rsize_t dmax, const char *src,
                             int *resultp, const size_t destbos,
                             const size_t srcbos) BOS_CHK(dest) BOS_NULL(src)
    BOS_NULL(resultp);
EXTERN errno_t _strcmp_s_uchk(const char *dest, rsize_t dmax, const char *src,
                              int *resultp, const size_t srcbos);
#define strcmp_s(dest, dmax, src, resultp)                                     \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) &&                \
                  _BOS_KNOWN(resultp),                                         \
              _strcmp_s_uchk(dest, dmax, src, resultp, BOS(src)),              \
              _strcmp_s_chk(dest, dmax, src, resultp, BOS(dest), BOS(src)))

/* string compare case-insensitive */
EXTERN errno_t _strcasecmp_s_chk(const char *dest, rsize_t dmax,
                                 const char *src, int *resultp,
                                 const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
EXTERN errno_t _strcasecmp_s_uchk(const char *dest, rsize_t dmax,
                                  const char *src, int *resultp);
#define strcasecmp_s(dest, dmax, src, resultp)                                 \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) &&                \
                  _BOS_KNOWN(resultp),                                         \
              _strcasecmp_s_uchk(dest, dmax, src, resultp),                    \
              _strcasecmp_s_chk(dest, dmax, src, resultp, BOS(dest)))

/* natural order string compare */
EXTERN errno_t _strnatcmp_s_chk(const char *dest, rsize_t dmax, const char *src,
                                const int fold_case, int *resultp,
                                const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(resultp);
EXTERN errno_t _strnatcmp_s_uchk(const char *dest, rsize_t dmax,
                                 const char *src, const int fold_case,
                                 int *resultp, const size_t srcbos);
#define _strnatcmp_s_dispatch(dest, dmax, src, fold_case, resultp)             \
    _BOS_UCHK(                                                                 \
        _BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) && _BOS_KNOWN(resultp), \
        _strnatcmp_s_uchk(dest, dmax, src, fold_case, resultp, BOS(src)),      \
        _strnatcmp_s_chk(dest, dmax, src, fold_case, resultp, BOS(dest),       \
                         BOS(src)))
#define strnatcmp_s(dest, dmax, src, resultp)                                  \
    _strnatcmp_s_dispatch(dest, dmax, src, 0, resultp)
#define strnatcasecmp_s(dest, dmax, src, resultp)                              \
    _strnatcmp_s_dispatch(dest, dmax, src, 1, resultp)

/* find a substring - case insensitive */
EXTERN errno_t _strcasestr_s_chk(char *dest, rsize_t dmax, const char *src,
                                 rsize_t slen, char **substring,
                                 const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(substring);
EXTERN errno_t _strcasestr_s_uchk(char *dest, rsize_t dmax, const char *src,
                                  rsize_t slen, char **substring);
#define strcasestr_s(dest, dmax, src, slen, substring)                         \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_SRC_OK(src, slen, 1) &&      \
                  (size_t)(slen) != 0 && (size_t)(slen) <= (size_t)(dmax) &&   \
                  _BOS_KNOWN(substring),                                       \
              _strcasestr_s_uchk(dest, dmax, src, slen, substring),            \
              _strcasestr_s_chk(dest, dmax, src, slen, substring, BOS(dest),   \
                                BOS(src)))

/* fixed field string compare */
EXTERN errno_t _strcmpfld_s_chk(const char *dest, rsize_t dmax, const char *src,
                                int *resultp, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(resultp);
EXTERN errno_t _strcmpfld_s_uchk(const char *dest, rsize_t dmax,
                                 const char *src, int *resultp);
#define strcmpfld_s(dest, dmax, src, resultp)                                  \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_SRC_OK(src, dmax, 1) &&      \
                  _BOS_KNOWN(resultp),                                         \
              _strcmpfld_s_uchk(dest, dmax, src, resultp),                     \
              _strcmpfld_s_chk(dest, dmax, src, resultp, BOS(dest)))

/* fixed char array copy */
EXTERN errno_t _strcpyfld_s_chk(char *dest, const rsize_t dmax, const char *src,
                                rsize_t slen, const size_t destbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2(slen, dmax);
#define strcpyfld_s(dest, dmax, src, slen)                                     \
    _strcpyfld_s_chk(dest, dmax, src, slen, BOS(dest))

/* copy from a zero terminated string to fixed char array */
EXTERN errno_t _strcpyfldin_s_chk(char *dest, rsize_t dmax, const char *src,
                                  rsize_t slen, const size_t destbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2(slen, dmax);
#define strcpyfldin_s(dest, dmax, src, slen)                                   \
    _strcpyfldin_s_chk(dest, dmax, src, slen, BOS(dest))

/* copy from a char array to zero terminated string */
EXTERN errno_t _strcpyfldout_s_chk(char *dest, rsize_t dmax, const char *src,
                                   rsize_t slen, const size_t destbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2(slen, dmax);
#define strcpyfldout_s(dest, dmax, src, slen)                                  \
    _strcpyfldout_s_chk(dest, dmax, src, slen, BOS(dest))

/* computes excluded prefix length */
EXTERN errno_t _strcspn_s_chk(const char *dest, rsize_t dmax, const char *src,
                              rsize_t slen, rsize_t *countp,
                              const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2(src, slen) BOS_NULL(countp);
EXTERN errno_t _strcspn_s_uchk(const char *dest, rsize_t dmax, const char *src,
                               rsize_t slen, rsize_t *countp);
#define strcspn_s(dest, dmax, src, slen, countp)                               \
    _BOS_UCHK(                                                                 \
        _BOS_DMAX_OK(dest, dmax, 1) && _BOS_SRC_OK(src, slen, 1) &&            \
            (size_t)(slen) != 0 && (size_t)(slen) <= RSIZE_MAX_STR &&          \
            _BOS_KNOWN(countp),                                                \
        _strcspn_s_uchk(dest, dmax, src, slen, countp),                        \
        _strcspn_s_chk(dest, dmax, src, slen, countp, BOS(dest), BOS(src)))

/* returns a pointer to the first occurrence of c in dest */
EXTERN errno_t _strfirstchar_s_chk(char *dest, rsize_t dmax, char c,
                                   char **firstp, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(firstp);
EXTERN errno_t _strfirstchar_s_uchk(char *dest, rsize_t dmax, char c,
                                    char **firstp);
#define strfirstchar_s(dest, dmax, c, firstp)                                  \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(firstp),               \
              _strfirstchar_s_uchk(dest, dmax, c, firstp),                     \
              _strfirstchar_s_chk(dest, dmax, c, firstp, BOS(dest)))

/* returns index of first difference */
EXTERN errno_t _strfirstdiff_s_chk(const char *dest, rsize_t dmax,
                                   const char *src, rsize_t *resultp,
                                   const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
EXTERN errno_t _strfirstdiff_s_uchk(const char *dest, rsize_t dmax,
                                    const char *src, rsize_t *resultp);
#define strfirstdiff_s(dest, dmax, src, resultp)                               \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) &&                \
                  _BOS_KNOWN(resultp),                                         \
              _strfirstdiff_s_uchk(dest, dmax, src, resultp),                  \
              _strfirstdiff_s_chk(dest, dmax, src, resultp, BOS(dest)))

/* validate alphanumeric string */
EXTERN bool _strisalphanumeric_s_chk(const char *dest, rsize_t dmax,
                                     const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN bool _strisalphanumeric_s_uchk(const char *dest, rsize_t dmax);
#define strisalphanumeric_s(dest, dmax)                                        \
    _BOS_UCHK_DEST(strisalphanumeric_s, dest, dmax)

/* validate ascii string */
EXTERN bool _strisascii_s_chk(const char *dest, rsize_t dmax,
                              const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN bool _strisascii_s_uchk(const char *dest, rsize_t dmax);
#define strisascii_s(dest, dmax) _BOS_UCHK_DEST(strisascii_s, dest, dmax)

/* validate string of digits */
EXTERN bool _strisdigit_s_chk(const char *dest, rsize_t dmax,
                              const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN bool _strisdigit_s_uchk(const char *dest, rsize_t dmax);
#define strisdigit_s(dest, dmax) _BOS_UCHK_DEST(strisdigit_s, dest, dmax)

/* validate hex string */
EXTERN bool _strishex_s_chk(const char *dest, rsize_t dmax,
                            const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN bool _strishex_s_uchk(const char *dest, rsize_t dmax);
#define strishex_s(dest, dmax) _BOS_UCHK_DEST(strishex_s, dest, dmax)

/* validate lower case */
EXTERN bool _strislowercase_s_chk(const char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN bool _strislowercase_s_uchk(const char *dest, rsize_t dmax);
#define strislowercase_s(dest, dmax)                                           \
    _BOS_UCHK_DEST(strislowercase_s, dest, dmax)

/* validate mixed case */
EXTERN bool _strismixedcase_s_chk(const char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN bool _strismixedcase_s_uchk(const char *dest, rsize_t dmax);
#define strismixedcase_s(dest, dmax)                                           \
    _BOS_UCHK_DEST(strismixedcase_s, dest, dmax)

/* validate password */
EXTERN bool _strispassword_s_chk(const char *dest, rsize_t dmax,
                                 const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN bool _strispassword_s_uchk(const char *dest, rsize_t dmax);
#define strispassword_s(dest, dmax) _BOS_UCHK_DEST(strispassword_s, dest, dmax)

/* validate upper case */
EXTERN bool _strisuppercase_s_chk(const char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN bool _strisuppercase_s_uchk(const char *dest, rsize_t dmax);
#define strisuppercase_s(dest, dmax)                                           \
    _BOS_UCHK_DEST(strisuppercase_s, dest, dmax)

/* returns  a pointer to the last occurrence of c in s1 */
EXTERN errno_t _strlastchar_s_chk(char *dest, rsize_t dmax, char c,
                                  char **lastp, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(lastp);
EXTERN errno_t _strlastchar_s_uchk(char *dest, rsize_t dmax, char c,
                                   char **lastp);
#define strlastchar_s(dest, dmax, c, lastp)                                    \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(lastp),                \
              _strlastchar_s_uchk(dest, dmax, c, lastp),                       \
              _strlastchar_s_chk(dest, dmax, c, lastp, BOS(dest)))

/* returns index of last difference */
EXTERN errno_t _strlastdiff_s_chk(const char *dest, rsize_t dmax,
                                  const char *src, rsize_t *resultp,
                                  const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
EXTERN errno_t _strlastdiff_s_uchk(const char *dest, rsize_t dmax,
                                   const char *src, rsize_t *resultp);
#define strlastdiff_s(dest, dmax, src, resultp)                                \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) &&                \
                  _BOS_KNOWN(resultp),                                         \
              _strlastdiff_s_uchk(dest, dmax, src, resultp),                   \
              _strlastdiff_s_chk(dest, dmax, src, resultp, BOS(dest)))

/* left justify */
EXTERN errno_t _strljustify_s_chk(char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK(dest);
EXTERN errno_t _strljustify_s_uchk(char *dest, rsize_t dmax);
#define strljustify_s(dest, dmax) _BOS_UCHK_DEST(strljustify_s, dest, dmax)

/* string terminate */
EXTERN rsize_t _strnterminate_s_chk(char *dest, rsize_t dmax,
                                    const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN rsize_t _strnterminate_s_uchk(char *dest, rsize_t dmax);
#define strnterminate_s(dest, dmax) _BOS_UCHK_DEST(strnterminate_s, dest, dmax)

/* get pointer to first occurrence from set of char */
EXTERN errno_t _strpbrk_s_chk(char *dest, rsize_t dmax, char *src, rsize_t slen,
                              char **firstp, const size_t destbos,
                              const size_t srcbos) BOS_CHK(dest)
    BOS_OVR2(src, slen) BOS_NULL(firstp);
EXTERN errno_t _strpbrk_s_uchk(char *dest, rsize_t dmax, char *src,
                               rsize_t slen, char **firstp);
#define strpbrk_s(dest, dmax, src, slen, firstp)                               \
    _BOS_UCHK(                                                                 \
        _BOS_DMAX_OK(dest, dmax, 1) && _BOS_SRC_OK(src, slen, 1) &&            \
            (size_t)(slen) != 0 && _BOS_KNOWN(firstp),                         \
        _strpbrk_s_uchk(dest, dmax, src, slen, firstp),                        \
        _strpbrk_s_chk(dest, dmax, src, slen, firstp, BOS(dest), BOS(src)))

EXTERN errno_t _strfirstsame_s_chk(const char *dest, rsize_t dmax,
                                   const char *src, rsize_t *resultp,
                                   const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
EXTERN errno_t _strfirstsame_s_uchk(const char *dest, rsize_t dmax,
                                    const char *src, rsize_t *resultp);
#define strfirstsame_s(dest, dmax, src, resultp)                               \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) &&                \
                  _BOS_KNOWN(resultp),                                         \
              _strfirstsame_s_uchk(dest, dmax, src, resultp),                  \
              _strfirstsame_s_chk(dest, dmax, src, resultp, BOS(dest)))

EXTERN errno_t _strlastsame_s_chk(const char *dest, rsize_t dmax,
                                  const char *src, rsize_t *resultp,
                                  const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
EXTERN errno_t _strlastsame_s_uchk(const char *dest, rsize_t dmax,
                                   const char *src, rsize_t *resultp);
#define strlastsame_s(dest, dmax, src, resultp)                                \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) &&                \
                  _BOS_KNOWN(resultp),                                         \
              _strlastsame_s_uchk(dest, dmax, src, resultp),                   \
              _strlastsame_s_chk(dest, dmax, src, resultp, BOS(dest)))

/* searches for a prefix */
EXTERN errno_t _strprefix_s_chk(const char *dest, rsize_t dmax, const char *src,
                                const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src);
EXTERN errno_t _strprefix_s_uchk(const char *dest, rsize_t dmax,
                                 const char *src);
#define strprefix_s(dest, dmax, src)                                           \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src),                  \
              _strprefix_s_uchk(dest, dmax, src),                              \
              _strprefix_s_chk(dest, dmax, src, BOS(dest)))

/* removes leading and trailing white space */
EXTERN errno_t _strremovews_s_chk(char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK(dest);
EXTERN errno_t _strremovews_s_uchk(char *dest, rsize_t dmax);
#define strremovews_s(dest, dmax) _BOS_UCHK_DEST(strremovews_s, dest, dmax)

/* computes inclusive prefix length */
EXTERN errno_t _strspn_s_chk(const char *dest, rsize_t dmax, const char *src,
                             rsize_t slen, rsize_t *countp,
                             const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(countp);
EXTERN errno_t _strspn_s_uchk(const char *dest, rsize_t dmax, const char *src,
                              rsize_t slen, rsize_t *countp);
#define strspn_s(dest, dmax, src, slen, countp)                                \
    _BOS_UCHK(                                                                 \
        _BOS_DMAX_OK(dest, dmax, 1) && _BOS_SRC_OK(src, slen, 1) &&            \
            (size_t)(slen) != 0 && _BOS_KNOWN(countp),                         \
        _strspn_s_uchk(dest, dmax, src, slen, countp),                         \
        _strspn_s_chk(dest, dmax, src, slen, countp, BOS(dest), BOS(src)))

/* find a substring */
EXTERN errno_t _strstr_s_chk(char *dest, rsize_t dmax, const char *src,
                             rsize_t slen, char **substringp,
                             const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2(src, slen) BOS_NULL(substringp);
EXTERN errno_t _strstr_s_uchk(char *dest, rsize_t dmax, const char *src,
                              rsize_t slen, char **substringp);
#define strstr_s(dest, dmax, src, slen, substringp)                            \
    _BOS_UCHK(                                                                 \
        _BOS_DMAX_OK(dest, dmax, 1) && _BOS_SRC_OK(src, slen, 1) &&            \
            (size_t)(slen) != 0 && _BOS_KNOWN(substringp),                     \
        _strstr_s_uchk(dest, dmax, src, slen, substringp),                     \
        _strstr_s_chk(dest, dmax, src, slen, substringp, BOS(dest), BOS(src)))

/* find a character */
EXTERN errno_t _strchr_s_chk(const char *restrict dest, rsize_t dmax,
                             const int ch, char **restrict resultp,
                             const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(ch, 255) BOS_NULL(resultp);
EXTERN errno_t _strchr_s_uchk(const char *dest, rsize_t dmax, const int ch,
                              char **resultp);
#ifdef HAVE___BUILTIN_STRCHR
#define _strchr_s_uchk_inl(dest, dmax, ch, resultp)                            \
    ((*(char **)(resultp) = (char *)__builtin_strchr(dest, ch)) &&             \
             (long)(*(char **)(resultp) - (const char *)(dest)) <=             \
                 (long)(dmax)                                                  \
         ? EOK                                                                 \
         : (*(char **)(resultp) = NULL, ESNOTFND))
#else
#define _strchr_s_uchk_inl _strchr_s_uchk
#endif
#define strchr_s(dest, dmax, ch, resultp)                                      \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && (int)(ch) <= 255 &&               \
                  _BOS_KNOWN(resultp),                                         \
              _strchr_s_uchk_inl(dest, dmax, ch, resultp),                     \
              _strchr_s_chk(dest, dmax, ch, resultp, BOS(dest)))

EXTERN errno_t _strrchr_s_chk(const char *restrict dest, rsize_t dmax,
                              const int ch, char **restrict resultp,
                              const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(ch, 255) BOS_NULL(resultp) BOS_ATTR(!*dest, "empty *dest");
EXTERN errno_t _strrchr_s_uchk(const char *dest, rsize_t dmax, const int ch,
                               char **resultp);
#define strrchr_s(dest, dmax, ch, resultp)                                     \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && (int)(ch) <= 255 &&               \
                  _BOS_KNOWN(resultp),                                         \
              _strrchr_s_uchk(dest, dmax, ch, resultp),                        \
              _strrchr_s_chk(dest, dmax, ch, resultp, BOS(dest)))

/* convert string to lowercase.
   mingw string_s.h: _strlwr_s */
EXTERN errno_t _strtolowercase_s_chk(char *dest, rsize_t dmax,
                                     const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN errno_t _strtolowercase_s_uchk(char *dest, rsize_t dmax);
#define strtolowercase_s(dest, dmax)                                           \
    _BOS_UCHK_DEST(strtolowercase_s, dest, dmax)

/* convert string to uppercase
   mingw string_s.h: _strupr_s */
EXTERN errno_t _strtouppercase_s_chk(char *dest, rsize_t dmax,
                                     const size_t destbos) BOS_CHK2(dest, dmax);
EXTERN errno_t _strtouppercase_s_uchk(char *dest, rsize_t dmax);
#define strtouppercase_s(dest, dmax)                                           \
    _BOS_UCHK_DEST(strtouppercase_s, dest, dmax)

#define strlwr_s(str, slen) strtolowercase_s((str), (slen))
#define strupr_s(str, slen) strtouppercase_s((str), (slen))

/* zero an entire string with nulls.
   mingw string_s.h has: _strset_s */
EXTERN errno_t _strzero_s_chk(char *dest, rsize_t dmax, const size_t destbos)
    BOS_CHK(dest);
EXTERN errno_t _strzero_s_uchk(char *dest, rsize_t dmax);
#define strzero_s(dest, dmax) _BOS_UCHK_DEST(strzero_s, dest, dmax)

EXTERN errno_t _strcoll_s_chk(const char *restrict dest, rsize_t dmax,
                              const char *restrict src, int *resultp,
                              const size_t destbos) BOS_CHK(dest) BOS_NULL(src)
    BOS_NULL(resultp);
EXTERN errno_t _strcoll_s_uchk(const char *dest, const char *src, int *resultp);
#define strcoll_s(dest, dmax, src, resultp)                                    \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && _BOS_KNOWN(src) &&                \
                  _BOS_KNOWN(resultp),                                         \
              _strcoll_s_uchk(dest, src, resultp),                             \
              _strcoll_s_chk(dest, dmax, src, resultp, BOS(dest)))

/* Derived from windows extensions sec_api/string_s.h
   defined(MINGW_HAS_SECURE_API) */

EXTERN errno_t _strset_s_chk(char *restrict dest, rsize_t dmax, int value,
                             const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(value, 255);
EXTERN errno_t _strset_s_uchk(char *dest, rsize_t dmax, int value);
#define strset_s(dest, dmax, value)                                            \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && (unsigned)(value) <= 255,         \
              _strset_s_uchk(dest, dmax, value),                               \
              _strset_s_chk(dest, dmax, value, BOS(dest)))

EXTERN errno_t _strnset_s_chk(char *restrict dest, rsize_t dmax, int value,
                              rsize_t n, const size_t destbos) BOS_CHK(dest)
    BOS_OVR2_BUTZERO(dest, n) VAL_OVR2(value, 255) VAL_OVR2_BUTZERO(n, dmax);
EXTERN errno_t _strnset_s_uchk(char *dest, rsize_t dmax, int value, rsize_t n);
#define strnset_s(dest, dmax, value, n)                                        \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, 1) && (unsigned)(value) <= 255 &&       \
                  (size_t)(n) <= (size_t)(dmax),                               \
              _strnset_s_uchk(dest, dmax, value, n),                           \
              _strnset_s_chk(dest, dmax, value, n, BOS(dest)))

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

#ifndef SAFECLIB_DISABLE_WCHAR

/* is c99
EXTERN wchar_t*
wcsstr(wchar_t *restrict dest, const wchar_t *restrict src);
*/

/* multibyte wchar */

EXTERN errno_t _mbstowcs_s_chk(size_t *restrict retvalp, wchar_t *restrict dest,
                               rsize_t dmax, const char *restrict src,
                               rsize_t len, const size_t destbos)
    BOS_NULL(retvalp)
        BOS_ATTR(!_BOS_NULL(dest) && _BOS_ZERO(dest, dmax), "empty dmax")
            BOS_ATTR(!_BOS_NULL(dest) && _BOSW_OVR(dest, dmax), "dest overflow")
                BOS_ATTR(!_BOS_NULL(dest) && (void *)dest == (void *)src,
                         "dest overlap") BOS_CHK2(src, len);
#define mbstowcs_s(retvalp, dest, dmax, src, len)                              \
    _mbstowcs_s_chk(retvalp, dest, dmax, src, len, BOS(dest))

EXTERN errno_t _mbsrtowcs_s_chk(size_t *restrict retvalp,
                                wchar_t *restrict dest, rsize_t dmax,
                                const char **restrict srcp, rsize_t len,
                                mbstate_t *restrict ps, const size_t destbos)
    BOS_NULL(retvalp) BOS_NULL(srcp) BOS_NULL(ps)
        BOS_ATTR(!_BOS_NULL(dest) && _BOS_ZERO(dest, dmax), "empty dmax")
            BOS_ATTR(!_BOS_NULL(dest) && _BOSW_OVR(dest, dmax), "dest overflow")
                BOS_ATTR(!_BOS_NULL(dest) && (char *)dest == *srcp,
                         "dest overlap") BOS_CHK2(*srcp, len)
                    BOS_ATTR(dmax &&len > dmax, "len overflow >dmax");
#define mbsrtowcs_s(retvalp, dest, dmax, srcp, len, ps)                        \
    _mbsrtowcs_s_chk(retvalp, dest, dmax, srcp, len, ps, BOS(dest))

EXTERN errno_t _wcsrtombs_s_chk(size_t *restrict retvalp, char *restrict dest,
                                rsize_t dmax, const wchar_t **restrict srcp,
                                rsize_t len, mbstate_t *restrict ps,
                                const size_t destbos) BOS_NULL(retvalp)
    BOS_NULL(ps) BOS_ATTR(!_BOS_NULL(dest) && !dmax, "empty dmax")
        BOS_ATTR(!_BOS_NULL(dest) && _BOS_OVR(dest, dmax), "dest overflow")
            BOS_ATTR(!_BOS_NULL(dest) && (void *)dest == (void *)srcp,
                     "dest overlap") BOS_NULL(srcp) BOSW_CHK2(*srcp, len)
                BOS_ATTR(dmax &&len > dmax, "len overflow >dmax");
#define wcsrtombs_s(retvalp, dest, dmax, srcp, len, ps)                        \
    _wcsrtombs_s_chk(retvalp, dest, dmax, srcp, len, ps, BOS(dest))

EXTERN errno_t _wcstombs_s_chk(size_t *restrict retvalp, char *restrict dest,
                               rsize_t dmax, const wchar_t *restrict src,
                               rsize_t len, const size_t destbos)
    BOS_NULL(retvalp) BOS_CHK(dest) BOSW_CHK2(src, len)
        BOS_ATTR(dmax &&len > dmax, "len overflow >dmax");
#define wcstombs_s(retvalp, dest, dmax, src, len)                              \
    _wcstombs_s_chk(retvalp, dest, dmax, src, len, BOS(dest))

EXTERN errno_t _wcrtomb_s_chk(size_t *restrict retvalp, char *restrict dest,
                              rsize_t dmax, wchar_t wc, mbstate_t *restrict ps,
                              const size_t destbos) BOS_NULL(retvalp)
    BOS_CHK(dest) BOS_NULL(ps) VAL_OVR2(wc, 0x10ffff);
#define wcrtomb_s(retvalp, dest, dmax, wc, ps)                                 \
    _wcrtomb_s_chk(retvalp, dest, dmax, wc, ps, BOS(dest))

EXTERN errno_t _wctomb_s_chk(int *restrict retvalp, char *restrict dest,
                             rsize_t dmax, wchar_t wc, const size_t destbos)
    BOS_NULL(retvalp)
        BOS_ATTR(!_BOS_NULL(dest) &&
                     (!dmax || dmax > RSIZE_MAX_STR || _BOS_OVR(dest, dmax)),
                 "dest overflow or empty") VAL_OVR2(wc, 0x10ffff);
#define wctomb_s(retvalp, dest, dmax, wc)                                      \
    _wctomb_s_chk(retvalp, dest, dmax, wc, BOS(dest))

EXTERN size_t _wcsnlen_s_chk(const wchar_t *str, size_t smax, size_t srcbos)
    BOSW_CHK2(str, smax);
EXTERN size_t _wcsnlen_s_uchk(const wchar_t *str, size_t smax);
/* the chk stops one wchar_t before a known str size multiple of wchar_t */
#define wcsnlen_s(str, smax)                                                   \
    _BOS_UCHK(                                                                 \
        _BOS_DMAX_OK(str, smax, sizeof(wchar_t)) &&                            \
            (size_t)(smax) <= RSIZE_MAX_WSTR,                                  \
        _wcsnlen_s_uchk(str, (BOS(str) % sizeof(wchar_t) == 0 &&               \
                              (size_t)(smax) == BOS(str) / sizeof(wchar_t))    \
                                 ? (size_t)(smax) - 1                          \
                                 : (size_t)(smax)),                            \
        _wcsnlen_s_chk(str, smax, BOS(str)))

EXTERN errno_t _wcscpy_s_chk(wchar_t *restrict dest, rsize_t dmax,
                             const wchar_t *restrict src, const size_t destbos)
    BOSW_CHK(dest) BOS_NULL(src);
#define wcscpy_s(dest, dmax, src) _wcscpy_s_chk(dest, dmax, src, BOS(dest))

EXTERN errno_t _wcsncpy_s_chk(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict src, rsize_t slen,
                              const size_t destbos, const size_t srcbos)
    BOSW_CHK(dest) BOSW_OVR2(src, slen);
#define wcsncpy_s(dest, dmax, src, slen)                                       \
    _wcsncpy_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

EXTERN errno_t _wcscat_s_chk(wchar_t *restrict dest, rsize_t dmax,
                             const wchar_t *restrict src, const size_t destbos)
    BOSW_CHK(dest) BOS_NULL(src);
#define wcscat_s(dest, dmax, src) _wcscat_s_chk(dest, dmax, src, BOS(dest))

EXTERN errno_t _wcsncat_s_chk(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict src, rsize_t slen,
                              const size_t destbos, const size_t srcbos)
    BOS_ATTR(slen && (_BOS_NULL(dest) || _BOS_ZERO(dest, dmax) || !dmax),
             "empty dest or dmax")
        BOS_ATTR(slen &&_BOSW_OVR(dest, dmax), "dest overflow")
            BOS_ATTR(!slen && !_BOS_NULL(dest), "empty slen")
                BOS_ATTR(slen && (_BOSW_OVR(src, slen) || _BOS_NULL(src)),
                         "src overflow or empty");
#define wcsncat_s(dest, dmax, src, slen)                                       \
    _wcsncat_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

EXTERN wchar_t *_wcstok_s_chk(wchar_t *restrict dest, rsize_t *restrict dmaxp,
                              const wchar_t *restrict delim,
                              wchar_t **restrict ptr, const size_t destbos)
    BOS_ATTR(_BOS_NULL(dmaxp) || !*dmaxp, "empty dmax")
        BOSW_OVR2_BUTNULL(dest, *dmaxp) BOS_NULL(delim) BOS_NULL(ptr);
#if !(defined(_WIN32) && defined(HAVE_WCSTOK_S))
/* they use a buggy:
wchar_t* wcstok_s(wchar_t *_Str, const wchar_t *_Delim, wchar_t **_Context); */
#define wcstok_s(dest, dmaxp, delim, ptr)                                      \
    _wcstok_s_chk(dest, dmaxp, delim, ptr, BOS(dest))
#endif

#if defined(SAFECLIB_HAVE_C99) && !defined(TEST_MSVCRT)
EXTERN int _swprintf_s_chk(wchar_t *restrict dest, rsize_t dmax,
                           const size_t destbos, const wchar_t *restrict fmt,
                           ...) __attribute_format_wprintf(4, 5) BOSW_CHK(dest)
    BOS_FMT(fmt);
#define swprintf_s(dest, dmax, ...)                                            \
    _swprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)
#else
EXTERN int swprintf_s(wchar_t *restrict dest, rsize_t dmax,
                      const wchar_t *restrict fmt, ...)
    __attribute_format_wprintf(3, 4) BOSW_CHK(dest) BOS_FMT(fmt);
#endif

EXTERN int _vswprintf_s_chk(wchar_t *restrict dest, rsize_t dmax,
                            const size_t destbos, const wchar_t *restrict fmt,
                            va_list ap) BOSW_CHK(dest) BOS_FMT(fmt);
#define vswprintf_s(dest, dmax, fmt, ap)                                       \
    _vswprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)

/* truncating, no ESNOSPC */
#if defined(SAFECLIB_HAVE_C99) && !defined(TEST_MSVCRT) && !defined(FOR_DOXYGEN)
EXTERN int _snwprintf_s_chk(wchar_t *restrict dest, rsize_t dmax,
                            const size_t destbos, const wchar_t *restrict fmt,
                            ...) __attribute_format_wprintf(4, 5) BOSW_CHK(dest)
    BOS_FMT(fmt);
#define snwprintf_s(dest, dmax, ...)                                           \
    _snwprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)
#else
EXTERN int snwprintf_s(wchar_t *restrict dest, rsize_t dmax,
                       const wchar_t *restrict fmt, ...)
    __attribute_format_wprintf(3, 4) BOSW_CHK(dest) BOS_FMT(fmt);
#endif

EXTERN int _vsnwprintf_s_chk(wchar_t *restrict dest, rsize_t dmax,
                             const size_t destbos, const wchar_t *restrict fmt,
                             va_list ap) BOSW_CHK(dest) BOS_FMT(fmt);
#define vsnwprintf_s(dest, dmax, fmt, ap)                                      \
    _vsnwprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)

EXTERN int wprintf_s(const wchar_t *restrict fmt, ...)
    __attribute_format_wprintf(1, 2) BOS_FMT(fmt);

EXTERN int vwprintf_s(const wchar_t *restrict fmt, va_list ap) BOS_FMT(fmt);

#if !defined(__KERNEL__) && !defined(SAFECLIB_DISABLE_IO)
EXTERN int fwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...)
    __attribute_format_wprintf(2, 3) BOS_NULL(stream) BOS_FMT(fmt);

EXTERN int vfwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt,
                       va_list ap) BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int swscanf_s(const wchar_t *restrict src, const wchar_t *restrict fmt,
                     ...) __attribute_format_wscanf(2, 3) BOS_NULL(src)
    BOS_FMT(fmt);

EXTERN int vswscanf_s(const wchar_t *restrict src, const wchar_t *restrict fmt,
                      va_list ap) BOS_NULL(src) BOS_FMT(fmt);

EXTERN int wscanf_s(const wchar_t *restrict fmt, ...)
    __attribute_format_wscanf(1, 2) BOS_FMT(fmt);

EXTERN int vwscanf_s(const wchar_t *restrict fmt, va_list ap) BOS_FMT(fmt);

#if !defined(__KERNEL__) && !defined(SAFECLIB_DISABLE_IO)
EXTERN int fwscanf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...)
    __attribute_format_wscanf(2, 3) BOS_NULL(stream) BOS_FMT(fmt);

EXTERN int vfwscanf_s(FILE *restrict stream, const wchar_t *restrict fmt,
                      va_list ap) BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* search wide substring */
EXTERN errno_t _wcsstr_s_chk(wchar_t *restrict dest, rsize_t dmax,
                             const wchar_t *restrict src, rsize_t slen,
                             wchar_t **restrict substringp,
                             const size_t destbos, const size_t srcbos)
    BOSW_CHK(dest) BOSW_OVR2(src, slen) BOS_NULL(substringp);
#define wcsstr_s(dest, dmax, src, slen, substringp)                            \
    _wcsstr_s_chk(dest, dmax, src, slen, substringp, BOS(dest), BOS(src))

/* compare */
EXTERN errno_t _wcscmp_s_chk(const wchar_t *restrict dest, rsize_t dmax,
                             const wchar_t *restrict src, rsize_t smax,
                             int *resultp, const size_t destbos,
                             const size_t srcbos) BOSW_CHK(dest)
    BOSW_CHK2(src, smax) BOS_NULL(resultp);
#define wcscmp_s(dest, dmax, src, smax, resultp)                               \
    _wcscmp_s_chk(dest, dmax, src, smax, resultp, BOS(dest), BOS(src))

EXTERN errno_t _wcsncmp_s_chk(const wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict src, rsize_t smax,
                              rsize_t count, int *resultp, const size_t destbos,
                              const size_t srcbos) BOSW_CHK(dest)
    BOSW_CHK2(src, smax) BOS_NULL(resultp);
#define wcsncmp_s(dest, dmax, src, smax, count, resultp)                       \
    _wcsncmp_s_chk(dest, dmax, src, smax, count, resultp, BOS(dest), BOS(src))

/* compare case-folded */
EXTERN errno_t _wcsicmp_s_chk(const wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict src, rsize_t smax,
                              int *resultp, const size_t destbos,
                              const size_t srcbos) BOSW_CHK(dest)
    BOSW_CHK2(src, smax) BOS_NULL(resultp);
#define wcsicmp_s(dest, dmax, src, smax, resultp)                              \
    _wcsicmp_s_chk(dest, dmax, src, smax, resultp, BOS(dest), BOS(src))

/* natural sort-order */
EXTERN errno_t _wcsnatcmp_s_chk(const wchar_t *restrict dest, rsize_t dmax,
                                const wchar_t *restrict src, rsize_t smax,
                                const int fold_case, int *resultp,
                                const size_t destbos, const size_t srcbos)
    BOSW_CHK(dest) BOSW_CHK2(src, smax) BOS_NULL(resultp);
#define wcsnatcmp_s(dest, dmax, src, smax, resultp)                            \
    _wcsnatcmp_s_chk(dest, dmax, src, smax, 0, resultp, BOS(dest), BOS(src))
#define wcsnaticmp_s(dest, dmax, src, smax, resultp)                           \
    _wcsnatcmp_s_chk(dest, dmax, src, smax, 1, resultp, BOS(dest), BOS(src))

EXTERN errno_t _wcsset_s_chk(wchar_t *restrict dest, rsize_t dmax,
                             const wchar_t value, const size_t destbos)
    BOSW_CHK(dest) VAL_OVR2(value, 0x10ffff);
EXTERN errno_t _wcsset_s_uchk(wchar_t *dest, rsize_t dmax, const wchar_t value);
#define wcsset_s(dest, dmax, value)                                            \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, sizeof(wchar_t)) &&                     \
                  (wchar_t)(value) <= 0x10ffff,                                \
              _wcsset_s_uchk(dest, dmax, value),                               \
              _wcsset_s_chk(dest, dmax, value, BOS(dest)))

EXTERN errno_t _wcsnset_s_chk(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t value, size_t n,
                              const size_t destbos) BOSW_CHK(dest)
    VAL_OVR2(value, 0x10ffff);
EXTERN errno_t _wcsnset_s_uchk(wchar_t *dest, rsize_t dmax, const wchar_t value,
                               size_t n);
#define wcsnset_s(dest, dmax, value, n)                                        \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, sizeof(wchar_t)) &&                     \
                  (wchar_t)(value) <= 0x10ffff &&                              \
                  (size_t)(n) <= (size_t)(dmax),                               \
              _wcsnset_s_uchk(dest, dmax, value, n),                           \
              _wcsnset_s_chk(dest, dmax, value, n, BOS(dest)))

EXTERN errno_t _wcscoll_s_chk(const wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict src, rsize_t smax,
                              int *resultp, const size_t destbos,
                              const size_t srcbos) BOSW_CHK(dest)
    BOSW_CHK2(src, smax) BOS_NULL(resultp);
#define wcscoll_s(dest, dmax, src, smax, resultp)                              \
    _wcscoll_s_chk(dest, dmax, src, smax, resultp, BOS(dest), BOS(src))

/* simple char-wise folding */
EXTERN errno_t _wcslwr_s_chk(wchar_t *restrict src, rsize_t slen,
                             const size_t srcbos) BOSW_OVR2_BUTZERO(src, slen);
#define wcslwr_s(src, slen) _wcslwr_s_chk(src, slen, BOS(src))

EXTERN errno_t _wcsupr_s_chk(wchar_t *restrict src, rsize_t slen,
                             const size_t srcbos) BOSW_OVR2_BUTZERO(src, slen);
#define wcsupr_s(src, slen) _wcsupr_s_chk(src, slen, BOS(src))

/* is a wide upper character which folds to multiple lowercase chars? how
 * many */
EXTERN int iswfc(const uint32_t wc) VAL_OVR2(wc, 0x10ffff);

/* full foldcase a single upper char to mult. lower chars */
EXTERN int _towfc_s_chk(wchar_t *restrict dest, rsize_t dmax,
                        const uint32_t src, const size_t destbos) BOSW_CHK(dest)
    BOS_ATTR(dmax < 4, "dmax underflow <4");
EXTERN int _towfc_s_uchk(wchar_t *dest, const uint32_t src);
#define towfc_s(dest, dmax, src)                                               \
    _BOS_UCHK(_BOS_DMAX_OK(dest, dmax, sizeof(wchar_t)) &&                     \
                  (size_t)(dmax) >= 4 && (size_t)(dmax) <= RSIZE_MAX_WSTR,     \
              _towfc_s_uchk(dest, src),                                        \
              _towfc_s_chk(dest, dmax, src, BOS(dest)))

/* full foldcase + NFD normalization */
EXTERN errno_t _wcsfc_s_chk(wchar_t *restrict dest, rsize_t dmax,
                            const wchar_t *restrict src, rsize_t *restrict lenp,
                            const size_t destbos) BOSW_CHK(dest) BOS_NULL(src);
#define wcsfc_s(dest, dmax, src, lenp)                                         \
    _wcsfc_s_chk(dest, dmax, src, lenp, BOS(dest))

/* Normalize to FCD/pre-NFKD */
EXTERN errno_t _wcsnorm_decompose_s_chk(wchar_t *restrict dest, rsize_t dmax,
                                        const wchar_t *restrict src,
                                        rsize_t *restrict lenp,
                                        const bool iscompat,
                                        const size_t destbos) BOSW_CHK(dest)
    BOS_NULL(src);
#define wcsnorm_decompose_s(dest, dmax, src, lenp, iscompat)                   \
    _wcsnorm_decompose_s_chk(dest, dmax, src, lenp, iscompat, BOS(dest))

/* Normalize to NCD/NFKD */
EXTERN errno_t _wcsnorm_reorder_s_chk(wchar_t *restrict dest, rsize_t dmax,
                                      const wchar_t *restrict src,
                                      const rsize_t len, const size_t destbos)
    BOSW_CHK(dest) BOSW_OVR2(src, len);
#define wcsnorm_reorder_s(dest, dmax, src, len)                                \
    _wcsnorm_reorder_s_chk(dest, dmax, src, len, BOS(dest))

/* Normalize to NFC/NFKC */
EXTERN errno_t _wcsnorm_compose_s_chk(wchar_t *restrict dest, rsize_t dmax,
                                      const wchar_t *restrict src,
                                      rsize_t *restrict lenp, bool iscontig,
                                      const size_t destbos) BOSW_CHK(dest)
    BOS_NULL(src) BOS_NULL(lenp);
#define wcsnorm_compose_s(dest, dmax, src, lenp, iscontig)                     \
    _wcsnorm_compose_s_chk(dest, dmax, src, lenp, iscontig, BOS(dest))

enum wcsnorm_mode {
    WCSNORM_NFD = 0,
    WCSNORM_NFC = 1,  /* default */
    WCSNORM_FCD = 2,  /* not reordered */
    WCSNORM_FCC = 3,  /* contiguous composition only */
    WCSNORM_NFKD = 4, /* compat. OPTIONAL with --enable-norm-compat */
    WCSNORM_NFKC = 5  /* compat. OPTIONAL with --enable-norm-compat */
};
typedef enum wcsnorm_mode wcsnorm_mode_t;

/* Normalize to NFC (default), NFD nfc=0.
   experim. nfc>1: FCD, FCC */
EXTERN errno_t _wcsnorm_s_chk(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t *restrict src,
                              const wcsnorm_mode_t mode, rsize_t *restrict lenp,
                              const size_t destbos) BOSW_CHK(dest)
    BOS_NULL(src);
#define wcsnorm_s(dest, dmax, src, mode, lenp)                                 \
    _wcsnorm_s_chk(dest, dmax, src, mode, lenp, BOS(dest))

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

#endif /* SAFECLIB_DISABLE_WCHAR */

#ifdef SAFECLIB_ENABLE_U8
#include "safe_u8_lib.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_STR_LIB_H__ */
