/*------------------------------------------------------------------
 * safe_str_lib.h -- Safe C Library String APIs
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
 *
 * Copyright (c) 2008-2011, 2013 by Cisco Systems, Inc.
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

#ifndef __SAFE_STR_LIB_H__
#define __SAFE_STR_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "safe_config.h"
#include "safe_lib_errno.h"
#include "safe_types.h"

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
strcat_s(char * restrict dest, rsize_t dmax, const char * restrict src)
    BOS_CHK(dest);

/* string copy */
EXTERN errno_t
strcpy_s(char * restrict dest, rsize_t dmax, const char * restrict src)
    BOS_CHK(dest);

/* fitted string concatenate */
EXTERN errno_t
strncat_s(char * restrict dest, rsize_t dmax, const char * restrict src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* fitted string copy */
EXTERN errno_t
strncpy_s(char * restrict dest, rsize_t dmax,
          const char * restrict src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);


/* string length */
EXTERN rsize_t
strnlen_s (const char*s, rsize_t smax)
    BOS_CHK2(s, smax);

/* string tokenizer */
#if !(defined(_WIN32) && defined(HAVE_STRTOK_S))
/* they use:
char * strtok_s(char *_Str,const char *_Delim,char **_Context); */
EXTERN char *
strtok_s(char *restrict s1, rsize_t *restrict s1max,
         const char *restrict src, char **restrict ptr)
    BOS_CHK2(s1, s1max);
#endif

/* secure stdio */

/* safe sprintf_s */
/* now __STDC_WANT_LIB_EXT1__ >= 1 compatible */
EXTERN int
sprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, ...)
    BOS_CHK(dest);

EXTERN int
vsprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, va_list ap)
    BOS_CHK(dest);

/* These 2 functions are defined in the C11 standard Annex K, but are still unsafe.
   Rather use the 2 non-truncating (without 'n') functions above. */
#ifdef SAFECLIB_ENABLE_UNSAFE

/* unsafe! use sprintf_s instead */
EXTERN int
snprintf_s(char *restrict dest, rsize_t dmax, const char * restrict fmt, ...)
    BOS_CHK(dest);

/* unsafe! use vsprintf_s instead */
#if !(defined(_WIN32) && defined(HAVE_VSNPRINTF_S))
/* they use:
int vsnprintf_s(char *_DstBuf, size_t _DstSize, size_t _MaxCount,
                const char *_Format, va_list _ArgList); */
EXTERN int
vsnprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, va_list ap)
    BOS_CHK(dest);
#endif

#endif /* SAFECLIB_ENABLE_UNSAFE */

EXTERN int
sscanf_s(const char *restrict buffer, const char *restrict fmt, ...);

#ifndef __KERNEL__
EXTERN int
fscanf_s(FILE *restrict stream, const char *restrict format, ...);
#endif /* __KERNEL__ */

EXTERN int
scanf_s(const char *restrict format, ...);

EXTERN int
vscanf_s(const char *restrict format, va_list vlist);

#ifndef __KERNEL__
EXTERN int
vfscanf_s(FILE *restrict stream, const char *restrict format,
          va_list vlist);
#endif /* __KERNEL__ */

EXTERN int
vsscanf_s(const char *restrict buffer, const char *restrict format,
          va_list vlist);

EXTERN int
printf_s(const char *restrict format, ...);

#ifndef __KERNEL__
EXTERN int
fprintf_s(FILE *restrict stream, const char *restrict format, ...);
#endif /* __KERNEL__ */

EXTERN int
vprintf_s(const char *restrict format, va_list arg);

#ifndef __KERNEL__
EXTERN int
vfprintf_s(FILE *restrict stream, const char *restrict format,
           va_list arg);
#endif /* __KERNEL__ */

EXTERN errno_t
strerror_s(char *dest, rsize_t dmax, errno_t errnum)
    BOS_CHK(dest);

EXTERN size_t
strerrorlen_s(errno_t errnum);



/* for the other safe IO funcs see safe_lib.h */

