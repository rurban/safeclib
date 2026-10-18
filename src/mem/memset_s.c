/*------------------------------------------------------------------
 * memset_s
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
 * January 2018, Reini Urban
 * December 2018, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2017-2018 Reini Urban
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

#if defined(TEST_MSVCRT) && defined(HAVE_MEMSET_S)
#else

/**
 * @def memset_s(dest,dmax,value,n)
 * @brief
 *    Sets the first n bytes starting at dest to the specified value,
 *    but maximal dmax bytes.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.4.1 The memset_s function (p: 621-622)
 *    http://en.cppreference.com/w/c/string/byte/memset
 *    * ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest   pointer to memory that will be set to the value
 * @param[in]   dmax   maximum number of bytes to be written
 * @param[in]   value  byte value to be written
 * @param[in]   n      number of bytes to be set
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax and n shall not be greater than RSIZE_MAX_MEM or sizeof(dest).
 * @pre  value shall not be greater than 255.
 * @pre  dmax may not be smaller than n.
 *
 * @note The behavior is undefined if the size of the character
 *       array pointed to by dest < count <= dmax; in other words, an
 *       erroneous value of dmax does not expose the impending buffer
 *       overflow.
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_MEM.
 * @note memset_s provides a memory barrier for modern out-of-order CPU's,
 *       or at least a compiler barrier fallback, to ensure that the stores
 *       are ordered before later loads and are not optimized away by
 *       optimizing compilers.
 * @note The memory barrier is not a cache flush. CPU caches are coherent, so
 *       after memset_s no core sees the old contents in its caches, and a
 *       flush would not help against Meltdown, MDS, L1TF or Spectre style
 *       attacks, which leak data that was visible before the overwrite
 *       (mitigated by microcode, VERW and kernel changes). Flush+Reload
 *       leaks only which lines were accessed, not their contents.
 * @note Cold boot attacks (and hibernation or crash dumps, or DMA by
 *       non-coherent devices) are different. The new contents may still be
 *       dirty in the cache while the old secret is still in DRAM, until the
 *       line is written back. To force the write-back of the range, use
 *       clflushopt or clflush followed by sfence on x86, clwb for persistent
 *       memory, dc cvac on ARM64. memset_s does not do this, as it is
 *       expensive. Use full memory encryption or avoid keeping secrets in
 *       memory for long when cold boot attacks are in your threat model.
 *
 * @return  If there is a runtime-constraints violation, and if dest is not a
 * null pointer, and if dmax is not too large, then, before reporting the
 * runtime-constraints violation, memset_s() copies dmax bytes to the
 * destination.
 * @retval  EOK         when operation is successful or n = 0
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESLEMAX     when dmax/n > RSIZE_MAX_MEM or value > 255
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESNOSPC     when dmax < n
 *
 * @see
 *    memset16_s(), memset32_s()
 */

#ifdef FOR_DOXYGEN
errno_t memset_s(void *dest, rsize_t dmax, int value, rsize_t n)
#else
EXPORT errno_t _memset_s_chk(void *dest, rsize_t dmax, int value, rsize_t n,
                             const size_t destbos)
#endif
{
    errno_t err;

    CHK_DEST_MEM_NULL("memset_s")
    if (unlikely(n == 0)) { /* since C11 n=0 is allowed */
        return EOK;
    }
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MEM_MAX("memset_s", RSIZE_MAX_MEM)
        BND_CHK_PTR_BOUNDS(dest, n);
    } else {
        CHK_DEST_MEM_OVR("memset_s", destbos)
        dmax = destbos;
    }

    if (unlikely(value > 255)) {
        invoke_safe_mem_constraint_handler("memset_s: value exceeds max", dest,
                                           ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    err = EOK;

    if (unlikely(n > dmax)) {
        err = n > RSIZE_MAX_MEM ? ESLEMAX : ESNOSPC;
        invoke_safe_mem_constraint_handler("memset_s: n exceeds dmax", dest,
                                           err);
        n = dmax;
    }

    mem_prim_set(dest, n, (uint8_t)value);
    MEMORY_FLUSH(dest, n);
    MEMORY_BARRIER;

    return (RCNEGATE(err));
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_memset_s_chk);
#endif

#ifndef FOR_DOXYGEN
/* All constraints are already proven at compile-time. GH #48 */
EXPORT errno_t _memset_s_uchk(void *dest, int value, rsize_t n) {
    mem_prim_set(dest, n, (uint8_t)value);
    MEMORY_FLUSH(dest, n);
    MEMORY_BARRIER;
    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_memset_s_uchk);
#endif
#endif

#endif /* TEST_MSVCRT */
