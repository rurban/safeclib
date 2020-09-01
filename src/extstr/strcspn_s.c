/*------------------------------------------------------------------
 * strcspn_s.c
 *
 * November 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def strcspn_s(dest,dmax,src,slen,countp)
 * @brief
 *    This function computes the prefix length of the string pointed
 *    to by dest which consists entirely of characters that are
 *    excluded from the string pointed to by src. The scanning stops
 *    at the first null in dest or after dmax characters. The
 *    exclusion string is checked to the null or after slen
 *    characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest   pointer to string to determine the prefix
 * @param[in]   dmax   restricted maximum length of string dest
 * @param[in]   src    pointer to exclusion string
 * @param[in]   slen   restricted maximum length of string src
 * @param[out]  countp pointer to a count variable that will be updated
 *                     with the dest substring length
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  count shall not be a null pointer.
 * @pre  dmax shall not be 0
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest/src/count is NULL pointer
 * @retval  ESZEROL     when dmax/slen = 0
 * @retval  ESLEMAX     when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    strspn_s(), strpbrk_s(), strstr_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strcspn_s(const char *dest, rsize_t dmax, const char *src,
                  rsize_t slen, rsize_t *countp)
#else
EXPORT errno_t _strcspn_s_chk(const char *dest, rsize_t dmax, const char *src,
                              rsize_t slen, rsize_t *countp,
                              const size_t destbos, const size_t srcbos)
#endif
{
    const char *scan2;
    rsize_t smax;

    CHK_SRC_NULL("strcspn_s", countp)
    *countp = 0;

    CHK_DEST_NULL("strcspn_s")
    CHK_SRC_NULL("strcspn_s", src)
    CHK_DMAX_ZERO("strcspn_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strcspn_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strcspn_s", destbos)
    }

    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("strcspn_s: slen is 0", NULL,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(slen > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strcspn_s: slen exceeds max", NULL,
                                           ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen);
    } else if (unlikely(slen > srcbos)) {
        invoke_safe_mem_constraint_handler("strcspn_s: slen exceeds src",
                                           (void *)src, EOVERFLOW);
        return (RCNEGATE(EOVERFLOW));
    }

    while (*dest && dmax) {

        /*
         * Scanning for exclusions, so if there is a match,
         * we're done!
         */
        smax = slen;
        scan2 = src;
        while (*scan2 && smax) {

            if (*dest == *scan2) {
                return RCNEGATE(EOK);
            }
            scan2++;
            smax--;
        }

        (*countp)++;
        dest++;
        dmax--;
    }

    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strcspn_s_chk);
#endif /* __KERNEL__ */
