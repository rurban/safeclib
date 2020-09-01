/*------------------------------------------------------------------
 * wcscmp_s.c -- string compare
 *
 * September 2017, Reini Urban
 *
 * Copyright (c) 2017,2018 by Reini Urban
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
 * @def wcscmp_s(dest,dmax,src,smax,resultp)
 * @brief
 *    Compares wide string src to wide string dest.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest       wide string to compare against
 * @param[in]   dmax       restricted maximum length of wide string dest
 * @param[in]   src        wide string to be compared to dest
 * @param[in]   smax       restricted maximum length of wide string src
 * @param[out]  resultp    pointer to result diff, greater than 0,
 *                         equal to 0 or less than 0, if the dest is greater
 *                         than, equal to or less than src respectively.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   resultp shall not be a null pointer.
 * @pre   dmax/smax shall not be 0
 * @pre   dmax/smax shall not be greater than RSIZE_MAX_WSTR and size of
 * dest/src
 *
 * @return  *resultp, when the return code is OK:
 *            >0 when dest greater than src
 *             0 when wide strings the same
 *            <0 when dest less than src
 *
 * @retval  EOK        when comparison is complete
 * @retval  ESNULLP    when dest/src/resultp is NULL pointer
 * @retval  ESZEROL    when dmax/smax = 0
 * @retval  ESLEMAX    when dmax/smax > RSIZE_MAX_WSTR
 * @retval  EOVERFLOW  when dmax/smax > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 *
 * @see
 *    strcmp_s(), wcsncmp_s(), wcsicmp_s()
 */

#ifdef FOR_DOXYGEN
errno_t wcscmp_s(const wchar_t *restrict dest, rsize_t dmax,
                 const wchar_t *restrict src, rsize_t smax,
                 int *resultp)
#else
EXPORT errno_t _wcscmp_s_chk(const wchar_t *restrict dest, rsize_t dmax,
                             const wchar_t *restrict src, rsize_t smax,
                             int *resultp, const size_t destbos,
                             const size_t srcbos)
#endif
{
    const size_t destsz = dmax * sizeof(wchar_t);
    const size_t srcsz = smax * sizeof(wchar_t);

    CHK_SRC_NULL("wcscmp_s", resultp)
    *resultp = 0;
    CHK_DEST_NULL("wcscmp_s")
    CHK_SRC_NULL("wcscmp_s", src)
    if (unlikely(dmax == 0 || smax == 0)) {
        invoke_safe_str_constraint_handler("wcscmp_s: dmax/smax is 0",
                                           (void *)dest, ESZEROL);
        return RCNEGATE(ESZEROL);
    }
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("wcscmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, destsz);
    } else {
        CHK_DESTW_OVR("wcscmp_s", destsz, destbos)
    }
    if (unlikely(smax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcscmp_s"
                                           ": smax exceeds max",
                                           (void *)src, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, srcsz);
    } else {
        if (unlikely(srcsz > srcbos)) {
            invoke_safe_str_constraint_handler("wcscmp_s"
                                               ": smax exceeds src",
                                               (void *)src, EOVERFLOW);
            return RCNEGATE(EOVERFLOW);
        }
    }

    while (*dest && *src && dmax && smax) {

        if (*dest != *src) {
            break;
        }

        dest++;
        src++;
        dmax--;
        smax--;
    }

    *resultp = *dest - *src;
    return RCNEGATE(EOK);
}
