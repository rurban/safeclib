/*------------------------------------------------------------------
 * memzero_s - zeros memory
 *
 * October 2008, Bo Berry
 * December 2018, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2018 Reini Urban
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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifdef FOR_DOXYGEN
#include "safe_mem_lib.h"
#else
#include "safeclib_private.h"
#include "mem/mem_primitives_lib.h"
#endif

/**
 * @def memzero_s(dest,len)
 * @brief
 *    Zeros len bytes starting at dest.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest  pointer to memory to be zeroed.
 * @param[in]  len   number of bytes to be zeroed
 *
 * @pre   dest shall not be a null pointer.
 * @pre   len shall not be 0 nor greater than RSIZE_MAX_MEM and size of dest
 *
 * @note memzero_s provides a memory barrier for modern out-of-order CPU's
 *       to ensure a cache flush or at least a compiler barrier fallback to
 *       ensure that is not optimized away by optimizing compilers.
 *
 * @return  If there is a runtime constraint, the operation is not performed.
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest is NULL POINTER
 * @retval  ESZEROL     when len = ZERO
 * @retval  ESLEMAX     when len > RSIZE_MAX_MEM
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when len != size of dest and --enable-error-dmax
 *
 * @see
 *    memzero16_s(), memzero32_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t memzero_s(void *dest, rsize_t len)
#else
EXPORT errno_t _memzero_s_chk(void *dest, rsize_t len, const size_t destbos)
#endif
{
    rsize_t dmax = len;
    CHK_DEST_MEM_NULL("memzero_s")
    CHK_DMAX_MEM_ZERO("memzero_s")

    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MEM_MAX("memzero_s", RSIZE_MAX_MEM)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_MEM_OVR("memzero_s", destbos);
    }

    /*
     * mem_prim_set(dest, len, 0xA5);
     * mem_prim_set(dest, len, 0x5A);
     */
#ifdef HAVE_EXPLICIT_BZERO
    explicit_bzero(dest, len);
#else
    mem_prim_set(dest, len, 0);
#endif
    MEMORY_BARRIER;

    return (RCNEGATE(EOK));
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_memzero_s_chk);
#endif
