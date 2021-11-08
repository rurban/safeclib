/*------------------------------------------------------------------
 * bsearch_s.c
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
#include <stdlib.h>
#endif

#if defined(TEST_MSVCRT) && defined(HAVE_BSEARCH_S)
#else

/**
 * @def bsearch_s(key,base,nmemb,size,compar,context)
 * @brief
 *    The \c bsearch_s function finds an element equal to element pointed to
 *    by key in an array pointed to by base. The array contains count elements
 *    of size bytes and must be partitioned with respect to key, that is, all
 *    the elements that compare less than must appear before all the elements
 *    that compare equal to, and those must appear before all the elements
 *    that compare greater than the key object. A fully sorted array satisfies
 *    these requirements. The elements are compared using function pointed to
 *    by comp. The behavior is undefined if the array is not already
 *    partitioned with respect to *key in ascending order according to the
 *    same criterion that compar uses.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.6.3.1 The bsearch_s function (p: 608-609)
 *    http://en.cppreference.com/w/c/algorithm/bsearch
 *
 * @param[in] key     pointer to the element to search for
 * @param[in] base    pointer to the array to examine
 * @param[in] nmemb   number of elements in the array
 * @param[in] size    size of each element in the array in bytes
 * @param[in] compar  comparison function which returns ​a negative integer
 *                    value if the first argument is less than the second, a
 *                    positive integer value if the first argument is greater
 *                    than the second and zero if the arguments are equal. key
 *                    is passed as the first argument, an element from the
 *                    array as the second.  The signature of the comparison
 *                    function should be equivalent to the following: int
 *                    cmp(const void *a, const void *b); The function must not
 *                    modify the objects passed to it and must return
 *                    consistent results when called for the same objects,
 *                    regardless of their positions in the array.
 * @param[in] context additional information (e.g., collating sequence), passed
 *                    to compar as the third argument
 *
 * @pre key, base or compar shall not be a null pointer (unless nmemb is zero).
 * @pre nmemb or size shall not be greater than RSIZE_MAX_MEM.
 *
 *    If the array contains several elements that compar would indicate as
 *    equal to the element searched for, then it is unspecified which element the
 *    function will return as the result.
 *
 * @return Pointer to an element in the array that compares equal to *key, or
 *         null pointer if such element has not been found, or any run-time
 *         constraint violation.
 *
 * @note
 *    Despite the name, neither C nor POSIX standards require this function to
 *    be implemented using binary search or make any complexity guarantees.
 *    Unlike other bounds-checked functions, bsearch_s does not treat arrays
 *    of zero size as a runtime constraint violation and instead indicates
 *    element not found (the other function that accepts arrays of zero size
 *    is qsort_s).
 *
 * @retval  NULL       when not found or any error
 *
 * errno values:
 *   ESNULLP   when key, base or compar are a NULL pointer, and nmemb is > 0
 *   ESLEMAX   when nmemb or size > RSIZE_MAX_MEM
 *   ESNOSPC   when nmemb * size > sizeof base
 *
 * @see
 *    qsort_s()
 */

#ifdef FOR_DOXYGEN
void *
bsearch_s(const void *key, const void *base, rsize_t nmemb, rsize_t size,
               int (*compar)(const void *k, const void *y, void *context),
               void *context)
#else
EXPORT void *
_bsearch_s_chk(const void *key, const void *base, rsize_t nmemb, rsize_t size,
               int (*compar)(const void *k, const void *y, void *context),
               void *context, const size_t basebos)
#endif
{
    if (likely(nmemb != 0)) {
        if (unlikely(key == NULL || base == NULL || compar == NULL)) {
            invoke_safe_mem_constraint_handler(
                "bsearch_s: key/base/compar is null", (void *)base, ESNULLP);
            errno = ESNULLP;
            return NULL;
        }
    }
    if (basebos == BOS_UNKNOWN) {
        if (unlikely(nmemb > RSIZE_MAX_MEM || size > RSIZE_MAX_MEM)) {
            invoke_safe_mem_constraint_handler(
                "bsearch_s: nmemb or size exceeds max", (void *)base, ESLEMAX);
            errno = ESLEMAX;
            return NULL;
        }
        BND_CHK_PTR_BOUNDS(base, nmemb * size);
    } else {
        rsize_t basesz = nmemb * size;
        if (unlikely(basesz > basebos)) {
            invoke_safe_mem_constraint_handler(
                "bsearch_s: nmemb*size exceeds sizeof base", (void *)base, ESNOSPC);
            errno = ESNOSPC;
            return NULL;
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(basesz != basebos)) {
            handle_mem_bos_chk_warn("bsearch_s", (void *)base, basesz, basebos);
#ifdef HAVE_ERROR_DMAX
            errno = ESLEWRNG;
            return NULL;
#endif
        }
#endif
    }

    errno = 0;

    while (nmemb > 0) {
        void *ptry = (char *)base + size * (nmemb / 2);
        int sign = compar(key, ptry, context);
        if (!sign)
            return ptry;
        else if (nmemb == 1)
            break;
        else if (sign < 0) {
            nmemb /= 2;
        } else {
            base = ptry;
            nmemb -= nmemb / 2;
        }
    }
    return NULL;
}

#endif /* TEST_MSVCRT */
