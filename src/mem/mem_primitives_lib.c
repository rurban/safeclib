/*------------------------------------------------------------------
 * mem_primitives_lib.c - Unguarded Memory Copy Routines
 *
 * February 2005, Bo Berry
 * February 2018, Reini Urban
 * December 2018, Reini Urban
 *
 * Copyright (c) 2005-2009 Cisco Systems
 * Copyright (c) 2018 Reini Urban
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

#include "mem/mem_primitives_lib.h"

/*
 * mem_primitives_lib.c  provides unguarded memory routines
 * that are used by the safe_mem_library.   These routines
 * may also be used by an application, but the application
 * is responsible for all parameter validation and alignment.
 */

/**
 * @def mem_prim_set(dest,len,value)
 * @brief
 *    Sets len bytes starting at dest to the specified value
 *
 * @param[out] dest   pointer to memory that will be set to value
 * @param[in]  len    number of bytes to be set
 * @param[in]  value  byte value
 *
 * @note mem_prim_set provides a memory barrier for modern out-of-order CPU's
 *       to ensure a cache flush or at least a compiler barrier fallback to
 *       ensure that is not optimized away by optimizing compilers.
 */

#if __WORDSIZE != 64

void mem_prim_set(void *dest, uint32_t len, uint8_t value) {
    /* Avoid possible compiler optimization */
    /* See
     * https://wiki.sei.cmu.edu/confluence/display/c/MSC06-C.+Beware+of+compiler+optimizations
     */
    volatile uint8_t *dp;
    uint32_t count;
    uint32_t lcount;

    uint32_t *lp;
    uint32_t value32;

    count = len;

    dp = (volatile uint8_t *)dest;

    value32 = value | (value << 8) | (value << 16) | (value << 24);

    /*
     * First, do the few bytes to get uint32_t aligned.
     */
    for (; count && ((uintptr_t)dp & (sizeof(uint32_t) - 1)); count--) {
        *dp++ = value;
    }

    /*
     * Then do the uint32_t's, unrolled the loop for performance
     */
    GCC_DIAG_IGNORE(-Wcast-align)
    lp = (uint32_t *)dp;
    GCC_DIAG_RESTORE

    lcount = count >> 2;

    while (lcount != 0) {

        switch (lcount) {
        /* Here we do blocks of 16 words. Once the remaining count
         * drops below 16, take the fast track to finish up.
         * 4 * 16, 16 words = 64 byte per loop */
        default:
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            *lp++ = value32;
            lcount -= 16;
            break;

        case 15:
            *lp++ = value32; /* FALLTHRU */
        case 14:
            *lp++ = value32; /* FALLTHRU */
        case 13:
            *lp++ = value32; /* FALLTHRU */
        case 12:
            *lp++ = value32; /* FALLTHRU */
        case 11:
            *lp++ = value32; /* FALLTHRU */
        case 10:
            *lp++ = value32; /* FALLTHRU */
        case 9:
            *lp++ = value32; /* FALLTHRU */
        case 8:
            *lp++ = value32; /* FALLTHRU */

        case 7:
            *lp++ = value32; /* FALLTHRU */
        case 6:
            *lp++ = value32; /* FALLTHRU */
        case 5:
            *lp++ = value32; /* FALLTHRU */
        case 4:
            *lp++ = value32; /* FALLTHRU */
        case 3:
            *lp++ = value32; /* FALLTHRU */
        case 2:
            *lp++ = value32; /* FALLTHRU */
        case 1:
            *lp++ = value32; /* FALLTHRU */
            lcount = 0;
            break;
        }
    } /* end 16*word loop */

    /* remaining bytes */
    dp = (uint8_t *)lp;
    count &= (sizeof(uint32_t) - 1);
    for (; count; dp++, count--) {
        *dp = value;
    }

    return;
}

