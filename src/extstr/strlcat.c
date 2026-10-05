/*------------------------------------------------------------------
 * strlcat.c
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
 *    \c strlcat appends the NUL-terminated string \c src to the end
 *    of the NUL-terminated string \c dest, but never writes more
 *    than \c dsize bytes in total (including the terminating NUL)
 *    and always NUL-terminates the result (unless \c dsize is 0 or
 *    dest is not already NUL-terminated within the first \c dsize
 *    bytes). The caller must check whether truncation occurred by
 *    comparing the return value against \c dsize.
 *
 *    This is only compiled in when the platform libc does not
 *    already provide \c strlcat (see configure's \c HAVE_STRLCAT /
 *    cmake's \c HAVE_STRLCAT probe).
 *
 * @remark SPECIFIED IN
 *    * BSD, since 4.4BSD-Lite2, glibc 2.38+, musl, macOS
 *
 * @param[in,out] dest   NUL-terminated buffer that src is appended to
 * @param[in]     src    pointer to the NUL-terminated string to append
 * @param[in]     dsize  total size of the dest buffer, in bytes
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dest shall be NUL-terminated within the first dsize bytes.
 *
 * @return  the total length of the string it tried to create, i.e.
 *          the initial length of dest plus the length of src. If the
 *          return value is >= dsize, the output was truncated.
 *
 * @see
 *    strlcpy(), strcat_s(), strncat_s()
 */
EXPORT size_t strlcat(char *restrict dest, const char *restrict src,
                      size_t dsize) {
    const char *odst = dest;
    const char *osrc;
    size_t n = dsize;
    size_t dlen;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strlcat: dest is null", NULL,
                                           ESNULLP);
        return 0;
    }
    if (unlikely(src == NULL)) {
        invoke_safe_str_constraint_handler("strlcat: src is null", NULL,
                                           ESNULLP);
        return strlen(dest);
    }
    osrc = src;

    /* find the end of dest and adjust bytes left but don't go past end */
    while (n-- != 0 && *dest != '\0')
        dest++;
    dlen = (size_t)(dest - odst);
    n = dsize - dlen;

    if (n == 0)
        return dlen + strlen(src);

    while (*src != '\0') {
        if (n != 1) {
            *dest++ = *src;
            n--;
        }
        src++;
    }
    *dest = '\0';

    return dlen + (size_t)(src - osrc); /* count does not include NUL */
}
#ifdef __KERNEL__
EXPORT_SYMBOL(strlcat);
#endif
