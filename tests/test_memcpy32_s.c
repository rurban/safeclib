/*------------------------------------------------------------------
 * test_memcpy32_s
 * File 'extmem/memcpy32_s.c'
 * Lines executed:100.00% of 29
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#define LEN (1024)
#define MAX (LEN * 4)

int main(void) {
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint32_t mem1[LEN];
    uint32_t mem2[LEN];
    rsize_t count = LEN;
    int errs = 0;

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    rc = memcpy32_s(NULL, MAX, mem2, count);
    ERR(ESNULLP);
    /*--------------------------------------------------*/

    rc = memcpy32_s(mem1, 0, mem2, count);
    ERR(ESZEROL); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);
    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memcpy32_s(mem1, RSIZE_MAX_MEM + 1, mem2, count);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);
#endif

    /*--------------------------------------------------*/

    rc = memcpy32_s(mem1, MAX, NULL, count);
    ERR(ESNULLP); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 4);
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    rc = memcpy32_s(mem1, 12, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    EXPECT_BOS("src overflow or empty") EXPECT_BOS("slen overflow >dmax / 4")
    rc = memcpy32_s(mem1, MAX, mem2, RSIZE_MAX_MEM32 + 1);
    ERR(ESLEMAX); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 4);
#endif
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = 1;
    rc = memcpy32_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 4);
    EXPMEM(mem1, len, LEN - len, 33, 4);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = 2;
    rc = memcpy32_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 4);
    EXPMEM(mem1, len, LEN - len, 33, 4);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* count*4 greater than dmax */
    rc = memcpy32_s(mem1, MAX, mem2, count + 1);
    ERR(ESNOSPC); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 4);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 25;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 35;
    }

    /* same ptr */
    rc = memcpy32_s(mem1, MAX, mem1, count);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 25, 4);
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 25;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 35;
    }

    /* overlap + */
    rc = memcpy32_s(&mem1[0], LEN, &mem1[10], 100);
    ERR(ESOVRLP); /* and cleared */
    EXPMEM(mem1, 0, 100, 0, 4);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 65;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 75;
    }

    /* overlap - */
    rc = memcpy32_s(&mem1[10], LEN - 10, &mem1[0], 100);
    ERR(ESOVRLP); /* and cleared */
    EXPMEM(mem1, 10, LEN - 10, 0, 4);
    EXPMEM(mem1, 0, 10, 65, 4);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 65;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 75;
    }

    rc = memcpy32_s(mem1, MAX, mem2, count - 10);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, count - 10, 75, 4);
    EXPMEM(mem1, count - 10, LEN, 65, 4);

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* unknown dest and src sizes, hidden from the compiler */
    {
        static volatile size_t xhsz = 64 * 4;
        uint32_t *xhp = (uint32_t *)malloc(xhsz);
        uint32_t *xhs = (uint32_t *)malloc(xhsz);
        size_t xk;

        if (!xhp || !xhs) {
            free(xhp);
            free(xhs);
            return errs + 1;
        }
        for (xk = 0; xk < 64; xk++) {
            xhs[xk] = (uint32_t)(xk + 1);
            xhp[xk] = 0x7E;
        }

        rc = memcpy32_s(xhp, RSIZE_MAX_MEM + 1, xhs, 8);
        ERR(ESLEMAX);
        if (xhp[0] != 0x7E) {
            debug_printf("%s %u  dest changed\n", __FUNCTION__, __LINE__);
            errs++;
        }

        rc = memcpy32_s(xhp, xhsz, xhs, 16);
        ERR(EOK);
        for (xk = 0; xk < 64; xk++) {
            if (xhp[xk] != (xk < 16 ? (uint32_t)(xk + 1) : (uint32_t)0x7E)) {
                debug_printf("%s %u  wrong at %lu\n", __FUNCTION__, __LINE__,
                             (unsigned long)xk);
                errs++;
                break;
            }
        }

        /* known too small src, unknown dest */
#ifndef HAVE_CT_BOS_OVR
        {
            uint32_t xsmall[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            rc = memcpy32_s(xhp, xhsz, xsmall, 16);
            ERR(ESLEMAX);
        }
#endif
        free(xhp);
        free(xhs);
    }

    /*--------------------------------------------------*/
    /* the unchecked worker, called directly */
    {
        uint32_t xex[32];
        size_t xk;

        for (xk = 0; xk < 32; xk++) {
            xex[xk] = (uint32_t)(xk + 1);
        }
        rc = _memcpy32_s_uchk(xex, 32 * 4, xex + 8, 16);
        ERR(ESOVRLP);
        for (xk = 0; xk < 32; xk++) {
            if (xex[xk] != 0) {
                debug_printf("%s %u  dest not cleared\n", __FUNCTION__,
                             __LINE__);
                errs++;
                break;
            }
        }
        for (xk = 0; xk < 32; xk++) {
            xex[xk] = (uint32_t)(xk + 1);
        }
        rc = _memcpy32_s_uchk(xex + 16, 16 * 4, xex, 16);
        ERR(EOK);
        if (xex[16] != 1 || xex[31] != 16 || xex[15] != 16) {
            debug_printf("%s %u  uchk copy wrong\n", __FUNCTION__, __LINE__);
            errs++;
        }
    }

    /*--------------------------------------------------*/
    /* all sizes and alignments through the mem primitives, both
       directions */
    {
        static uint32_t xa[1100];
        static uint32_t xb[1100];
        static const size_t xbig[] = {100, 127, 128, 255, 256, 257, 511, 512, 1000};
        size_t xsz, xso, xdo, xk, xn;
        int xdir;

        for (xdir = 0; xdir < 2; xdir++) {
            uint32_t *xfrom = xdir ? xb : xa;
            uint32_t *xto = xdir ? xa : xb;
            for (xn = 0; xn < 71 + sizeof(xbig) / sizeof(xbig[0]); xn++) {
                xsz = xn < 71 ? xn : xbig[xn - 71];
                for (xso = 0; xso < 6; xso++) {
                    for (xdo = 0; xdo < 6; xdo++) {
                        for (xk = 0; xk < 1100; xk++) {
                            xfrom[xk] = (uint32_t)(xk * 7 + 3);
                            xto[xk] = 0xEE;
                        }
                        rc = memcpy32_s(xto + xdo, (xsz + 1) * 4, xfrom + xso,
                                  xsz);
                        ERR(EOK);
                        for (xk = 0; xk < 1100; xk++) {
                            uint32_t xexp = (xk >= xdo && xk < xdo + xsz)
                                           ? xfrom[xso + xk - xdo]
                                           : (uint32_t)0xEE;
                            if (xto[xk] != xexp) {
                                debug_printf("%s %u  size %lu so %lu do %lu "
                                             "wrong at %lu\n",
                                             __FUNCTION__, __LINE__,
                                             (unsigned long)xsz,
                                             (unsigned long)xso,
                                             (unsigned long)xdo,
                                             (unsigned long)xk);
                                errs++;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return (errs);
}
