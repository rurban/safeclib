/*------------------------------------------------------------------
 * test_memset16_s
 * File 'extmem/memset16_s.c'
 * Lines executed:77.78% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#define LEN (256)

int main(void) {
    errno_t rc;
    uint32_t len;
    uint32_t i;
    uint16_t value;

    rsize_t MAX = LEN * 2;
    uint16_t mem1[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

    value = 34;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = memset16_s(NULL, MAX, value, 0);
    ERR(ESNULLP);

    /* first check dest, then n */
    EXPECT_BOS("empty dest") EXPECT_BOS("dest overflow or empty")
    rc = memset16_s(NULL, MAX, value, LEN);
    ERR(ESNULLP);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    value = 34;

    EXPECT_BOS("dest overflow or empty")
    rc = memset16_s(mem1, RSIZE_MAX_MEM + 1, value, LEN);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    EXPECT_BOS("n*2 overflow >dest/dmax")
    rc = memset16_s(mem1, LEN, value, RSIZE_MAX_MEM16 + 1);
    ERR(ESLEMAX); /* and set all */
    EXPMEM(mem1, 0, LEN, value, 2);

    EXPECT_BOS("n*2 overflow >dest/dmax")
    rc = memset16_s(mem1, MAX, value, LEN + 1);
    ERR(ESNOSPC) /* and set all */
    EXPMEM(mem1, 0, LEN, value, 2);
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    value = 34;

    /* check n first, then args 2-3 */
    rc = memset16_s(mem1, MAX, value, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    rc = memset16_s(mem1, 0, value, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    rc = memset16_s(mem1, MAX, 256, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 1;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 2;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 12;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 31;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 133;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* unknown dest size, hidden from the compiler */
    {
        static volatile size_t xhsz = 64 * 2;
        uint16_t *xhp = (uint16_t *)malloc(xhsz);
        size_t xk;

        if (!xhp) {
            return errs + 1;
        }
        for (xk = 0; xk < 64; xk++) {
            xhp[xk] = 1;
        }
        rc = memset16_s(xhp, RSIZE_MAX_MEM + 1, 2, 8);
        ERR(ESLEMAX);
        if (xhp[0] != 1) {
            debug_printf("%s %u  dest changed\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = memset16_s(xhp, xhsz, 2, 8);
        ERR(EOK);
        for (xk = 0; xk < 64; xk++) {
            if (xhp[xk] != (xk < 8 ? 2 : 1)) {
                debug_printf("%s %u  wrong at %lu\n", __FUNCTION__, __LINE__,
                             (unsigned long)xk);
                errs++;
                break;
            }
        }
        free(xhp);
    }

    /*--------------------------------------------------*/
    /* the unchecked worker, called directly */
    {
        uint16_t xex[16];
        size_t xk;
        for (xk = 0; xk < 16; xk++) {
            xex[xk] = 1;
        }
        rc = _memset16_s_uchk(xex + 2, 5, 12);
        ERR(EOK);
        for (xk = 0; xk < 16; xk++) {
            if (xex[xk] != ((xk >= 2 && xk < 14) ? 5 : 1)) {
                debug_printf("%s %u  wrong at %lu\n", __FUNCTION__, __LINE__,
                             (unsigned long)xk);
                errs++;
                break;
            }
        }
    }

    /*--------------------------------------------------*/
    /* all sizes and alignments through the mem primitives */
    {
        static uint16_t xbuf[1100];
        static const size_t xbig[] = {100, 127, 128, 255, 256, 257, 511, 512, 1000};
        size_t xsz, xoff, xn, xk;

        for (xn = 0; xn < 71 + sizeof(xbig) / sizeof(xbig[0]); xn++) {
            xsz = xn < 71 ? xn : xbig[xn - 71];
            for (xoff = 0; xoff < 6; xoff++) {
                for (xk = 0; xk < 1100; xk++) {
                    xbuf[xk] = 0xEE;
                }
                rc = memset16_s(xbuf + xoff, (xsz + 1) * 2, 0x5A, xsz);
                ERR(EOK);
                for (xk = 0; xk < 1100; xk++) {
                    uint16_t xexp = (xk >= xoff && xk < xoff + xsz) ? 0x5A : 0xEE;
                    if (xbuf[xk] != xexp) {
                        debug_printf("%s %u  size %lu off %lu wrong at %lu\n",
                                     __FUNCTION__, __LINE__,
                                     (unsigned long)xsz, (unsigned long)xoff,
                                     (unsigned long)xk);
                        errs++;
                        break;
                    }
                }
            }
        }
    }

    return (errs);
}
