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

#ifndef __SAFE_LIB_H__
#include "safe_lib.h"
#endif
#include <stdarg.h>
#include <wchar.h>

/*
 * The shortest string is a null string!!
 */
#define RSIZE_MIN_STR      ( 1 )

/* wide chars */
#define RSIZE_MAX_STR16    ( RSIZE_MAX_STR/2 )

/* The makeup of a password */
#define SAFE_STR_MIN_LOWERCASE     ( 2 )
#define SAFE_STR_MIN_UPPERCASE     ( 2 )
#define SAFE_STR_MIN_NUMBERS       ( 1 )
#define SAFE_STR_MIN_SPECIALS      ( 1 )

#define SAFE_STR_PASSWORD_MIN_LENGTH   ( 6 )
#define SAFE_STR_PASSWORD_MAX_LENGTH   ( 32 )


/* set string constraint handler */
extern constraint_handler_t
set_str_constraint_handler_s(constraint_handler_t handler);


/* string concatenate */
extern errno_t
strcat_s(char * restrict dest, rsize_t dmax, const char * restrict src);


/* string copy */
extern errno_t
strcpy_s(char * restrict dest, rsize_t dmax, const char * restrict src);

/* fitted string concatenate */
extern errno_t
strncat_s(char * restrict dest, rsize_t dmax, const char * restrict src, rsize_t slen);


/* fitted string copy */
extern errno_t
strncpy_s(char * restrict dest, rsize_t dmax, const char * restrict src, rsize_t slen);


/* string length */
extern rsize_t
strnlen_s (const char *s, rsize_t smax);

/* string tokenizer */
extern char *
strtok_s(char * restrict s1, rsize_t *restrict s1max, const char * restrict src, char ** restrict ptr);


/* safe sprintf_s */
/* now __STDC_WANT_LIB_EXT1__ >= 1 compatible */
extern int
sprintf_s(char *restrict dest, rsize_t dmax, const char * restrict fmt, ...);

extern int
vsprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, va_list ap);

/* These functions are defined in the C11 standard Annex K, but are still unsafe.
   Rather use the 2 functions above. */
#ifdef SAFECLIB_ENABLE_UNSAFE

/* unsafe! */
extern int
snprintf_s(char *restrict dest, rsize_t dmax, const char * restrict fmt, ...);
extern int
vsnprintf_s(char *restrict dest, rsize_t dmax, const char *restrict fmt, va_list ap);

#endif /* SAFECLIB_ENABLE_UNSAFE */


#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* string compare */
extern errno_t
strcmp_s(const char *dest, rsize_t dmax,
         const char *src, int *indicator);

/* string compare */
extern errno_t
strcasecmp_s(const char *dest, rsize_t dmax,
             const char *src, int *indicator);


/* find a substring _ case insensitive */
extern errno_t
strcasestr_s(char *dest, rsize_t dmax,
             const char *src, rsize_t slen, char **substring);


/* fixed field string compare */
extern errno_t
strcmpfld_s(const char *dest, rsize_t dmax,
            const char *src, int *indicator);

/* fixed char array copy */
extern errno_t
strcpyfld_s(char *dest, rsize_t dmax, const char *src, rsize_t slen);


/* copy from a null terminated string to fixed char array */
extern errno_t
strcpyfldin_s(char *dest, rsize_t dmax, const char *src, rsize_t slen);


/* copy from a char array to null terminated string */
extern errno_t
strcpyfldout_s(char *dest, rsize_t dmax, const char *src, rsize_t slen);


/* computes excluded prefix length */
extern errno_t
strcspn_s(const char *dest, rsize_t dmax,
          const char *src,  rsize_t slen, rsize_t *count);


/* returns a pointer to the first occurrence of c in dest */
extern errno_t
strfirstchar_s(char *dest, rsize_t dmax, char c, char **first);


/* returns index of first difference */
extern  errno_t
strfirstdiff_s(const char *dest, rsize_t dmax,
               const char *src, rsize_t *index);


/* validate alphanumeric string */
extern bool
strisalphanumeric_s(const char *str, rsize_t slen);


/* validate ascii string */
extern bool
strisascii_s(const char *str, rsize_t slen);


/* validate string of digits */
extern bool
strisdigit_s(const char *str, rsize_t slen);


/* validate hex string */
extern bool
strishex_s(const char *str, rsize_t slen);


/* validate lower case */
extern bool
strislowercase_s(const char *str, rsize_t slen);


/* validate mixed case */
extern bool
strismixedcase_s(const char *str, rsize_t slen);


/* validate password */
extern bool
strispassword_s(const char *str, rsize_t slen);


/* validate upper case */
extern bool
strisuppercase_s(const char *str, rsize_t slen);


/* returns  a pointer to the last occurrence of c in s1 */
extern errno_t
strlastchar_s(char *str, rsize_t smax, char c, char **first);


/* returns index of last difference */
extern  errno_t
strlastdiff_s(const char *dest, rsize_t dmax,
              const char *src, rsize_t *index);


