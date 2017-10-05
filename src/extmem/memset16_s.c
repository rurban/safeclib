/*------------------------------------------------------------------
 * memset16_s
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
#include "mem_primitives_lib.h"
#endif

/**
 * @brief
 *    Sets the first n uint16_t values starting at dest to the specified value,
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
 * @param[in]   n      number of short (2-byte) words to be set
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall not be 0
 * @pre  dmax shall not be greater than RSIZE_MAX_MEM.
 * @pre  n shall not be greater than RSIZE_MAX_MEM16.
 * @pre  dmax*2 may not be smaller than n.
 *
 * @return  If there is a runtime-constraints violation, and if dest is not a null
 *          pointer, and if dmax is not larger than RSIZE_MAX_MEM, then, before
 *          reporting the runtime-constraints violation, memset16_s() copies
 *          dmax bytes to the destination.
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest is NULL POINTER
 * @retval  ESZEROL     Only before C11 when n = ZERO
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_MEM
 * @retval  ESLEMAX     when n > RSIZE_MAX_MEM16
 * @retval  ESNOSPC     when dmax/2 < n
 *
 * @see 
 *    memset_s(), memset32_s()
 *
 */
EXPORT errno_t
memset16_s(uint16_t *dest, rsize_t dmax, uint16_t value, rsize_t n)
{
    errno_t err = EOK;

    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("memset16_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(n == 0)) {
        /* Since C11 n=0 is allowed */
#ifdef HAVE_C11
        return EOK;
#else
        invoke_safe_mem_constraint_handler("memset16_s: n is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
#endif
    }

    if (unlikely(dmax > RSIZE_MAX_MEM)) {
        invoke_safe_mem_constraint_handler("memset16_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(n > RSIZE_MAX_MEM16)) {
        invoke_safe_mem_constraint_handler("memset16_s: n exceeds max",
                   NULL, ESLEMAX);
        err = ESLEMAX;
        n = dmax/2;
    }

    if (unlikely(n > dmax/2)) {
        invoke_safe_mem_constraint_handler("memset16_s: n exceeds dmax/2",
                   NULL, ESNOSPC);
        err = ESNOSPC;
        n = dmax/2;
    }

    mem_prim_set16(dest, n, value);

    return (RCNEGATE(err));
}
EXPORT_SYMBOL(memset16_s)
