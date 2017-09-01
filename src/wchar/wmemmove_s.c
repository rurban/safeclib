/*------------------------------------------------------------------
 * wmemmove_s.c
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
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

#include "config.h"
#ifndef SAFECLIB_DISABLE_WCHAR

#include "safeclib_private.h"
#include "safe_mem_constraint.h"
#include "mem_primitives_lib.h"
#include "safe_mem_lib.h"

#if SIZEOF_WCHAR_T == 2
#define wmem_type uint16_t
#define wmem_set  mem_prim_set16
#define wmem_move mem_prim_move16
#elif SIZEOF_WCHAR_T == 4
#define wmem_type uint32_t
#define wmem_set  mem_prim_set32
#define wmem_move mem_prim_move32
#else
#error sizeof(wchar_t)?
#endif

/**
 * @brief
 *    The wmemmove_s function copies smax wchar_t's from the region
 *    pointed to by src into the region pointed to by dest. 
 * @details
 *    This copying takes place as if the smax wchar_ts from the region
 *    pointed to by src are ﬁrst copied into a temporary array of
 *    smax wchar_ts that does not overlap the regions pointed to
 *    by dest or src, and then the smax wchar_ts from the temporary
 *    array are copied into the region pointed to by dest.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest pointer to the memory that will be replaced by src.
 * @param[in]  dmax   maximum length of the resulting dest, in bytes
 * @param[in]  src    pointer to the memory that will be copied to dest
 * @param[in]  smax   maximum number bytes of src that can be copied
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   Neither dmax nor smax shall be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_MEM.
 * @pre   smax shall not be greater than dmax.
 *
 * @return  If there is a runtime-constraint violation, the memmove_s function
 *          stores zeros in the ﬁrst dmax characters of the region pointed to
 *          by dest if dest is not a null pointer and dmax is not greater
 *          than RSIZE_MAX_MEM.
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dst/src is NULL POINTER
 * @retval  ESZEROL     when dmax/smax = ZERO
 * @retval  ESLEMAX     when dmax/smax > RSIZE_MAX_MEM
 * @retval  ESNOSPC     when dmax < smax
 *
 * @see 
 *    memmove_s(), memmove16_s(), memcpy_s(), memcpy16_s() memcpy32_s()
 *
 */
errno_t
wmemmove_s (wchar_t *dest, rsize_t dmax, const wchar_t *src, rsize_t smax)
{
    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("wmemmove_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("wmemmove_s: dest is zero",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(dmax > RSIZE_MAX_MEM32 || smax > RSIZE_MAX_MEM32)) {
        invoke_safe_mem_constraint_handler("wmemmove_s: dmax/smax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(smax == 0)) {
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemmove_s: smax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(smax > dmax)) {
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemmove_s: smax exceeds dmax",
                   NULL, ESNOSPC);
        return (RCNEGATE(ESNOSPC));
    }

    if (unlikely(src == NULL)) {
        wmem_set((wmem_type*)dest, (uint32_t)dmax, 0);
        invoke_safe_mem_constraint_handler("wmemmove_s: src is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    /*
     * now perform the copy
     */
    wmem_move((wmem_type*)dest, (wmem_type*)src, (uint32_t)smax);

    return (RCNEGATE(EOK));
}
EXPORT_SYMBOL(wmemmove_s)

#endif /* SAFECLIB_DISABLE_WCHAR */
