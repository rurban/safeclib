/*------------------------------------------------------------------
 * strchr_s.c
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

/**
 * @def strchr_s(dest,dmax,ch,resultp)
 * @brief
 *    Finds the first occurrence of ch (after conversion to char as if by
 *    (char)ch) in the null-terminated byte string pointed to by dest (each
 *    character interpreted as unsigned char). The terminating null character
 *    is considered to be a part of the string and can be found when searching
 *    for '\0'.
 *
 * @remark IMPLEMENTED IN
 *
 * @param[in]  dest    pointer to string to compare against
 * @param[in]  dmax    restricted maximum length of dest
 * @param[in]  ch      character to search for
 * @param[out] resultp pointer to char* in dest
 *
 * @pre  Neither dest nor resultp shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 * @pre  ch shall not be greater than 255
 *
 * @retval  EOK        when successfully character found.
 * @retval  ESNULLP    when dest/resultp is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR, or ch > 255
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOTFND   when ch not found in dest
 *
 * @see
 *    memchr_s(), strspn_s(), strcspn_s(), strpbrk_s(), strstr_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strchr_s(const char *restrict dest, rsize_t dmax,
                 const int ch, char **restrict resultp)
#else
EXPORT errno_t _strchr_s_chk(const char *restrict dest, rsize_t dmax,
                             const int ch, char **restrict resultp,
                             const size_t destbos)
#endif
{
    CHK_SRC_NULL("strchr_s", resultp)
    *resultp = NULL;

    CHK_DEST_NULL("strchr_s")
    CHK_DMAX_ZERO("strchr_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strchr_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strchr_s", destbos)
    }

    if (unlikely(ch > 255)) {
        invoke_safe_str_constraint_handler("strchr_s: ch exceeds max",
                                           (void *)dest, ESLEMAX);
        return (ESLEMAX);
    }

    /* compares wordwise */
    /* XXX gcc-4.4 fails with logical ‘&&’ with non-zero constant
       will always evaluate as true.
       Expands to *result = (char*)(__extension__ (__builtin_constant_p (ch)
        && !__builtin_constant_p ((const char *)dest) && (ch) == '\0'
          ? (char *) __rawmemchr ((const char *)dest, ch)
          : __builtin_strchr ((const char *)dest, ch)));
    */
#if defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) == 404)
    *resultp = (char *)__builtin_strchr((const char *)dest, ch);
#else
    *resultp = (char *)strchr((const char *)dest, ch);
#endif

    if (!*resultp)
        return (ESNOTFND);
    else if ((long)(*resultp - dest) > (long)dmax) {
        *resultp = NULL;
        return (ESNOTFND);
    }
    return (EOK);
}
