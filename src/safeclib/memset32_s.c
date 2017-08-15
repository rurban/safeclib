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

#include "safeclib_private.h"
#include "safe_mem_constraint.h"
#include "mem_primitives_lib.h"
#include "safe_mem_lib.h"


/**
 * NAME
 *    memset32_s - Sets a block of memory to value
 *
 * SYNOPSIS
 *    #include "safe_mem_lib.h"
 *    errno_t
 *    memset32_s(uint32_t *dest, rsize_t smax, uint32_t value, rsize_t n)
 *
 * DESCRIPTION
 *    Sets len uint32_t starting at dest to the specified value.
 *
 * EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * INPUT PARAMETERS
 *    dest       pointer to memory that will be set to the value
 *
 *    smax       maximum number of bytes to be written
 *
 *    value      uint32_t value to be written
 *
 *    n          number of uint32_t values to be written
 *
 * OUTPUT PARAMETERS
 *    dest      is updated
 *
 * RUNTIME CONSTRAINTS
 *    dest shall not be a null pointer.
 *    smax shall not be 0 nor greater than RSIZE_MAX_MEM.
 *    n shall not be 0 nor greater than RSIZE_MAX_MEM32.
 *    smax*4 may not be smaller than n.
 *
 *    If there is a runtime-constraints violation, and if dest is not a null
 *    pointer, and if smax is not larger than RSIZE_MAX_MEM, then, before
 *    reporting the runtime-constraints violation, memset32_s() copies
 *    smax bytes to the destination.
 *
 * RETURN VALUE
 *    EOK        successful operation
 *    ESNULLP    NULL pointer
 *    ESZEROL    zero length
 *    ESLEMAX    length exceeds max limit
 *
 * ALSO SEE
 *    memset_s(), memset16_s()
 *
 */
errno_t
memset32_s(uint32_t *dest, rsize_t smax, uint32_t value, rsize_t n)
{
    errno_t err = EOK;

    if (dest == NULL) {
        invoke_safe_mem_constraint_handler("memset32_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (n == 0) {
        invoke_safe_mem_constraint_handler("memset32_s: n is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (smax > RSIZE_MAX_MEM) {
        invoke_safe_mem_constraint_handler("memset32_s: smax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (n > RSIZE_MAX_MEM16) {
        invoke_safe_mem_constraint_handler("memset32_s: n exceeds max",
                   NULL, ESLEMAX);
        err = ESLEMAX;
        n = smax/4;
    }

    if (n > smax/4) {
        invoke_safe_mem_constraint_handler("memset32_s: n exceeds smax/4",
                   NULL, ESNOSPC);
        err = ESNOSPC;
        n = smax/4;
    }

    mem_prim_set32(dest, n, value);

    return (RCNEGATE(err));
}
EXPORT_SYMBOL(memset32_s)
