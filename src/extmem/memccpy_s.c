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
#include "mem_primitives_lib.h"
#endif

/**
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
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_MEM.
 * @pre  n shall not be greater than dmax.
 * @pre  Copying shall not take place between regions that overlap.
 *
 * @note memccpy from BSD, Windows sec_api, glibc, newlib and everywhere else.
 *
 * @return  The memccpy() function returns a pointer to the next character in
 *          dest after c, or NULL if c was not found in the first n characters of
 *          src. If there is a runtime-constraint violation, the memccpy_s function
 *          stores zeros in the first dmax bytes of the region pointed to
 *          by dest if dest is not a null pointer and n is valid.
 *          With n=0, dest[0] is set to '\0', as with \c memccpy().
 * @retval  EOK         when operation is successful or n = 0
 * @retval  ESNULLP     when dest/src is NULL POINTER
 * @retval  ESZEROL     when dmax = ZERO.
 * @retval  ESLEMAX     when dmax/n > RSIZE_MAX_MEM
 * @retval  ESNOSPC     when dmax < n
 * @retval  ESOVRLP     when src memory overlaps dst
 *
 * @see
 *    memcpy_s(), strncpy_s()
 */
EXPORT errno_t
memccpy_s (void *restrict dest, rsize_t dmax, const void *restrict src,
           int c, rsize_t n)
{
    uint8_t *dp;
    const uint8_t  *sp;
    rsize_t orig_dmax = dmax;

    dp = (uint8_t*) dest;
    sp = (uint8_t*) src;

    if (unlikely(dp == NULL)) {
        invoke_safe_mem_constraint_handler("memccpy_s: dest is NULL",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("memccpy_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_MEM || n > RSIZE_MAX_MEM)) {
        invoke_safe_mem_constraint_handler("memccpy_s: dmax/n exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (unlikely(n == 0)) {
        /* Since C11 n=0 is allowed */
        *dp = '\0';
        return EOK;
    }

    if (unlikely(n > dmax)) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memccpy_s: n exceeds dmax",
                   NULL, ESNOSPC);
        return RCNEGATE(ESNOSPC);
    }

    if (unlikely(sp == NULL)) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memccpy_s: src is NULL",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    /*
     * overlap is an error
     */
    if (unlikely( ((dp >= sp) && (dp < (sp+n))) ||
                  ((sp > dp) && (sp < (dp+dmax))) )) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memccpy_s: overlap undefined",
                   NULL, ESOVRLP);
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
            memset(dp, 0, n);
#endif
            return RCNEGATE(EOK);
        }

        dmax--;
        n--;
        dp++;
        sp++;
    }

    /* the entire src was not copied, so zero the whole buffer */
    handle_error((char*)dest, orig_dmax, "memccpy_s: not enough "
                 "space for src",
                 ESNOSPC);
    return RCNEGATE(ESNOSPC);
}
EXPORT_SYMBOL(memccpy_s)
