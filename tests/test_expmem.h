/*------------------------------------------------------------------
 * test_expmem.h - Compare memory buffers
 *
 * 2018  Reini Urban
 *
 * Copyright (c) 2018 by Reini Urban
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

#ifndef __TEST_EXPMEM_H__
#define __TEST_EXPMEM_H__

#include "safe_mem_lib.h"

#ifdef __KERNEL__

#define EXPMEM(mem, from, to, what, size)

#else

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

void _expmem(char *mem, int from, int to, int what, int size, int *errp,
             const char *f, const unsigned l);
#define EXPMEM(mem, from, to, what, size)                                      \
    _expmem((char *)mem, (from), (to), (what), (size), &errs, __FUNCTION__,    \
            __LINE__)

void _expmem(char *mem, int from, int to, int what, int size, int *errp,
             const char *f, const unsigned l) {
    int len = (to - from) * size;
    void *mem2 = calloc(len, 1);
    if (what != 0) {
        if (size == 1) {
            memset(mem2, what, len);
        } else if (size == 2) {
            int i;
            uint16_t *mem16 = (uint16_t *)mem2;
            for (i = 0; i < to - from; i++) {
                mem16[i] = (uint16_t)what;
            }
        } else if (size == 4) {
            int i;
            uint32_t *mem32 = (uint32_t *)mem2;
            for (i = 0; i < to - from; i++) {
                mem32[i] = (uint32_t)what;
            }
        } else {
            printf("%s %u  Invalid size %d - Error\n", f, l, size);
        }
    }
    if (memcmp(&mem[from * size], mem2, size) != 0) {
        debug_printf("%s %u  Expected \"%s\", got \"%s\" \n", f, l,
                     (char *)mem2, (char *)&mem[from * size]);
        (*errp)++;
    }
    free(mem2);
}

#endif /* __KERNEL__ */
#endif /* __TEST_EXPMEM_H__ */
