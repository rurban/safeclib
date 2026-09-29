/*------------------------------------------------------------------
 * u8pbrk_s.c
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
 * @def u8pbrk_s(dest,dmax,src,slen,firstp)
 * @brief
 *    Returns a pointer, firstp, to the first occurrence in the utf-8
 *    string dest of any byte contained in the utf-8 string src.
 *
 * @param  dest    pointer to utf-8 string to search
 * @param  dmax    restricted maximum byte-length of string dest
 * @param  src     pointer to the utf-8 set of bytes
 * @param  slen    restricted byte-length of string src
 * @param  firstp  returned pointer to first occurrence
 *
 * @pre  Neither dest, src nor firstp shall be a null pointer.
 * @pre  Neither dmax nor slen shall not be 0.
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_STR and
 *       the size of dest/src
 *
 * @return  pointer to the first occurrence of any byte contained in src
 * @retval  EOK         when successful operation
 * @retval  ESNULLP     when dest/src/firstp is NULL pointer
 * @retval  ESZEROL     when dmax/slen = 0
 * @retval  ESLEMAX     when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically).
 *                      when slen > size of src
 * @retval  ESLEWRNG    with --enable-error-dmax, when dmax != sizeof(dest) or
 *                      slen > size of src
 *
 * @see
 *    u8cspn_s(), u8spn_s(), u8str_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8pbrk_s(char8_t *dest, rsize_t dmax, char8_t *src, rsize_t slen,
                 char8_t **firstp)
#else
EXPORT errno_t _u8pbrk_s_chk(char8_t *dest, rsize_t dmax, char8_t *src,
                             rsize_t slen, char8_t **firstp,
                             const size_t destbos, const size_t srcbos)
#endif
{
    char8_t *ps;
    rsize_t len;

    CHK_SRC_NULL("u8pbrk_s", firstp)
    *firstp = NULL;

    CHK_DEST_NULL("u8pbrk_s")
    CHK_SRC_NULL("u8pbrk_s", src)
    CHK_DMAX_ZERO("u8pbrk_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("u8pbrk_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("u8pbrk_s", destbos)
    }

    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("u8pbrk_s: slen exceeds dmax",
                                               (void *)src, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
        BND_CHK_PTR_BOUNDS(src, slen);
    } else {
        if (unlikely(slen > srcbos)) {
            return handle_str_bos_overflow("u8pbrk_s: slen exceeds src",
                                           (char *)dest, destbos);
        }
    }
    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("u8pbrk_s: slen is 0", (void *)src,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    /*
     * look for a matching byte in the set src
     */
    while (*dest && dmax) {

        ps = src;
        len = slen;
        while (*ps) {

            /* check for a match with the set */
            if (*dest == *ps) {
                *firstp = dest;
                return RCNEGATE(EOK);
            }
            if (unlikely(!len)) {
                return RCNEGATE(ESNOTFND);
            }
            ps++;
            len--;
        }
        dest++;
        dmax--;
    }

    return RCNEGATE(ESNOTFND);
}
