/*------------------------------------------------------------------
 * u8error_s.c
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
 * @def u8error_s(dest,dmax,errnum)
 * @brief
 *    The \c u8error_s function returns a pointer to the textual description
 *    of the system error code errnum, identical to the description that
 *    would be printed by perror(). Error messages are plain ASCII, which
 *    is always well-formed UTF-8. In addition to \c strerror() it adds the
 *    size of the destination array in order to prevent buffer overflow, and
 *    it truncates overlong error messages with "...".
 *
 * @param[out]  dest    pointer to a user-provided utf-8 string buffer
 * @param[in]   dmax    restricted maximum byte-length of dest
 * @param[in]   errnum  integer value referring to an error code
 *
 * @pre dest shall not be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR and size of dest
 * @pre dmax shall not equal zero.
 *
 * @return  Zero if the entire message was successfully stored in dest,
 *          non-zero otherwise.
 * @retval  EOK        on success
 * @retval  ESNULLP    when dest is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESLEMIN    when the result would be longer than 4 and dmax < 4
 *
 * @see
 *    u8errorlen_s(), strerror_s()
 */
#ifdef FOR_DOXYGEN
errno_t u8error_s(char8_t *dest, rsize_t dmax, errno_t errnum)
#else
EXPORT errno_t _u8error_s_chk(char8_t *dest, rsize_t dmax, errno_t errnum,
                              const size_t destbos)
#endif
{
    return _strerror_s_chk((char *)dest, dmax, errnum, destbos);
}

/**
 * @brief
 *    The \c u8errorlen_s function returns the untruncated byte-length of
 *    the textual description of the system error code \c errnum, identical
 *    to the description that would be printed by \c perror(). Error
 *    messages are plain ASCII, so their byte-length equals their utf-8
 *    character length.
 *
 * @param[in]   errnum  integer value referring to an error code
 *
 * @return The length of the error message or 0
 *
 * @see
 *    u8error_s(), strerrorlen_s()
 */
EXPORT size_t u8errorlen_s(errno_t errnum) { return strerrorlen_s(errnum); }
