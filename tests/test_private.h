/*------------------------------------------------------------------
 * test_private.h - Internal test routines & library references
 *
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 * 2017  Reini Urban
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

#ifndef __TEST_PRIVATE_H__
#define __TEST_PRIVATE_H__

#include "config.h"
#include "safe_config.h"

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ctype.h>

#include <linux/string.h>
#ifdef HAVE_C99
#define printf(...) printk(KERN_INFO __VA_ARGS__)
#endif

#else

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

/* --disable-shared: ignore the sec_api */
#if defined(_WIN32) && defined(DISABLE_DLLIMPORT)
/* ignore the sec_api imports. with DISABLE_DLLIMPORT we want our own */
#  undef _SECIMP
#  define _SECIMP
#  undef MINGW_HAS_SECURE_API
#endif

/* for glibc use the GNU extensions: strcasestr */
#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#endif

#ifdef _WIN32
# define __STRICT_ANSI__
# include <io.h>
# define pipe(p) _pipe(p,2,0)
#endif

#if defined __MINGW64_VERSION_MAJOR
#  define HAVE_MINGW64  /* mingw-w64 (either 32 or 64bit) */
#  ifdef DISABLE_DLLIMPORT /* --disable-shared: ignore the sec_api */
#    undef MINGW_HAS_SECURE_API
#  endif
#elif defined __MINGW32__
#  define HAVE_MINGW32  /* old mingw */
#endif
#if defined(__CYGWIN__) && defined(__x86_64)
# define HAVE_CYGWIN64
#endif
/* libc variants:
   musl not detectable. and there's __GLIBC__ and _WIN32.
   TODO uClibc? dietlibc? minilibc? exots? */
#if defined(__FreeBSD__) || \
    defined(__NetBSD__)  || \
    defined(__OpenBSD__) || \
    defined(__bsdi__)    || \
    defined(__DragonFly__)
# define BSD_LIKE
#endif
#if defined(__APPLE__) || defined(BSD_LIKE)
# define BSD_OR_DARWIN_LIKE
#endif
#if defined(__NEWLIB__) || defined(BSD_LIKE)
# define BSD_OR_NEWLIB_LIKE
#endif
#if defined(__MINGW32__) || defined( BSD_OR_NEWLIB_LIKE)
# define BSD_OR_WINDOWS_LIKE
#endif
#if defined(__APPLE__) || defined(__NEWLIB__) || defined(BSD_LIKE)
# define BSD_ALL_LIKE
#endif

#if defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1)
# define WANT_C11
#endif

#if defined(__has_feature)
# if __has_feature(address_sanitizer)
#  define HAVE_ASAN 1
# endif
#endif
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#if defined(__clang__) || defined(__clang) || \
       (defined( __GNUC__) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406)
