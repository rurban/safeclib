/*------------------------------------------------------------------
 * strlcpy.c
 *
 * October 2026, Reini Urban
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @brief
 *    \c strlcpy copies up to \c dsize - 1 characters from the
 *    NUL-terminated string \c src to \c dest, NUL-terminating the
 *    result (unless \c dsize is 0). Unlike \c strncpy, it never
 *    writes more than \c dsize bytes in total and always
 *    NUL-terminates a non-zero sized buffer. The caller must check
 *    whether truncation occurred by comparing the return value
 *    against \c dsize.
 *
 *    This is only compiled in when the platform libc does not
 *    already provide \c strlcpy (see configure's \c HAVE_STRLCPY /
 *    cmake's \c HAVE_STRLCPY probe).
 *
 * @remark SPECIFIED IN
 *    * BSD, since 4.4BSD-Lite2, glibc 2.38+, musl, macOS
 *
 * @param[out]  dest   pointer to the buffer that will receive src
 * @param[in]   src    pointer to the NUL-terminated string to copy
 * @param[in]   dsize  size of the dest buffer, in bytes
 *
 * @pre  Neither dest nor src shall be a null pointer, unless dsize is 0.
 *
 * @return  the total length of the string it tried to create, i.e.
 *          \c strlen(src). If the return value is >= dsize, the
 *          output was truncated.
 *
 * @see
 *    strlcat(), strcpy_s(), stpcpy_s()
 */
EXPORT size_t strlcpy(char *restrict dest, const char *restrict src,
                      size_t dsize) {
    const char *osrc = src;
    size_t nleft = dsize;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strlcpy: dest is null", NULL,
                                           ESNULLP);
        return 0;
    }
    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("strlcpy: src is null", NULL,
                                           ESNULLP);
        if (dsize != 0)
            *dest = '\0';
        return 0;
    }

    /* copy as many bytes as will fit */
    if (nleft != 0) {
        while (--nleft != 0) {
            if ((*dest++ = *src++) == '\0')
                break;
        }
    }

    /* not enough room in dest, add NUL and traverse rest of src */
    if (nleft == 0) {
        if (dsize != 0)
            *dest = '\0'; /* NUL-terminate dest */
        while (*src++)
            ;
    }

    return (size_t)(src - osrc - 1); /* count does not include NUL */
}
#ifdef __KERNEL__
EXPORT_SYMBOL(strlcpy);
#endif