#else /* __WORDSIZE != 64 */
/* optimized for 64bit words */
void mem_prim_set(void *dest, uint32_t len, uint8_t value) {
    volatile uint8_t *dp;
    uint64_t count;
    uint64_t lcount;

    uint64_t *lp;
    uint64_t value64;

    count = len;

    dp = (volatile uint8_t *)dest;

    value64 = (uint64_t)value;
    if (value) { /* value is mostly 0 */
        /* spread the byte 8x over the qword */
        value64 |= (value64 << 8) | (value64 << 16) | (value64 << 24) |
                   (value64 << 32) | (value64 << 40) | (value64 << 48) |
                   (value64 << 56);
    }

    /* First, do the few bytes to get uint64_t aligned. (rarely needed) */
    for (; count && ((uintptr_t)dp & (sizeof(uint64_t) - 1)); count--) {
        *dp++ = value;
    }

    /* Then do the uint64_t's, unrolled the loop for performance. */
    GCC_DIAG_IGNORE(-Wcast-align)
    lp = (uint64_t *)dp;
    GCC_DIAG_RESTORE

    lcount = count >> 3; /* div 8 */

    while (lcount != 0) {

        switch (lcount) {
        /* Here we do blocks of 16.  Once the remaining count
         * drops below 16, take the fast track to finish up.
         * 8 * 16, 16 qwords = 128 byte per loop */
        default:
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            *lp++ = value64;
            lcount -= 16;
            break;

        case 15:
            *lp++ = value64; /* FALLTHRU */
        case 14:
            *lp++ = value64; /* FALLTHRU */
        case 13:
            *lp++ = value64; /* FALLTHRU */
        case 12:
            *lp++ = value64; /* FALLTHRU */
        case 11:
            *lp++ = value64; /* FALLTHRU */
        case 10:
            *lp++ = value64; /* FALLTHRU */
        case 9:
            *lp++ = value64; /* FALLTHRU */
        case 8:
            *lp++ = value64; /* FALLTHRU */

        case 7:
            *lp++ = value64; /* FALLTHRU */
        case 6:
            *lp++ = value64; /* FALLTHRU */
        case 5:
            *lp++ = value64; /* FALLTHRU */
        case 4:
            *lp++ = value64; /* FALLTHRU */
        case 3:
            *lp++ = value64; /* FALLTHRU */
        case 2:
            *lp++ = value64; /* FALLTHRU */
        case 1:
            *lp++ = value64; /* FALLTHRU */
            lcount = 0;
            break;
        }
    } /* end 16*qword loop */

    /* remaining bytes */
    dp = (uint8_t *)lp;
    count &= (sizeof(uint64_t) - 1);
    for (; count; dp++, count--) {
        *dp = value;
    }

    return;
}

#endif /* __WORDSIZE != 64 */

/**
 * @brief
 *    Sets len uint16_t's starting at dest to the specified value.
 *    Pointers must meet system alignment requirements.
 *
 * @param[out] dest  pointer to memory that will be set to value
 * @param[in]  len   number of uint16_t's to be set
 * @param[in]  value uint16_t value
 *
 */
void mem_prim_set16(uint16_t *dest, uint32_t len, uint16_t value) {

    volatile uint16_t *dp = dest;
    while (len != 0) {

        switch (len) {
        /*
         * Here we do blocks of 16.  Once the remaining count
         * drops below 16, take the fast track to finish up.
         */
        default:
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            len -= 16;
            break;

        case 15:
            *dp++ = value; /* FALLTHRU */
        case 14:
            *dp++ = value; /* FALLTHRU */
        case 13:
            *dp++ = value; /* FALLTHRU */
        case 12:
            *dp++ = value; /* FALLTHRU */
        case 11:
            *dp++ = value; /* FALLTHRU */
        case 10:
            *dp++ = value; /* FALLTHRU */
        case 9:
            *dp++ = value; /* FALLTHRU */
        case 8:
            *dp++ = value; /* FALLTHRU */

        case 7:
            *dp++ = value; /* FALLTHRU */
        case 6:
            *dp++ = value; /* FALLTHRU */
        case 5:
            *dp++ = value; /* FALLTHRU */
        case 4:
            *dp++ = value; /* FALLTHRU */
        case 3:
            *dp++ = value; /* FALLTHRU */
        case 2:
            *dp++ = value; /* FALLTHRU */
        case 1:
            *dp++ = value; /* FALLTHRU */
            len = 0;
            break;
        }
    } /* end while */

    MEMORY_BARRIER;
    return;
}

/**
 * @brief
 *    Sets len uint32_t's starting at dest to the specified value.
 *    Pointers must meet system alignment requirements.
 *
 * @param[out] dest   pointer to memory that will be set to value
 * @param[in]  len    number of uint32_t's to be set
 * @param[in]  value  uint32_t value
 *
 *
 */
