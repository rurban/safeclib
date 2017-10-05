/*------------------------------------------------------------------
 * wcsupr_s.c
 *
 * September 2017, Reini Urban
 *
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

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

#ifdef HAVE_TOWUPPER
#include <wctype.h>
#endif
#ifndef HAVE_TOWUPPER_OK
EXTERN wint_t _towupper(wint_t wc);
#endif

/**
 * @brief
 *    Scans the string converting lowercase characters to uppercase, leaving
 *    all other characters unchanged.  The scanning stops at the first null or
 *    after slen characters.  The conversion is determined by the LC_CTYPE
 *    category setting of the locale. Other characters are not affected.  It
 *    converts only single chars via \c towupper().  It returns a pointer to
 *    the altered string. Because the modification is done in place, the
 *    pointer returned is the same as the pointer passed as the input
 *    argument.
 *
 * @remark IMPLEMENTED IN
 *    * Microsoft Windows Secure API
 *    * Novell NDK
 *    * Nokia openc s60.com
 *
 * @param[out]  src   wide string
 * @param[in]   slen  maximum length of string
 *
 * @pre  src shall not be a null pointer.
 * @pre  slen shall not equal zero.
 * @pre  slen shall not be greater than RSIZE_MAX_WSTR.
 *
 * @retval  EOK         on successful operation
 * @retval  ESNULLP     when src is NULL pointer
 * @retval  ESZEROL     when slen = 0
 * @retval  ESLEMAX     when slen > RSIZE_MAX_WSTR
 *
 * @see
 *    strtouppercase_s(), strlwr_s(), wcslwr_s()
 *
 */
EXPORT errno_t
wcsupr_s(wchar_t *restrict src, rsize_t slen)
{
    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("wcsupr_s: "
                   "src is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("wcsupr_s: "
                   "slenx is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(slen > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("wcsupr_s: "
                   "slen exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    while (*src && slen) {
#ifdef HAVE_TOWUPPER_OK
        *src = towupper(*src);
#else
        *src = _towupper(*src);
#endif
        src++;
        slen--;
    }

    return (EOK);
}
