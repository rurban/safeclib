/*------------------------------------------------------------------
 * qsort_s.c
 *
 * September 2017, Reini Urban
 *
 * Copyright (C) 2011 by Valentin Ochs
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

/* Minor changes by Rich Felker for integration in musl, 2011-04-27. */

/* Smoothsort, an adaptive variant of Heapsort.  Memory usage: O(1).
   Run time: Worst case O(n log n), close to O(n) in the mostly-sorted case. */

#ifdef FOR_DOXYGEN
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#include <stdlib.h>
#endif

/* conflicting API */
#if defined(MINGW_HAS_SECURE_API) && __MINGW64_VERSION_MAJOR < 7
#else

#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
/* ignored on 32bit, thus safe */
typedef unsigned long uint64_t;
#endif
#include <stdlib.h>
#include <string.h>

/* in mingw sec_api returning void */

/**
 * @def qsort_s(base,nmemb,size,compar,context)
 * @brief
 *    The \c qsort_s function sorts the given array pointed to by base in
 *    ascending order. The array contains nmemb elements of size
 *    bytes. Function pointed to by compar is used for object comparison.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.6.3.2 The qsort_s function (p: 609)
 *    http://en.cppreference.com/w/c/algorithm/qsort
 *    * ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in] base   pointer to the array to sort
 * @param[in] nmemb  number of elements in the array
 * @param[in] size   size of each element in the array in bytes
 * @param[in] compar comparison function which returns a negative integer
 *                   value if the first argument is less than the second, a
 *                   positive integer value if the first argument is greater
 *                   than the second and zero if the arguments are equal. key
 *                   is passed as the first argument, an element from the
 *                   array as the second.  The signature of the comparison
 *                   function should be equivalent to the following: int
 *                   cmp(const void *a, const void *b, const void *context);
 *                   The function must not
 *                   modify the objects passed to it and must return
 *                   consistent results when called for the same objects,
 *                   regardless of their positions in the array.
 * @param[in] context additional information (e.g., collating sequence), passed
 *                   to compar as the third argument
 *
 * @pre  Neither base nor compar shall not be a null pointer (unless nmemb is
 * zero).
 * @pre  nmemb or size shall not be greater than RSIZE_MAX_MEM.
 *
 *    If comp indicates two elements as equivalent, their order in the
 *    resulting sorted array is unspecified.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_MEM.
 *
 * @return zero on success, non-zero if a runtime constraints violation was
 *    detected.
 *
 * @note
 *    Despite the name, neither C nor POSIX standards require this function to
 *    be implemented using \b quicksort or make any complexity or stability
 *    guarantees.  Unlike other bounds-checked functions, \c qsort_s does not
 *    treat arrays of zero size as a runtime constraint violation and instead
 *    returns successfully without altering the array (the other function that
 *    accepts arrays of zero size is \c bsearch_s).  Until \c qsort_s, users of
 *    \c qsort often used global variables to pass additional context to the
 *    comparison function.
 *
 *    This function is available under windows with a different API, no return
 *    type, and is not available with safeclib.
 *
 * @return  Zero on success, an errno_t on errors.
 * @retval  EOK         when operation is successful
 * @retval  -ESNULLP    when base * compar is NULL pointer and nmemb > 0
 * @retval  -ESLEMAX    when nmemb or size > RSIZE_MAX_MEM
 * @retval  -ESNOSPC    when nmemb*size > sizeof base
 *
 * @see
 *    bsearch_s()
 */

typedef int (*cmpfun)(const void *, const void *, void *);
static unsigned char tmp[256];
#ifdef HAVE___BUILTIN_CTZ
#define ntz(x) __builtin_ctz((x))
#else
static const char debruijn32[32] = {0,  1,  23, 2,  29, 24, 19, 3,  30, 27, 25,
                                    11, 20, 8,  4,  13, 31, 22, 28, 18, 26, 10,
                                    7,  12, 21, 17, 9,  6,  16, 5,  15, 14};