void mem_prim_set32(uint32_t *dest, uint32_t len, uint32_t value) {
    volatile uint32_t *dp = dest;
    while (len != 0) {

        switch (len) {
        /*
         * Here we do blocks of 16.  Once the remaining count
         * drops below 16, take the fast track to finish up.
         */
        default:
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            *dp++ = value;
            len -= 16;
            break;

        case 15:
            *dp++ = value; /* FALLTHRU */
        case 14:
            *dp++ = value; /* FALLTHRU */
        case 13:
            *dp++ = value; /* FALLTHRU */
        case 12:
            *dp++ = value; /* FALLTHRU */
        case 11:
            *dp++ = value; /* FALLTHRU */
        case 10:
            *dp++ = value; /* FALLTHRU */
        case 9:
            *dp++ = value; /* FALLTHRU */
        case 8:
            *dp++ = value; /* FALLTHRU */

        case 7:
            *dp++ = value; /* FALLTHRU */
        case 6:
            *dp++ = value; /* FALLTHRU */
        case 5:
            *dp++ = value; /* FALLTHRU */
        case 4:
            *dp++ = value; /* FALLTHRU */
        case 3:
            *dp++ = value; /* FALLTHRU */
        case 2:
            *dp++ = value; /* FALLTHRU */
        case 1:
            *dp++ = value; /* FALLTHRU */
            len = 0;
            break;
        }
    } /* end while */

    MEMORY_BARRIER;
    return;
}

/**
 * @brief
 *    Moves at most len of bytes from src to dest.
 *    Dest may overlap with src.
 *
 * @param[out] dest     pointer to the memory that will be replaced by src.
 * @param[in]  src      pointer to the memory that will be copied to dest
 * @param[in]  len      maximum number bytes of src that can be copied
 *
 */

#if __WORDSIZE != 64

void mem_prim_move(void *dest, const void *src, uint32_t len) {

#define wsize sizeof(uint32_t)
#define wmask (wsize - 1)

    uint8_t *dp = (uint8_t *)dest;
    const uint8_t *sp = (uint8_t *)src;

    uint32_t tsp;

    /*
     * Determine if we need to copy forward or backward (overlap)
     */
    if ((uintptr_t)dp < (uintptr_t)sp) {
        /*
         * Copy forward.
         */

        /*
         * get a working copy of src for bit operations
         */
        tsp = (uintptr_t)sp;

        /*
         * Try to align both operands.  This cannot be done
         * unless the low bits match.
         */
        if ((tsp | (uintptr_t)dp) & wmask) {
            /*
             * determine how many bytes to copy to align operands
             */
            if ((tsp ^ (uintptr_t)dp) & wmask || len < wsize) {
                tsp = len;
            } else {
                tsp = wsize - (tsp & wmask);
            }

            len -= tsp;

            /*
             * make the alignment
             */
            do {
                *dp++ = *sp++;
            } while (--tsp);
        }

        /*
         * Now copy, then mop up any trailing bytes.
         */
        tsp = len / wsize;

        if (tsp > 0) {

            do {
                GCC_DIAG_IGNORE(-Wcast-align)
                *(uint32_t *)dp = *(uint32_t *)sp;
                GCC_DIAG_RESTORE

                sp += wsize;
                dp += wsize;
            } while (--tsp);
        }

        /*
         * copy over the remaining bytes and we're done
         */
        tsp = len & wmask;

        if (tsp > 0) {
            do {
                *dp++ = *sp++;
            } while (--tsp);
        }
    } else {
        /*
         * This section is used to copy backwards, to handle any
         * overlap.  The alignment requires (tps&wmask) bytes to
         * align.
         */

        /*
         * go to end of the memory to copy
         */
        sp += len;
        dp += len;

        /*
         * get a working copy of src for bit operations
         */
        tsp = (uintptr_t)sp;

        /*
         * Try to align both operands.
         */
        if ((tsp | (uintptr_t)dp) & wmask) {

            if ((tsp ^ (uintptr_t)dp) & wmask || len <= wsize) {
                tsp = len;
            } else {
                tsp &= wmask;
            }

            len -= tsp;

            /*
             * make the alignment
             */
            do {
                *--dp = *--sp;
            } while (--tsp);
        }

        /*
         * Now copy in uint32_t units, then mop up any trailing bytes.
         */
        tsp = len / wsize;

        if (tsp > 0) {
            do {
                sp -= wsize;
                dp -= wsize;

                GCC_DIAG_IGNORE(-Wcast-align)
                *(uint32_t *)dp = *(uint32_t *)sp;
                GCC_DIAG_RESTORE
            } while (--tsp);
        }

        /*
         * copy over the remaining bytes and we're done
         */
        tsp = len & wmask;
        if (tsp > 0) {
            tsp = len & wmask;
            do {
                *--dp = *--sp;
            } while (--tsp);
        }
    }

    return;
}

