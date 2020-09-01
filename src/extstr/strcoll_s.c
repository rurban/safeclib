/*------------------------------------------------------------------
 * strcoll_s.c -- string locale-aware compare
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
 * @def strcoll_s(dest,dmax,src,resultp)
 * @brief
 *    Compares two null-terminated byte strings according to the
 *    current locale as defined by the \c LC_COLLATE category.
 *
 * @details
 *    Collation order is the dictionary order: the position of the letter in
 *    the national alphabet (its equivalence class) has higher priority than
 *    its case or variant. Within an equivalence class, lowercase characters
 *    collate before their uppercase equivalents and locale-specific order may
 *    apply to the characters with diacritics. In some locales, groups of
 *    characters compare as single collation units. For example, "ch" in Czech
 *    follows "h" and precedes "i", and "dzs" in Hungarian follows "dz" and
 *    precedes "g".
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest       pointer to string to compare against
 * @param[in]   dmax       restricted maximum length of string dest
 * @param[in]   src        pointer to the string to be compared to dest
 * @param[out]  resultp    pointer to result indicator, greater than 0,
 *                         equal to 0 or less than 0, if the string pointed
 *                         to by dest is greater than, equal to or less
 *                         than the string pointed to by src respectively.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   resultp shall not be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @return  indicator (when the return code is OK)
 * @retval  >0 when dest greater than src
 * @retval  0 when strings the same
 * @retval  <0 when dest less than src
 * @retval  EOK          when comparison is complete
 * @retval  ESNULLP      when dest/src/resultp is NULL pointer
 * @retval  ESZEROL      when dmax = 0
 * @retval  ESLEMAX      when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW    when dmax > size of dest (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG     when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    wcscoll_s(), strcmp_s(), strcasecmp_s()
 */

#ifdef FOR_DOXYGEN
errno_t strcoll_s(const char *restrict dest, rsize_t dmax,
                  const char *restrict src, int *resultp)
#else
EXPORT errno_t _strcoll_s_chk(const char *restrict dest, rsize_t dmax,
                              const char *restrict src, int *resultp,
                              const size_t destbos)
#endif
{
    CHK_SRC_NULL("strcoll_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("strcoll_s")
    CHK_SRC_NULL("strcoll_s", src)
    CHK_DMAX_ZERO("strcoll_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strcoll_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strcoll_s", destbos)
    }

    *resultp = strcoll(dest, src);

    return RCNEGATE(EOK);
}
