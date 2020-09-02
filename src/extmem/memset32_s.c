/*------------------------------------------------------------------
 * memset32_s
 *
 * October 2008, Bo Berry
 * Copyright (c) 2017 Reini Urban
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
 * @def memset32_s(dest,dmax,value,n)
 * @brief
 *    Sets n uint32_t values starting at dest to the specified value,
 *    but maximal dmax bytes.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest   pointer to memory that will be set to the value
 * @param[in]   dmax   maximum number of bytes to be written
 * @param[in]   value  byte value to be written
 * @param[in]   n      number of 4-byte words to be set
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall not be 0
 * @pre  dmax shall not be greater than RSIZE_MAX_MEM or size of dest.
 * @pre  n shall not be greater than RSIZE_MAX_MEM32.
 * @pre  4*n may not be greater than dmax.
 *
 * @return  If there is a runtime-constraints violation, and if dest is
 *          not a null pointer, and if dmax is not larger than
 *          RSIZE_MAX_MEM, then, before reporting the
 *          runtime-constraints violation, memset32_s() copies dmax
 *          bytes to the destination.
 * @retval  EOK         when operation is successful or n = 0
 * @retval  ESNULLP     when dest is NULL POINTER
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_MEM or n > RSIZE_MAX_MEM32
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOSPC     when 4*n > dmax
 *
 * @see
 *    memset_s(), memset16_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t memset32_s(uint32_t *dest, rsize_t dmax, uint32_t value, rsize_t n)
#else
EXPORT errno_t _memset32_s_chk(uint32_t *dest, rsize_t dmax, uint32_t value,
                               rsize_t n, const size_t destbos)
#endif
{
    errno_t err;

    CHK_DEST_MEM_NULL("memset32_s")
    if (unlikely(n == 0)) { /* Since C11 n=0 is allowed */
        return EOK;
    }

    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MEM_MAX("memset32_s", RSIZE_MAX_MEM)
        BND_CHK_PTR_BOUNDS(dest, n);
    } else {
        CHK_DEST_MEM_OVR("memset32_s", destbos)
        dmax = destbos;
    }

    err = EOK;

    if (unlikely(n > dmax / 4)) {
        err = n > RSIZE_MAX_MEM32 ? ESLEMAX : ESNOSPC;
        invoke_safe_mem_constraint_handler("memset32_s: n exceeds dmax/4", dest,
                                           err);
        n = dmax / 4;
    }

    mem_prim_set32(dest, n, value);
    MEMORY_BARRIER;

    return (RCNEGATE(err));
}
