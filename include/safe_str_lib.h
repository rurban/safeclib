/*------------------------------------------------------------------
 * safe_str_lib.h -- Safe C Library String APIs
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
 *
 * Copyright (c) 2008-2011, 2013 by Cisco Systems, Inc.
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

#ifndef __SAFE_STR_LIB_H__
#define __SAFE_STR_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "safe_config.h"
#include "safe_lib_errno.h"
#include "safe_types.h"
#include "safe_compile.h"

#include <stdarg.h>
#ifndef __KERNEL__
#include <time.h>
#if defined HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#endif /* __KERNEL__ */
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

/**
 * The shortest string is a null string!!
 */
#define RSIZE_MIN_STR (1)

/** wide chars */
#define RSIZE_MAX_WSTR (RSIZE_MAX_STR / sizeof(wchar_t))

/** The makeup of a password */
#define SAFE_STR_MIN_LOWERCASE (2)
#define SAFE_STR_MIN_UPPERCASE (2)
#define SAFE_STR_MIN_NUMBERS (1)
#define SAFE_STR_MIN_SPECIALS (1)

#define SAFE_STR_PASSWORD_MIN_LENGTH (6)
#define SAFE_STR_PASSWORD_MAX_LENGTH (32)

EXTERN void abort_handler_s(const char *restrict msg, void *restrict ptr,
                            errno_t error)
#ifdef __GNUC__
    __attribute__((noreturn))
#endif
    ;

EXTERN void ignore_handler_s(const char *restrict msg, void *restrict ptr,
                             errno_t error);

#define sl_default_handler ignore_handler_s

/* set string constraint handler */
EXTERN constraint_handler_t
set_str_constraint_handler_s(constraint_handler_t handler);

/* string concatenate */
#if !defined(TEST_MSVCRT)
EXTERN errno_t _strcat_s_chk(char *restrict dest, rsize_t dmax,
                             const char *restrict src, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define strcat_s(dest, dmax, src) _strcat_s_chk(dest, dmax, src, BOS(dest))

/* string copy */
EXTERN errno_t _strcpy_s_chk(char *restrict dest, rsize_t dmax,
                             const char *restrict src, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define strcpy_s(dest, dmax, src) _strcpy_s_chk(dest, dmax, src, BOS(dest))
#endif

/* fitted string concatenate */
EXTERN errno_t _strncat_s_chk(char *restrict dest, rsize_t dmax,
                              const char *restrict src, rsize_t slen,
                              const size_t destbos, const size_t srcbos)
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
#define strnlen_s(str, smax) _strnlen_s_chk(str, smax, BOS(str))

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
#ifdef SAFECLIB_HAVE_C99
EXTERN int _sprintf_s_chk(char *restrict dest, const rsize_t dmax,
                          const size_t destbos, const char *restrict fmt, ...)
    BOS_CHK(dest) BOS_FMT(fmt);
#define sprintf_s(dest, dmax, ...)                                             \
    _sprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)
#else
EXTERN int sprintf_s(char *restrict dest, rsize_t dmax,
                     const char *restrict fmt, ...) BOS_CHK(dest) BOS_FMT(fmt);
#endif

EXTERN int _vsprintf_s_chk(char *restrict dest, rsize_t dmax,
                           const size_t destbos, const char *restrict fmt,
                           va_list ap) BOS_CHK(dest) BOS_FMT(fmt);
#define vsprintf_s(dest, dmax, fmt, ap)                                        \
    _vsprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)

/* truncating, no ESNOSPC */
#if defined(SAFECLIB_HAVE_C99) && !defined(TEST_MSVCRT)
EXTERN int _snprintf_s_chk(char *restrict dest, rsize_t dmax,
                           const size_t destbos, const char *restrict fmt, ...)
    BOS_CHK(dest) BOS_FMT(fmt);
#define snprintf_s(dest, dmax, ...)                                            \
    _snprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)
#else
EXTERN int snprintf_s(char *restrict dest, rsize_t dmax,
                      const char *restrict fmt, ...) BOS_CHK(dest) BOS_FMT(fmt);
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
    BOS_NULL(buffer) BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int fscanf_s(FILE *restrict stream, const char *restrict fmt, ...)
    BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int scanf_s(const char *restrict fmt, ...) BOS_FMT(fmt);

