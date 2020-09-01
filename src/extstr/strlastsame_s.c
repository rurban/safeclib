/*------------------------------------------------------------------
 * strlastsame_s.c
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
 * @def strlastsame_s(dest,dmax,src,resultp)
 * @brief
 *    Returns the index of the last character that is the
 *    same between dest and src. The scanning stops at the
 *    first nul in dest or src, or after dmax characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest    pointer to string to compare against
 * @param[in]   dmax    restricted maximum length of string dest
 * @param[in]   src     pointer to the string to be compared to dest
 * @param[out]  resultp pointer to returned index
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  resultp shall not be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @return  index to last same char, when the return code is OK
 * @retval  EOK         when index to last same char is returned
 * @retval  ESNULLP     when dst/src/resultp is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW   when dmax > size of dest (optionally, when the compiler
 *                      knows the object_size statically)
 * @retval  ESLEWRNG    when dmax != sizeof(dest) and --enable-error-dmax
 * @retval  ESNOTFND    when not found
 *
 * @see
 *    strfirstchar_s(), strfirstdiff_s(), strfirstsame_s(),
 *    strlastchar_s(), strlastdiff_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strlastsame_s(const char *dest, rsize_t dmax,
                      const char *src, rsize_t *resultp)
#else
EXPORT errno_t _strlastsame_s_chk(const char *dest, rsize_t dmax,
                                  const char *src, rsize_t *resultp,
                                  const size_t destbos)
#endif
{
    const char *rp;
    bool found;

    CHK_SRC_NULL("strfirstsame_s", resultp)
    *resultp = 0;

    CHK_DEST_NULL("strlastsame_s")
    CHK_SRC_NULL("strlastsame_s", src)
    CHK_DMAX_ZERO("strlastsame_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strlastsame_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strlastsame_s", destbos)
    }

    /* hold reference point */
    rp = dest;

    /*
     * find the last offset
     */
    found = false;
    while (*dest && *src && dmax) {

        if (*dest == *src) {
            found = true;
            *resultp = (uint32_t)(dest - rp);
        }

        dest++;
        src++;
        dmax--;
    }

    if (found) {
        return (EOK);
    } else {
        return (ESNOTFND);
    }
}
