/*------------------------------------------------------------------
 * strcasecmp_s.c
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
 * @def strcasecmp_s(dest,dmax,src,resultp)
 * @brief
 *    Case insensitive string comparison by converting
 *    to uppercase prior to the compare.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
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
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   resultp shall not be a null pointer.
 * @pre   dmax shall not be 0
 * @pre   dmax shall not be greater than RSIZE_MAX_STR or size of dest
 *
 * @return  resultp (when the return code is OK)
 * @retval  >0 when dest greater than src
 * @retval  0 when strings the same
 * @retval  <0 when dest less than src
 * @retval  EOK        when comparison is complete
 * @retval  ESNULLP    when dest/src/resultp is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 *
 * @see
 *    strcmp_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strcasecmp_s(const char *dest, rsize_t dmax,
                     const char *src, int *resultp)
#else
EXPORT errno_t _strcasecmp_s_chk(const char *dest, rsize_t dmax,
                                 const char *src, int *resultp,
                                 const size_t destbos)
#endif
{
    const unsigned char *udest = (const unsigned char *)dest;
    const unsigned char *usrc = (const unsigned char *)src;
    int result = 0;

    CHK_SRC_NULL("strcasecmp_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("strcasecmp_s")
    CHK_SRC_NULL("strcasecmp_s", src)
    CHK_DMAX_ZERO("strcasecmp_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strcasecmp_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strcasecmp_s", destbos)
    }

    while (*udest && *usrc && dmax) {

        result = toupper(*udest) - toupper(*usrc);
        if (result) {
            *resultp = result;
            return RCNEGATE(EOK);
        }

        udest++;
        usrc++;
        dmax--;
    }

    *resultp = toupper(*udest) - toupper(*usrc);
    return RCNEGATE(EOK);
}
#ifdef __KERNEL__
EXPORT_SYMBOL(_strcasecmp_s_chk);
#endif /* __KERNEL__ */
