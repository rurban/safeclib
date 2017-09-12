/*------------------------------------------------------------------
 * mem_primitives_lib.c - Unguarded Memory Copy Routines
 *
 * February 2005, Bo Berry
 *
 * Copyright (c) 2005-2009 Cisco Systems
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

#include "mem_primitives_lib.h"

/*
 * mem_primitives_lib.c  provides unguarded memory routines
 * that are used by the safe_mem_library.   These routines
 * may also be used by an application, but the application
 * is responsible for all parameter validation and alignment.
 */

/**
 * @brief
 *    Sets len bytes starting at dest to the specified value
 *
 * @param[out] dest   pointer to memory that will be set to value
 * @param[in]  len    number of bytes to be set
 * @param[in]  value  byte value
 *
 */
void
mem_prim_set (void *dest, uint32_t len, uint8_t value)
{
    uint8_t *dp;
    uint32_t count;
    uint32_t lcount;

    uint32_t *lp;
    uint32_t value32;

    count = len;

    dp = (uint8_t*) dest;

    value32 = value | (value << 8) | (value << 16) | (value << 24);

    /*
     * First, do the few bytes to get uint32_t aligned.
     */
    for (; count && ( (uintptr_t)dp & (sizeof(uint32_t)-1) ); count--) {
        *dp++ = value;
    }

    /*
     * Then do the uint32_ts, unrolled the loop for performance
     */
    lp = (uint32_t *)dp;
    lcount = count >> 2;

    while (lcount != 0) {

        switch (lcount) {
        /*
         * Here we do blocks of 8.  Once the remaining count
         * drops below 8, take the fast track to finish up.
         */
        default:
            *lp++ = value32; *lp++ = value32; *lp++ = value32; *lp++ = value32;
            *lp++ = value32; *lp++ = value32; *lp++ = value32; *lp++ = value32;
            *lp++ = value32; *lp++ = value32; *lp++ = value32; *lp++ = value32;
            *lp++ = value32; *lp++ = value32; *lp++ = value32; *lp++ = value32;
            lcount -= 16;
            break;

        case 15:  *lp++ = value32; /* FALLTHRU */
        case 14:  *lp++ = value32; /* FALLTHRU */
        case 13:  *lp++ = value32; /* FALLTHRU */
        case 12:  *lp++ = value32; /* FALLTHRU */
        case 11:  *lp++ = value32; /* FALLTHRU */
        case 10:  *lp++ = value32; /* FALLTHRU */
        case 9:  *lp++ = value32;  /* FALLTHRU */
        case 8:  *lp++ = value32;  /* FALLTHRU */

        case 7:  *lp++ = value32;  /* FALLTHRU */
        case 6:  *lp++ = value32;  /* FALLTHRU */
        case 5:  *lp++ = value32;  /* FALLTHRU */
        case 4:  *lp++ = value32;  /* FALLTHRU */
        case 3:  *lp++ = value32;  /* FALLTHRU */
        case 2:  *lp++ = value32;  /* FALLTHRU */
        case 1:  *lp++ = value32;  /* FALLTHRU */
            lcount = 0;
            break;
        }
    } /* end while */


    dp = (uint8_t *)lp;

    /*
     * compute the number of remaining bytes
     */
    count &= (sizeof(uint32_t)-1);

    /*
     * remaining bytes
     */
    for (; count; dp++, count--) {
        *dp = value;
    }

    return;
}


/**
 * @brief
 *    Sets len uint16_t's starting at dest to the specified value.
 *    Pointers must meet system alignment requirements.
 *
 * @param[out] dest  pointer to memory that will be set to value
 * @param[in]  len   number of uint16_ts to be set
 * @param[in]  value uint16_t value
 *
 */