#else /* __WORDSIZE != 64 */

/* moves memory, optimized for 64bit words */
void mem_prim_move(void *dest, const void *src, uint32_t len) {
#undef wsize
#undef wmask
#define wsize sizeof(uint64_t)
#define wmask (wsize - 1)

    uint8_t *dp = (uint8_t *)dest;
    const uint8_t *sp = (uint8_t *)src;

    uint64_t tsp;

    /*
     * Determine if we need to copy forward or backward (overlap)
     */
    if ((uintptr_t)dp < (uintptr_t)sp) {
        /*
         * Copy forward.
         */

        /*
         * get a working copy of src for bit operations
         */
        tsp = (uintptr_t)sp;

        /*
         * Try to align both operands.  This cannot be done
         * unless the low bits match.
         */
        if ((tsp | (uintptr_t)dp) & wmask) {
            /*
             * determine how many bytes to copy to align operands
             */
            if ((tsp ^ (uintptr_t)dp) & wmask || len < wsize) {
                tsp = len;
            } else {
                tsp = wsize - (tsp & wmask);
            }

            len -= tsp;

            /*
             * make the alignment
             */
            do {
                *dp++ = *sp++;
            } while (--tsp);
        }

        /*
         * Now copy, then mop up any trailing bytes.
         */
        tsp = len / wsize;

        if (tsp > 0) {

            do {
                GCC_DIAG_IGNORE(-Wcast-align)
                *(uint64_t *)dp = *(uint64_t *)sp;
                GCC_DIAG_RESTORE

                sp += wsize;
                dp += wsize;
            } while (--tsp);
        }

        /*
         * copy over the remaining bytes and we're done
         */
        tsp = len & wmask;

        if (tsp > 0) {
            do {
                *dp++ = *sp++;
            } while (--tsp);
        }
    } else {
        /*
         * This section is used to copy backwards, to handle any
         * overlap.  The alignment requires (tps&wmask) bytes to
         * align.
         */

        /*
         * go to end of the memory to copy
         */
        sp += len;
        dp += len;

        /*
         * get a working copy of src for bit operations
         */
        tsp = (uintptr_t)sp;

        /*
         * Try to align both operands.
         */
        if ((tsp | (uintptr_t)dp) & wmask) {

            if ((tsp ^ (uintptr_t)dp) & wmask || len <= wsize) {
                tsp = len;
            } else {
                tsp &= wmask;
            }

            len -= tsp;

            /*
             * make the alignment
             */
            do {
                *--dp = *--sp;
            } while (--tsp);
        }

        /*
         * Now copy in uint64_t units, then mop up any trailing bytes.
         */
        tsp = len / wsize;

        if (tsp > 0) {
            do {
                sp -= wsize;
                dp -= wsize;

                GCC_DIAG_IGNORE(-Wcast-align)
                *(uint64_t *)dp = *(uint64_t *)sp;
                GCC_DIAG_RESTORE
            } while (--tsp);
        }

        /*
         * copy over the remaining bytes and we're done
         */
        tsp = len & wmask;
        if (tsp > 0) {
            tsp = len & wmask;
            do {
                *--dp = *--sp;
            } while (--tsp);
        }
    }

    return;
}

#endif /* __WORDSIZE != 64 */

/**
 * @brief
 *    Moves at most len of uint8_t's from src to dest.
 *    The destination may overlap with source.
 *
 * @param[out] dest    pointer to the memory that will be replaced by src
 * @param[in]  src     pointer to the memory that will be copied to dest
 * @param[in]  len     maximum number uint8_t of src that can be copied
 *
 */

