/*------------------------------------------------------------------
 * u8coll_s.c
 *
 * September 2026, Reini Urban
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
 * @def u8coll_s(dest,dmax,src,resultp)
 * @brief
 *    Compares two null-terminated utf-8 strings according to the
 *    current locale as defined by the \c LC_COLLATE category.
 *
 * @details
 *    The strings are passed byte-wise to strcoll() as locale collation
 *    of utf-8 is byte-order preserving. This is the utf-8 analogue of
 *    strcoll_s().
 *
 * @param[in]   dest       pointer to utf-8 string to compare against
 * @param[in]   dmax       restricted maximum byte-length of string dest
 * @param[in]   src        pointer to the utf-8 string to be compared to dest
 * @param[out]  resultp    pointer to result indicator, greater than 0,
 *                         equal to 0 or less than 0, if the string pointed
 *                         to by dest is greater than, equal to or less
 *                         than the string pointed to by src respectively.
 *
 * @pre   Neither dest, src nor resultp shall be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @return  The error code of the result. On EOK, see resultp.
 * @retval  EOK          when comparison is complete and the result is
 *                       returned in resultp
 * @retval  ESNULLP      when dest, src, or resultp is the NULL pointer
 * @retval  ESZEROL      when dmax = 0
 * @retval  ESLEMAX      when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW    when dmax > size of dest (optionally, when the
 *                       compiler knows the object_size statically)
 * @retval  ESLEWRNG     when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    u8cmp_s(), u8natcmp_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8coll_s(const char8_t *restrict dest, rsize_t dmax,
                 const char8_t *restrict src, int *resultp)
#else
EXPORT errno_t _u8coll_s_chk(const char8_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src, int *resultp,
                             const size_t destbos)
#endif
{
    CHK_SRC_NULL("u8coll_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("u8coll_s")
    CHK_SRC_NULL("u8coll_s", src)
    CHK_DMAX_ZERO("u8coll_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8coll_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8coll_s", destbos)
    }

    return _u8coll_s_uchk(dest, src, resultp);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_u8coll_s_chk);
#endif /* __KERNEL__ */

#ifndef FOR_DOXYGEN
/* The constraints are already checked by _u8coll_s_chk or proven at
   compile-time. GH #48 */
EXPORT errno_t _u8coll_s_uchk(const char8_t *dest, const char8_t *src,
                              int *resultp) {
    *resultp = strcoll((const char *)dest, (const char *)src);
    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_u8coll_s_uchk);
#endif /* __KERNEL__ */
#endif
