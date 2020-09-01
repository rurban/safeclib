/*------------------------------------------------------------------
 * memccpy_s
 *
 * September 2017, Reini Urban
 * January 2018, Reini Urban
 *
 * Copyright (c) 2017, 2018 by Reini Urban
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
 * @def memccpy_s(dest,dmax,src,c,n)
 * @brief
 *    \c memccpy_s copies no more than n bytes from memory area src to memory
 *    area dest, stopping when the character c is found. Throws error if the
 *    memory areas overlap.
 *    With SAFECLIB_STR_NULL_SLACK defined the rest (max. n bytes, not dmax)
 *    is cleared with NULL bytes.
 *
 * @param[out] dest  pointer to the memory that will be replaced by src.
 * @param[in]  dmax  maximum length of the resulting dest, in bytes
 * @param[in]  src   pointer to the memory that will be copied to dest
 * @param[in]  c     character to be found
 * @param[in]  n     maximum number bytes of src that can be copied
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dmax shall be sizeof(dest)
 * @pre  dmax shall not be 0, not be greater than RSIZE_MAX_MEM and
 * sizeof(dest)
 * @pre  n shall not be greater than dmax.
 * @pre  Copying shall not take place between regions that overlap.
 *
 * @note memccpy from BSD, Windows sec_api, glibc, newlib and everywhere else.
 *
 * @return  The memccpy() function returns a pointer to the next character in
 *          dest after c, or NULL if c was not found in the first n characters
 * of src. If there is a runtime-constraint violation, the memccpy_s function
 *          stores zeros in the first dmax bytes of the region pointed to
 *          by dest if dest is not a null pointer and n is valid.
 *          With n=0, dest[0] is set to '\0', as with \c memccpy().
 * @retval  EOK         when operation is successful or n = 0
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dmax = ZERO.
 * @retval  ESLEMAX     when dmax/n > RSIZE_MAX_MEM
 * @retval  EOVERFLOW   when dmax/n > size of dest (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESNOSPC     when n > dmax
 * @retval  ESOVRLP     when src memory overlaps dst
 *
 * @see
 *    memcpy_s(), strncpy_s()
 */
#ifdef FOR_DOXYGEN
errno_t memccpy_s(void *restrict dest, rsize_t dmax,
                  const void *restrict src, int c, rsize_t n)
#else
EXPORT errno_t _memccpy_s_chk(void *restrict dest, rsize_t dmax,
                              const void *restrict src, int c, rsize_t n,
                              const size_t destbos, const size_t srcbos)
#endif
{
    uint8_t *dp;
    const uint8_t *sp;
    rsize_t orig_dmax = dmax;

    dp = (uint8_t *)dest;
    sp = (uint8_t *)src;

    CHK_DEST_MEM_NULL("memccpy_s")
    CHK_DMAX_MEM_ZERO("memccpy_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MEM_MAX("memccpy_s", RSIZE_MAX_MEM)
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, n);
    } else {
        CHK_DEST_MEM_OVR("memccpy_s", destbos)
    }
    if (unlikely(n == 0)) { /* Since C11 n=0 is allowed */
        *dp = '\0';
        return EOK;
    }
    CHK_SRC_MEM_NULL_CLEAR("memccpy_s", src)
    CHK_SLEN_MEM_MAX_NOSPC_CLEAR("memccpy_s", n, RSIZE_MAX_MEM)

    /* overlap is disallowed */
    if (unlikely(CHK_OVRLP(dp, dmax, sp, n))) {
        mem_prim_set(dp, dmax, 0);
        MEMORY_BARRIER;
        invoke_safe_mem_constraint_handler("memccpy_s: overlap undefined", NULL,
                                           ESOVRLP);
        return RCNEGATE(ESOVRLP);
    }

    while (dmax > 0) {
        if (unlikely(n == 0)) { /* truncation */
            *dp = '\0';
            return RCNEGATE(EOK);
        }

        *dp = *sp;
        if (*dp == c) { /* found */
#ifdef SAFECLIB_STR_NULL_SLACK
            mem_prim_set(dp, n, 0);
            MEMORY_BARRIER;
#endif
            return RCNEGATE(EOK);
        }

        dmax--;
        n--;
        dp++;
        sp++;
    }

    /* the entire src was not copied, so zero the whole buffer */
    handle_error((char *)dest, orig_dmax,
                 "memccpy_s: not enough "
                 "space for src",
                 ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_memccpy_s_chk);
#endif
