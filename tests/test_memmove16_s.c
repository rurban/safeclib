/*------------------------------------------------------------------
 * test_memmove16_s
 * File 'extmem/memmove16_s.c'
 * Lines executed:100.00% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#define LEN (1024)
#define MAX (LEN * 2)

int main(void) {
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint16_t mem1[LEN];
    uint16_t mem2[LEN];
    rsize_t count = LEN;
    int errs = 0;

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    rc = memmove16_s(NULL, MAX, mem2, count);
    ERR(ESNULLP);
    /*--------------------------------------------------*/

    rc = memmove16_s(mem1, 0, mem2, count);
    ERR(ESZEROL); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);
    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memmove16_s(mem1, RSIZE_MAX_MEM + 1, mem2, count);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    rc = memmove16_s(mem1, MAX, NULL, count);
    ERR(ESNULLP); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);
    /*--------------------------------------------------*/

    for (i = 0; i < 10; i++) {
        mem1[i] = 33;
    }
    rc = memmove16_s(mem1, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("src overflow or empty") EXPECT_BOS("slen overflow >dmax / 2")
    rc = memmove16_s(mem1, MAX, mem2, RSIZE_MAX_MEM16 + 1);
    ERR(ESLEMAX); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = 1;
    rc = memmove16_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 2);
    EXPMEM(mem1, len, LEN - len, 33, 2);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = 2;
    rc = memmove16_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 2);
    EXPMEM(mem1, len, LEN - len, 33, 2);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* a valid move */
    len = LEN;
    rc = memmove16_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* count*2 greater than dmax */
    rc = memmove16_s(mem1, MAX, mem2, count + 1);
    ERR(ESNOSPC); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* empty count */
    rc = memmove16_s(mem1, MAX, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* same ptr - no move */
    rc = memmove16_s(mem1, MAX, mem1, count);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 25;
    }
    for (i = 10; i < LEN - 10; i++) {
        mem1[i] = 35;
    }

    /* overlap move + */
    len = 20;
    rc = memmove16_s(&mem1[0], LEN, &mem1[10], len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 35, 2);
    EXPMEM(mem1, len, LEN, 35, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 25;
    }
    for (i = 10; i < LEN - 10; i++) {
        mem1[i] = 35;
    }

    /* overlap move - */
    len = 20;
    rc = memmove16_s(&mem1[10], (LEN - 10) * 2, &mem1[0], len);
    ERR(EOK);
    EXPMEM(mem1, 0, 10, 25, 2);
    EXPMEM(mem1, len, LEN, 35, 2);

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* unknown dest and src sizes, hidden from the compiler */
    {
        static volatile size_t xhsz = 64 * 2;
        uint16_t *xhp = (uint16_t *)malloc(xhsz);
        uint16_t *xhs = (uint16_t *)malloc(xhsz);
        size_t xk;

        if (!xhp || !xhs) {
            free(xhp);
            free(xhs);
            return errs + 1;
        }
        for (xk = 0; xk < 64; xk++) {
            xhs[xk] = (uint16_t)(xk + 1);
            xhp[xk] = 0x7E;
        }

        rc = memmove16_s(xhp, RSIZE_MAX_MEM + 1, xhs, 8);
        ERR(ESLEMAX);
        if (xhp[0] != 0x7E) {
            debug_printf("%s %u  dest changed\n", __FUNCTION__, __LINE__);
            errs++;
        }

        rc = memmove16_s(xhp, xhsz, xhs, 16);
        ERR(EOK);
        for (xk = 0; xk < 64; xk++) {
            if (xhp[xk] != (xk < 16 ? (uint16_t)(xk + 1) : (uint16_t)0x7E)) {
                debug_printf("%s %u  wrong at %lu\n", __FUNCTION__, __LINE__,
                             (unsigned long)xk);
                errs++;
                break;
            }
        }

        /* known too small src, unknown dest */
#ifndef HAVE_CT_BOS_OVR
        {
            uint16_t xsmall[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            rc = memmove16_s(xhp, xhsz, xsmall, 16);
            ERR(EOVERFLOW);
        }
#endif
        free(xhp);
        free(xhs);
    }

    /*--------------------------------------------------*/
    /* overlapping forward and backward through the mem primitives */
    {
        static uint16_t xbuf[2048];
        static uint16_t xref[2048];
        static const size_t xbig[] = {100, 127, 128, 255, 256, 257, 511, 512, 1000};
        size_t xsz, xso, xdo, xk, xn;

        for (xn = 0; xn < 71 + sizeof(xbig) / sizeof(xbig[0]); xn++) {
            xsz = xn < 71 ? xn : xbig[xn - 71];
            for (xso = 0; xso < 6; xso++) {
                for (xdo = 0; xdo < 6; xdo++) {
                    for (xk = 0; xk < 2048; xk++) {
                        xbuf[xk] = (uint16_t)(xk * 13 + 5);
                    }
                    memcpy(xref, xbuf, sizeof(xbuf));
                    memmove(xref + 16 + xdo, xref + 16 + xso, xsz * 2);
                    rc = memmove16_s(xbuf + 16 + xdo, (xsz + 1) * 2,
                              xbuf + 16 + xso, xsz);
                    ERR(EOK);
                    if (memcmp(xbuf, xref, sizeof(xbuf)) != 0) {
                        debug_printf("%s %u  size %lu so %lu do %lu wrong\n",
                                     __FUNCTION__, __LINE__,
                                     (unsigned long)xsz, (unsigned long)xso,
                                     (unsigned long)xdo);
                        errs++;
                    }
                }
            }
        }
        /* separate buffers, both directions */
        for (xn = 0; xn < 71; xn++) {
            for (xso = 0; xso < 6; xso++) {
                for (xdo = 0; xdo < 6; xdo++) {
                    for (xk = 0; xk < 1024; xk++) {
                        xbuf[xk] = (uint16_t)(xk * 13 + 5);
                        xbuf[1024 + xk] = 0xEE;
                    }
                    rc = memmove16_s(xbuf + 1024 + 8 + xdo, (xn + 1) * 2,
                              xbuf + 8 + xso, xn);
                    ERR(EOK);
                    if (memcmp(xbuf + 1024 + 8 + xdo, xbuf + 8 + xso,
                               xn * 2) != 0) {
                        errs++;
                    }
                    rc = memmove16_s(xbuf + 8 + xdo, (xn + 1) * 2,
                              xbuf + 1024 + 8 + xso, xn);
                    ERR(EOK);
                    if (memcmp(xbuf + 8 + xdo, xbuf + 1024 + 8 + xso,
                               xn * 2) != 0) {
                        errs++;
                    }
                }
            }
        }
    }

    return (errs);
}
