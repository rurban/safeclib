/*------------------------------------------------------------------
 * strljustify_s.c
 *
 * November 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
 * All rights reseved.
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
 * @def strljustify_s(dest,dmax)
 * @brief
 *    Removes beginning whitespace from the string pointed to by
 *    dest by shifting the text left over writting the beginning
 *    whitespace, left justifying the text.
 * @details
 *    The left justified text is null terminated.
 *    The text is shifted so the original pointer can continue
 *    to be used.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string to left justify
 * @param[in]   dmax  restricted maximum length of string
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 * @pre  dest shall be null terminated
 *
 * @retval  EOK         when successful operation
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESUNTERM    when dest was not null terminated
 *
 * @see
 *    strremovews_s(),
 *
 */
#ifdef FOR_DOXYGEN
errno_t strljustify_s(char *dest, rsize_t dmax)
#else
EXPORT errno_t _strljustify_s_chk(char *dest, rsize_t dmax,
                                  const size_t destbos)
#endif
{
    char *orig_dest;
    rsize_t orig_dmax;

    CHK_DEST_NULL("strljustify_s")
    CHK_DMAX_ZERO("strljustify_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strljustify_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strljustify_s", destbos)
    }

    /*
     * corner cases, a dmax of one allows only for a null
     */
    if (unlikely(dmax <= RSIZE_MIN_STR)) {
        *dest = '\0';
        return (EOK);
    }
    if (unlikely(*dest == '\0')) {
        return (EOK);
    }

    orig_dmax = dmax;
    orig_dest = dest;

    /*
     * scan the string to be sure it is properly terminated
     */
    while (*dest) {
        if (unlikely(dmax == 0)) {
            while (orig_dmax) {
                *orig_dest++ = '\0';
                orig_dmax--;
            }

            invoke_safe_str_constraint_handler(
                "strljustify_s: dest is unterminated", NULL, ESUNTERM);
            return (ESUNTERM);
        }
        dmax--;
        dest++;
    }

    /*
     * find first non-white space char
     */
    dest = orig_dest;
    while ((*dest == ' ') || (*dest == '\t')) {
        dest++;
    }

    /*
     * shift text, removing spaces, to left justify
     */
    if (orig_dest != dest) {
        while (*dest) {
            *orig_dest++ = *dest;
            *dest++ = ' ';
        }
        *orig_dest = '\0';
    }

    return (EOK);
}
