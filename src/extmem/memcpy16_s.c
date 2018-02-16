/*------------------------------------------------------------------
 * memcpy16_s
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
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

/**
 * @def memcpy16_s(dest,dmax,src,slen)
 * @brief
 *    This function copies at most slen uint16_t's from src to dest, up to
 *    dmax bytes.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest   pointer to the memory that will be replaced by src.
 * @param[in]  dmax   maximum length of the resulting dest, in bytes
 * @param[in]  src    pointer to the memory that will be copied to dest
 * @param[in]  slen  number of uint16_t's to be copied
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   dmax shall not be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_MEM and sizeof(dest)
 * @pre   slen shall not be greater than dmax/2.
 * @pre   Copying shall not take place between regions that overlap.
 *
 * @return  If there is a runtime-constraint violation, memcpy16_s
 *          stores zeros in the first dmax bytes of the region pointed to
 *          by dest if dest and dmax are valid.
 * @retval  EOK         when operation is successful or slen = 0
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dmax = ZERO
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_MEM or > sizeof(dest)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESLEMAX     when slen > RSIZE_MAX_MEM16 or > sizeof(src)/2
 * @retval  ESNOSPC     when slen*2 > dmax
 * @retval  ESOVRLP     when src memory overlaps dest
 *
 * @see
 *    wmemcpy_s(), memcpy_s(), memcpy32_s(), wmemmove_s(), memmove16_s()
 *
 */
EXPORT errno_t
_memcpy16_s_chk (uint16_t *dest, rsize_t dmax,
                 const uint16_t *src, rsize_t slen,
                 const size_t destbos, const size_t srcbos)
{
    size_t smax; /* in bytes */

    if (unlikely(slen == 0)) { /* Since C11 slen=0 is allowed */
        return EOK;
    }

    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("memcpy16_s: dest is NULL",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("memcpy16_s: dmax is 0",
                   (void*)dest, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    smax = slen*2;
    if (destbos == BOS_UNKNOWN) {
        if (unlikely(dmax > RSIZE_MAX_MEM)) {
            invoke_safe_mem_constraint_handler("memcpy16_s: dmax exceeds max",
                   (void*)dest, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, smax);
    } else {
        if (unlikely(dmax > destbos)) {
            invoke_safe_mem_constraint_handler("memcpy16_s: dmax exceeds dest",
                   (void*)dest, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(dmax != destbos)) {
            handle_mem_bos_chk_warn("memcpy16_s", dest, dmax, destbos);
            RETURN_ESLEWRNG;
        }
#endif
        dmax = destbos;
    }

    if (unlikely(smax > dmax)) {
        errno_t rc = slen > RSIZE_MAX_MEM16 ? ESLEMAX : ESNOSPC;
        mem_prim_set(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy16_s: slen exceeds dmax/2",
                   (void*)dest, rc);
        return (RCNEGATE(rc));
    }

    if (unlikely(src == NULL)) {
        mem_prim_set(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy16_s: src is NULL",
                   (void*)dest, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, smax);
    } else {
        if (unlikely(smax > srcbos)) {
            invoke_safe_mem_constraint_handler("memcmp16_s: slen exceeds src",
                       (void*)src, ESLEMAX);
            return (RCNEGATE(ESLEMAX));
        }
    }

    /*
     * overlap is undefined behavior, do not allow
     */
    if (unlikely( ((dest > src) && (dest < (src+slen))) ||
                  ((src > dest) && (src < (dest+dmax/2))) )) {
        mem_prim_set(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy16_s: overlap undefined",
                   (void*)dest, ESOVRLP);
        return (RCNEGATE(ESOVRLP));
    }

    /*
     * now perform the copy
     */
    mem_prim_move16(dest, src, slen);

    return (RCNEGATE(EOK));
}
