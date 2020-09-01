/*------------------------------------------------------------------
 * strnlen_s.c
 *
 * October 2008, Bo Berry
 * May 2019, Reini Urban
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
 * Copyright (c) 2019 by Reini Urban
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

/**
 * @def strnlen_s(str,smax)
 * @brief
 *    The \b strnlen_s function computes the length of the string pointed
 *    to by \c str, refusing to read past smax.
 *
 * @remark SPECIFIED IN
 *    ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  str  pointer to string
 * @param  smax maximum length of string
 *
 * @pre  str  shall not be a null pointer.
 * @pre  smax shall not equal zero.
 * @pre  smax shall not be greater than RSIZE_MAX_STR nor sizeof(str).
 *
 * @return   The function returns the string length, excluding the terminating
 *           null character.  If \c str is NULL, then \c strnlen_s returns \c
 *           0.  Otherwise, the \c strnlen_s function returns the number of
 *           characters that precede the terminating null character. If there
 *           is no null character in the first \c smax characters of \c str
 *           then \c strnlen_s returns smax. At most the first \c smax or \c
 *           sizeof(str) characters of str are accessed by \c strnlen_s.
 *
 * @see
 *    strnterminate_s()
 *    http://www.informit.com/articles/article.aspx?p=2036582&seqNum=5
 *
 */
#ifdef FOR_DOXYGEN
rsize_t strnlen_s(const char *str, rsize_t smax, size_t strbos)
#else
EXPORT rsize_t _strnlen_s_chk(const char *str, rsize_t smax, size_t strbos)
#endif
{
    rsize_t count;

    if (unlikely(str == NULL)) {
        invoke_safe_str_constraint_handler("strnlen_s: str is null", NULL,
                                           ESNULLP);
        return 0;
    }

    if (unlikely(smax == 0)) {
        invoke_safe_str_constraint_handler("strnlen_s: smax is 0", (void *)str,
                                           ESZEROL);
        return 0;
    }
    if (unlikely(smax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strnlen_s: smax exceeds max",
                                           (void *)str, ESLEMAX);
        return 0;
    }
    if (strbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(str, smax);
    } else {
        if (unlikely(smax != strbos)) {
#ifdef HAVE_WARN_DMAX
            handle_str_src_bos_chk_warn("strnlen_s", (char *)str, smax, strbos,
                                        "str", "smax");
#ifdef HAVE_ERROR_DMAX
            return 0;
#endif
#endif
        }
    }

    count = 0;
    while (*str && smax) {
        count++;
        smax--;
        str++;
        /* Dont touch past strbos */
        if (strbos != BOS_UNKNOWN) {
            strbos--;
            if (unlikely(!strbos))
                return count;
        }
    }

    return count;
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strnlen_s_chk);
#endif /* __KERNEL__ */