#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* string compare */
EXTERN errno_t
strcmp_s(const char *dest, rsize_t dmax,
         const char *src, int *indicator)
    BOS_CHK(dest);

/* string compare */
EXTERN errno_t
strcasecmp_s(const char *dest, rsize_t dmax,
             const char *src, int *indicator)
    BOS_CHK(dest);


/* find a substring _ case insensitive */
EXTERN errno_t
strcasestr_s(char *dest, rsize_t dmax,
             const char *src, rsize_t slen, char **substring)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* fixed field string compare */
EXTERN errno_t
strcmpfld_s(const char *dest, rsize_t dmax,
            const char *src, int *indicator) BOS_CHK(dest);

/* fixed char array copy */
EXTERN errno_t
strcpyfld_s(char *dest, rsize_t dmax, const char *src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* copy from a null terminated string to fixed char array */
EXTERN errno_t
strcpyfldin_s(char *dest, rsize_t dmax, const char *src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* copy from a char array to null terminated string */
EXTERN errno_t
strcpyfldout_s(char *dest, rsize_t dmax, const char *src, rsize_t slen)
    BOS_CHK(dest) BOS_OVR2(src, slen);


/* computes excluded prefix length */
EXTERN errno_t
strcspn_s(const char *dest, rsize_t dmax,
          const char *src,  rsize_t slen, rsize_t *count)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* returns a pointer to the first occurrence of c in dest */
EXTERN errno_t
strfirstchar_s(char *dest, rsize_t dmax, char c, char **first)
    BOS_CHK(dest);

/* returns index of first difference */
EXTERN errno_t
strfirstdiff_s(const char *dest, rsize_t dmax,
               const char *src, rsize_t *idx)
    BOS_CHK(dest);

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
    BOS_CHK2(str, smax);

/* returns index of last difference */
EXTERN errno_t
strlastdiff_s(const char *dest, rsize_t dmax,
              const char *src, rsize_t *idx)
    BOS_CHK(dest);

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
    BOS_CHK(dest) BOS_OVR2(src, slen);

EXTERN errno_t
strfirstsame_s(const char *dest, rsize_t dmax,
               const char *src,  rsize_t *idx)
    BOS_CHK(dest);

EXTERN errno_t
strlastsame_s(const char *dest, rsize_t dmax,
              const char *src, rsize_t *idx)
    BOS_CHK(dest);

/* searches for a prefix */
EXTERN errno_t
strprefix_s(const char *dest, rsize_t dmax, const char *src)
    BOS_CHK(dest);

/* removes leading and trailing white space */
EXTERN errno_t
strremovews_s(char *dest, rsize_t dmax)
    BOS_CHK(dest);

/* computes inclusive prefix length */
EXTERN errno_t
strspn_s(const char *dest, rsize_t dmax,
         const char *src,  rsize_t slen, rsize_t *count)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* find a substring */
EXTERN errno_t
strstr_s(char *dest, rsize_t dmax,
         const char *src, rsize_t slen, char **substring)
    BOS_CHK(dest) BOS_OVR2(src, slen);

/* find a character */
EXTERN errno_t
strchr_s(const char *restrict dest, rsize_t dmax,
         const int ch, char **result)
    BOS_CHK(dest);

EXTERN errno_t
strrchr_s(const char *restrict dest, rsize_t dmax,
         const int ch, char **result)
    BOS_CHK(dest);

/* convert string to lowercase.
   mingw string_s.h: _strlwr_s */
EXTERN errno_t
strtolowercase_s(char *restrict str, rsize_t slen)
    BOS_OVR2(str, slen);

/* convert string to uppercase
   mingw string_s.h: _strupr_s */
EXTERN errno_t
strtouppercase_s(char *str, rsize_t slen)
    BOS_OVR2(str, slen);

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
    BOS_CHK(dest);

/* Derived from windows extensions sec_api/string_s.h
   defined(MINGW_HAS_SECURE_API) */

EXTERN errno_t
strset_s(char *restrict dest, rsize_t dmax, int value)
    BOS_CHK(dest) VAL_OVR2(value, 255);

EXTERN errno_t
strnset_s(char *restrict dest, rsize_t dmax, int value, rsize_t n)
    BOS_CHK(dest) VAL_OVR2(value, 255);

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
    BOSW_CHK(dest) BOS_OVR2(src, len);

EXTERN errno_t
mbsrtowcs_s(size_t *restrict retval,
            wchar_t *restrict dest, rsize_t dmax,
            const char **restrict src, rsize_t len,
            mbstate_t *restrict ps)
    BOSW_CHK(dest) BOS_OVR2(src, len);

EXTERN errno_t
wcsrtombs_s(size_t *restrict retval,
            char *restrict dest, rsize_t dmax,
            const wchar_t **restrict src, rsize_t len,
            mbstate_t *restrict ps)
    BOS_CHK(dest) BOS_OVR2(src, len);

EXTERN errno_t
wcstombs_s(size_t *restrict retval,
           char *restrict dest, rsize_t dmax,
           const wchar_t *restrict src, rsize_t len)
    BOS_CHK(dest) BOSW_OVR2(src, len);

EXTERN errno_t
wcrtomb_s(size_t *restrict retval, char *restrict dest, rsize_t dmax,
          wchar_t wc, mbstate_t *restrict ps)
    BOS_CHK(dest);

EXTERN errno_t
wctomb_s(int *restrict retval, char *restrict dest, rsize_t dmax, wchar_t wc)
    BOS_CHK(dest);

EXTERN size_t
wcsnlen_s(const wchar_t *dest, size_t dmax)
    BOSW_CHK(dest);

EXTERN errno_t
wcscpy_s(wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src)
    BOSW_CHK(dest);

EXTERN errno_t
wcsncpy_s(wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t slen)
    BOSW_CHK(dest) BOSW_OVR2(src, slen);

EXTERN errno_t
wcscat_s(wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src)
    BOSW_CHK(dest);

EXTERN errno_t
wcsncat_s(wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t slen)
    BOSW_CHK(dest) BOSW_OVR2(src, slen);

#if !(defined(_WIN32) && defined(HAVE_WCSTOK_S))
/* they use a buggy:
wchar_t* wcstok_s(wchar_t *_Str, const wchar_t *_Delim, wchar_t **_Context); */
EXTERN wchar_t *
wcstok_s(wchar_t *restrict dest, rsize_t *restrict dmax,
         const wchar_t *restrict delim, wchar_t **restrict ptr)
    BOSW_CHK(dest);
#endif

EXTERN int
swprintf_s(wchar_t *restrict dest, rsize_t dmax,
           const wchar_t* restrict fmt, ...)
    BOSW_CHK(dest);

EXTERN int
vswprintf_s(wchar_t *restrict dest, rsize_t dmax,
            const wchar_t *restrict fmt, va_list ap)
    BOSW_CHK(dest);

#ifdef SAFECLIB_ENABLE_UNSAFE

/* unsafe! use vswprintf_s instead */
EXTERN int
snwprintf_s(wchar_t * restrict dest, rsize_t dmax,
            const wchar_t * restrict fmt, ...)
    BOSW_CHK(dest);

/* unsafe! use vswprintf_s instead */
EXTERN int
vsnwprintf_s(wchar_t *restrict dest, rsize_t dmax,
             const wchar_t *restrict fmt, va_list ap)
    BOSW_CHK(dest);

#endif /* SAFECLIB_ENABLE_UNSAFE */

EXTERN int
wprintf_s( const wchar_t *restrict fmt, ...);

EXTERN int
vwprintf_s(const wchar_t *restrict fmt, va_list ap);

#ifndef __KERNEL__
EXTERN int
fwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...);

EXTERN int
vfwprintf_s(FILE * restrict stream,
            const wchar_t *restrict fmt, va_list ap);
#endif /* __KERNEL__ */

EXTERN int
swscanf_s(const wchar_t *restrict buffer,
          const wchar_t *restrict fmt, ...);

EXTERN int
vswscanf_s(const wchar_t *restrict buffer,
           const wchar_t *restrict fmt, va_list ap);

EXTERN int
wscanf_s( const wchar_t *restrict fmt, ...);

EXTERN int
vwscanf_s(const wchar_t *restrict fmt, va_list ap);

#ifndef __KERNEL__
EXTERN int
fwscanf_s(FILE *restrict stream,
          const wchar_t *restrict fmt, ...);

EXTERN int
vfwscanf_s(FILE *restrict stream,
           const wchar_t *restrict fmt, va_list ap);
#endif /* __KERNEL__ */


#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* search wide substring */
EXTERN errno_t
wcsstr_s(wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src, rsize_t slen,
         wchar_t **restrict substring) BOSW_CHK(dest);

/* compare */
EXTERN errno_t
wcscmp_s(const wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src, rsize_t smax,
         int *diff)
    BOSW_CHK(dest) BOSW_CHK2(src, smax);

EXTERN errno_t
wcsncmp_s(const wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t smax,
          rsize_t count, int *diff)
    BOSW_CHK(dest) BOSW_CHK2(src, smax);

/* compare case-folded */
EXTERN errno_t
wcsicmp_s(const wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t smax,
          int *diff)
    BOSW_CHK(dest) BOSW_CHK2(src, smax);

EXTERN errno_t
wcsset_s(wchar_t *restrict dest, rsize_t dmax, wchar_t value)
    BOSW_CHK(dest) VAL_OVR2(value, 0x10ffff);

EXTERN errno_t
wcsnset_s(wchar_t *restrict dest, rsize_t dmax, wchar_t value, size_t n)
    BOSW_CHK(dest) VAL_OVR2(value, 0x10ffff);

EXTERN errno_t
wcscoll_s(const wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src,  rsize_t smax, int *indicator)
    BOSW_CHK(dest) BOSW_CHK2(src, smax);

/* simple char-wise folding */
EXTERN errno_t
wcslwr_s(wchar_t *restrict src, rsize_t slen)
    BOSW_OVR2(src, slen);

EXTERN errno_t
wcsupr_s(wchar_t *restrict src, rsize_t slen)
    BOSW_OVR2(src, slen);

/* is a wide upper character which folds to multiple lowercase chars? how many */
EXTERN int
iswfc(uint32_t wc)
    VAL_OVR2(wc, 0x10ffff);

/* full foldcase a single upper char to mult. lower chars */
EXTERN int
towfc_s(wchar_t *restrict dest, rsize_t dmax, const uint32_t src)
    BOSW_CHK(dest);

/* full foldcase + NFD normalization */
EXTERN errno_t
wcsfc_s(wchar_t *restrict dest, rsize_t dmax,
        wchar_t *restrict src, rsize_t *restrict lenp)
    BOSW_CHK(dest);

/* Normalize to FCD/pre-NFKD */
EXTERN errno_t
wcsnorm_decompose_s(wchar_t *restrict dest, rsize_t dmax,
                    wchar_t *restrict src, rsize_t *restrict lenp,
                    bool iscompat)
    BOSW_CHK(dest);
/* Normalize to NCD/NFKD */
EXTERN errno_t
wcsnorm_reorder_s(wchar_t *restrict dest, rsize_t dmax,
                  wchar_t *restrict src, rsize_t len)
    BOSW_CHK(dest);
/* Normalize to NFC/NFKC */
EXTERN errno_t
wcsnorm_compose_s(wchar_t *restrict dest, rsize_t dmax,
                  wchar_t *restrict src, rsize_t *restrict lenp,
                  bool iscontig)
    BOSW_CHK(dest);

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
    BOSW_CHK(dest);

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

#endif /* SAFECLIB_DISABLE_WCHAR */

#ifdef __cplusplus
}
#endif

#endif   /* __SAFE_STR_LIB_H__ */
