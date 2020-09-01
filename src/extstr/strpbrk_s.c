/*------------------------------------------------------------------
 * strpbrk_s.c
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
 * @def strpbrk_s(dest,dmax,src,slen,firstp)
 * @brief
 *    Returns a pointer, first, to the first ocurrence of any character
 *    in src which is contained in dest.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest    pointer to string to compare against
 * @param  dmax    restricted maximum length of string dest
 * @param  src     pointer to the string
 * @param  slen    restricted length of string src
 * @param  firstp  returned pointer to first occurence
 *
 * @pre  Neither dest, src nor firstp shall be a null pointer.
 * @pre  Neither dmax nor slen shall not be 0.
 * @pre  Neither dmax nor slen shall be greater than RSIZE_MAX_STR and size of
 * dest/src
 *
 * @return  pointer to the first ocurrence of any character contained in src
 * @retval  EOK         when successful operation
 * @retval  ESNULLP     when dest/src/firstp is NULL pointer
 * @retval  ESZEROL     when dmax/slen = 0
 * @retval  ESLEMAX     when dmax/slen > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    strfirstchar_s(), strlastchar_s(), strfirstdiff_s(),
 *    strfirstsame_s(), strlastdiff_s(), strlastsame_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strpbrk_s(char *dest, rsize_t dmax, char *src, rsize_t slen, char **firstp)
#else
EXPORT errno_t _strpbrk_s_chk(char *dest, rsize_t dmax, char *src, rsize_t slen, char **firstp,
                              const size_t destbos, const size_t srcbos)
#endif
{
    char *ps;
    rsize_t len;

    CHK_SRC_NULL("strpbrk_s", firstp)
    *firstp = NULL;

    CHK_DEST_NULL("strpbrk_s")
    CHK_SRC_NULL("strpbrk_s", src)
    CHK_DMAX_ZERO("strpbrk_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strpbrk_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
        BND_CHK_PTR_BOUNDS(dest, slen);
    } else {
        CHK_DEST_OVR("strpbrk_s", destbos)
    }

    if (srcbos == BOS_UNKNOWN) {
        if (unlikely(slen > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("strpbrk_s: slen exceeds dmax",
                                               (void *)src, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
        BND_CHK_PTR_BOUNDS(src, slen);
    } else {
        if (unlikely(slen > srcbos)) {
            invoke_safe_str_constraint_handler("strpbrk_s: slen exceeds src",
                                               (void *)src, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
    }
    if (unlikely(slen == 0)) {
        invoke_safe_str_constraint_handler("strpbrk_s: slen is 0", (void *)src,
                                           ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    /*
     * look for a matching char in the substring src
     */
    while (*dest && dmax) {

        ps = src;
        len = slen;
        while (*ps) {

            /* check for a match with the substring */
            if (*dest == *ps) {
                *firstp = dest;
                return RCNEGATE(EOK);
            }
            ps++;
            len--;
        }
        dest++;
        dmax--;
    }

    return RCNEGATE(ESNOTFND);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strpbrk_s_chk);
#endif /* __KERNEL__ */
