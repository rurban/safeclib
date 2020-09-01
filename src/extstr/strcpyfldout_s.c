/*------------------------------------------------------------------
 * strcpyfldout_s.c
 *
 * November 2008, Bo Berry
 * October 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
 * Copyright (c) 2017 by Reini Urban
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
 * @def strncpyfldout_s(dest,dmax,src,slen)
 * @brief
 *    The \b strcpyfldout_s function copies \c slen characters from
 *    the character array pointed to by \c src into the string
 *    pointed to by \c dest. A null is included to properly
 *    terminate the dest string. The copy operation does not
 *    stop on the null character as the function copies \c slen
 *    characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest  pointer to string that will be replaced by src.
 * @param[in]   dmax  restricted maximum length of dest
 * @param[in]   src   pointer to the character array to be copied
 *                    to dest and null terminated.
 * @param[in]   slen  number of bytes that will be
 *                    copied from the src field into the dest string.
 *
 * @pre  Neither dest nor src shall be a null pointer.
 * @pre  dmax shall not equal zero.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 * @pre  slen shall not exceed dmax and size of src
 * @pre  Copying shall not take place between objects that overlap.
 *
 * @return  If there is a runtime-constraint violation and if dest and dmax
 *          are valid, then strcpyfldout_s nulls dest.
 * @retval  EOK        when operation is successful or slen = 0
 * @retval  ESNULLP    when dest/src is NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESOVRLP    when strings overlap
 * @retval  ESNOSPC    when slen > dmax
 * @see
 *    strcpyfld_s(), strcpyfldin_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t strcpyfldout_s(char *dest, rsize_t dmax, const char *src, rsize_t slen)
#else
EXPORT errno_t _strcpyfldout_s_chk(char *dest, rsize_t dmax, const char *src,
                                   rsize_t slen, const size_t destbos)
#endif
{
    rsize_t orig_dmax;
    char *orig_dest;
    const char *overlap_bumper;

    if (unlikely(slen == 0)) { /* Since C11 slen=0 is allowed */
        return EOK;
    }
    CHK_DEST_NULL("strcpyfldout_s")
    CHK_DMAX_ZERO("strcpyfldout_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("strcpyfldout_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("strcpyfldout_s", destbos)
    }
    CHK_SRC_NULL_CLEAR("strcpyfldout_s", src)
    CHK_SLEN_MAX_NOSPC_CLEAR("strcpyfldout_s", slen, RSIZE_MAX_STR)

    /* hold base of dest in case src was not copied */
    orig_dmax = dmax;
    orig_dest = dest;

    if (unlikely(dest < src)) {
        overlap_bumper = src;

        while (dmax > 1 && slen) {

            if (unlikely(dest == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strcpyfldout_s: overlapping objects", ESOVRLP);
                return (ESOVRLP);
            }

            dmax--;
            slen--;
            *dest++ = *src++;
        }
    } else {
        overlap_bumper = dest;

        while (dmax > 1 && slen) {

            if (unlikely(src == overlap_bumper)) {
                handle_error(orig_dest, orig_dmax,
                             "strcpyfldout_s: overlapping objects", ESOVRLP);
                return (ESOVRLP);
            }

            dmax--;
            slen--;
            *dest++ = *src++;
        }
    }

    /* null slack space */
    if (dmax > 0x20)
        memset(dest, 0, dmax);
    else {
        while (dmax) {
            *dest = '\0';
            dmax--;
            dest++;
        }
    }
    return (EOK);
}
