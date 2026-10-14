/*------------------------------------------------------------------
 * test_memzero16_s
 * File 'extmem/memzero16_s.c'
 * Lines executed:66.67% of 12
 *
 *
 *=
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN (512)

int main(void) {
    errno_t rc;
    uint32_t i;
    uint32_t len;

    uint16_t mem1[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = memzero16_s(NULL, LEN);
    ERR(ESNULLP)
    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or len * 2")
    rc = memzero16_s(mem1, 0);
    ERR(ESZEROL)
    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = memzero16_s(mem1, RSIZE_MAX_MEM16 + 1);
    ERR(ESLEMAX)
#endif
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    len = 1;
    rc = memzero16_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i = 0; i < len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %" PRIu32 " m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    len = 2;
    rc = memzero16_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i = 0; i < len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %" PRIu32 " m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    len = 3;
    rc = memzero16_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i = 0; i < len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %" PRIu32 " m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    len = LEN;
    rc = memzero16_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i = 0; i < len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %" PRIu32 " m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

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
        rc = memzero16_s(xhp, RSIZE_MAX_MEM16 + 1);
        ERR(ESLEMAX);
        if (xhp[0] != 1) {
            debug_printf("%s %u  dest changed\n", __FUNCTION__, __LINE__);
            errs++;
        }
        rc = memzero16_s(xhp, 8);
        ERR(EOK);
        for (xk = 0; xk < 64; xk++) {
            if (xhp[xk] != (xk < 8 ? 0 : 1)) {
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
        rc = _memzero16_s_uchk(xex + 2, 12);
        ERR(EOK);
        for (xk = 0; xk < 16; xk++) {
            if (xex[xk] != ((xk >= 2 && xk < 14) ? 0 : 1)) {
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

        for (xn = 1; xn < 71 + sizeof(xbig) / sizeof(xbig[0]); xn++) {
            xsz = xn < 71 ? xn : xbig[xn - 71];
            for (xoff = 0; xoff < 6; xoff++) {
                for (xk = 0; xk < 1100; xk++) {
                    xbuf[xk] = 0xEE;
                }
                rc = memzero16_s(xbuf + xoff, xsz);
                ERR(EOK);
                for (xk = 0; xk < 1100; xk++) {
                    uint16_t xexp = (xk >= xoff && xk < xoff + xsz) ? 0 : 0xEE;
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
