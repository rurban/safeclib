/*------------------------------------------------------------------
 * wmemcmp_s.c - Compares wide char buffers
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2017 Reini Urban
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
#include "safe_mem_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def wmemcmp_s(dest,dlen,src,slen,diff)
 * @brief
 *    Compares buffers of wide chars until they differ, and return 0 if the
 * same or -1 or 1 in diff.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest   pointer to wchar_t buffer to compare against
 * @param  dlen   maximum length of dest, in number of wchar_t
 * @param  src    pointer to the source wchar_t buffer to compare with dest
 * @param  slen   length of the source memory block
 * @param  *diff  pointer to the diff which is -1, 0 or 1 according to
 *                whether the object pointed to by dest is
 *                greater than, equal to or less than the object
 *                pointed to by src.
 *                Note that musl returns the diff here.
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   Neither dlen nor slen shall be 0.
 * @pre   dlen shall not be greater than RSIZE_MAX_WMEM and size of dest.
 * @pre   slen shall not be greater than dlen and size of src.
 *
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dlen/slen = ZERO
 * @retval  ESLEMAX     when dlen/slen > RSIZE_MAX_WMEM
 * @retval  EOVERFLOW   when dlen/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG    when dlen != size of dest and --enable-error-dmax
 * @retval  ESNOSPC     when slen > dlen
 *
 * @see
 *    memcmp16_s(), memcmp32_s()
 *
 */

#ifdef FOR_DOXYGEN
errno_t wmemcmp_s(const wchar_t *dest, rsize_t dlen,
                  const wchar_t *src, rsize_t slen, int *diff)
#else
EXPORT errno_t _wmemcmp_s_chk(const wchar_t *dest, rsize_t dlen,
                              const wchar_t *src, rsize_t slen, int *diff,
                              const size_t destbos, const size_t srcbos)
#endif
{
    const rsize_t dmax = dlen * SIZEOF_WCHAR_T;
    const rsize_t smax = slen * SIZEOF_WCHAR_T;
    const wchar_t *dp;
    const wchar_t *sp;

    /* must be able to return the diff */
    if (unlikely(diff == NULL)) {
        invoke_safe_mem_constraint_handler("wmemcmp_s: diff is null", NULL,
                                           ESNULLP);
        return (RCNEGATE(ESNULLP));
    }
    *diff = -1; /* default diff */

    CHK_DEST_MEM_NULL("wmemcmp_s")
    if (unlikely(src == NULL)) {
        invoke_safe_mem_constraint_handler("wmemcmp_s: src is null",
                                           (void *)dest, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }
    CHK_DMAX_MEM_ZERO("wmemcmp_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MEM_MAX("wmemcmp_s", RSIZE_MAX_MEM)
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, smax);
    } else {
        CHK_DEST_MEM_OVR("wmemcmp_s", destbos)
    }
    if (unlikely(slen == 0)) {
        invoke_safe_mem_constraint_handler("wmemcmp_s: slen is 0", (void *)src,
                                           ESZEROL);
        return (RCNEGATE(ESZEROL));
    }
    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_WMEM)) {
            invoke_safe_mem_constraint_handler("wmemcmp_s: slen exceeds max",
                                               (void *)src, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
        BND_CHK_PTR_BOUNDS(src, smax);
    } else {
        if (unlikely(smax > srcbos)) {
            invoke_safe_mem_constraint_handler("wmemcmp_s: slen exceeds src",
                                               (void *)src, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
    }
    if (unlikely(slen > dlen)) {
        invoke_safe_mem_constraint_handler("wmemcmp_s: slen exceeds dlen",
                                           (void *)dest, ESNOSPC);
        return (RCNEGATE(ESNOSPC));
    }

    /* no need to compare the same memory */
    if (unlikely(dest == src)) {
        *diff = 0;
        return (RCNEGATE(EOK));
    }

    dp = (wchar_t *)dest;
    sp = (wchar_t *)src;

    /* now compare sp to dp */
    *diff = 0;
    while (dlen > 0 && slen > 0) {
        if (*dp != *sp) {
            /***  *diff = *dp - *sp; ***/
            *diff = *dp < *sp ? -1 : 1;
            break;
        }

        dlen--;
        slen--;

        dp++;
        sp++;
    }

    return (RCNEGATE(EOK));
}
