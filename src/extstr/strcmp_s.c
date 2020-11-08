/*------------------------------------------------------------------
 * strcmp_s.c -- string compare
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
 * @def strcmp_s(dest,dmax,src,resultp)
 * @brief
 *    Compares string src to string dest.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest       pointer to string to compare against
 * @param[in]   dmax       restricted maximum length of string dest
 * @param[in]   src        pointer to the string to be compared to dest
 * @param[out]  resultp    pointer to int result, greater than 0,
 *                         equal to 0 or less than 0, if the string pointed
 *                         to by dest is greater than, equal to or less
 *                         than the string pointed to by src respectively.
 *
 * @pre   Neither dest, src nor resultp shall be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @return  resultp (when the return code is OK)
 * @retval  >0         when dest greater than src
 * @retval   0         when strings the same
 * @retval  <0         when dest less than src
 * @retval  EOK        when comparison is complete
 * @retval  ESUNTERM   when src is unterminated
 * @retval  ESNULLP    when dest/src/resultp is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    strcasecmp_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strcmp_s(const char *dest, rsize_t dmax, const char *src,
                 int *resultp)
#else
EXPORT errno_t _strcmp_s_chk(const char *dest, rsize_t dmax, const char *src,
                             int *resultp, const size_t destbos,
                             size_t srcbos)
#endif
{
    size_t slen;
    CHK_SRC_NULL("strcmp_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("strcmp_s")
    CHK_SRC_NULL("strcmp_s", src)
    CHK_DMAX_ZERO("strcmp_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strcmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strcmp_s", destbos)
    }

    slen = 0;
    while (*dest && *src && dmax) {

        if (*dest != *src) {
            break;
        }

        dest++;
        src++;
        dmax--;
        slen++;
        /* sentinel srcbos -1 = ULONG_MAX */
        if (unlikely(slen >= srcbos)) {
            invoke_safe_str_constraint_handler("strcmp_s"
                                               ": src unterminated",
                                               (void *)src, ESUNTERM);
            return RCNEGATE(ESUNTERM);
        }
    }
    *resultp = *dest - *src;
    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strcmp_s_chk);
#endif /* __KERNEL__ */
