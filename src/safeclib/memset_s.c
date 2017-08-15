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
 * NAME
 *    memset_s
 *
 * SYNOPSIS
 *    #include "safe_mem_lib.h"
 *    errno_t
 *    memset_s(void *dest, rsize_t smax, uint8_t value, rsize_t n)
 *
 * DESCRIPTION
 *    Sets the first n bytes starting at dest to the specified value,
 *    but maximal smax bytes.
 *
 * SPECIFIED IN
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * INPUT PARAMETERS
 *    dest       pointer to memory that will be set to the value
 *
 *    smax       maximum number of bytes to be written
 *
 *    value      byte value to be written
 *
 *    n          number of bytes to be set
 *
 * OUTPUT PARAMETERS
 *    dest      is updated
 *
 * RUNTIME CONSTRAINTS
 *    dest shall not be a null pointer.
 *    smax and n shall not be 0 nor greater than RSIZE_MAX_MEM.
 *    smax may not be smaller than n.

 *    If there is a runtime-constraints violation, and if dest is not a null
 *    pointer, and if smax is not larger than RSIZE_MAX_MEM, then, before
 *    reporting the runtime-constraints violation, memset_s() copies
 *    smax bytes to the destination.
 *
 * RETURN VALUE
 *    EOK        successful operation
 *    ESNULLP    NULL pointer
 *    ESZEROL    zero length
 *    ESLEMAX    length exceeds max limit
 *
 * ALSO SEE
 *    memset16_s(), memset32_s()
 *
 */
#if !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
errno_t
memset_s (void *dest, rsize_t smax, uint8_t value, rsize_t n)
{
    errno_t err = EOK;

    if (dest == NULL) {
        invoke_safe_mem_constraint_handler("memset_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (n == 0) {
        invoke_safe_mem_constraint_handler("memset_s: n is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (smax > RSIZE_MAX_MEM) {
        invoke_safe_mem_constraint_handler("memset_s: smax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (n > RSIZE_MAX_MEM) {
        invoke_safe_mem_constraint_handler("memset_s: n exceeds max",
                   NULL, ESLEMAX);
        err = ESLEMAX;
        n = smax;
    }

    if (n > smax) {
        invoke_safe_mem_constraint_handler("memset_s: n exceeds smax",
                   NULL, ESNOSPC);
        err = ESNOSPC;
        n = smax;
    }

    mem_prim_set(dest, n, value);

    return (RCNEGATE(err));
}
EXPORT_SYMBOL(memset_s)
#endif
