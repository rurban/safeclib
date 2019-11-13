/*------------------------------------------------------------------
 * tmpnam_s.c
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
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#endif

#if !(defined(TEST_MSVCRT) && defined(HAVE_TMPNAM_S))

/**
 * @def tmpnam_s(dest,dmax)
 * @brief
 *    Creates a unique valid file name (no longer than L_tmpnam in
 *    length) and stores it in character string pointed to by
 *    filename. The function is capable of generating up to TMP_MAX_S
 *    of unique filenames, but some or all of them may be in use in
 *    the filesystem and thus not suitable return values.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.1.2 The tmpnam_s function (p: 587-588)
 *    http://en.cppreference.com/w/c/io/tmpnam
 *    * Deprecated in favor of mkstemp
 *
 * @param[out] dest    pointer to the string capable of holding at
 *                     least L_tmpnam_s bytes, to be used as a result buffer.
 * @param[in]  dmax    maximum number of characters the function is allowed
 *                     to write (typically the size of the dest buffer).
 *
 * @pre No more than TMP_MAX_S files may be opened
 * @pre dest shall not be a null pointer
 * @pre dmax shall not be greater than RSIZE_MAX_STR and size of dest
 * @pre dmax shall not be smaller than the generated file name string,
 *      which is at least strlen(dest) + 3.
 *
 * @return Returns zero and writes the file name to dest on
 * success. On error, returns non-zero and writes the null character
 * to dest[0] (only if dest is not null and dmax is valid).
 *
 * @retval  EOK        on success
 * @retval  ESNULLP    when dest is a NULL pointer
 * @retval  ESZEROL    when dmax = 0
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR or
 *                     more than TMP_MAX_S files were opened.
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  errno()    when tmpnam() failed, typically -ENOENT
 *
 * @note
 *   Although the names generated by tmpnam_s are difficult to guess, it
 *   is possible that a file with that name is created by another
 *   process between the moment tmpnam returns and the moment this
 *   program attempts to use the returned name to create a file. The
 *   standard function tmpfile and the POSIX function mkstemp do not
 *   have this problem (creating a unique directory using only the
 *   standard C library still requires the use of tmpnam_s).
 *
 *   POSIX systems additionally define the similarly named function
 *   tempnam(), which offers the choice of a directory (which defaults
 *   to the optionally defined macro P_tmpdir).
 */

EXPORT errno_t _tmpnam_s_chk(const char *dest, rsize_t dmax,
                             const size_t destbos) {
    static int count = 0;
    char *result = NULL;
    char *dp = (char *)dest;

    CHK_DEST_NULL("tmpnam_s")
    CHK_DMAX_ZERO("tmpnam_s")
#if 0
    if (unlikely(dmax < strnlen_s(dp, dmax) + 3)) {
        invoke_safe_str_constraint_handler("tmpnam_s: dmax underflow < dest+3",
                   (void*)dest, ESLEMIN);
        return ESLEMIN;
    }
#endif
    if (unlikely(dmax > RSIZE_MAX_STR || dmax > L_tmpnam_s)) {
        invoke_safe_str_constraint_handler("tmpnam_s: dmax exceeds max",
                                           (void *)dest, ESLEMAX);
        return ESLEMAX;
    }
    if (destbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        if (unlikely(dmax > destbos)) {
            invoke_safe_str_constraint_handler("tmpnam_s: dmax exceeds dest",
                                               (void *)dest, EOVERFLOW);
            return EOVERFLOW;
        }
    }

    ++count;
    if (unlikely(count > TMP_MAX_S)) {
        invoke_safe_str_constraint_handler("tmpnam_s: count exceeds TMP_MAX_S",
                                           NULL, ESLEMAX);
        return ESLEMAX;
    }

#if !defined SAFECLIB_HAVE_C99 && defined HAVE_CXX
    result = tmpnam(dp);
#else
#ifdef __clang
#pragma clang diagnostic push
#pragma clang diagnostic warning "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#endif

    result = tmpnam(dp);

#ifdef __clang
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#endif

    if (result) {
        size_t len = strlen(result);

        if (unlikely(len > dmax)) {
            *(char *)dest = '\0';
            invoke_safe_str_constraint_handler("tmpnam_s: length exceeds size",
                                               (void *)dest, ESNOSPC);
            return ESNOSPC;
        }

        if (unlikely(len > L_tmpnam_s)) {
            *(char *)dest = '\0';
            invoke_safe_str_constraint_handler(
                "tmpnam_s: length exceeds L_tmpnam_s", (void *)dest, ESLEMAX);
            return ESLEMAX;
        }
        strncpy_s((char *)dest, dmax, result, len);
#ifdef SAFECLIB_STR_NULL_SLACK
        memset_s((void *)&dest[len], dmax, 0, dmax - len);
#endif
        return EOK;
    } else {
        invoke_safe_str_constraint_handler("tmpnam_s: tmpnam() failed",
                                           (void *)dest, ESNOTFND);
        return errno;
    }
}

#endif /* TEST_MSVCRT */
