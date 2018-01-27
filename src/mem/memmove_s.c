/*------------------------------------------------------------------
 * memmove_s.c
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2017 Reini Urban
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

#if defined(TEST_MSVCRT) && defined(HAVE_MEMMOVE_S)
#else

/**
 * @brief
 *    The memmove_s function copies count bytes from the region pointed to by
 *    src into the region pointed to by dest.
 * @details
 *    This copying takes place as if the count bytes from the region pointed
 *    to by src are first copied into a temporary array of count bytes that does
 *    not overlap the region pointed to by dest or src, and then the count
 *    bytes from the temporary array are copied into the object region to by
 *    dest.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.1.2 The memmove_s function (p: 615)
 *    http://en.cppreference.com/w/c/string/byte/memmove
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest  pointer to the memory that will be replaced by src.
 * @param[in]  dmax  maximum length of the resulting dest, in bytes
 * @param[in]  src   pointer to the memory that will be copied to dest
 * @param[in]  count  maximum number bytes of src that can be copied
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_MEM.
 * @pre  count shall not be greater than dmax.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_MEM.
 *
 * @return  If there is a runtime-constraint violation, the memmove_s function
 *          stores zeros in the first dmax characters of the region pointed to
 *          by dest if dest is not a null pointer and dmax is not greater
 *          than RSIZE_MAX_MEM.
 * @retval  EOK         when operation is successful or count = 0
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax/count > RSIZE_MAX_MEM
 * @retval  ESNOSPC     when dmax < count
 *
 * @see
 *    memmove16_s(), memmove32_s(), memcpy_s(), memcpy16_s() memcpy32_s()
 *
 */

EXPORT errno_t
memmove_s (void *dest, rsize_t dmax, const void *src, rsize_t count)
{
    uint8_t *dp;
    const uint8_t  *sp;

    dp = (uint8_t*) dest;
    sp = (uint8_t*) src;

    /* Note that MSVC checks this at very first. We do also now */
    if (unlikely(count == 0)) {
        /* Since C11 n=0 is allowed */
        return EOK;
    }

    if (unlikely(dp == NULL)) {
        invoke_safe_mem_constraint_handler("memmove_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("memmove_s: dmax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(dmax > RSIZE_MAX_MEM)) {
        invoke_safe_mem_constraint_handler("memmove_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(sp == NULL)) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memmove_s: src is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(count > dmax)) {
        errno_t rc = count > RSIZE_MAX_MEM ? ESLEMAX : ESNOSPC;
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memmove_s: count exceeds max",
                   NULL, rc);
        return (RCNEGATE(rc));
    }

    /*
     * now perform the copy, with overlap allowed
     */
    mem_prim_move(dp, sp, count);

    return (RCNEGATE(EOK));
}
#ifdef __KERNEL__
EXPORT_SYMBOL(memmove_s);
#endif /* __KERNEL__ */

#endif
