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
# if defined(EXPORT) && defined(__SAFECLIB_PRIVATE_H__)
#  define EXTERN extern __declspec(dllexport)
# else
#  define EXTERN extern __declspec(dllimport)
# endif
#else
# define EXTERN extern
#endif

/*
 * The shortest string is a null string!!
 */
#define RSIZE_MIN_STR      ( 1 )

/* wide chars */
#define RSIZE_MAX_WSTR    ( RSIZE_MAX_STR/sizeof(wchar_t) )

/* The makeup of a password */
#define SAFE_STR_MIN_LOWERCASE     ( 2 )
#define SAFE_STR_MIN_UPPERCASE     ( 2 )
#define SAFE_STR_MIN_NUMBERS       ( 1 )
#define SAFE_STR_MIN_SPECIALS      ( 1 )

#define SAFE_STR_PASSWORD_MIN_LENGTH   ( 6 )
#define SAFE_STR_PASSWORD_MAX_LENGTH   ( 32 )

EXTERN void
abort_handler_s(const char *restrict msg, void *restrict ptr, errno_t error)
#ifdef __GNUC__
__attribute__((noreturn))
#endif
;

EXTERN void
ignore_handler_s(const char *restrict msg, void *restrict ptr, errno_t error);

#define sl_default_handler ignore_handler_s

/* set string constraint handler */
EXTERN constraint_handler_t
set_str_constraint_handler_s(constraint_handler_t handler);