EXTERN int vscanf_s(const char *restrict fmt, va_list ap) BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int vfscanf_s(FILE *restrict stream, const char *restrict fmt,
                     va_list ap) BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int vsscanf_s(const char *restrict dest, const char *restrict fmt,
                     va_list ap) BOS_NULL(dest) BOS_FMT(fmt);

EXTERN int printf_s(const char *restrict fmt, ...) BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int fprintf_s(FILE *restrict stream, const char *restrict fmt, ...)
    BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int vprintf_s(const char *restrict fmt, va_list ap) BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int vfprintf_s(FILE *restrict stream, const char *restrict fmt,
                      va_list arg) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN errno_t _strerror_s_chk(char *dest, rsize_t dmax, errno_t errnum,
                               const size_t destbos) BOS_CHK(dest);
#define strerror_s(dest, dmax, errnum)                                         \
    _strerror_s_chk(dest, dmax, errnum, BOS(dest))

EXTERN size_t strerrorlen_s(errno_t errnum);

/* for the other safe IO funcs see safe_lib.h */

#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* string compare */
EXTERN errno_t _strcmp_s_chk(const char *dest, rsize_t dmax, const char *src,
                             int *resultp, const size_t destbos,
                             const size_t srcbos) BOS_CHK(dest) BOS_NULL(src)
    BOS_NULL(resultp);
#define strcmp_s(dest, dmax, src, resultp)                                     \
    _strcmp_s_chk(dest, dmax, src, resultp, BOS(dest), BOS(src))

