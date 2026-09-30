/*------------------------------------------------------------------
 * u8spn_s.c
 *
 * September 2026
 *
 * Copyright (c) 2026 by Reini Urban
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
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def u8spn_s(dest,dmax,src,slen,countp)
 * @brief
 *    This function computes the byte-length of the prefix of the
 *    utf-8 string pointed to by dest which consists entirely of bytes
 *    that are included in the utf-8 string pointed to by src.
 *
 * @param[in]   dest   pointer to utf-8 string to determine the prefix
 * @param[in]   dmax   restricted maximum byte-length of string dest
 * @param[in]   src    pointer to utf-8 inclusion string
 * @param[in]   slen   restricted maximum byte-length of string src
 * @param[out]  countp pointer to a count variable that will be updated
 *                     with the dest prefix byte-length
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
 *    u8cspn_s(), u8pbrk_s(), u8str_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8spn_s(const char8_t *dest, rsize_t dmax, const char8_t *src,
                rsize_t slen, rsize_t *countp)
#else
EXPORT errno_t _u8spn_s_chk(const char8_t *dest, rsize_t dmax,
                            const char8_t *src, rsize_t slen,
                            rsize_t *countp, const size_t destbos,
                            const size_t srcbos)
#endif
{
    const char8_t *scan2;
    rsize_t smax;
    bool match_found;

    CHK_SRC_NULL("u8spn_s", countp)
    *countp = 0;

    CHK_DEST_NULL("u8spn_s")
    CHK_SRC_NULL("u8spn_s", src)
    CHK_DMAX_ZERO("u8spn_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8spn_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, slen);
    } else {
        CHK_DEST_OVR("u8spn_s", destbos)
    }

    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("u8spn_s: slen exceeds dmax",
                                               (void *)src, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
        BND_CHK_PTR_BOUNDS(src, slen);
    } else {
        if (unlikely(slen > srcbos)) {
            if (unlikely(slen > RSIZE_MAX_STR)) {
                invoke_safe_str_constraint_handler(
                    "u8spn_s: slen exceeds dmax", (void *)src, ESLEMAX);
                return RCNEGATE(ESLEMAX);
            } else {
                invoke_safe_str_constraint_handler("u8spn_s: slen exceeds src",
                                                   (void *)src, EOVERFLOW);
                return RCNEGATE(EOVERFLOW);
            }
        }
    }
    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("u8spn_s: slen is 0", (void *)src,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    while (*dest && dmax) {
        /*
         * Scan the entire src string for each dest byte, counting
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
