/*------------------------------------------------------------------
 * memzero_s - zeros memory
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
#include "mem_primitives_lib.h"
#endif

/**
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
 * @pre   len shall not be 0 nor greater than RSIZE_MAX_MEM.
 *
 * @return  If there is a runtime constraint, the operation is not performed.
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest is NULL POINTER
 * @retval  ESZEROL     when len = ZERO
 * @retval  ESLEMAX     when len > RSIZE_MAX_MEM
 *
 * @see 
 *    memzero16_s(), memzero32_s()
 *
 */
EXPORT errno_t
memzero_s (void *dest, rsize_t len)
{
    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("memzero_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(len == 0)) {
        invoke_safe_mem_constraint_handler("memzero_s: len is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(len > RSIZE_MAX_MEM)) {
        invoke_safe_mem_constraint_handler("memzero_s: len exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    /*
     * mem_prim_set(dest, len, 0xA5);
     * mem_prim_set(dest, len, 0x5A);
     */
    mem_prim_set(dest, len, 0);

    return (RCNEGATE(EOK));
}
EXPORT_SYMBOL(memzero_s)