EXPORT void mem_prim_move8(uint8_t *dest, const uint8_t *src, uint32_t len) {
    uint8_t *dp = dest;
    const uint8_t *sp = src;
    /*
     * Determine if we need to copy forward or backward (overlap)
     */
    if (dp < sp) {
        /*
         * Copy forward.
         */

        while (len != 0) {

            switch (len) {
            /*
             * Here we do blocks of 16.  Once the remaining count
             * drops below 16, take the fast track to finish up.
             */
            default:
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                len -= 16;
                break;

            case 15:
                *dp++ = *sp++; /* FALLTHRU */
            case 14:
                *dp++ = *sp++; /* FALLTHRU */
            case 13:
                *dp++ = *sp++; /* FALLTHRU */
            case 12:
                *dp++ = *sp++; /* FALLTHRU */
            case 11:
                *dp++ = *sp++; /* FALLTHRU */
            case 10:
                *dp++ = *sp++; /* FALLTHRU */
            case 9:
                *dp++ = *sp++; /* FALLTHRU */
            case 8:
                *dp++ = *sp++; /* FALLTHRU */

            case 7:
                *dp++ = *sp++; /* FALLTHRU */
            case 6:
                *dp++ = *sp++; /* FALLTHRU */
            case 5:
                *dp++ = *sp++; /* FALLTHRU */
            case 4:
                *dp++ = *sp++; /* FALLTHRU */
            case 3:
                *dp++ = *sp++; /* FALLTHRU */
            case 2:
                *dp++ = *sp++; /* FALLTHRU */
            case 1:
                *dp++ = *sp++; /* FALLTHRU */
                len = 0;
                break;
            }
        } /* end while */
    } else {
        /*
         * This section is used to copy backwards, to handle any
         * overlap.  The alignment requires (tps&wmask) bytes to
         * align.
         */

        /*
         * go to end of the memory to copy
         */
        sp += len;
        dp += len;

        while (len != 0) {

            switch (len) {
            /*
             * Here we do blocks of 16.  Once the remaining count
             * drops below 16, take the fast track to finish up.
             */
            default:
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                len -= 16;
                break;

            case 15:
                *--dp = *--sp; /* FALLTHRU */
            case 14:
                *--dp = *--sp; /* FALLTHRU */
            case 13:
                *--dp = *--sp; /* FALLTHRU */
            case 12:
                *--dp = *--sp; /* FALLTHRU */
            case 11:
                *--dp = *--sp; /* FALLTHRU */
            case 10:
                *--dp = *--sp; /* FALLTHRU */
            case 9:
                *--dp = *--sp; /* FALLTHRU */
            case 8:
                *--dp = *--sp; /* FALLTHRU */

            case 7:
                *--dp = *--sp; /* FALLTHRU */
            case 6:
                *--dp = *--sp; /* FALLTHRU */
            case 5:
                *--dp = *--sp; /* FALLTHRU */
            case 4:
                *--dp = *--sp; /* FALLTHRU */
            case 3:
                *--dp = *--sp; /* FALLTHRU */
            case 2:
                *--dp = *--sp; /* FALLTHRU */
            case 1:
                *--dp = *--sp; /* FALLTHRU */
                len = 0;
                break;
            }
        } /* end while */
    }

    return;
}
#ifdef __KERNEL__
#include <linux/export.h>
EXPORT_SYMBOL(mem_prim_move8);
#endif

/**
 * @brief
 *    Moves at most len uint16_t's from src to dest.
 *    The destination may overlap with source.
 *
 * @param[out] dest    pointer to the memory that will be replaced by src.
 * @param[in]  src     pointer to the memory that will be copied to dest
 * @param[in]  len     maximum number uint16_t of src that can be copied
 *
 */
