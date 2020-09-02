/*------------------------------------------------------------------
 * memchr_s.c
 *
 * September 2017, Reini Urban
 *
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
#include "safe_mem_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @def memchr_s(dest,dmax,ch,result)
 * @brief
 *    Finds the first occurrence of \c ch (after conversion to unsigned char
 *    as if by <tt>(unsigned char)ch</tt>) in the null-terminated buffer
 *    pointed to by \c dest (each character interpreted as <tt>unsigned
 *    char</tt>).
 *
 * @remark IMPLEMENTED IN
 *
 * @param[in]  dest    pointer to buffer to compare against
 * @param[in]  dmax    length of dest to search in
 * @param[in]  ch      character to search for
 * @param[out] result  pointer to result in dest
 *
 * @pre  Neither dest nor result shall be a null pointer.
 * @pre  dmax shall not be 0.
 * @pre  dmax shall not be greater than RSIZE_MAX_MEM and sizeof(dest)
 * @pre  ch shall not be greater than 255
 *
 * @retval  EOK        when the character was successfully found.
 * @retval  ESNULLP    when dest/result is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_MEM or > sizeof(dest).
 *                     Or when ch > 255
 * @retval  ESNOTFND   when ch not found in dest
 *
 * @see
 *    strchr_s(), strspn_s(), strcspn_s(), strpbrk_s(), strstr_s()
 *
 */
#ifdef FOR_DOXYGEN
errno_t memchr_s(const void *restrict dest, rsize_t dmax,
                 const int ch, void **result)
#else
EXPORT errno_t _memchr_s_chk(const void *restrict dest, rsize_t dmax,
                             const int ch, void **result,
                             const size_t destbos)
#endif
{
    if (unlikely(result == NULL)) {
        invoke_safe_mem_constraint_handler("memchr_s: result is null",
                                           (void *)dest, ESNULLP);
        return (ESNULLP);
    }
    *result = NULL;
    CHK_DEST_MEM_NULL("memchr_s")
    CHK_DMAX_MEM_ZERO("memchr_s")
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MEM_MAX("memchr_s", RSIZE_MAX_MEM)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_MEM_OVR("memchr_s", destbos)
    }
    if (unlikely(ch > 255)) {
        invoke_safe_str_constraint_handler("memchr_s: ch exceeds max", NULL,
                                           ESLEMAX);
        return (ESLEMAX);
    }

    /* compares wordwise */
    *result = memchr((void *)dest, ch, (size_t)dmax);

    if (!*result)
        return (ESNOTFND);
    return (EOK);
}
