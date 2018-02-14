/*------------------------------------------------------------------
 * memcpy_s
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
 * January 2018, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2017,2018 Reini Urban
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

#if defined(TEST_MSVCRT) && defined(HAVE_MEMCPY_S)
#else

/**
 * @def memcpy_s(dest,dmax,src,slen)
 * @brief
 *    This function copies at most slen bytes from src to dest, up to
 *    dmax resp. sizeof(dest).
 *    If slen is zero, the function does nothing.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.1.1 The memcpy_s function (p: 614)
 *    http://en.cppreference.com/w/c/string/byte/memcpy
 *    * ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest   pointer to the memory that will be replaced by src.
 * @param[in]  dmax   maximum length of the resulting dest, in bytes
 * @param[in]  src    pointer to the memory that will be copied to dest
 * @param[in]  slen   maximum number bytes of src that can be copied
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_MEM or sizeof(dest).
 * @pre  slen shall not be greater than dmax.
 * @pre  Copying shall not take place between regions that overlap.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_MEM.
 *
 * @return  If there is a runtime-constraint violation, the memcpy_s function
 *          stores zeros in the first dmax bytes of the region pointed to
 *          by dest if dest is not a null pointer and slen is valid.
 * @retval  EOK         when operation is successful or slen = 0
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax/slen > RSIZE_MAX_MEM or sizeof(dest)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESNOSPC     when dmax < slen
 * @retval  ESOVRLP     when src memory overlaps dst
 *
 * @see
 *    memcpy16_s(), memcpy32_s(), memmove_s(), memmove16_s(), memmove32_s()
 *
 */
EXPORT errno_t
_memcpy_s_chk (void * restrict dest, rsize_t dmax, const void * restrict src,
               rsize_t slen, const size_t destbos)
{
    uint8_t *dp;
    const uint8_t  *sp;

    /* Note that MSVC checks this at very first. We do also now */
    if (unlikely(slen == 0)) { /* Since C11 slen=0 is allowed */
        return EOK;
    }

    dp = (uint8_t*) dest;
    sp = (uint8_t*) src;

    if (unlikely(dp == NULL)) {
        invoke_safe_mem_constraint_handler("memcpy_s: dest is NULL",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("memcpy_s: dmax is 0",
                   dest, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (destbos == BOS_UNKNOWN) {
        if (unlikely(dmax > RSIZE_MAX_MEM)) {
            invoke_safe_mem_constraint_handler("memcpy_s: dmax exceeds max",
                       dest, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
    } else {
        if (unlikely(dmax > destbos)) {
            invoke_safe_mem_constraint_handler("memcpy_s: dmax exceeds dest",
                       dest, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(dmax != destbos)) {
            handle_mem_bos_chk_warn("memcpy_s", dest, dmax, destbos);
# ifdef HAVE_ERROR_DMAX
            return (RCNEGATE(ESLEWRNG));
# endif
        }
#endif
        /* Note: unlike to memset_s, we don't set dmax to destbos */
    }

    if (unlikely(sp == NULL)) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy_s: src is NULL",
                   dest, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(slen > dmax)) {
        errno_t rc = slen > RSIZE_MAX_MEM ? ESLEMAX : ESNOSPC;
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy_s: slen exceeds max",
                   dest, rc);
        return RCNEGATE(rc);
    }
#ifdef  HAVE___BND_CHK_PTR_BOUNDS
    __bnd_chk_ptr_bounds(dest, slen);
    __bnd_chk_ptr_bounds(src, slen);
#endif

    /* overlap is disallowed */
    if (unlikely( ((dp > sp) && (dp < (sp+slen))) ||
                  ((sp > dp) && (sp < (dp+dmax))) )) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy_s: overlap undefined",
                   dest, ESOVRLP);
        return RCNEGATE(ESOVRLP);
    }

    /*
     * now perform the copy
     */
    mem_prim_move(dp, sp, slen);

    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_memcpy_s_chk);
#endif

#endif