void mem_prim_move16(uint16_t *dest, const uint16_t *src, uint32_t len) {
    uint16_t *dp = dest;
    const uint16_t *sp = src;
    /*
     * Determine if we need to copy forward or backward (overlap)
     */
    if (dp < sp) {
        /*
         * Copy forward.
         */

        while (len != 0) {

            switch (len) {
            /*
             * Here we do blocks of 16.  Once the remaining count
             * drops below 16, take the fast track to finish up.
             */
            default:
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                len -= 16;
                break;

            case 15:
                *dp++ = *sp++; /* FALLTHRU */
            case 14:
                *dp++ = *sp++; /* FALLTHRU */
            case 13:
                *dp++ = *sp++; /* FALLTHRU */
            case 12:
                *dp++ = *sp++; /* FALLTHRU */
            case 11:
                *dp++ = *sp++; /* FALLTHRU */
            case 10:
                *dp++ = *sp++; /* FALLTHRU */
            case 9:
                *dp++ = *sp++; /* FALLTHRU */
            case 8:
                *dp++ = *sp++; /* FALLTHRU */

            case 7:
                *dp++ = *sp++; /* FALLTHRU */
            case 6:
                *dp++ = *sp++; /* FALLTHRU */
            case 5:
                *dp++ = *sp++; /* FALLTHRU */
            case 4:
                *dp++ = *sp++; /* FALLTHRU */
            case 3:
                *dp++ = *sp++; /* FALLTHRU */
            case 2:
                *dp++ = *sp++; /* FALLTHRU */
            case 1:
                *dp++ = *sp++; /* FALLTHRU */
                len = 0;
                break;
            }
        } /* end while */
    } else {
        /*
         * This section is used to copy backwards, to handle any
         * overlap.  The alignment requires (tps&wmask) bytes to
         * align.
         */

        /*
         * go to end of the memory to copy
         */
        sp += len;
        dp += len;

        while (len != 0) {

            switch (len) {
            /*
             * Here we do blocks of 16.  Once the remaining count
             * drops below 16, take the fast track to finish up.
             */
            default:
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                len -= 16;
                break;

            case 15:
                *--dp = *--sp; /* FALLTHRU */
            case 14:
                *--dp = *--sp; /* FALLTHRU */
            case 13:
                *--dp = *--sp; /* FALLTHRU */
            case 12:
                *--dp = *--sp; /* FALLTHRU */
            case 11:
                *--dp = *--sp; /* FALLTHRU */
            case 10:
                *--dp = *--sp; /* FALLTHRU */
            case 9:
                *--dp = *--sp; /* FALLTHRU */
            case 8:
                *--dp = *--sp; /* FALLTHRU */

            case 7:
                *--dp = *--sp; /* FALLTHRU */
            case 6:
                *--dp = *--sp; /* FALLTHRU */
            case 5:
                *--dp = *--sp; /* FALLTHRU */
            case 4:
                *--dp = *--sp; /* FALLTHRU */
            case 3:
                *--dp = *--sp; /* FALLTHRU */
            case 2:
                *--dp = *--sp; /* FALLTHRU */
            case 1:
                *--dp = *--sp; /* FALLTHRU */
                len = 0;
                break;
            }
        } /* end while */
    }

    return;
}

/**
 * @brief
 *    Moves at most len of uint32_t's from src to dest.
 *    The destination may overlap with source.
 *
 * @param[out] dest    pointer to the memory that will be replaced by src.
 * @param[in]  src     pointer to the memory that will be copied to dest
 * @param[in]  len     maximum number uint32_t of sp that can be copied
 *
 */
void mem_prim_move32(uint32_t *dest, const uint32_t *src, uint32_t len) {
    uint32_t *dp = dest;
    const uint32_t *sp = src;
    /*
     * Determine if we need to copy forward or backward (overlap)
     */
    if (dp < sp) {
        /*
         * Copy forward.
         */

        while (len != 0) {

            switch (len) {
            /*
             * Here we do blocks of 16.  Once the remaining count
             * drops below 16, take the fast track to finish up.
             */
            default:
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                *dp++ = *sp++;
                len -= 16;
                break;

            case 15:
                *dp++ = *sp++; /* FALLTHRU */
            case 14:
                *dp++ = *sp++; /* FALLTHRU */
            case 13:
                *dp++ = *sp++; /* FALLTHRU */
            case 12:
                *dp++ = *sp++; /* FALLTHRU */
            case 11:
                *dp++ = *sp++; /* FALLTHRU */
            case 10:
                *dp++ = *sp++; /* FALLTHRU */
            case 9:
                *dp++ = *sp++; /* FALLTHRU */
            case 8:
                *dp++ = *sp++; /* FALLTHRU */

            case 7:
                *dp++ = *sp++; /* FALLTHRU */
            case 6:
                *dp++ = *sp++; /* FALLTHRU */
            case 5:
                *dp++ = *sp++; /* FALLTHRU */
            case 4:
                *dp++ = *sp++; /* FALLTHRU */
            case 3:
                *dp++ = *sp++; /* FALLTHRU */
            case 2:
                *dp++ = *sp++; /* FALLTHRU */
            case 1:
                *dp++ = *sp++; /* FALLTHRU */
                len = 0;
                break;
            }
        } /* end while */
    } else {
        /*
         * This section is used to copy backwards, to handle any
         * overlap.
         */

        /*
         * go to end of the memory to copy
         */
        sp += len;
        dp += len;

        while (len != 0) {

            switch (len) {
            /*
             * Here we do blocks of 16.  Once the remaining count
             * drops below 16, take the fast track to finish up.
             */
            default:
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                *--dp = *--sp;
                len -= 16;
                break;

            case 15:
                *--dp = *--sp; /* FALLTHRU */
            case 14:
                *--dp = *--sp; /* FALLTHRU */
            case 13:
                *--dp = *--sp; /* FALLTHRU */
            case 12:
                *--dp = *--sp; /* FALLTHRU */
            case 11:
                *--dp = *--sp; /* FALLTHRU */
            case 10:
                *--dp = *--sp; /* FALLTHRU */
            case 9:
                *--dp = *--sp; /* FALLTHRU */
            case 8:
                *--dp = *--sp; /* FALLTHRU */

            case 7:
                *--dp = *--sp; /* FALLTHRU */
            case 6:
                *--dp = *--sp; /* FALLTHRU */
            case 5:
                *--dp = *--sp; /* FALLTHRU */
            case 4:
                *--dp = *--sp; /* FALLTHRU */
            case 3:
                *--dp = *--sp; /* FALLTHRU */
            case 2:
                *--dp = *--sp; /* FALLTHRU */
            case 1:
                *--dp = *--sp; /* FALLTHRU */
                len = 0;
                break;
            }
        } /* end while */
    }

    return;
}

