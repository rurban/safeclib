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

#include "config.h"
#include "safe_config.h"
#include "test_private.h"
#include <stdbool.h>

#ifndef __KERNEL__
#include <errno.h>
#endif

/* With static linkage we can enforce our -lsafec over -lc.
   But not all. Some forceinline API's will still use msvcrt.
   With shared linkage we always get native msvcrt.
 */
#if defined(TEST_MSVCRT) && (HAVE_NATIVE)
#define USE_MSVCRT
#endif

#ifndef __TEST_MSVCRT_C__
extern bool use_msvcrt;
#endif

void init_msvcrt(bool is_msvcrt, bool *msvcrtp);
void print_msvcrt(bool use_msvcrt);
void _err_msvc(int rc, const int n, const int winerr, int *errp, const char *f,
               const unsigned l);
void _errno_msvc(const int n, const int winerr, int *errp, const char *f,
                 const unsigned l);

#define ERR_MSVC(n, winerr)                                                    \
    _err_msvc((int)rc, n, winerr, &errs, __FUNCTION__, __LINE__)
#define NEGERR_MSVC(n, winerr)                                                 \
    _err_msvc((int)rc, -(n), (winerr == EOF) ? EOF : -(winerr), &errs,         \
              __FUNCTION__, __LINE__)

#ifdef __KERNEL__
#define ERRNO_MSVC(n, winerr)
#define ERREOF_MSVC(n, winerr)
#else
#define ERRNO_MSVC(n, winerr)                                                  \
    _errno_msvc(n, winerr, &errs, __FUNCTION__, __LINE__)
#define ERREOF_MSVC(n, winerr)                                                 \
    _errno_msvc(n, winerr, &errs, __FUNCTION__, __LINE__);                     \
    ERR(EOF)
#endif

#ifdef USE_MSVCRT
#undef CHECK_SLACK
#define CHECK_SLACK(a, b) EXPSTR(a, "")
#define CHECK_WSLACK(a, b) WEXPSTR(a, L"")
#endif

#ifdef USE_MSVCRT
#undef tmpnam_s
#undef gets_s
#undef asctime_s
#undef ctime_s
#undef getenv_s
#undef bsearch_s
#undef qsort_s
#undef memcpy_s
#undef memmove_s
#undef memset_s
#undef memset16_s
#undef memset32_s
#undef memcmp_s
#undef memcmp16_s
#undef memcmp32_s
#undef memcpy16_s
#undef memcpy32_s
#undef memmove16_s
#undef memmove32_s
#undef memzero_s
#undef memzero16_s
#undef memzero32_s
#undef memchr_s
#undef memrchr_s
#undef timingsafe_bcmp
#undef timingsafe_memcmp
#undef memccpy_s
#undef wmemcpy_s
#undef wmemmove_s
#undef wmemcmp_s
#undef strcat_s
#undef strcpy_s
#undef stpcpy_s
#undef strncat_s
#undef strncpy_s
#undef strnlen_s
#undef strtok_s
#undef sprintf_s
#undef vsprintf_s
#undef snprintf_s
#undef vsnprintf_s
#undef strerror_s
#undef strcmp_s
#undef strcasecmp_s
#undef strnatcmp_s
#undef strnatcasecmp_s
#undef strcasestr_s
#undef strcmpfld_s
#undef strcpyfld_s
#undef strcpyfldin_s
#undef strcpyfldout_s
#undef strcspn_s
#undef strfirstchar_s
#undef strfirstdiff_s
#undef strisalphanumeric_s
#undef strisascii_s
#undef strisdigit_s
#undef strishex_s
#undef strislowercase_s
#undef strismixedcase_s
#undef strispassword_s
#undef strisuppercase_s
#undef strlastchar_s
#undef strlastdiff_s
#undef strljustify_s
#undef strnterminate_s
#undef strpbrk_s
#undef strfirstsame_s
#undef strlastsame_s
#undef strprefix_s
#undef strremovews_s
#undef strspn_s
#undef strstr_s
#undef strchr_s
#undef strrchr_s
#undef strtolowercase_s
#undef strtouppercase_s
#undef strlwr_s
#undef strupr_s
#undef strzero_s
#undef strcoll_s
#undef strset_s
#undef strnset_s
#undef mbstowcs_s
#undef mbsrtowcs_s
#undef wcsrtombs_s
#undef wcstombs_s
#undef wcrtomb_s
#undef wctomb_s
#undef wcsnlen_s
#undef wcscpy_s
#undef wcsncpy_s
#undef wcscat_s
#undef wcsncat_s
#undef wcstok_s
#undef swprintf_s
#undef vswprintf_s
#endif

#endif /* __TEST_MSVCRT_H__ */
