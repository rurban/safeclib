/*------------------------------------------------------------------
 * test_private.h - Internal test routines & library references
 *
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
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

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ctype.h>

#include <linux/string.h>
#define printf(...) printk(KERN_INFO __VA_ARGS__)

#else

/* for glibc use the GNU extensions: strcasestr */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#endif

#ifdef DEBUG
#  ifdef __KERNEL__
#    define debug_printf(...)  printk(KERN_DEBUG __VA_ARGS__)
#  else
#    define debug_printf printf
#  endif
#else
#define debug_printf(...)
#endif

#define ERR(n)                                     \
    if (rc != (n)) {                               \
        debug_printf("%s %u   Error rc=%u \n",     \
                     __FUNCTION__, __LINE__,  rc); \
        errs++;                                    \
    }
#define NOERR()                                    \
    if (rc < 0) {                                  \
        debug_printf("%s %u   Error rc=%u \n",     \
                 __FUNCTION__, __LINE__,  rc);     \
        errs++;                                    \
    }
#define NOERRNULL()                                \
    if (rc <= 0) {                                 \
        debug_printf("%s %u   Error rc=%u \n",     \
                 __FUNCTION__, __LINE__,  rc);     \
        errs++;                                    \
    }
#define EXPNULL(str1)                              \
    if ((str1)[0] != '\0') {                       \
        debug_printf("%s %u   Expected null, got \%s\" \n", \
                     __FUNCTION__, __LINE__, str1);  \
        errs++;                                    \
    }
#define EXPSTR(str1, str2)                         \
    ind = strcmp(str1, str2);                      \
    if (ind != 0) {                                \
        debug_printf("%s %u   Expected \%s\", got \%s\" \n", \
                     __FUNCTION__, __LINE__,  str2, str1);  \
        errs++;                                    \
    }
#define INDNULL()                                  \
    if (ind != 0) {                                \
        printf("%s %u  Error  ind=%d rc=%d \n",    \
                     __FUNCTION__, __LINE__, ind, rc); \
        errs++;                                    \
    }
#define SUBNULL()                                  \
    if (sub) {                                     \
        printf("%s %u  Error  sub=\"%s\" rc=%d \n",    \
                     __FUNCTION__, __LINE__, sub, rc); \
        errs++;                                    \
    }
#define PTREQ(str1, str2)                          \
    if (str1 != str2) {                            \
        debug_printf("%s %u   Expected \%s\", got \%s\" \n", \
                     __FUNCTION__, __LINE__,  str2, str1);  \
        errs++;                                    \
    }


#if !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
#  define NO_C11
#endif

#endif /* __TEST_PRIVATE_H__ */