/* string compare case-insensitive */
EXTERN errno_t _strcasecmp_s_chk(const char *dest, rsize_t dmax,
                                 const char *src, int *resultp,
                                 const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
#define strcasecmp_s(dest, dmax, src, resultp)                                 \
    _strcasecmp_s_chk(dest, dmax, src, resultp, BOS(dest))

/* natural order string compare */
EXTERN errno_t _strnatcmp_s_chk(const char *dest, rsize_t dmax, const char *src,
                                const int fold_case, int *resultp,
                                const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(resultp);
#define strnatcmp_s(dest, dmax, src, resultp)                                  \
    _strnatcmp_s_chk(dest, dmax, src, 0, resultp, BOS(dest), BOS(src))
#define strnatcasecmp_s(dest, dmax, src, resultp)                              \
    _strnatcasecmp_s_chk(dest, dmax, src, 1, resultp, BOS(dest), BOS(src))

/* find a substring - case insensitive */
EXTERN errno_t _strcasestr_s_chk(char *dest, rsize_t dmax, const char *src,
                                 rsize_t slen, char **substring,
                                 const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(substring);
#define strcasestr_s(dest, dmax, src, slen, substring)                         \
    _strcasestr_s_chk(dest, dmax, src, slen, substring, BOS(dest), BOS(src))

/* fixed field string compare */
EXTERN errno_t _strcmpfld_s_chk(const char *dest, rsize_t dmax, const char *src,
                                int *resultp, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(resultp);
#define strcmpfld_s(dest, dmax, src, resultp)                                  \
    _strcmpfld_s_chk(dest, dmax, src, resultp, BOS(dest))

/* fixed char array copy */
EXTERN errno_t _strcpyfld_s_chk(char *dest, const rsize_t dmax, const char *src,
                                rsize_t slen, const size_t destbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2(slen, dmax);
#define strcpyfld_s(dest, dmax, src, slen)                                     \
    _strcpyfld_s_chk(dest, dmax, src, slen, BOS(dest))

/* copy from a null terminated string to fixed char array */
EXTERN errno_t _strcpyfldin_s_chk(char *dest, rsize_t dmax, const char *src,
                                  rsize_t slen, const size_t destbos)
    BOS_CHK_BUTZERO(dest, slen) BOS_OVR2_BUTZERO(src, slen)
        VAL_OVR2(slen, dmax);
#define strcpyfldin_s(dest, dmax, src, slen)                                   \
    _strcpyfldin_s_chk(dest, dmax, src, slen, BOS(dest))

/* copy from a char array to null terminated string */
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
#define strcspn_s(dest, dmax, src, slen, countp)                               \
    _strcspn_s_chk(dest, dmax, src, slen, countp, BOS(dest), BOS(src))

/* returns a pointer to the first occurrence of c in dest */
EXTERN errno_t _strfirstchar_s_chk(char *dest, rsize_t dmax, char c,
                                   char **firstp, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(firstp);
#define strfirstchar_s(dest, dmax, c, firstp)                                  \
    _strfirstchar_s_chk(dest, dmax, c, firstp, BOS(dest))

/* returns index of first difference */
EXTERN errno_t _strfirstdiff_s_chk(const char *dest, rsize_t dmax,
                                   const char *src, rsize_t *resultp,
                                   const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
#define strfirstdiff_s(dest, dmax, src, resultp)                               \
    _strfirstdiff_s_chk(dest, dmax, src, resultp, BOS(dest))

/* validate alphanumeric string */
EXTERN bool _strisalphanumeric_s_chk(const char *dest, rsize_t dmax,
                                     const size_t destbos) BOS_CHK2(dest, dmax);
#define strisalphanumeric_s(dest, dmax)                                        \
    _strisalphanumeric_s_chk(dest, dmax, BOS(dest))

/* validate ascii string */
EXTERN bool _strisascii_s_chk(const char *dest, rsize_t dmax,
                              const size_t destbos) BOS_CHK2(dest, dmax);
#define strisascii_s(dest, dmax) _strisascii_s_chk(dest, dmax, BOS(dest))

/* validate string of digits */
EXTERN bool _strisdigit_s_chk(const char *dest, rsize_t dmax,
                              const size_t destbos) BOS_CHK2(dest, dmax);
#define strisdigit_s(dest, dmax) _strisdigit_s_chk(dest, dmax, BOS(dest))

/* validate hex string */
EXTERN bool _strishex_s_chk(const char *dest, rsize_t dmax,
                            const size_t destbos) BOS_CHK2(dest, dmax);
#define strishex_s(dest, dmax) _strishex_s_chk(dest, dmax, BOS(dest))

/* validate lower case */
EXTERN bool _strislowercase_s_chk(const char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK2(dest, dmax);
#define strislowercase_s(dest, dmax)                                           \
    _strislowercase_s_chk(dest, dmax, BOS(dest))

/* validate mixed case */
EXTERN bool _strismixedcase_s_chk(const char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK2(dest, dmax);
#define strismixedcase_s(dest, dmax)                                           \
    _strismixedcase_s_chk(dest, dmax, BOS(dest))

/* validate password */
EXTERN bool _strispassword_s_chk(const char *dest, rsize_t dmax,
                                 const size_t destbos) BOS_CHK2(dest, dmax);
#define strispassword_s(dest, dmax) _strispassword_s_chk(dest, dmax, BOS(dest))

/* validate upper case */
EXTERN bool _strisuppercase_s_chk(const char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK2(dest, dmax);
#define strisuppercase_s(dest, dmax)                                           \
    _strisuppercase_s_chk(dest, dmax, BOS(dest))

/* returns  a pointer to the last occurrence of c in s1 */
EXTERN errno_t _strlastchar_s_chk(char *dest, rsize_t dmax, char c,
                                  char **lastp, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(lastp);
#define strlastchar_s(dest, dmax, c, lastp)                                    \
    _strlastchar_s_chk(dest, dmax, c, lastp, BOS(dest))

/* returns index of last difference */
EXTERN errno_t _strlastdiff_s_chk(const char *dest, rsize_t dmax,
                                  const char *src, rsize_t *resultp,
                                  const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
#define strlastdiff_s(dest, dmax, src, resultp)                                \
    _strlastdiff_s_chk(dest, dmax, src, resultp, BOS(dest))

/* left justify */
EXTERN errno_t _strljustify_s_chk(char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK(dest);
#define strljustify_s(dest, dmax) _strljustify_s_chk(dest, dmax, BOS(dest))

/* string terminate */
EXTERN rsize_t _strnterminate_s_chk(char *dest, rsize_t dmax,
                                    const size_t destbos) BOS_CHK2(dest, dmax);
#define strnterminate_s(dest, dmax) _strnterminate_s_chk(dest, dmax, BOS(dest))

/* get pointer to first occurrence from set of char */
EXTERN errno_t _strpbrk_s_chk(char *dest, rsize_t dmax, char *src, rsize_t slen,
                              char **firstp, const size_t destbos,
                              const size_t srcbos) BOS_CHK(dest)
    BOS_OVR2(src, slen) BOS_NULL(firstp);
#define strpbrk_s(dest, dmax, src, slen, firstp)                               \
    _strpbrk_s_chk(dest, dmax, src, slen, firstp, BOS(dest), BOS(src))

EXTERN errno_t _strfirstsame_s_chk(const char *dest, rsize_t dmax,
                                   const char *src, rsize_t *resultp,
                                   const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
#define strfirstsame_s(dest, dmax, src, resultp)                               \
    _strfirstsame_s_chk(dest, dmax, src, resultp, BOS(dest))

EXTERN errno_t _strlastsame_s_chk(const char *dest, rsize_t dmax,
                                  const char *src, rsize_t *resultp,
                                  const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
#define strlastsame_s(dest, dmax, src, resultp)                                \
    _strlastsame_s_chk(dest, dmax, src, resultp, BOS(dest))

/* searches for a prefix */
EXTERN errno_t _strprefix_s_chk(const char *dest, rsize_t dmax, const char *src,
                                const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src);
#define strprefix_s(dest, dmax, src)                                           \
    _strprefix_s_chk(dest, dmax, src, BOS(dest))

/* removes leading and trailing white space */
EXTERN errno_t _strremovews_s_chk(char *dest, rsize_t dmax,
                                  const size_t destbos) BOS_CHK(dest);
#define strremovews_s(dest, dmax) _strremovews_s_chk(dest, dmax, BOS(dest))

/* computes inclusive prefix length */
EXTERN errno_t _strspn_s_chk(const char *dest, rsize_t dmax, const char *src,
                             rsize_t slen, rsize_t *countp,
                             const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(countp);
#define strspn_s(dest, dmax, src, slen, countp)                                \
    _strspn_s_chk(dest, dmax, src, slen, countp, BOS(dest), BOS(src))

/* find a substring */
EXTERN errno_t _strstr_s_chk(char *dest, rsize_t dmax, const char *src,
                             rsize_t slen, char **substringp,
                             const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2(src, slen) BOS_NULL(substringp);
#define strstr_s(dest, dmax, src, slen, substringp)                            \
    _strstr_s_chk(dest, dmax, src, slen, substringp, BOS(dest), BOS(src))

/* find a character */
EXTERN errno_t _strchr_s_chk(const char *restrict dest, rsize_t dmax,
                             const int ch, char **restrict resultp,
                             const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(ch, 255) BOS_NULL(resultp);
#define strchr_s(dest, dmax, ch, resultp)                                      \
    _strchr_s_chk(dest, dmax, ch, resultp, BOS(dest))

EXTERN errno_t _strrchr_s_chk(const char *restrict dest, rsize_t dmax,
                              const int ch, char **restrict resultp,
                              const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(ch, 255) BOS_NULL(resultp) BOS_ATTR(!*dest, "empty *dest");
#define strrchr_s(dest, dmax, ch, resultp)                                     \
    _strrchr_s_chk(dest, dmax, ch, resultp, BOS(dest))

/* convert string to lowercase.
   mingw string_s.h: _strlwr_s */
EXTERN errno_t _strtolowercase_s_chk(char *dest, rsize_t dmax,
                                     const size_t destbos) BOS_CHK2(dest, dmax);
#define strtolowercase_s(dest, dmax)                                           \
    _strtolowercase_s_chk(dest, dmax, BOS(dest))

/* convert string to uppercase
   mingw string_s.h: _strupr_s */
EXTERN errno_t _strtouppercase_s_chk(char *dest, rsize_t dmax,
                                     const size_t destbos) BOS_CHK2(dest, dmax);
#define strtouppercase_s(dest, dmax)                                           \
    _strtouppercase_s_chk(dest, dmax, BOS(dest))

#define strlwr_s(str, slen) strtolowercase_s((str), (slen))
#define strupr_s(str, slen) strtouppercase_s((str), (slen))

/* zero an entire string with nulls.
   mingw string_s.h has: _strset_s */
EXTERN errno_t _strzero_s_chk(char *dest, rsize_t dmax, const size_t destbos)
    BOS_CHK(dest);
#define strzero_s(dest, dmax) _strzero_s_chk(dest, dmax, BOS(dest))

EXTERN errno_t _strcoll_s_chk(const char *restrict dest, rsize_t dmax,
                              const char *restrict src, int *resultp,
                              const size_t destbos) BOS_CHK(dest) BOS_NULL(src)
    BOS_NULL(resultp);
#define strcoll_s(dest, dmax, src, resultp)                                    \
    _strcoll_s_chk(dest, dmax, src, resultp, BOS(dest))

/* Derived from windows extensions sec_api/string_s.h
   defined(MINGW_HAS_SECURE_API) */

EXTERN errno_t _strset_s_chk(char *restrict dest, rsize_t dmax, int value,
                             const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(value, 255);
#define strset_s(dest, dmax, value) _strset_s_chk(dest, dmax, value, BOS(dest))

EXTERN errno_t _strnset_s_chk(char *restrict dest, rsize_t dmax, int value,
                              rsize_t n, const size_t destbos) BOS_CHK(dest)
    BOS_OVR2_BUTZERO(dest, n) VAL_OVR2(value, 255) VAL_OVR2_BUTZERO(n, dmax);
#define strnset_s(dest, dmax, value, n)                                        \
    _strnset_s_chk(dest, dmax, value, n, BOS(dest))

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
#define wcsnlen_s(str, smax) _wcsnlen_s_chk(str, smax, BOS(str))

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
                           ...) BOSW_CHK(dest) BOS_FMT(fmt);
#define swprintf_s(dest, dmax, ...)                                            \
    _swprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)
#else
EXTERN int swprintf_s(wchar_t *restrict dest, rsize_t dmax,
                      const wchar_t *restrict fmt, ...) BOSW_CHK(dest)
    BOS_FMT(fmt);
#endif

EXTERN int _vswprintf_s_chk(wchar_t *restrict dest, rsize_t dmax,
                            const size_t destbos, const wchar_t *restrict fmt,
                            va_list ap) BOSW_CHK(dest) BOS_FMT(fmt);
#define vswprintf_s(dest, dmax, fmt, ap)                                       \
    _vswprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)

/* truncating, no ESNOSPC */
#if defined(SAFECLIB_HAVE_C99) && !defined(TEST_MSVCRT)
EXTERN int _snwprintf_s_chk(wchar_t *restrict dest, rsize_t dmax,
                            const size_t destbos, const wchar_t *restrict fmt,
                            ...) BOSW_CHK(dest) BOS_FMT(fmt);
#define snwprintf_s(dest, dmax, ...)                                           \
    _snwprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)
#else
EXTERN int snwprintf_s(wchar_t *restrict dest, rsize_t dmax,
                       const wchar_t *restrict fmt, ...) BOSW_CHK(dest)
    BOS_FMT(fmt);
#endif

EXTERN int _vsnwprintf_s_chk(wchar_t *restrict dest, rsize_t dmax,
                             const size_t destbos, const wchar_t *restrict fmt,
                             va_list ap) BOSW_CHK(dest) BOS_FMT(fmt);
#define vsnwprintf_s(dest, dmax, fmt, ap)                                      \
    _vsnwprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)

EXTERN int wprintf_s(const wchar_t *restrict fmt, ...) BOS_FMT(fmt);

EXTERN int vwprintf_s(const wchar_t *restrict fmt, va_list ap) BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int fwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...)
    BOS_NULL(stream) BOS_FMT(fmt);

EXTERN int vfwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt,
                       va_list ap) BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int swscanf_s(const wchar_t *restrict src, const wchar_t *restrict fmt,
                     ...) BOS_NULL(src) BOS_FMT(fmt);

EXTERN int vswscanf_s(const wchar_t *restrict src, const wchar_t *restrict fmt,
                      va_list ap) BOS_NULL(src) BOS_FMT(fmt);

EXTERN int wscanf_s(const wchar_t *restrict fmt, ...) BOS_FMT(fmt);

EXTERN int vwscanf_s(const wchar_t *restrict fmt, va_list ap) BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int fwscanf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...)
    BOS_NULL(stream) BOS_FMT(fmt);

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
#define wcsset_s(dest, dmax, value) _wcsset_s_chk(dest, dmax, value, BOS(dest))

EXTERN errno_t _wcsnset_s_chk(wchar_t *restrict dest, rsize_t dmax,
                              const wchar_t value, size_t n,
                              const size_t destbos) BOSW_CHK(dest)
    VAL_OVR2(value, 0x10ffff);
#define wcsnset_s(dest, dmax, value, n)                                        \
    _wcsnset_s_chk(dest, dmax, value, n, BOS(dest))

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
#define towfc_s(dest, dmax, src) _towfc_s_chk(dest, dmax, src, BOS(dest))

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

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_STR_LIB_H__ */