void
mem_prim_set16 (uint16_t *dest, uint32_t len, uint16_t value)
{

    uint16_t *dp = dest;
    while (len != 0) {

        switch (len) {
        /*
         * Here we do blocks of 8.  Once the remaining count
         * drops below 8, take the fast track to finish up.
         */
        default:
            *dp++ = value; *dp++ = value; *dp++ = value; *dp++ = value;
            *dp++ = value; *dp++ = value; *dp++ = value; *dp++ = value;
            *dp++ = value; *dp++ = value; *dp++ = value; *dp++ = value;
            *dp++ = value; *dp++ = value; *dp++ = value; *dp++ = value;
            len -= 16;
            break;

        case 15:  *dp++ = value; /* FALLTHRU */
        case 14:  *dp++ = value; /* FALLTHRU */
        case 13:  *dp++ = value; /* FALLTHRU */
        case 12:  *dp++ = value; /* FALLTHRU */
        case 11:  *dp++ = value; /* FALLTHRU */
        case 10:  *dp++ = value; /* FALLTHRU */
        case 9:   *dp++ = value; /* FALLTHRU */
        case 8:   *dp++ = value; /* FALLTHRU */

        case 7:   *dp++ = value; /* FALLTHRU */
        case 6:   *dp++ = value; /* FALLTHRU */
        case 5:   *dp++ = value; /* FALLTHRU */
        case 4:   *dp++ = value; /* FALLTHRU */
        case 3:   *dp++ = value; /* FALLTHRU */
        case 2:   *dp++ = value; /* FALLTHRU */
        case 1:   *dp++ = value; /* FALLTHRU */
            len = 0;
            break;
        }
    } /* end while */

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
void
mem_prim_set32 (uint32_t *dest, uint32_t len, uint32_t value)
{
    uint32_t *dp = dest;
    while (len != 0) {

        switch (len) {
        /*
         * Here we do blocks of 8.  Once the remaining count
         * drops below 8, take the fast track to finish up.
         */
        default:
            *dp++ = value; *dp++ = value; *dp++ = value; *dp++ = value;
            *dp++ = value; *dp++ = value; *dp++ = value; *dp++ = value;
            *dp++ = value; *dp++ = value; *dp++ = value; *dp++ = value;
            *dp++ = value; *dp++ = value; *dp++ = value; *dp++ = value;
            len -= 16;
            break;

        case 15:  *dp++ = value; /* FALLTHRU */
        case 14:  *dp++ = value; /* FALLTHRU */
        case 13:  *dp++ = value; /* FALLTHRU */
        case 12:  *dp++ = value; /* FALLTHRU */
        case 11:  *dp++ = value; /* FALLTHRU */
        case 10:  *dp++ = value; /* FALLTHRU */
        case 9:   *dp++ = value; /* FALLTHRU */
        case 8:   *dp++ = value; /* FALLTHRU */

        case 7:   *dp++ = value; /* FALLTHRU */
        case 6:   *dp++ = value; /* FALLTHRU */
        case 5:   *dp++ = value; /* FALLTHRU */
        case 4:   *dp++ = value; /* FALLTHRU */
        case 3:   *dp++ = value; /* FALLTHRU */
        case 2:   *dp++ = value; /* FALLTHRU */
        case 1:   *dp++ = value; /* FALLTHRU */
            len = 0;
            break;
        }
    } /* end while */

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
void
mem_prim_move (void *dest, const void *src, uint32_t len)
{

#define wsize   sizeof(uint32_t)
#define wmask   (wsize - 1)

    uint8_t *dp = (uint8_t*) dest;
    const uint8_t *sp = (uint8_t*) src;

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
                *(uint32_t *)dp = *(uint32_t *)sp;

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

                *(uint32_t *)dp = *(uint32_t *)sp;
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


/**
 * @brief
 *    Moves at most len of uint8_t's from src to dest.
 *    The destination may overlap with source.
 *
 * @param[out] dest    pointer to the memory that will be replaced by src
 * @param[in]  src     pointer to the memory that will be copied to dest
 * @param[in]  len     maximum number uint8_t of src that can be copied
 *
 *
 */
void
mem_prim_move8 (uint8_t *dest, const uint8_t *src, uint32_t len)
{
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
              * Here we do blocks of 8.  Once the remaining count
              * drops below 8, take the fast track to finish up.
              */
             default:
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  len -= 16;
                  break;

             case 15:  *dp++ = *sp++; /* FALLTHRU */
             case 14:  *dp++ = *sp++; /* FALLTHRU */
             case 13:  *dp++ = *sp++; /* FALLTHRU */
             case 12:  *dp++ = *sp++; /* FALLTHRU */
             case 11:  *dp++ = *sp++; /* FALLTHRU */
             case 10:  *dp++ = *sp++; /* FALLTHRU */
             case 9:   *dp++ = *sp++; /* FALLTHRU */
             case 8:   *dp++ = *sp++; /* FALLTHRU */

             case 7:   *dp++ = *sp++; /* FALLTHRU */
             case 6:   *dp++ = *sp++; /* FALLTHRU */
             case 5:   *dp++ = *sp++; /* FALLTHRU */
             case 4:   *dp++ = *sp++; /* FALLTHRU */
             case 3:   *dp++ = *sp++; /* FALLTHRU */
             case 2:   *dp++ = *sp++; /* FALLTHRU */
             case 1:   *dp++ = *sp++; /* FALLTHRU */
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
             * Here we do blocks of 8.  Once the remaining count
             * drops below 8, take the fast track to finish up.
             */
            default:
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 len -= 16;
                 break;

            case 15:  *--dp = *--sp; /* FALLTHRU */
            case 14:  *--dp = *--sp; /* FALLTHRU */
            case 13:  *--dp = *--sp; /* FALLTHRU */
            case 12:  *--dp = *--sp; /* FALLTHRU */
            case 11:  *--dp = *--sp; /* FALLTHRU */
            case 10:  *--dp = *--sp; /* FALLTHRU */
            case 9:   *--dp = *--sp; /* FALLTHRU */
            case 8:   *--dp = *--sp; /* FALLTHRU */

            case 7:   *--dp = *--sp; /* FALLTHRU */
            case 6:   *--dp = *--sp; /* FALLTHRU */
            case 5:   *--dp = *--sp; /* FALLTHRU */
            case 4:   *--dp = *--sp; /* FALLTHRU */
            case 3:   *--dp = *--sp; /* FALLTHRU */
            case 2:   *--dp = *--sp; /* FALLTHRU */
            case 1:   *--dp = *--sp; /* FALLTHRU */
                len = 0;
                break;
            }
        } /* end while */
    }

    return;
}


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
void
mem_prim_move16 (uint16_t *dest, const uint16_t *src, uint32_t len)
{
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
              * Here we do blocks of 8.  Once the remaining count
              * drops below 8, take the fast track to finish up.
              */
             default:
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  len -= 16;
                  break;

             case 15:  *dp++ = *sp++; /* FALLTHRU */
             case 14:  *dp++ = *sp++; /* FALLTHRU */
             case 13:  *dp++ = *sp++; /* FALLTHRU */
             case 12:  *dp++ = *sp++; /* FALLTHRU */
             case 11:  *dp++ = *sp++; /* FALLTHRU */
             case 10:  *dp++ = *sp++; /* FALLTHRU */
             case 9:   *dp++ = *sp++; /* FALLTHRU */
             case 8:   *dp++ = *sp++; /* FALLTHRU */

             case 7:   *dp++ = *sp++; /* FALLTHRU */
             case 6:   *dp++ = *sp++; /* FALLTHRU */
             case 5:   *dp++ = *sp++; /* FALLTHRU */
             case 4:   *dp++ = *sp++; /* FALLTHRU */
             case 3:   *dp++ = *sp++; /* FALLTHRU */
             case 2:   *dp++ = *sp++; /* FALLTHRU */
             case 1:   *dp++ = *sp++; /* FALLTHRU */
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
             * Here we do blocks of 8.  Once the remaining count
             * drops below 8, take the fast track to finish up.
             */
            default:
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 len -= 16;
                 break;

            case 15:  *--dp = *--sp; /* FALLTHRU */
            case 14:  *--dp = *--sp; /* FALLTHRU */
            case 13:  *--dp = *--sp; /* FALLTHRU */
            case 12:  *--dp = *--sp; /* FALLTHRU */
            case 11:  *--dp = *--sp; /* FALLTHRU */
            case 10:  *--dp = *--sp; /* FALLTHRU */
            case 9:   *--dp = *--sp; /* FALLTHRU */
            case 8:   *--dp = *--sp; /* FALLTHRU */

            case 7:   *--dp = *--sp; /* FALLTHRU */
            case 6:   *--dp = *--sp; /* FALLTHRU */
            case 5:   *--dp = *--sp; /* FALLTHRU */
            case 4:   *--dp = *--sp; /* FALLTHRU */
            case 3:   *--dp = *--sp; /* FALLTHRU */
            case 2:   *--dp = *--sp; /* FALLTHRU */
            case 1:   *--dp = *--sp; /* FALLTHRU */
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
void
mem_prim_move32 (uint32_t *dest, const uint32_t *src, uint32_t len)
{
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
              * Here we do blocks of 8.  Once the remaining count
              * drops below 8, take the fast track to finish up.
              */
             default:
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp++;
                  len -= 16;
                  break;

             case 15:  *dp++ = *sp++; /* FALLTHRU */
             case 14:  *dp++ = *sp++; /* FALLTHRU */
             case 13:  *dp++ = *sp++; /* FALLTHRU */
             case 12:  *dp++ = *sp++; /* FALLTHRU */
             case 11:  *dp++ = *sp++; /* FALLTHRU */
             case 10:  *dp++ = *sp++; /* FALLTHRU */
             case 9:   *dp++ = *sp++; /* FALLTHRU */
             case 8:   *dp++ = *sp++; /* FALLTHRU */

             case 7:   *dp++ = *sp++; /* FALLTHRU */
             case 6:   *dp++ = *sp++; /* FALLTHRU */
             case 5:   *dp++ = *sp++; /* FALLTHRU */
             case 4:   *dp++ = *sp++; /* FALLTHRU */
             case 3:   *dp++ = *sp++; /* FALLTHRU */
             case 2:   *dp++ = *sp++; /* FALLTHRU */
             case 1:   *dp++ = *sp++; /* FALLTHRU */
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
             * Here we do blocks of 8.  Once the remaining count
             * drops below 8, take the fast track to finish up.
             */
            default:
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 *--dp = *--sp; *--dp = *--sp; *--dp = *--sp; *--dp = *--sp;
                 len -= 16;
                 break;

            case 15:  *--dp = *--sp; /* FALLTHRU */
            case 14:  *--dp = *--sp; /* FALLTHRU */
            case 13:  *--dp = *--sp; /* FALLTHRU */
            case 12:  *--dp = *--sp; /* FALLTHRU */
            case 11:  *--dp = *--sp; /* FALLTHRU */
            case 10:  *--dp = *--sp; /* FALLTHRU */
            case 9:   *--dp = *--sp; /* FALLTHRU */
            case 8:   *--dp = *--sp; /* FALLTHRU */

            case 7:   *--dp = *--sp; /* FALLTHRU */
            case 6:   *--dp = *--sp; /* FALLTHRU */
            case 5:   *--dp = *--sp; /* FALLTHRU */
            case 4:   *--dp = *--sp; /* FALLTHRU */
            case 3:   *--dp = *--sp; /* FALLTHRU */
            case 2:   *--dp = *--sp; /* FALLTHRU */
            case 1:   *--dp = *--sp; /* FALLTHRU */
                len = 0;
                break;
            }
        } /* end while */
    }

    return;
}