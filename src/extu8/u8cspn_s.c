/*------------------------------------------------------------------
 * u8cspn_s.c
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
 * @def u8cspn_s(dest,dmax,src,slen,countp)
 * @brief
 *    This function computes the byte-length of the prefix of the
 *    utf-8 string pointed to by dest which consists entirely of bytes
 *    that are excluded from the utf-8 string pointed to by src. The
 *    scanning stops at the first NUL in dest or after dmax bytes. The
 *    exclusion string is checked to the NUL or after slen bytes.
 *    Bytes are matched individually, like strcspn_s(); this is exact
 *    for excluding single-byte (ASCII) delimiters.
 *
 * @param[in]   dest   pointer to utf-8 string to determine the prefix
 * @param[in]   dmax   restricted maximum byte-length of string dest
 * @param[in]   src    pointer to utf-8 exclusion string
 * @param[in]   slen   restricted maximum byte-length of string src
 * @param[out]  countp pointer to a count variable that will be updated
 *                     with the dest prefix byte-length
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  countp shall not be a null pointer.
 * @pre  dmax shall not be 0
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dest/src/countp is NULL pointer
 * @retval  ESZEROL     when dmax/slen = 0
 * @retval  ESLEMAX     when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    u8spn_s(), u8pbrk_s(), u8str_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8cspn_s(const char8_t *dest, rsize_t dmax, const char8_t *src,
                 rsize_t slen, rsize_t *countp)
#else
EXPORT errno_t _u8cspn_s_chk(const char8_t *dest, rsize_t dmax,
                             const char8_t *src, rsize_t slen, rsize_t *countp,
                             const size_t destbos, const size_t srcbos)
#endif
{
    const char8_t *scan2;
    rsize_t smax;

    CHK_SRC_NULL("u8cspn_s", countp)
    *countp = 0;

    CHK_DEST_NULL("u8cspn_s")
    CHK_SRC_NULL("u8cspn_s", src)
    CHK_DMAX_ZERO("u8cspn_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8cspn_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8cspn_s", destbos)
    }

    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("u8cspn_s: slen is 0", NULL,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    if (unlikely(slen > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("u8cspn_s: slen exceeds max", NULL,
                                           ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, slen);
    } else if (unlikely(slen > srcbos)) {
        invoke_safe_mem_constraint_handler("u8cspn_s: slen exceeds src",
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