#  define GCC_DIAG_PRAGMA(x) _Pragma (#x)
/* clang has "clang diagnostic" pragmas, but also understands gcc. */
#  define GCC_DIAG_IGNORE(x) _Pragma("GCC diagnostic push") \
                             GCC_DIAG_PRAGMA(GCC diagnostic ignored #x)
#  define GCC_DIAG_RESTORE   _Pragma("GCC diagnostic pop")
#else
#  define GCC_DIAG_IGNORE(w)
#  define GCC_DIAG_RESTORE
#endif

/* so far clang-7 only */
#if __has_attribute(diagnose_if) && defined(HAVE___BUILTIN_OBJECT_SIZE)
# define HAVE_CT_BOS_OVR
# ifdef HAVE_USER_DEFINED_WARNINGS
#  define GCC_PUSH_WARN_DMAX GCC_DIAG_IGNORE(-Wuser-defined-warnings)
#  define GCC_POP_WARN_DMAX  GCC_DIAG_RESTORE
# else
#  define GCC_PUSH_WARN_DMAX
#  define GCC_POP_WARN_DMAX
# endif
#elif defined(HAVE_WARN_DMAX)
# define HAVE_RT_BOS_CHK
# define GCC_PUSH_WARN_DMAX
# define GCC_POP_WARN_DMAX
#else
# define GCC_PUSH_WARN_DMAX
# define GCC_POP_WARN_DMAX
#endif

/* mingw 3.4 */
#ifndef EOVERFLOW
#define EOVERFLOW 139
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

/* ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
   rc = strtolowercase_s("test", len); */
#ifdef __cplusplus
# ifdef __clang
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wwrite-strings"
# elif defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wwrite-strings"
# endif
#endif

#ifdef DEBUG
#  if defined(HAVE_C99) && defined(__KERNEL__)
#    define debug_printf(...)  printk(KERN_DEBUG __VA_ARGS__)
#  else
#    define debug_printf printf
#  endif
#else
# ifdef HAVE_C99
#   define debug_printf(...)
# else
#   define debug_printf printf
# endif
#endif

#ifdef XDEBUG
#  if defined(HAVE_C99)
#    define xdebug_printf(...)  printf(__VA_ARGS__)
#  else
#    define xdebug_printf printf
#  endif
#else
# ifdef HAVE_C99
#   define xdebug_printf(...)
# else
#   define xdebug_printf printf
# endif
#endif

#define ERR(n)                                     \
    if (rc != (n)) {                               \
        debug_printf("%s %u  Error rc=%d \n",      \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
/* msvcrt wprintf prints one byte less. cygwin not */
#ifdef __MINGW32__
#define ERRWCHAR(n)                                \
    if (rc != (n)-1) {                             \
        debug_printf("%s %u  Error rc=%d \n",      \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#else
#define ERRWCHAR(n)                                \
    if (rc != (n)) {                               \
        debug_printf("%s %u  Error rc=%d \n",      \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#endif
#define ERRPTR(n)                                 \
    if (rc != (n)) {                              \
        debug_printf("%s %u  Error rc=%p \n",     \
                     __FUNCTION__, __LINE__,  rc);\
        errs++;                                   \
    }
#define NEGERR(n)                                 \
    if (rc != -(n)) {                             \
        debug_printf("%s %u  Error rc=%d \n",     \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                   \
    }
#define ANYERR()                                    \
    if (rc >= 0) {                                  \
        debug_printf("%s %u  Error rc=%d \n",       \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                     \
    }
#define ERRNO(n)                                   \
    if (errno != (n)) {                            \
        debug_printf("%s %u  Error errno=%d \n",  \
                     __FUNCTION__, __LINE__,  (int)errno); \
        errs++;                                    \
    }
#define ERREOF(n)                                  \
    if (errno != (n)) {                            \
        debug_printf("%s %u  Error errno=%d \n",  \
                     __FUNCTION__, __LINE__,  (int)errno); \
        errs++;                                    \
    }                                              \
    ERR(EOF);
#define NOERR()                                    \
    if (rc < 0) {                                  \
        debug_printf("%s %u  Error rc=%d \n",     \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#define NOERRNULL()                                \
    if (rc <= 0) {                                 \
        debug_printf("%s %u  Error rc=%d \n",     \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#define EXPNULL(s1)                                \
    if ((s1)[0] != '\0') {                         \
        debug_printf("%s %u  Expected null, got \"%s\" \n", \
                     __FUNCTION__, __LINE__, s1);  \
        errs++;                                    \
    }
#define EXPSTR(s1, s2)                             \
    ind = strcmp((s1), (s2));                      \
    if (ind != 0) {                                \
        debug_printf("%s %u  Expected \"%s\", got \"%s\" \n", \
                     __FUNCTION__, __LINE__,  (s2), s1);      \
        errs++;                                    \
    }
#define WEXPNULL(s1)                               \
    if ((s1)[0] != L'\0') {                        \
        debug_printf("%s %u  Expected null, got ", \
                     __FUNCTION__, __LINE__);      \
        WPRINTLSN(s1);                             \
        errs++;                                    \
    }
#define WPRINTLSN(str)                             \
    WPRINTLS(str);                                 \
    debug_printf("\n");
#define WPRINTLS(str)                              \
    debug_printf("\"");                            \
    { int i;                                       \
      for (i=0; (size_t)i<wcslen(str); i++) {      \
          if ((str)[i] >= 32 && (str)[i] < 0x7f)   \
              debug_printf("%c", (str)[i]);        \
          else                                     \
              debug_printf("\\x%x", (str)[i]);     \
    }}                                             \
    debug_printf("\"");
#define WEXPSTR(str1, str2)                        \
    if (wcscmp(str1, str2)) {                      \
        debug_printf("%s %u  Expected ",           \
                     __FUNCTION__, __LINE__);      \
        WPRINTLS(str2);                            \
        debug_printf(", got ");                    \
        WPRINTLSN(str1);                           \
        errs++;                                    \
    }
#ifdef SAFECLIB_STR_NULL_SLACK
#define CHECK_SLACK(dest,dmax)                  \
    {   int i;                                  \
        for (i=0; i<(int)(dmax); i++) {         \
        int e = 0;                              \
        if ((dest)[i] != '\0') {                \
            debug_printf("%s %u   Error rc=%u. no slack at dest[%d] %c of %d\n", \
                __FUNCTION__, __LINE__,  rc, i, (dest)[i], (int)(dmax)); \
            if (!e) { errs++; e++; }            \
        } \
    }}
#else
#define CHECK_SLACK(dest,dmax) \
    EXPNULL(dest)
#endif

#ifdef SAFECLIB_STR_NULL_SLACK
#define WCHECK_SLACK(dest,dmax)                 \
    {   int i;                                  \
        for (i=0; i<(int)(dmax); i++) {         \
        int e = 0;                              \
        if ((dest)[i] != L'\0') {               \
            debug_printf("%s %u   Error rc=%u. no slack at dest[%d] %lc \n", \
                         __FUNCTION__, __LINE__,  rc, i, (dest)[i]); \
            if (!e) { errs++; e++; }            \
        } \
    }}
#else
#define WCHECK_SLACK(dest,dmax) \
    WEXPNULL(dest)
#endif

#define INDZERO()                                  \
    if (ind != 0) {                                \
        printf("%s %u  Error  ind=%d rc=%d \n",    \
               __FUNCTION__, __LINE__, (int)ind, rc); \
        errs++;                                    \
    }
#define INDCMP(cmp)                                \
    if ((int)ind cmp) {                            \
        printf("%s %u  Error  ind=%d rc=%d \n",    \
               __FUNCTION__, __LINE__, (int)ind, rc); \
        errs++;                                    \
    }
#define SUBNULL()                                  \
    if (sub) {                                     \
        printf("%s %u  Error  sub=\"%s\" rc=%d \n",\
               __FUNCTION__, __LINE__, (char*)sub, rc); \
        errs++;                                    \
    }
#define WSUBNULL()                                 \
    if (sub) {                                     \
        wprintf(L"%s %u  Error  sub=\"%ls\" rc=%d \n",\
                     __FUNCTION__, __LINE__, sub, rc); \
        errs++;                                    \
    }
#define SUBNN()                                     \
    if (!sub) {                                     \
        printf("%s %u  Error  sub=null errno=%d \n",\
                     __FUNCTION__, __LINE__, errno); \
        errs++;                                     \
    }
#define PTREQ(str1, str2)                          \
    if (str1 != str2) {                            \
        debug_printf("%s %u  Expected \"%s\", got \"%s\" \n", \
                     __FUNCTION__, __LINE__,  (char*)str2, (char*)str1); \
        errs++;                                    \
    }
#define PTRNN(ptr)                                          \
    if (!ptr) {                                             \
        debug_printf("%s %u  Error  ptr=null errno=%d \n",  \
                     __FUNCTION__, __LINE__, errno);        \
        errs++;                                             \
    }
#define PTRNULL(ptr)                                        \
    if (ptr) {                                              \
        debug_printf("%s %u  Error  ptr=!null errno=%d \n", \
                     __FUNCTION__, __LINE__, errno);        \
        errs++;                                             \
    }

#define SETLOCALE_C \
    lc_cat = setlocale(LC_CTYPE, "C")

#define SETLOCALE_UTF8                                  \
    lc_cat = setlocale(LC_CTYPE, "en_US.UTF-8");        \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "en_GB.UTF-8");    \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "en.UTF-8");       \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "POSIX.UTF-8");    \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "C.UTF-8");        \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "UTF-8");          \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "");               \
    debug_printf(__FILE__ ": set locale %s "            \
        "lc_cat=%s, codeset=%s\n", "UTF-8", lc_cat, lang)

#define SETLOCALE(lc)                                   \
    lc_cat = setlocale(LC_CTYPE, lc);                   \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, lc ".UTF-8");      \
    debug_printf(__FILE__ ": set locale %s "            \
        "lc_cat=%s, codeset=%s\n", lc, lc_cat, lang)

#define SETLOCALE_LT \
    lc_cat = setlocale(LC_CTYPE, "lt_LT");              \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "lt_LT.UTF-8")

#ifdef HAVE_LANGINFO_H
#define SETLANG(l) lang = nl_langinfo(CODESET)
#else
#define SETLANG(l) lang = (l)
#endif

/* not fatal */
#define CHKLOCALE_C \
    debug_printf(__FILE__ ": set locale C "     \
                 "lc_cat=%s, codeset=%s\n", lc_cat, lang); \
    if ( !strcmp(lang, "C") ||                  \
         !strcmp(lang, "ASCII") ||              \
         !strcmp(lang, "ANSI_X3.4-1968") ||     \
         !strcmp(lang, "US-ASCII") )            \
        ; /* all fine */                        \
    else /* dont inspect the values */          \
        printf(__FILE__ ": cannot test locale C" \
               " (lc_cat=%s, codeset=%s)\n", lc_cat, lang)

#define CHKLOCALE(l)                              \
    if (!lc_cat || !strstr(lc_cat, l)) {          \
        printf(__FILE__ ": cannot test locale %s" \
               " (lc_cat=%s, codeset=%s)\n", l, lc_cat, lang);  \
    } else

#define REQLOCALE(l)                              \
    if (!lc_cat || !strstr(lc_cat, l)) {          \
        printf(__FILE__ ": cannot test locale %s" \
               " (lc_cat=%s, codeset=%s)\n", l, lc_cat, lang);  \
        return errs; \
    }

#ifdef _WIN32
/* hangs at appveyor */
#define SKIP_APPVEYOR(str) \
    if (getenv_s(NULL, (char * restrict)&(str), LEN, "APPVEYOR") == EOK) {  \
        printf(__FILE__ ": SKIP appveyor %u\n", __LINE__); \
        return 0; \
    }
#else
#define SKIP_APPVEYOR(str)
#endif

#ifndef __KERNEL__

#if SIZEOF_WCHAR_T > 2
#define _ENC_W16(dest,dmax,cp) *(dest)++ = (cp); (dmax)--
#else
/* convert unicode codepoint to surrogate pair, advancing dest */
#define _ENC_W16(dest,dmax,cp)                  \
    if ((cp) < 0x10000) {                       \
        *(dest)++ = (cp); (dmax)--;             \
    } else {                                    \
        *dest++ = ((cp) >> 10) + 0xd7c0;        \
        *dest++ = ((cp) & 0x3ff) + 0xdc00;      \
        (dmax)--; (dmax)--;                     \
    }
#endif
#endif /* __KERNEL__ */

#endif /* __TEST_PRIVATE_H__ */
