/*------------------------------------------------------------------
 * u8nset_s.c
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

/**
 * @def u8nset_s(dest,dmax,value,n)
 * @brief
 *    Sets maximal n bytes of the utf-8 string dest to a byte value,
 *    but not the final NUL character.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating NUL character (if any) written in the
 *    array of dmax characters pointed to by dest are nulled.
 *
 * @param[out]  dest    utf-8 string that will be set.
 * @param[in]   dmax    restricted maximum byte-length of dest
 * @param[in]   value   byte value to write
 * @param[in]   n       number of bytes to be written
 *
 * @pre dest shall not be a null pointer, and shall be null-terminated.
 * @pre dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 * @pre dmax shall not equal zero.
 * @pre n shall not be greater than dmax
 * @pre value shall not be greater than 255
 *
 * @retval  EOK         when successful
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when value > 255 or dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESNOSPC     when n > dmax
 *
 * @see
 *    u8zero_s(), u8set_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8nset_s(char8_t *restrict dest, rsize_t dmax, int value, rsize_t n)
#else
EXPORT errno_t _u8nset_s_chk(char8_t *restrict dest, rsize_t dmax, int value,
                             rsize_t n, const size_t destbos)
#endif
{
#ifdef SAFECLIB_STR_NULL_SLACK
    char8_t *orig_dest;
#endif

    CHK_DEST_NULL("u8nset_s")
    CHK_DMAX_ZERO("u8nset_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8nset_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8nset_s", destbos)
    }
    if (unlikely((unsigned)value > 255)) {
        invoke_safe_str_constraint_handler("u8nset_s: value exceeds max",
                                           (void *)dest, ESLEMAX);
        return (ESLEMAX);
    }
    if (unlikely(n > dmax)) {
        invoke_safe_str_constraint_handler("u8nset_s: n exceeds dmax", NULL,
                                           ESNOSPC);
        return (ESNOSPC);
    }

#ifdef SAFECLIB_STR_NULL_SLACK
    orig_dest = dest;
#endif
    while (n && *dest) {
        *dest = (char8_t)value;
        n--;
        dest++;
    }
#ifdef SAFECLIB_STR_NULL_SLACK
    /* null slack to clear any data */
    if (!*dest)
        memset(dest, 0, dmax - (dest - orig_dest));
#endif

    return (EOK);
}
