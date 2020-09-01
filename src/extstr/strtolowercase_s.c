/*------------------------------------------------------------------
 * strtolowercase_s.c
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
 * @def strtolowercase_s(dest, dmax)
 * @brief
 *    Converts all uppercase characters to
 *    lowercase, leaving all other characters unchanged.
 *    The conversion stops at the first null or after dmax
 *    characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string
 * @param[in]   dmax  maximum length of string
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall not equal zero.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @retval  EOK         when successful operation
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 *
 * @see
 *    strtouppercase_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strtolowercase_s(char *restrict dest, rsize_t dmax)
#else
EXPORT errno_t _strtolowercase_s_chk(char *restrict dest, rsize_t dmax,
                                     const size_t destbos)
#endif
{
    CHK_DEST_NULL("strtolowercase_s")
    CHK_DMAX_ZERO("strtolowercase_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strtolowercase_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strtolowercase_s", destbos)
    }

    while (*dest && dmax) {

        if ((*dest >= 'A') && (*dest <= 'Z')) {
            *dest = (char)(*dest + (char)32);
        }
        dest++;
        dmax--;
    }

    return (EOK);
}
