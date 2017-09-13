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

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ctype.h>

#include <linux/string.h>
#ifdef HAVE_C99
#define printf(...) printk(KERN_INFO __VA_ARGS__)
#endif

#else

/* for glibc use the GNU extensions: strcasestr */
#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#endif

#ifdef _WIN32
#define __STRICT_ANSI__
#include <io.h>
#define pipe(p) _pipe(p,2,0)
#endif

/* libc variant: musl not detectable. and there's __GLIBC__ and _WIN32.
   uClibc? dietlibc? minilibc? exots? */
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
#if defined(__APPLE__) || defined(__NEWLIB__) || defined(BSD_LIKE)
# define BSD_ALL_LIKE
#endif

#if !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
#  define NO_C11
#endif

#if defined(__has_feature)
# if __has_feature(address_sanitizer)
#  define HAVE_ASAN 1
# endif
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

#define ERR(n)                                     \
    if (rc != (n)) {                               \
        debug_printf("%s %u  Error rc=%d \n",     \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#define ANYERR()                                    \
    if (rc >= 0) {                                  \
        debug_printf("%s %u  Error rc=%d \n",       \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#define ERRNO(n)                                   \
    if (errno != (n)) {                            \
        debug_printf("%s %u  Error errno=%d \n",  \
                     __FUNCTION__, __LINE__,  (int)errno); \
        errs++;                                    \
    }
#define ERREOF(n)                                  \
    ERR(EOF);                                      \
    if (errno != (n)) {                            \
        debug_printf("%s %u  Error errno=%d \n",  \
                     __FUNCTION__, __LINE__,  (int)errno); \
        errs++;                                    \
    }
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
#define EXPNULL(str1)                              \
    if ((str1)[0] != '\0') {                       \
        debug_printf("%s %u  Expected null, got \"%s\" \n", \
                     __FUNCTION__, __LINE__, str1); \
        errs++;                                    \
    }
#define EXPSTR(str1, str2)                         \
    ind = strcmp(str1, str2);                      \
    if (ind != 0) {                                \
        debug_printf("%s %u  Expected \"%s\", got \"%s\" \n", \
                     __FUNCTION__, __LINE__,  str2, str1);  \
        errs++;                                    \
    }
#define WEXPNULL(str1)                             \
    if ((str1)[0] != L'\0') {                      \
        debug_printf("%s %u  Expected null, got L\"%ls\"\n", \
                     __FUNCTION__, __LINE__, str1); \
        errs++;                                    \
    }
#define WEXPSTR(str1, str2)                        \
    if (wcscmp(str1, str2)) {                      \
        debug_printf("%s %u  Expected L\"%ls\", got L\"%ls\"  \"", \
            __FUNCTION__, __LINE__, str2, str1);   \
        for (ind=0; (size_t)ind<wcslen(str2); ind++) { \
            debug_printf("\\x%x", str2[ind]);      \
        }                                          \
        debug_printf("\" <=> \"");                 \
        for (ind=0; (size_t)ind<wcslen(str1); ind++) { \
            debug_printf("\\x%x", str1[ind]);      \
        }                                          \
        debug_printf("\"\n");                      \
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
        printf("%s %u  Error  sub=\"%ls\" rc=%d \n",\
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
        lc_cat = setlocale(LC_CTYPE, "")

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

#define CHKLOCALE(l)                             \
    if (!lc_cat || strncmp(lc_cat, l, 2)) {                 \
        printf(__FILE__ ": cannot test locale %s" \
               " (lc_cat=%s, codeset=%s)\n", l, lc_cat, lang); \
    } else

#endif /* __TEST_PRIVATE_H__ */