#ifdef ENABLE_CACHE_FLUSH
/*
 * Write the cache lines of [p, p+len) back to DRAM, so that the old
 * contents of a cleared secret do not survive there (cold boot attacks,
 * hibernation or crash dumps, DMA from non-coherent devices). Expensive.
 * x86: clflush + mfence. ARM64: dc cvac + dsb. Elsewhere only the
 * MEMORY_BARRIER of the caller.
 */
#if defined(__x86_64__) || defined(_M_X64) || defined(_M_IX86) ||              \
    (defined(__i386__) && defined(__SSE2__))
#include <emmintrin.h>
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <cpuid.h>
#endif

/* clflush line size from CPUID.1:EBX[15:8] * 8, cached. The unsynchronized
   cache is benign, all threads compute the same value. */
static size_t flush_linesize(void) {
    static size_t cached;
    size_t line = cached;
    if (line)
        return line;
    line = 64;
#if defined(_MSC_VER)
    {
        int r[4];
        __cpuid(r, 1);
        if (r[3] & (1 << 19) && ((r[1] >> 8) & 0xff))
            line = (size_t)((r[1] >> 8) & 0xff) * 8;
    }
#elif defined(__GNUC__)
    {
        unsigned int a, b, c, d;
        if (__get_cpuid(1, &a, &b, &c, &d) && (d & (1u << 19)) &&
            ((b >> 8) & 0xff))
            line = (size_t)((b >> 8) & 0xff) * 8;
    }
#endif
    cached = line;
    return line;
}

void mem_prim_flush(const void *ptr, size_t len) {
    if (len) {
        const size_t line = flush_linesize();
        uintptr_t p = (uintptr_t)ptr & ~(uintptr_t)(line - 1);
        const uintptr_t end = (uintptr_t)ptr + len;
        for (; p < end; p += line)
            _mm_clflush((const void *)p);
        _mm_mfence();
    }
}
#elif defined(__aarch64__) && defined(__GNUC__)
void mem_prim_flush(const void *ptr, size_t len) {
    if (len) {
        unsigned long ctr;
        size_t line;
        uintptr_t p, end;
        __asm__ volatile("mrs %0, ctr_el0" : "=r"(ctr));
        line = (size_t)4 << ((ctr >> 16) & 0xf); /* CTR_EL0.DminLine */
        p = (uintptr_t)ptr & ~(uintptr_t)(line - 1);
        end = (uintptr_t)ptr + len;
        for (; p < end; p += line)
            __asm__ volatile("dc cvac, %0" : : "r"(p) : "memory");
        __asm__ volatile("dsb sy" : : : "memory");
    }
}
#else
void mem_prim_flush(const void *ptr, size_t len) {
    (void)ptr;
    (void)len;
    MEMORY_BARRIER;
}
#endif
#endif /* ENABLE_CACHE_FLUSH */