static inline int a_ctz_64(uint64_t x) {
    static const char debruijn64[64] = {
        0,  1,  2,  53, 3,  7,  54, 27, 4,  38, 41, 8,  34, 55, 48, 28,
        62, 5,  39, 46, 44, 42, 22, 9,  24, 35, 59, 56, 49, 18, 29, 11,
        63, 52, 6,  26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
        51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12};
    if (sizeof(long) < 8) {
        uint32_t y = x;
        if (!y) {
            y = x >> 32;
            return 32 + debruijn32[(y & -y) * 0x076be629 >> 27];
        } else {
            return debruijn32[(y & -y) * 0x076be629 >> 27];
        }
    } else {
        return debruijn64[(x & -x) * 0x022fdd63cc95386dull >> 58];
    }
}
static inline int a_ctz_l(unsigned long x) {
    if (sizeof(long) == 8)
        return a_ctz_64(x);
    return debruijn32[(x & -x) * 0x076be629 >> 27];
}
#define ntz(x) a_ctz_l((x))
#endif

static inline int pntz(size_t p[2]) {
    int r = ntz(p[0] - 1);
    if (r != 0 || (r = 8 * sizeof(size_t) + ntz(p[1])) != 8 * sizeof(size_t)) {
        return r;
    }
    return 0;
}

static void cycle(size_t width, unsigned char *ar[], int n) {
    size_t l;
    int i;

    if (n < 2)
        return;
    ar[n] = tmp;
    while (width) {
        l = sizeof(tmp) < width ? sizeof(tmp) : width;
        memcpy(ar[n], ar[0], l);
        for (i = 0; i < n; i++) {
            memcpy(ar[i], ar[i + 1], l);
            ar[i] += l;
        }
        width -= l;
    }
}

/* shl() and shr() need n > 0 */
static inline void shl(size_t p[2], int n) {
    if (n >= (int)(8 * sizeof(size_t))) {
        n -= 8 * sizeof(size_t);
        p[1] = p[0];
        p[0] = 0;
    }
    p[1] <<= n;
    p[1] |= p[0] >> (sizeof(size_t) * 8 - n);
    p[0] <<= n;
}

static inline void shr(size_t p[2], int n) {
    if (n >= (int)(8 * sizeof(size_t))) {
        n -= 8 * sizeof(size_t);
        p[0] = p[1];
        p[1] = 0;
    }
    p[0] >>= n;
    p[0] |= p[1] << (sizeof(size_t) * 8 - n);
    p[1] >>= n;
}

static void sift(unsigned char *head, size_t width, cmpfun cmp, int pshift,
                 size_t lp[], void *ctx) {
    unsigned char *rt, *lf;
    unsigned char *ar[14 * sizeof(size_t) + 1];
    int i = 1;

    ar[0] = head;
    while (pshift > 1) {
        rt = head - width;
        lf = head - width - lp[pshift - 2];

        if ((*cmp)(ar[0], lf, ctx) >= 0 && (*cmp)(ar[0], rt, ctx) >= 0) {
            break;
        }
        if ((*cmp)(lf, rt, ctx) >= 0) {
            ar[i++] = lf;
            head = lf;
            pshift -= 1;
        } else {
            ar[i++] = rt;
            head = rt;
            pshift -= 2;
        }
    }
    cycle(width, ar, i);
}

static void trinkle(unsigned char *head, size_t width, cmpfun cmp, size_t pp[2],
                    int pshift, int trusty, size_t lp[], void *ctx) {
    unsigned char *stepson, *rt, *lf;
    size_t p[2];
    unsigned char *ar[14 * sizeof(size_t) + 1];
    int i = 1;
    int trail;

    p[0] = pp[0];
    p[1] = pp[1];

    ar[0] = head;
    while (p[0] != 1 || p[1] != 0) {
        stepson = head - lp[pshift];
        if ((*cmp)(stepson, ar[0], ctx) <= 0) {
            break;
        }
        if (!trusty && pshift > 1) {
            rt = head - width;
            lf = head - width - lp[pshift - 2];
            if ((*cmp)(rt, stepson, ctx) >= 0 ||
                (*cmp)(lf, stepson, ctx) >= 0) {
                break;
            }
        }

        ar[i++] = stepson;
        head = stepson;
        trail = pntz(p);
        shr(p, trail);
        pshift += trail;
        trusty = 0;
    }
    if (!trusty) {
        cycle(width, ar, i);
        sift(head, width, cmp, pshift, lp, ctx);
    }
}

