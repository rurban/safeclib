/*------------------------------------------------------------------
 * u8zero_s.c
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

#ifndef FOR_DOXYGEN
/* The dest/dmax constraints are already checked by _u8zero_s_chk or proven at
   compile-time. GH #48 */
EXPORT errno_t _u8zero_s_uchk(char8_t *dest, rsize_t dmax) {
    /* null string to eliminate data */
    while (dmax && *dest) {
        *dest = '\0';
        dmax--;
        dest++;
    }
#ifdef SAFECLIB_STR_NULL_SLACK
    if (!*dest)
        memset(dest, 0, dmax);
#endif

    return (EOK);
}
#endif

/**
 * @def u8zero_s(dest,dmax)
 * @brief
 *    Nulls maximal dmax bytes of the utf-8 string dest.  This function can
 *    be used to clear strings that contained sensitive data, until the
 *    terminating NUL character. With SAFECLIB_STR_NULL_SLACK defined all
 *    elements following the terminating NUL character (if any) written in
 *    the array of dmax characters pointed to by dest are nulled.
 *
 * @param[out]  dest  pointer to utf-8 string that will be nulled.
 * @param[in]   dmax  restricted maximum byte-length of dest
 *
 * @retval  EOK         when successful operation
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    u8set_s(), u8nset_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8zero_s(char8_t *dest, rsize_t dmax)
#else
EXPORT errno_t _u8zero_s_chk(char8_t *dest, rsize_t dmax, const size_t destbos)
#endif
{
    CHK_DEST_NULL("u8zero_s")
    CHK_DMAX_ZERO("u8zero_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8zero_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8zero_s", destbos)
    }

    return _u8zero_s_uchk(dest, dmax);
}
