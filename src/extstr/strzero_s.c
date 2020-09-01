/*------------------------------------------------------------------
 * strzero_s.c
 *
 * October 2008, Bo Berry
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
 * @def strzero_s(dest,dmax)
 * @brief
 *    Nulls maximal dmax characters of dest.  This function can be used
 *    to clear strings that contained sensitive data, until the terminating
 * NULL character. With SAFECLIB_STR_NULL_SLACK defined all elements following
 * the terminating null character (if any) written in the array of dmax
 * characters pointed to by dest are nulled.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string that will be nulled.
 * @param[in]   dmax  restricted maximum length of dest
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
 *    strispassword_s()
 */

#ifdef FOR_DOXYGEN
errno_t strzero_s(char *dest, rsize_t dmax)
#else
EXPORT errno_t _strzero_s_chk(char *dest, rsize_t dmax,
                              const size_t destbos)
#endif
{
    CHK_DEST_NULL("strzero_s")
    CHK_DMAX_ZERO("strzero_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strzero_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strzero_s", destbos)
    }

    /* null string to eliminate data */
    while (dmax && *dest) {
        *dest = '\0';
        dmax--;
        dest++;
    }
#ifdef SAFECLIB_STR_NULL_SLACK
    if (!*dest)
        memset(dest, 0, dmax);
#endif

    return (EOK);
}