static void qsort_musl(void *base, size_t nel, size_t width, cmpfun cmp,
                       void *ctx) {
    size_t lp[12 * sizeof(size_t)];
    size_t i, size = width * nel;
    unsigned char *head, *high;
    size_t p[2] = {1, 0};
    int pshift = 1;
    int trail;

    if (!size)
        return;

    head = (unsigned char *)base;
    high = head + size - width;

    /* Precompute Leonardo numbers, scaled by element width */
    for (lp[0] = lp[1] = width, i = 2;
         (lp[i] = lp[i - 2] + lp[i - 1] + width) < size; i++)
        ;

    while (head < high) {
        if ((p[0] & 3) == 3) {
            sift(head, width, cmp, pshift, lp, ctx);
            shr(p, 2);
            pshift += 2;
        } else {
            if (lp[pshift - 1] >= (size_t)(high - head)) {
                trinkle(head, width, cmp, p, pshift, 0, lp, ctx);
            } else {
                sift(head, width, cmp, pshift, lp, ctx);
            }

            if (pshift == 1) {
                shl(p, 1);
                pshift = 0;
            } else {
                shl(p, pshift - 1);
                pshift = 1;
            }
        }

        p[0] |= 1;
        head += width;
    }

    trinkle(head, width, cmp, p, pshift, 0, lp, ctx);

    while (pshift != 1 || p[0] != 1 || p[1] != 0) {
        if (pshift <= 1) {
            trail = pntz(p);
            shr(p, trail);
            pshift += trail;
        } else {
            shl(p, 2);
            pshift -= 2;
            p[0] ^= 7;
            shr(p, 1);
            trinkle(head - lp[pshift] - width, width, cmp, p, pshift + 1, 1, lp,
                    ctx);
            shl(p, 1);
            p[0] |= 1;
            trinkle(head - width, width, cmp, p, pshift, 1, lp, ctx);
        }
        head -= width;
    }
}

#ifdef FOR_DOXYGEN
errno_t qsort_s(void *base, rsize_t nmemb, rsize_t size,
                int (*compar)(const void *k, const void *y,
                              void *context),
                void *context)
#else
EXPORT errno_t _qsort_s_chk(void *base, rsize_t nmemb, rsize_t size,
                            int (*compar)(const void *k, const void *y,
                                          void *context),
                            void *context, const size_t basebos)
#endif
{
    (void)basebos;
    if (likely(nmemb != 0)) {
        if (unlikely(base == NULL || compar == NULL)) {
            invoke_safe_str_constraint_handler("qsort_s: base or compar is null",
                                               NULL, ESNULLP);
            return RCNEGATE(ESNULLP);
        }
    }
    if (basebos == BOS_UNKNOWN) {
        if (unlikely(nmemb > RSIZE_MAX_MEM || size > RSIZE_MAX_MEM ||
                     ((basebos != BOS_UNKNOWN) && (basebos > RSIZE_MAX_MEM)))) {
            invoke_safe_str_constraint_handler("qsort_s: nmemb or size exceeds max",
                                               NULL, ESLEMAX);
            return RCNEGATE(ESLEMAX);
        }
        BND_CHK_PTR_BOUNDS(base, nmemb * size);
    } else {
        rsize_t basesz = nmemb * size;
        if (unlikely(basesz > basebos)) {
            invoke_safe_str_constraint_handler("qsort_s: nmemb*size exceeds sizeof base",
                                               NULL, ESNOSPC);
            return RCNEGATE(ESNOSPC);
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(basesz != basebos)) {
            handle_mem_bos_chk_warn("qsort_s", (void *)base, basesz, basebos);
#ifdef HAVE_ERROR_DMAX
            errno = ESLEWRNG;
            return NULL;
#endif
        }
#endif
    }

    qsort_musl(base, (size_t)nmemb, (size_t)size, compar, context);

    return EOK;
}

#endif /* MINGW_HAS_SECURE_API */
