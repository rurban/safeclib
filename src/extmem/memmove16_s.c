/*------------------------------------------------------------------
 * memmove16_s.c
 *
 * October 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 Cisco Systems
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
#include "mem/mem_primitives_lib.h"
#endif

/**
 * @def memmove16_s(dest,dmax,src,slen)
 * @brief
 *    The memmove16_s function copies slen uint16_t from the region
 *    pointed to by src into the region pointed to by dest.
 * @details
 *    This copying takes place as if the slen uint16_t from the region
 *    pointed to by src are first copied into a temporary array of
 *    slen uint16_t that does not overlap the regions pointed to
 *    by dest or src, and then the slen uint16_t from the temporary
 *    array are copied into the region pointed to by dest.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest   pointer to the memory that will be replaced by src.
 * @param[in]  dmax   maximum length of the resulting dest, in bytes
 * @param[in]  src    pointer to the memory that will be copied to dest
 * @param[in]  slen   number of uint16_t's to be copied
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   dmax shall not be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_MEM or size of dest.
 * @pre   2*slen shall not be greater than dmax and size of src.
 *
 * @return  If there is a runtime-constraint violation, memmove16_s
 *          stores zeros in the first dmax bytes of the region pointed to
 *          by dest if dest and dmax are valid.
 * @retval  EOK         when operation is successful or slen = 0
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dmax = ZERO
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_MEM or slen > RSIZE_MAX_MEM16
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the
 *                      compiler knows the object_size statically).
 *                      Or when 2*slen > size of src (optionally, when the
 *                      compiler knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESNOSPC     when 2*slen > dmax
 *
 * @see
 *    memmove_s(), memmove32_s(), memcpy_s(), memcpy16_s() memcpy32_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t memmove16_s(uint16_t *dest, rsize_t dmax, const uint16_t *src, rsize_t slen)
#else
EXPORT errno_t _memmove16_s_chk(uint16_t *dest, rsize_t dmax,
                                const uint16_t *src, rsize_t slen,
                                const size_t destbos, const size_t srcbos)
#endif
{
    size_t smax; /* in bytes */

    if (unlikely(slen == 0)) { /* Since C11 slen=0 is allowed */
        return EOK;
    }

    CHK_DEST_MEM_NULL("memmove16_s")
    CHK_DMAX_MEM_ZERO("memmove16_s")
    smax = slen * 2;
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MEM_MAX("memmove16_s", RSIZE_MAX_MEM)
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, smax);
    } else {
        CHK_DEST_MEM_OVR("memmove16_s", destbos)
        dmax = destbos;
    }
    CHK_SRC_MEM_NULL_CLEAR("memmove16_s", src)
    CHK_SLEN_MEM_MAX_NOSPC_CLEAR("memmove16_s", smax, RSIZE_MAX_MEM)

    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, smax);
    } else {
        if (unlikely(smax > srcbos)) {
            invoke_safe_mem_constraint_handler("memmove16_s: slen exceeds src",
                                               (void *)src, EOVERFLOW);
            return (RCNEGATE(EOVERFLOW));
        }
    }

    /*
     * now perform the copy
     */
    mem_prim_move16(dest, src, slen);

    return (RCNEGATE(EOK));
}
