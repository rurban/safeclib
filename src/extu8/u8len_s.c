/*------------------------------------------------------------------
 * u8len_s.c
 *
 * September 2026
 *
 * Copyright (c) 2026 by Reini Urban
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

#ifdef FOR_DOXYGEN
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def u8len_s(str)
 * @brief
 *    The u8len_s function computes the byte-length of the null-terminated
 *    utf-8 string pointed to by str, not examining more than RSIZE_MAX_STR
 *    bytes.
 *
 * @param  str   pointer to utf-8 string
 *
 * @return The function returns the utf-8 string length in bytes, excluding
 * the terminating null character. If \c str is NULL, then \c u8len_s
 * returns 0. If there is no null character in the first RSIZE_MAX_STR bytes
 * of str then \c u8len_s returns RSIZE_MAX_STR. At most RSIZE_MAX_STR bytes
 * of str are accessed by \c u8len_s.
 *
 * @see
 *    u8nlen_s(), strnlen_s()
 */
#ifdef FOR_DOXYGEN
rsize_t u8len_s(const char8_t *str)
#else
EXPORT rsize_t _u8len_s_chk(const char8_t *str, size_t strbos)
#endif
{
    return _u8nlen_s_chk(str, RSIZE_MAX_STR, strbos);
}
