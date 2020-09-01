/*------------------------------------------------------------------
 * strspn_s.c
 *
 * November 2008, Bo Berry
 *
 * Copyright (c) 2008-2011, 2013 by Cisco Systems, Inc
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
 * @def strspn_s(dest,dmax,src,slen,count)
 * @brief
 *    This function computes the prefix length of the string
 *    pointed to by dest which consists entirely of characters
 *    that are included from the string pointed to by src.
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
 * @pre  Neither dest, src nor countp shall be a null pointer.
 * @pre  Neither dmax nor slen shall be 0.
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_STR and size of
 * dest/src.
 *
 * @retval  EOK        when successful operation, substring found.
 * @retval  ESNULLP    when dest/src/countp is NULL pointer
 * @retval  ESZEROL    when dmax/slen = 0
 * @retval  ESLEMAX    when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax/slen > size of dest/src (optionally, when the
 * compiler knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    strcspn_s(), strpbrk_s(), strstr_s(), strprefix_s()
 */

#ifdef FOR_DOXYGEN
errno_t strspn_s(const char *dest, rsize_t dmax, const char *src,
                 rsize_t slen, rsize_t *countp)
#else
EXPORT errno_t _strspn_s_chk(const char *dest, rsize_t dmax, const char *src,
                             rsize_t slen, rsize_t *countp,
                             const size_t destbos, const size_t srcbos)
#endif
{
    const char *scan2;
    rsize_t smax;
    bool match_found;

    CHK_SRC_NULL("strspn_s", countp)
    *countp = 0;

    CHK_DEST_NULL("strspn_s")
    CHK_SRC_NULL("strspn_s", src)
    CHK_DMAX_ZERO("strspn_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strspn_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, slen);
    } else {
        CHK_DEST_OVR("strspn_s", destbos)
    }

    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("strspn_s: slen exceeds dmax",
                                               (void *)src, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
        BND_CHK_PTR_BOUNDS(src, slen);
    } else {
        if (unlikely(slen > srcbos)) {
            if (unlikely(slen > RSIZE_MAX_STR)) {
                invoke_safe_str_constraint_handler(
                    "strspn_s: slen exceeds dmax", (void *)src, ESLEMAX);
                return RCNEGATE(ESLEMAX);
            } else {
                invoke_safe_str_constraint_handler("strspn_s: slen exceeds src",
                                                   (void *)src, EOVERFLOW);
                return RCNEGATE(EOVERFLOW);
            }
        }
    }
    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("strspn_s: slen is 0", (void *)src,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    while (*dest && dmax) {
        /*
         * Scan the entire src string for each dest character, counting
         * inclusions.
         */
        match_found = false;
        smax = slen;
        scan2 = src;
        while (*scan2 && smax) {

            if (*dest == *scan2) {
                match_found = true;
                break;
            }
            scan2++;
            smax--;
        }

        if (match_found) {
            (*countp)++;
        } else {
            break;
        }

        dest++;
        dmax--;
    }

    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strspn_s_chk);
#endif /* __KERNEL__ */