/* string concatenate */
EXTERN errno_t
_strcat_s_chk(char * restrict dest, rsize_t dmax, const char * restrict src,
              const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define strcat_s(dest,dmax,src) _strcat_s_chk(dest,dmax,src,BOS(dest))

/* string copy */
EXTERN errno_t
_strcpy_s_chk(char * restrict dest, rsize_t dmax, const char * restrict src,
              const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define strcpy_s(dest,dmax,src) _strcpy_s_chk(dest,dmax,src,BOS(dest))

/* fitted string concatenate */
EXTERN errno_t
_strncat_s_chk(char * restrict dest, rsize_t dmax, const char * restrict src, rsize_t slen,
               const size_t destbos, const size_t srcbos)
    BOS_ATTR((slen || dest || dmax) && ( _BOS_NULL(dest) || _BOS_ZERO(dest,dmax)), "empty dest or dmax")
    BOS_ATTR((slen || dest || dmax) && _BOS_OVR(dest,dmax), "dest overflow")
    BOS_OVR2_BUTZERO(src, slen);
#define strncat_s(dest,dmax,src,slen) _strncat_s_chk(dest,dmax,src,slen,BOS(dest),BOS(src))

/* fitted string copy */
EXTERN errno_t
_strncpy_s_chk(char * restrict dest, rsize_t dmax,
               const char * restrict src, rsize_t slen,
               const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2_BUTZERO(src, slen);
#define strncpy_s(dest,dmax,src,slen) _strncpy_s_chk(dest,dmax,src,slen,BOS(dest),BOS(src))

/* string length */
EXTERN rsize_t
_strnlen_s_chk (const char* str, rsize_t smax, const size_t strbos)
    BOS_CHK2(str, smax);
#define strnlen_s(str,smax) _strnlen_s_chk(str,smax,BOS(str))

/* string tokenizer */
EXTERN char *
_strtok_s_chk(char *restrict dest, rsize_t *restrict dmaxp,
              const char *restrict delim, char **restrict ptr,
              const size_t destbos)
    BOS_OVR2_BUTNULL(dest, *dmaxp)
    BOS_ATTR(dest && _BOS_NULL(dmaxp), "empty dmax")
    BOS_NULL(delim)
    BOS_NULL(ptr);
#if !(defined(_WIN32) && defined(HAVE_STRTOK_S))
/* they use:
char * strtok_s(char *_Str,const char *_Delim,char **_Context); */
#define strtok_s(dest,dmaxp,delim,ptr) _strtok_s_chk(dest,dmaxp,delim,ptr,BOS(dest))
#endif

/* secure stdio */

/* safe sprintf_s */
/* now __STDC_WANT_LIB_EXT1__ >= 1 compatible */
#ifdef HAVE_C99
EXTERN int
_sprintf_s_chk(char *restrict dest, const rsize_t dmax, const size_t destbos,
               const char *restrict fmt, ...)
    BOS_CHK(dest) BOS_FMT(fmt);
#define sprintf_s(dest,dmax,...) _sprintf_s_chk(dest,dmax,BOS(dest),__VA_ARGS__)
#else
EXTERN int
sprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, ...)
    BOS_CHK(dest) BOS_FMT(fmt);
#endif

EXTERN int
_vsprintf_s_chk(char *restrict dest, rsize_t dmax, const size_t destbos,
                const char *restrict fmt, va_list ap)
    BOS_CHK(dest) BOS_FMT(fmt);
#define vsprintf_s(dest,dmax,fmt,ap) _vsprintf_s_chk(dest,dmax,BOS(dest),fmt,ap)


/* These 2 functions are defined in the C11 standard Annex K, but are still unsafe.
   Rather use the 2 non-truncating (without 'n') functions above. */
#ifdef SAFECLIB_ENABLE_UNSAFE

/* unsafe! use sprintf_s instead */
#ifdef HAVE_C99
EXTERN int
_snprintf_s_chk(char *restrict dest, rsize_t dmax, const size_t destbos,
                const char * restrict fmt, ...)
    BOS_CHK(dest) BOS_FMT(fmt);
#define snprintf_s(dest,dmax,...) _snprintf_s_chk(dest,dmax,BOS(dest),__VA_ARGS__)
#else
EXTERN int
snprintf_s(char *restrict dest, rsize_t dmax,
           const char * restrict fmt, ...)
    BOS_CHK(dest) BOS_FMT(fmt);
#endif

/* unsafe! use vsprintf_s instead */
EXTERN int
_vsnprintf_s_chk(char *restrict dest, rsize_t dmax, const size_t destbos,
                 const char *restrict fmt, va_list ap)
    BOS_CHK(dest) BOS_FMT(fmt);
#if !(defined(_WIN32) && defined(HAVE_VSNPRINTF_S))
/* they use:
int vsnprintf_s(char *_DstBuf, size_t _DstSize, size_t _MaxCount,
                const char *_Format, va_list _ArgList); */
#define vsnprintf_s(dest,dmax,fmt,ap) _vsnprintf_s_chk(dest,dmax,BOS(dest),fmt,ap)
#endif

#endif /* SAFECLIB_ENABLE_UNSAFE */

/* Note: there is no __vsscanf_chk yet. Unchecked */
EXTERN int
sscanf_s(const char *restrict buffer, const char *restrict fmt, ...)
    BOS_NULL(buffer) BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int
fscanf_s(FILE *restrict stream, const char *restrict fmt, ...)
    BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int
scanf_s(const char *restrict fmt, ...)
    BOS_FMT(fmt);

EXTERN int
vscanf_s(const char *restrict fmt, va_list ap)
    BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int
vfscanf_s(FILE *restrict stream, const char *restrict fmt, va_list ap)
    BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int
vsscanf_s(const char *restrict dest, const char *restrict fmt, va_list ap)
    BOS_NULL(dest) BOS_FMT(fmt);

EXTERN int
printf_s(const char *restrict fmt, ...)
    BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int
fprintf_s(FILE *restrict stream, const char *restrict fmt, ...)
    BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int
vprintf_s(const char *restrict fmt, va_list ap)
    BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int
vfprintf_s(FILE *restrict stream, const char *restrict fmt,
           va_list arg)
    BOS_FMT(fmt);
#endif /* __KERNEL__ */


EXTERN errno_t
_strerror_s_chk(char *dest, rsize_t dmax, errno_t errnum, const size_t destbos)
    BOS_CHK(dest);
#define strerror_s(dest,dmax,errnum) _strerror_s_chk(dest,dmax,errnum,BOS(dest))

EXTERN size_t
strerrorlen_s(errno_t errnum);



/* for the other safe IO funcs see safe_lib.h */

#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* string compare */
EXTERN errno_t
_strcmp_s_chk(const char *dest, rsize_t dmax,
              const char *src, int *resultp,
              const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(resultp);
#define strcmp_s(dest,dmax,src,resultp) _strcmp_s_chk(dest,dmax,src,resultp,BOS(dest))

/* string compare */
EXTERN errno_t
_strcasecmp_s_chk(const char *dest, rsize_t dmax,
                  const char *src, int *resultp,
                  const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(resultp);
#define strcasecmp_s(dest,dmax,src,resultp) _strcasecmp_s_chk(dest,dmax,src,resultp,BOS(dest))


/* find a substring - case insensitive */
EXTERN errno_t
strcasestr_s(char *dest, rsize_t dmax,
             const char *src, rsize_t slen, char **substring)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(substring);

/* fixed field string compare */
EXTERN errno_t
strcmpfld_s(const char *dest, rsize_t dmax,
            const char *src, int *indicator)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(indicator);

/* fixed char array copy */
EXTERN errno_t
_strcpyfld_s_chk(char *dest, const rsize_t dmax, const char *src, rsize_t slen,
                 const size_t destbos)
    BOS_CHK_BUTZERO(dest,slen) BOS_OVR2_BUTZERO(src,slen)
    VAL_OVR2(slen, dmax);
#define strcpyfld_s(dest,dmax,src,slen) _strcpyfld_s_chk(dest,dmax,src,slen,BOS(dest))

/* copy from a null terminated string to fixed char array */
EXTERN errno_t
strcpyfldin_s(char *dest, rsize_t dmax, const char *src, rsize_t slen)
    BOS_CHK_BUTZERO(dest,slen) BOS_OVR2_BUTZERO(src,slen)
    VAL_OVR2(slen, dmax);

/* copy from a char array to null terminated string */
EXTERN errno_t
strcpyfldout_s(char *dest, rsize_t dmax, const char *src, rsize_t slen)
    BOS_CHK_BUTZERO(dest,slen) BOS_OVR2_BUTZERO(src,slen)
    VAL_OVR2(slen, dmax);


/* computes excluded prefix length */
EXTERN errno_t
strcspn_s(const char *dest, rsize_t dmax,
          const char *src,  rsize_t slen, rsize_t *count)
    BOS_CHK(dest) BOS_OVR2(src, slen) BOS_NULL(count);

/* returns a pointer to the first occurrence of c in dest */
EXTERN errno_t
strfirstchar_s(char *dest, rsize_t dmax, char c, char **first)
    BOS_CHK(dest) BOS_NULL(first);

/* returns index of first difference */
EXTERN errno_t
strfirstdiff_s(const char *dest, rsize_t dmax,
               const char *src, rsize_t *idx)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(idx);

/* validate alphanumeric string */
EXTERN bool
strisalphanumeric_s(const char *str, rsize_t slen)
    BOS_OVR2(str, slen);

/* validate ascii string */
EXTERN bool
strisascii_s(const char *str, rsize_t slen)
    BOS_OVR2(str, slen);

/* validate string of digits */
EXTERN bool
strisdigit_s(const char *str, rsize_t slen)
    BOS_OVR2(str, slen);

/* validate hex string */
EXTERN bool
strishex_s(const char *str, rsize_t slen)
    BOS_OVR2(str, slen);

/* validate lower case */
EXTERN bool
strislowercase_s(const char *str, rsize_t slen)
    BOS_OVR2(str, slen);

/* validate mixed case */
EXTERN bool
strismixedcase_s(const char *str, rsize_t slen)
    BOS_OVR2(str, slen);

/* validate password */
EXTERN bool
strispassword_s(const char *str, rsize_t slen)
    BOS_OVR2(str, slen);

/* validate upper case */
EXTERN bool
strisuppercase_s(const char *str, rsize_t slen)
    BOS_OVR2(str, slen);

/* returns  a pointer to the last occurrence of c in s1 */
EXTERN errno_t
strlastchar_s(char *str, rsize_t smax, char c, char **first)
    BOS_CHK2(str, smax) BOS_NULL(first);

/* returns index of last difference */
EXTERN errno_t
strlastdiff_s(const char *dest, rsize_t dmax,
              const char *src, rsize_t *idx)
    BOS_CHK(dest) BOS_NULL(idx);

/* left justify */
EXTERN errno_t
strljustify_s(char *dest, rsize_t dmax)
    BOS_CHK(dest);

/* string terminate */
EXTERN rsize_t
strnterminate_s (char *s, rsize_t smax)
    BOS_CHK2(s, smax);

/* get pointer to first occurrence from set of char */
EXTERN errno_t
strpbrk_s(char *dest, rsize_t dmax,
          char *src,  rsize_t slen, char **first)
    BOS_CHK(dest) BOS_OVR2(src, slen) BOS_NULL(first);

EXTERN errno_t
strfirstsame_s(const char *dest, rsize_t dmax,
               const char *src,  rsize_t *idx)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(idx);

EXTERN errno_t
strlastsame_s(const char *dest, rsize_t dmax,
              const char *src, rsize_t *idx)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(idx);

/* searches for a prefix */
EXTERN errno_t
strprefix_s(const char *dest, rsize_t dmax, const char *src)
    BOS_CHK(dest) BOS_NULL(src);

/* removes leading and trailing white space */
EXTERN errno_t
strremovews_s(char *dest, rsize_t dmax)
    BOS_CHK(dest);

/* computes inclusive prefix length */
EXTERN errno_t
strspn_s(const char *dest, rsize_t dmax,
         const char *src,  rsize_t slen, rsize_t *count)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(count);

/* find a substring */
EXTERN errno_t
strstr_s(char *dest, rsize_t dmax,
         const char *src, rsize_t slen, char **substring)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(substring);

/* find a character */
EXTERN errno_t
strchr_s(const char *restrict dest, rsize_t dmax,
         const int ch, char **result)
    BOS_CHK(dest) VAL_OVR2(ch, 255) BOS_NULL(result);

EXTERN errno_t
strrchr_s(const char *restrict dest, rsize_t dmax,
         const int ch, char **result)
    BOS_CHK(dest) VAL_OVR2(ch, 255) BOS_NULL(result)
    BOS_ATTR(!*dest, "empty *dest");

/* convert string to lowercase.
   mingw string_s.h: _strlwr_s */
EXTERN errno_t
strtolowercase_s(char *restrict str, rsize_t slen)
    BOS_CHK2(str, slen);

/* convert string to uppercase
   mingw string_s.h: _strupr_s */
EXTERN errno_t
strtouppercase_s(char *str, rsize_t slen)
    BOS_CHK2(str, slen);

#define strlwr_s(str, slen) strtolowercase_s((str), (slen))
#define strupr_s(str, slen) strtouppercase_s((str), (slen))

/* zero an entire string with nulls.
   mingw string_s.h has: _strset_s */
EXTERN errno_t
strzero_s(char *dest, rsize_t dmax)
    BOS_CHK(dest);

EXTERN errno_t
strcoll_s(const char *restrict dest, rsize_t dmax,
          const char *restrict src, int *indicator)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(indicator);

/* Derived from windows extensions sec_api/string_s.h
   defined(MINGW_HAS_SECURE_API) */

EXTERN errno_t
strset_s(char *restrict dest, rsize_t dmax, int value)
    BOS_CHK(dest) VAL_OVR2(value, 255);

EXTERN errno_t
strnset_s(char *restrict dest, rsize_t dmax, int value, rsize_t n)
    BOS_CHK(dest) BOS_OVR2_BUTZERO(dest, n) VAL_OVR2(value, 255)
    VAL_OVR2_BUTZERO(n, dmax);

#endif /* SAFECLIB_DISABLE_EXTENSIONS */


#ifndef SAFECLIB_DISABLE_WCHAR

/* is c99
EXTERN wchar_t*
wcsstr(wchar_t *restrict dest, const wchar_t *restrict src);
*/

/* multibyte wchar */

EXTERN errno_t
mbstowcs_s(size_t *restrict retval,
           wchar_t *restrict dest, rsize_t dmax,
           const char *restrict src, rsize_t len)
    BOS_NULL(retval)
    BOS_ATTR(!_BOS_NULL(dest) && _BOS_ZERO(dest,dmax), "empty dmax")
    BOS_ATTR(!_BOS_NULL(dest) && _BOSW_OVR(dest,dmax), "dest overflow")
    BOS_ATTR(!_BOS_NULL(dest) && (void*)dest == (void*)src, "dest overlap")
    BOS_CHK2(src, len);

EXTERN errno_t
mbsrtowcs_s(size_t *restrict retval,
            wchar_t *restrict dest, rsize_t dmax,
            const char **restrict src, rsize_t len,
            mbstate_t *restrict ps)
    BOS_NULL(retval)
    BOS_ATTR(!_BOS_NULL(dest) && _BOS_ZERO(dest,dmax), "empty dmax")
    BOS_ATTR(!_BOS_NULL(dest) && _BOSW_OVR(dest,dmax), "dest overflow")
    BOS_ATTR(!_BOS_NULL(dest) && (void*)dest == (void*)src, "dest overlap")
    BOS_CHK2(src, len)
    BOS_ATTR(dmax && len > dmax, "len overflow >dmax")
    BOS_NULL(ps);

EXTERN errno_t
wcsrtombs_s(size_t *restrict retval,
            char *restrict dest, rsize_t dmax,
            const wchar_t **restrict src, rsize_t len,
            mbstate_t *restrict ps)
    BOS_NULL(retval) BOS_NULL(ps)
    BOS_ATTR(!_BOS_NULL(dest) && !dmax, "empty dmax")
    BOS_ATTR(!_BOS_NULL(dest) && _BOS_OVR(dest,dmax), "dest overflow")
    BOS_ATTR(!_BOS_NULL(dest) && (void*)dest == (void*)src, "dest overlap")
    BOS_NULL(src)
    BOSW_CHK2(*src, len)
    BOS_ATTR(dmax && len > dmax, "len overflow >dmax");

EXTERN errno_t
wcstombs_s(size_t *restrict retval,
           char *restrict dest, rsize_t dmax,
           const wchar_t *restrict src, rsize_t len)
    BOS_NULL(retval) BOS_CHK(dest) BOSW_CHK2(src, len)
    BOS_ATTR(dmax && len > dmax, "len overflow >dmax");

EXTERN errno_t
wcrtomb_s(size_t *restrict retval, char *restrict dest, rsize_t dmax,
          wchar_t wc, mbstate_t *restrict ps)
    BOS_NULL(retval) BOS_CHK(dest) BOS_NULL(ps)
    VAL_OVR2(wc, 0x10ffff);

EXTERN errno_t
wctomb_s(int *restrict retval, char *restrict dest, rsize_t dmax, wchar_t wc)
    BOS_NULL(retval)
    BOS_ATTR(!_BOS_NULL(dest) && (!dmax || dmax > RSIZE_MAX_STR || _BOS_OVR(dest,dmax)),
             "dest overflow or empty")
    VAL_OVR2(wc, 0x10ffff);

EXTERN size_t
wcsnlen_s(const wchar_t *dest, size_t dmax)
    BOSW_CHK(dest);

EXTERN errno_t
wcscpy_s(wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src)
    BOSW_CHK(dest) BOS_NULL(src);

EXTERN errno_t
wcsncpy_s(wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t slen)
    BOSW_CHK(dest) BOSW_OVR2(src, slen);

EXTERN errno_t
wcscat_s(wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src)
    BOSW_CHK(dest) BOS_NULL(src);

EXTERN errno_t
wcsncat_s(wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t slen)
    BOS_ATTR(slen && (_BOS_NULL(dest) || _BOS_ZERO(dest,dmax) || !dmax), "empty dest or dmax")
    BOS_ATTR(slen && _BOSW_OVR(dest,dmax), "dest overflow")
    BOS_ATTR(!slen && !_BOS_NULL(dest), "empty slen")
    BOS_ATTR(slen && (_BOSW_OVR(src,slen) || _BOS_NULL(src)), "src overflow or empty");

#if !(defined(_WIN32) && defined(HAVE_WCSTOK_S))
/* they use a buggy:
wchar_t* wcstok_s(wchar_t *_Str, const wchar_t *_Delim, wchar_t **_Context); */
EXTERN wchar_t *
wcstok_s(wchar_t *restrict dest, rsize_t *restrict dmax,
         const wchar_t *restrict delim, wchar_t **restrict ptr)
    BOS_ATTR(_BOS_NULL(dmax) || !*dmax, "empty dmax")
    BOSW_OVR2_BUTNULL(dest, *dmax)
    BOS_NULL(delim)
    BOS_NULL(ptr);
#endif

EXTERN int
swprintf_s(wchar_t *restrict dest, rsize_t dmax,
           const wchar_t* restrict fmt, ...)
    BOSW_CHK(dest) BOS_FMT(fmt);

EXTERN int
vswprintf_s(wchar_t *restrict dest, rsize_t dmax,
            const wchar_t *restrict fmt, va_list ap)
    BOSW_CHK(dest) BOS_FMT(fmt);

#ifdef SAFECLIB_ENABLE_UNSAFE

/* unsafe! use vswprintf_s instead */
EXTERN int
snwprintf_s(wchar_t * restrict dest, rsize_t dmax,
            const wchar_t * restrict fmt, ...)
    BOSW_CHK(dest) BOS_FMT(fmt);

/* unsafe! use vswprintf_s instead */
EXTERN int
vsnwprintf_s(wchar_t *restrict dest, rsize_t dmax,
             const wchar_t *restrict fmt, va_list ap)
    BOSW_CHK(dest) BOS_FMT(fmt);

#endif /* SAFECLIB_ENABLE_UNSAFE */

EXTERN int
wprintf_s( const wchar_t *restrict fmt, ...)
     BOS_FMT(fmt);

EXTERN int
vwprintf_s(const wchar_t *restrict fmt, va_list ap)
     BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int
fwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...)
    BOS_NULL(stream) BOS_FMT(fmt);

EXTERN int
vfwprintf_s(FILE * restrict stream,
            const wchar_t *restrict fmt, va_list ap)
    BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */

EXTERN int
swscanf_s(const wchar_t *restrict buffer,
          const wchar_t *restrict fmt, ...)
    BOS_NULL(buffer) BOS_FMT(fmt);

EXTERN int
vswscanf_s(const wchar_t *restrict buffer,
           const wchar_t *restrict fmt, va_list ap)
    BOS_NULL(buffer) BOS_FMT(fmt);

EXTERN int
wscanf_s( const wchar_t *restrict fmt, ...)
    BOS_FMT(fmt);

EXTERN int
vwscanf_s(const wchar_t *restrict fmt, va_list ap)
    BOS_FMT(fmt);

#ifndef __KERNEL__
EXTERN int
fwscanf_s(FILE *restrict stream,
          const wchar_t *restrict fmt, ...)
    BOS_NULL(stream) BOS_FMT(fmt);

EXTERN int
vfwscanf_s(FILE *restrict stream,
           const wchar_t *restrict fmt, va_list ap)
    BOS_NULL(stream) BOS_FMT(fmt);
#endif /* __KERNEL__ */


#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* search wide substring */
EXTERN errno_t
wcsstr_s(wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src, rsize_t slen,
         wchar_t **restrict substring)
    BOSW_CHK(dest) BOSW_OVR2(src, slen) BOS_NULL(substring);

/* compare */
EXTERN errno_t
wcscmp_s(const wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src, rsize_t smax,
         int *diff)
    BOSW_CHK(dest) BOSW_CHK2(src, smax) BOS_NULL(diff);

EXTERN errno_t
wcsncmp_s(const wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t smax,
          rsize_t count, int *diff)
    BOSW_CHK(dest) BOSW_CHK2(src, smax) BOS_NULL(diff);

/* compare case-folded */
EXTERN errno_t
wcsicmp_s(const wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t smax,
          int *diff)
    BOSW_CHK(dest) BOSW_CHK2(src, smax) BOS_NULL(diff);

EXTERN errno_t
wcsset_s(wchar_t *restrict dest, rsize_t dmax, wchar_t value)
    BOSW_CHK(dest) VAL_OVR2(value, 0x10ffff);

EXTERN errno_t
wcsnset_s(wchar_t *restrict dest, rsize_t dmax, wchar_t value, size_t n)
    BOSW_CHK(dest) VAL_OVR2(value, 0x10ffff);

EXTERN errno_t
wcscoll_s(const wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src,  rsize_t smax, int *indicator)
    BOSW_CHK(dest) BOSW_CHK2(src, smax) BOS_NULL(indicator);

/* simple char-wise folding */
EXTERN errno_t
wcslwr_s(wchar_t *restrict src, rsize_t slen)
    BOSW_OVR2_BUTZERO(src, slen);

EXTERN errno_t
wcsupr_s(wchar_t *restrict src, rsize_t slen)
    BOSW_OVR2_BUTZERO(src, slen);

/* is a wide upper character which folds to multiple lowercase chars? how many */
EXTERN int
iswfc(uint32_t wc)
    VAL_OVR2(wc, 0x10ffff);

/* full foldcase a single upper char to mult. lower chars */
EXTERN int
towfc_s(wchar_t *restrict dest, rsize_t dmax, const uint32_t src)
    BOSW_CHK(dest) BOS_ATTR(dmax < 4, "dmax underflow <4");

/* full foldcase + NFD normalization */
EXTERN errno_t
wcsfc_s(wchar_t *restrict dest, rsize_t dmax,
        wchar_t *restrict src, rsize_t *restrict lenp)
    BOSW_CHK(dest) BOS_NULL(src);

/* Normalize to FCD/pre-NFKD */
EXTERN errno_t
wcsnorm_decompose_s(wchar_t *restrict dest, rsize_t dmax,
                    wchar_t *restrict src, rsize_t *restrict lenp,
                    bool iscompat)
    BOSW_CHK(dest) BOS_NULL(src);

/* Normalize to NCD/NFKD */
EXTERN errno_t
wcsnorm_reorder_s(wchar_t *restrict dest, rsize_t dmax,
                  wchar_t *restrict src, rsize_t len)
    BOSW_CHK(dest)  BOSW_OVR2(src, len);

/* Normalize to NFC/NFKC */
EXTERN errno_t
wcsnorm_compose_s(wchar_t *restrict dest, rsize_t dmax,
                  wchar_t *restrict src, rsize_t *restrict lenp,
                  bool iscontig)
    BOSW_CHK(dest) BOS_NULL(src) BOS_NULL(lenp);

enum wcsnorm_mode {
    WCSNORM_NFD  = 0,
    WCSNORM_NFC  = 1, /* default */
    WCSNORM_FCD  = 2, /* not reordered */
    WCSNORM_FCC  = 3, /* contiguous composition only */
    WCSNORM_NFKD = 4, /* compat. OPTIONAL with --enable-norm-compat */
    WCSNORM_NFKC = 5  /* compat. OPTIONAL with --enable-norm-compat */
};
typedef enum wcsnorm_mode wcsnorm_mode_t;

/* Normalize to NFC (default), NFD nfc=0.
   experim. nfc>1: FCD, FCC */
EXTERN errno_t
wcsnorm_s(wchar_t *restrict dest, rsize_t dmax, wchar_t *restrict src,
          wcsnorm_mode_t mode, rsize_t *restrict lenp)
    BOSW_CHK(dest) BOS_NULL(src);

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

#endif /* SAFECLIB_DISABLE_WCHAR */

#ifdef __cplusplus
}
#endif

#endif   /* __SAFE_STR_LIB_H__ */
