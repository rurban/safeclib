/*------------------------------------------------------------------
 * strset_s.c
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
 * @def strset_s(dest,dmax,value)
 * @brief
 *    Sets maximal dmax characters of dest to a character value, but not
 *    the final NULL character.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the
 *    array of dmax characters pointed to by dest are nulled.
 *
 * @remark EXTENSION TO
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *    * Implemented in the Windows secure API as _strset_s()
 *
 * @param[out]  dest    string that will be set.
 * @param[in]   dmax    restricted maximum length of dest
 * @param[in]   value   character value to write
 *
 * @pre dest shall not be a null pointer, and shall be null-terminated.
 * @pre dmax shall not be greater than RSIZE_MAX_STR and size of dest.
 * @pre dmax shall not equal zero.
 * @pre value shall not be greater than 255
 *
 * @retval  EOK         when successful
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when value > 255 or dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    strzero_s(), strnset_s(), strispassword_s()
 */

#ifdef FOR_DOXYGEN
errno_t strset_s(char *restrict dest, rsize_t dmax, int value)
#else
EXPORT errno_t _strset_s_chk(char *restrict dest, rsize_t dmax, int value,
                             const size_t destbos)
#endif
{
    CHK_DEST_NULL("strset_s")
    CHK_DMAX_ZERO("strset_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strset_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strset_s", destbos)
    }
    if (unlikely((unsigned)value > 255)) {
        invoke_safe_str_constraint_handler("strset_s: value exceeds max",
                                           (void *)dest, ESLEMAX);
        return (ESLEMAX);
    }

    while (dmax && *dest) {
        *dest = (char)value;
        dmax--;
        dest++;
    }
#ifdef SAFECLIB_STR_NULL_SLACK
    /* null slack to clear any data */
    if (!*dest)
        memset(dest, 0, dmax);
#endif

    return (EOK);
}
