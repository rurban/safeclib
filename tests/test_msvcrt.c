/*------------------------------------------------------------------
 * test_msvcrt.c - Seperate safec from native msvcrt sec_api
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

#define __TEST_MSVCRT_C__
#include "test_msvcrt.h"

bool use_msvcrt;
void print_msvcrt(bool is_msvcrt) {
#if defined(_WIN32) || defined(TEST_MSVCRT)
    printf("Using %s, %s...\n", is_msvcrt ? "msvcrt" : "safec",
#ifdef DISABLE_DLLIMPORT
           "static"
#else
           "shared"
#endif
    );
#else
    (void)is_msvcrt;
#endif
}

void init_msvcrt(bool is_msvcrt, bool *msvcrtp) {
#if defined(_WIN32) || defined(TEST_MSVCRT)
    if (is_msvcrt) {
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
    *msvcrtp = false;
#endif
}

void _err_msvc(int rc, const int n, const int winerr, int *errp, const char *f,
               const unsigned l) {
    const int chk = use_msvcrt ? winerr : n;
#ifndef DEBUG
    (void)f;
    (void)l;
#endif
    if (rc != chk) {
        debug_printf("%s %u  Error rc=%d \n", f, l, rc);
        (*errp)++;
    }
}

#ifndef __KERNEL__
void _errno_msvc(const int n, const int winerr, int *errp, const char *f,
                 const unsigned l) {
    const int chk = use_msvcrt ? winerr : n;
#ifndef DEBUG
    (void)f;
    (void)l;
#endif
    if (errno != chk) {
        debug_printf("%s %u  Error errno=%d \n", f, l, (int)errno);
        (*errp)++;
    }
    if (use_msvcrt)
        errno = 0;
}
#endif
