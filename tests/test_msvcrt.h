/*------------------------------------------------------------------
 * test_msvcrt.h - Seperate safec from native msvcrt sec_api
 *
 * 2018  Reini Urban
 *
 * Copyright (c) 2018 Reini Urban
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

#ifndef __TEST_MSVCRT_H__
#define __TEST_MSVCRT_H__

#ifndef HAVE_NATIVE
# error define HAVE_NATIVE defined(HAVE_api_S)
#endif

/* With static linkage we can enforce our -lsafec over -lc */
#if defined(_WIN32) && HAVE_NATIVE && !defined(DISABLE_DLLIMPORT)
# define USE_MSVCRT
#endif

#ifdef USE_MSVCRT
#define ERR_MSVC(n, winerr)                        \
    if (rc != (winerr)) {                          \
        debug_printf("%s %u  Error rc=%d \n",      \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
/* msvcrt also doesn't reset errno */
#define ERRNO_MSVC(n, winerr)                      \
    if (errno != (winerr)) {                       \
        debug_printf("%s %u  Error errno=%d \n",   \
                     __FUNCTION__, __LINE__,  (int)errno); \
        errs++;                                    \
    }                                              \
    errno = 0
#undef CHECK_SLACK
#define CHECK_SLACK(a,b) \
    EXPSTR(a, "")
#else
#define ERR_MSVC(n, winerr)                        \
    if (rc != (n)) {                               \
        debug_printf("%s %u  Error rc=%d \n",      \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#define ERRNO_MSVC(n, winerr)                      \
    if (errno != (n)) {                            \
        debug_printf("%s %u  Error errno=%d \n",   \
                     __FUNCTION__, __LINE__,  (int)errno); \
        errs++;                                    \
    }
#endif

#endif /* __TEST_MSVCRT_H__ */