/* left justify */
extern errno_t
strljustify_s(char *dest, rsize_t dmax);



/* string terminate */
extern rsize_t
strnterminate_s (char *s, rsize_t smax);


/* get pointer to first occurrence from set of char */
extern errno_t
strpbrk_s(char *dest, rsize_t dmax,
          char *src,  rsize_t slen, char **first);


extern errno_t
strfirstsame_s(const char *dest, rsize_t dmax,
               const char *src,  rsize_t *index);

extern errno_t
strlastsame_s(const char *dest, rsize_t dmax,
              const char *src, rsize_t *index);


/* searches for a prefix */
extern errno_t
strprefix_s(const char *dest, rsize_t dmax, const char *src);


/* removes leading and trailing white space */
extern errno_t
strremovews_s(char *dest, rsize_t dmax);


/* computes inclusive prefix length */
extern errno_t
strspn_s(const char *dest, rsize_t dmax,
         const char *src,  rsize_t slen, rsize_t *count);


/* find a substring */
extern errno_t
strstr_s(char *dest, rsize_t dmax,
         const char *src, rsize_t slen, char **substring);


/* convert string to lowercase */
extern errno_t
strtolowercase_s(char * restrict str, rsize_t slen);


/* convert string to uppercase */
extern errno_t
strtouppercase_s(char *str, rsize_t slen);


/* zero an entire string with nulls */
extern errno_t
strzero_s(char *dest, rsize_t dmax);

#endif /* SAFECLIB_DISABLE_EXTENSIONS */


#ifndef SAFECLIB_DISABLE_WCHAR
/* multibyte wchar */

extern errno_t
mbstowcs_s(size_t *restrict retval,
           wchar_t *restrict dest, rsize_t dmax,
           const char *restrict src, rsize_t len);

extern errno_t
mbsrtowcs_s(size_t *restrict retval,
            wchar_t *restrict dest, rsize_t dmax,
            const char **restrict src, rsize_t len,
            mbstate_t *restrict ps);

extern errno_t
wcsrtombs_s(size_t *restrict retval,
            char *restrict dest, rsize_t dmax,
            const wchar_t **restrict src, rsize_t len,
            mbstate_t *restrict ps);

extern errno_t
wcstombs_s(size_t *restrict retval,
           char *restrict dest, rsize_t dmax,
           const wchar_t *restrict src, rsize_t len);

extern errno_t
wcrtomb_s(size_t *restrict retval, char *restrict dest, rsize_t dmax,
          wchar_t wc, mbstate_t *restrict ps);

extern errno_t
wctomb_s(int *restrict retval, char *restrict dest, rsize_t dmax,
         wchar_t wc);

extern size_t
wcsnlen_s(const wchar_t *dest, size_t dmax);

extern errno_t
wcscpy_s(wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src);

extern errno_t
wcsncpy_s(wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t slen);

extern errno_t
wcscat_s(wchar_t *restrict dest, rsize_t dmax,
         const wchar_t *restrict src);

extern errno_t
wcsncat_s(wchar_t *restrict dest, rsize_t dmax,
          const wchar_t *restrict src, rsize_t slen);

extern wchar_t *
wcstok_s(wchar_t *restrict dest, rsize_t *restrict dmax,
         const wchar_t *restrict delim, wchar_t **restrict ptr);

/* TODO */
#if 0

extern int
wprintf_s( const wchar_t *restrict fmt, ...);

extern int
fwprintf_s(FILE *restrict stream, const wchar_t *restrict fmt, ...);

/* unsafe! */
extern int
snwprintf_s(wchar_t * restrict dest, rsize_t dmax,
            const wchar_t * restrict fmt, ...);

extern int
swprintf_s(wchar_t *restrict dest, rsize_t dmax,
           const wchar_t* restrict fmt, ...);

extern int
vwprintf_s(const wchar_t *restrict fmt, va_list ap);

extern int
vfwprintf_s(FILE * restrict stream,
            const wchar_t *restrict fmt, va_list ap);

extern int
vswprintf_s(wchar_t *restrict dest, rsize_t dmax,
            const wchar_t * restrict fmt, va_list ap);

/* unsafe! */
extern int
vsnwprintf_s(wchar_t *restrict dest, rsize_t dmax,
             const wchar_t *restrict fmt, va_list ap);

extern int
wscanf_s( const wchar_t *restrict fmt, ...);  

extern int
fwscanf_s(FILE *restrict stream,
          const wchar_t *restrict fmt, ...);  

extern int
swscanf_s(const wchar_t *restrict s,
          const wchar_t *restrict fmt, ...);

extern int
vwscanf_s(const wchar_t *restrict fmt, va_list ap);

extern int
vfwscanf_s(FILE *restrict stream,
           const wchar_t *restrict fmt, va_list ap);

extern int
vswscanf_s(const wchar_t *restrict buffer,
           const wchar_t *restrict fmt, va_list ap);  

#endif

#endif /* SAFECLIB_DISABLE_WCHAR */
  
#ifdef __cplusplus
}
#endif

#endif   /* __SAFE_STR_LIB_H__ */
