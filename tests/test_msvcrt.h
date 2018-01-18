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

/* With static linkage we can enforce our -lsafec over -lc.
   But not all. Some forceinline API's will still use msvcrt.
   With shared linkage we always get native msvcrt.
 */
#if defined(_WIN32) && (HAVE_NATIVE) && !defined(DISABLE_DLLIMPORT)
# define USE_MSVCRT
bool use_msvcrt = true;
#else
bool use_msvcrt = false;
#endif

void print_msvcrt(bool use_msvcrt) {
#ifdef _WIN32
    printf("Using %s, have_native=%s %s...\n",
        use_msvcrt ? "msvcrt" : "safec",
# if (HAVE_NATIVE)
        "yes",
# else
        "no",
# endif
# ifdef DISABLE_DLLIMPORT
        "static"
# else
        "shared"
# endif
        );
#else
    (void)use_msvcrt;
#endif
}

void init_msvcrt(bool is_msvcrt, bool *msvcrtp) {
#ifdef _WIN32
    if ( is_msvcrt ) {
        if (*msvcrtp)
            printf("No, safec.dll overriding msvcrt.dll\n");
        *msvcrtp = false;
    } else {
        if (!*msvcrtp)
            printf("No, msvcrt.dll overriding safec.dll\n");
        *msvcrtp = true;
    }
#else
    (void)is_msvcrt;
    (void)msvcrtp;
#endif
}

#define ERR_MSVC(n, winerr)   \
    _err_msvc((int)rc, n, winerr, &errs, __FUNCTION__, __LINE__)
#define ERRNO_MSVC(n, winerr) \
    _errno_msvc(n, winerr, &errs, __FUNCTION__, __LINE__)

void _err_msvc(int rc, const int n, const int winerr, int *errp,
               const char *f, const unsigned l)
{
    const int chk = use_msvcrt ? winerr : n;
    if (rc != chk) {
        debug_printf("%s %u  Error rc=%d \n", f, l, rc);
        (*errp)++;
    }
}
void _errno_msvc(const int n, const int winerr, int *errp,
                 const char *f, const unsigned l)
{
    const int chk = use_msvcrt ? winerr : n;
    if (errno != chk) {
        debug_printf("%s %u  Error errno=%d \n", f, l, (int)errno);
        (*errp)++;
    }
    if (use_msvcrt)
        errno = 0;
}

#ifdef USE_MSVCRT
#undef CHECK_SLACK
#define CHECK_SLACK(a,b)  EXPSTR(a, "")
#define CHECK_WSLACK(a,b) WEXPSTR(a, L"")
#endif

#endif /* __TEST_MSVCRT_H__ */
