/*------------------------------------------------------------------
 * memset_s
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

#include "safeclib_private.h"
#include "safe_mem_constraint.h"
#include "mem_primitives_lib.h"
#include "safe_mem_lib.h"


/**
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
 * @pre  dmax and n shall not be greater than RSIZE_MAX_MEM.
 * @pre  dmax may not be smaller than n.
 * @pre  Without C11 dmax and n shall not be 0
 *
 * @note The behavior is undefined if the size of the character
 * array pointed to by dest < count <= dmax; in other words, an
 * erroneous value of dmax does not expose the impending buffer
 * overflow.
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_MEM.
 * @note C11 returns 0 when n = ZERO.
 *
 * @return  If there is a runtime-constraints violation, and if dest is not a null
 *          pointer, and if dmax is not larger than RSIZE_MAX_MEM, then, before
 *          reporting the runtime-constraints violation, memset_s() copies
 *          dmax bytes to the destination. 
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest is NULL POINTER (EINVAL with C11)
 * @retval  ESZEROL     when n = ZERO (unless C11)
 * @retval  ESLEMAX     when dmax/n > RSIZE_MAX_MEM
 * @retval  ESNOSPC     when dmax < n
 *
 * @see 
 *    memset16_s(), memset32_s()
 *
 */
#if !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
errno_t
memset_s (void *dest, rsize_t dmax, uint8_t value, rsize_t n)
{
    errno_t err = EOK;

    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("memset_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    /* different on C11! */
    if (unlikely(n == 0)) {
        invoke_safe_mem_constraint_handler("memset_s: n is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(dmax > RSIZE_MAX_MEM)) {
        invoke_safe_mem_constraint_handler("memset_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(n > RSIZE_MAX_MEM)) {
        invoke_safe_mem_constraint_handler("memset_s: n exceeds max",
                   NULL, ESLEMAX);
        err = ESLEMAX;
        n = dmax;
    }

    if (unlikely(n > dmax)) {
        invoke_safe_mem_constraint_handler("memset_s: n exceeds dmax",
                   NULL, ESNOSPC);
        err = ESNOSPC;
        n = dmax;
    }

    mem_prim_set(dest, n, value);

    return (RCNEGATE(err));
}
EXPORT_SYMBOL(memset_s)
#endif
