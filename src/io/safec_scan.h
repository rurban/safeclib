/*------------------------------------------------------------------
 * safec_scan.h - shared integer/float scanning engine
 *
 * February 2022, Reini Urban
 * October 2026, Reini Urban
 *
 * Copyright (c) 2022, 2026 by Reini Urban
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
#ifndef __SAFEC_SCAN_H__
#define __SAFEC_SCAN_H__

/* safec_intscan()/safec_floatscan() are the numeric conversion engines
 * shared by the ascii (_safec_vfscanf_impl) and wide (_safec_vfwscanf_impl)
 * scanners. Both flavors drive them through this tiny cursor so the
 * numeric grammar only has to be implemented once: the ascii side binds
 * it to shgetc()/shunget()/shlim() on a byte-oriented _SAFEC_FILE, the
 * wide side binds it to a wchar_t cursor (either a real FILE* via
 * fgetwc()/ungetwc(), or a plain in-memory wchar_t* walk).
 *
 * get()    returns the next character (any non-negative code point for
 *          wide sources, 0-255 for byte sources), or a negative value
 *          at end of input.
 * unget()  pushes the single most-recently gotten character back.
 * setlim() establishes a new field-width limit (0 == unlimited) and
 *          resets the "characters consumed since the last setlim()"
 *          count that get()/unget() maintain internally.
 */
typedef struct safec_scan_cursor {
    void *ctx;
    int (*get)(void *ctx);
    void (*unget)(void *ctx);
    void (*setlim)(void *ctx, long lim);
} safec_scan_cursor;

unsigned long long safec_intscan(safec_scan_cursor *cur, unsigned base,
                                 int pok, unsigned long long lim);
long double safec_floatscan(safec_scan_cursor *cur, int prec, int pok);

#endif /* __SAFEC_SCAN_H__ */
