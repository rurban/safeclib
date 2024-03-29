/*------------------------------------------------------------------
 * wcsnset_s.c
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
 * @def wcsnset_s(dest,dmax,value,n)
 * @brief
 *    Sets maximal n wide characters of dest to a wide character value,
 *    but not the final NULL character.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating NUL character (if any) written in the
 *    array of dmax wide characters pointed to by dest are nulled.
 *
 * @remark EXTENSION TO
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *    * Implemented in the Windows secure API as _wcsnset_s()
 *
 * @param[out]  dest    wide string that will be set.
 * @param[in]   dmax    restricted maximum length of dest
 * @param[in]   value   wide character value to write
 * @param[in]   n       number of wide characters to be written
 *
 * @pre   dest shall not be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_WSTR and size of dest.
 * @pre   value shall not be greater than _UNICODE_MAX
 * @pre   n shall not be greater than dmax
 *
 * @retval  EOK         when successful
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_WSTR or value > _UNICODE_MAX
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOSPC     when n > dmax
 *
 * @see
 *    wcsset_s(), wmemset_s(), strzero_s(), strnset_s(), strispassword_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t wcsnset_s(wchar_t *restrict dest, rsize_t dmax, wchar_t value, rsize_t n)
#else
EXPORT errno_t _wcsnset_s_chk(wchar_t *restrict dest, rsize_t dmax, wchar_t value, rsize_t n,
                              const size_t destbos)
#endif
{
#ifdef SAFECLIB_STR_NULL_SLACK
    wchar_t *orig_dest;
#endif
    const size_t destsz = dmax * sizeof(wchar_t);

    CHK_DEST_NULL("wcsnset_s")
    CHK_DMAX_ZERO("wcsnset_s")
#if SIZEOF_WCHAR_T > 2
    if (unlikely(value > _UNICODE_MAX)) {
        invoke_safe_str_constraint_handler("wcsnset_s: value exceeds max",
                                           (void *)&value, ESLEMAX);
        return (ESLEMAX);
    }
#endif
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("wcsnset_s", RSIZE_MAX_WSTR)
        BND_CHK_PTR_BOUNDS(dest, destsz);
    } else {
        CHK_DESTW_OVR_CLEAR("wcsnset_s", destsz, destbos)
    }
    if (unlikely(n > dmax)) {
        handle_werror(dest, dmax, "wcsnset_s: n exceeds dmax", ESNOSPC);
        return (ESNOSPC);
    }

#ifdef SAFECLIB_STR_NULL_SLACK
    orig_dest = dest;
#endif
    while (n && *dest) {
        *dest = value;
        n--;
        dest++;
    }
#ifdef SAFECLIB_STR_NULL_SLACK
    /* null slack to clear any data */
    if (!*dest)
        memset(dest, 0, (dmax - (dest - orig_dest)) * sizeof(wchar_t));
#endif

    return (EOK);
}
