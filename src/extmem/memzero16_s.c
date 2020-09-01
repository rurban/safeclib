/*------------------------------------------------------------------
 * memzero16_s - zeros memory
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
 * @def memzero16_s(dest,len)
 * @brief
 *    Zeros len uint16_t's starting at dest.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest  pointer to memory to be zeroed.
 * @param[in]  len   number of uint16_t's to be zeroed

 * @pre dest shall not be a null pointer.
 * @pre len shall not be 0 nor greater than RSIZE_MAX_MEM16 and sizeof(dest)/2.
 *
 * @return  If there is a runtime constraint, the operation is not performed.
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest is NULL POINTER
 * @retval  ESZEROL     when len = ZERO
 * @retval  ESLEMAX     when len > RSIZE_MAX_MEM16
 * @retval  EOVERFLOW   when 2*dmax > size of dest (optionally, when the
 compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when len != sizeof(dest)/2 and --enable-error-dmax
 *
 * @see
 *    memzero_s(), memzero32_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t memzero16_s(uint16_t *dest, rsize_t len)
#else
EXPORT errno_t _memzero16_s_chk(uint16_t *dest, rsize_t len,
                                const size_t destbos)
#endif
{
    rsize_t dmax = len * 2;
    CHK_DEST_MEM_NULL("memzero16_s")
    CHK_DMAX_MEM_ZERO("memzero16_s")

    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MEM_MAX("memzero16_s", RSIZE_MAX_MEM)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_MEM_OVR("memzero16_s", destbos);
    }

    /*
     * mem_prim_set16(dest, len, 0xDEAD);
     * mem_prim_set16(dest, len, 0xBEEF);
     */
    mem_prim_set16(dest, len, 0);
    MEMORY_BARRIER;

    return (RCNEGATE(EOK));
}
