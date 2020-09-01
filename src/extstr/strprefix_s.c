/*------------------------------------------------------------------
 * strprefix_s.c
 *
 * November 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
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
 * @def strprefix_s(dest,dmax,src)
 * @brief
 *    Determines if the prefix pointed to by src is at the
 *    beginning of string pointed to by dest.  The prefix
 *    must be a complete match in dest.  Useful for command
 *    or user input parsing.  The scanning stops at the first
 *    null in dest or src, or after dmax characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest  pointer to string to compare against
 * @param  dmax  restricted maximum length of dest
 * @param  src   pointer to the prefix
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dest/src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOTFND   when prefix not found in dest
 *
 * @see
 *    strspn_s(), strcspn_s(), strpbrk_s(), strstr_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strprefix_s(const char *dest, rsize_t dmax, const char *src)
#else
EXPORT errno_t _strprefix_s_chk(const char *dest, rsize_t dmax, const char *src,
                                const size_t destbos)
#endif
{
    CHK_DEST_NULL("strprefix_s")
    CHK_SRC_NULL("strprefix_s", src)
    CHK_DMAX_ZERO("strprefix_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strprefix_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strprefix_s", destbos)
    }

    if (unlikely(*src == '\0')) {
        return (ESNOTFND);
    }

    /* TODO: if src and dmax are at least a word long, compare wordwise */
    while (*src && dmax) {

        if (*dest != *src) {
            return (ESNOTFND);
        }

        dmax--;
        dest++;
        src++;
    }

    return (EOK);
}
