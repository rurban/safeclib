/*------------------------------------------------------------------
 * test_memcpy16_s
 * File 'extmem/memcpy16_s.c'
 * Lines executed:100.00% of 29
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
    rc = memcpy16_s(NULL, MAX, mem2, count);
    ERR(ESNULLP);
    /*--------------------------------------------------*/

    rc = memcpy16_s(mem1, 0, mem2, count);
    ERR(ESZEROL); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);
    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memcpy16_s(mem1, RSIZE_MAX_MEM + 1, mem2, count);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    rc = memcpy16_s(mem1, MAX, NULL, count);
    ERR(ESNULLP); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);
    /*--------------------------------------------------*/

    for (i = 0; i < 10; i++) {
        mem1[i] = 33;
    }

    /* check n=0 first */
    rc = memcpy16_s(mem1, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);

    rc = memcpy16_s(NULL, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);

    rc = memcpy16_s(mem1, 0, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);

    rc = memcpy16_s(mem1, 10, NULL, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);
    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("src overflow or empty") EXPECT_BOS("slen overflow >dmax / 2")
    rc = memcpy16_s(mem1, MAX, mem2, RSIZE_MAX_MEM16 + 1);
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
    rc = memcpy16_s(mem1, MAX, mem2, len);
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
    rc = memcpy16_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 2);
    EXPMEM(mem1, len, LEN - len, 33, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* count*2 greater than dmax */
    rc = memcpy16_s(mem1, MAX, mem2, count + 1);
    ERR(ESNOSPC); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 55;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 65;
    }

    /* same ptr - no move */
    rc = memcpy16_s(mem1, MAX, mem1, count);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 55, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < 100; i++) {
        mem1[i] = 25;
    }
    for (i = 10; i < 100; i++) {
        mem1[i] = 35;
    }

    /* overlap + */
    rc = memcpy16_s(&mem1[0], MAX, &mem1[10], 100);
    ERR(ESOVRLP); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < 100; i++) {
        mem1[i] = 25;
    }
    for (i = 10; i < 100; i++) {
        mem1[i] = 45;
    }

    /* overlap - */
    rc = memcpy16_s(&mem1[10], (LEN - 10) * 2, &mem1[0], 100);
    ERR(ESOVRLP); /* and cleared */
    EXPMEM(mem1, 10, (LEN - 10) * 2, 0, 2);
    EXPMEM(mem1, 0, 10, 25, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    rc = memcpy16_s(mem1, MAX, mem2, count - 10);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, count - 10, 44, 2);
    EXPMEM(mem1, count - 10, LEN, 33, 2);

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

        rc = memcpy16_s(xhp, RSIZE_MAX_MEM + 1, xhs, 8);
        ERR(ESLEMAX);
        if (xhp[0] != 0x7E) {
            debug_printf("%s %u  dest changed\n", __FUNCTION__, __LINE__);
            errs++;
        }

        rc = memcpy16_s(xhp, xhsz, xhs, 16);
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
#if !defined(HAVE_CT_BOS_OVR) && defined(HAVE___BUILTIN_OBJECT_SIZE)
        {
            uint16_t xsmall[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            rc = memcpy16_s(xhp, xhsz, xsmall, 16);
            ERR(ESLEMAX);
        }
#endif
        free(xhp);
        free(xhs);
    }

    /*--------------------------------------------------*/
    /* the unchecked worker, called directly */
    {
        uint16_t xex[32];
        size_t xk;

        for (xk = 0; xk < 32; xk++) {
            xex[xk] = (uint16_t)(xk + 1);
        }
        rc = _memcpy16_s_uchk(xex, 32 * 2, xex + 8, 16);
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
            xex[xk] = (uint16_t)(xk + 1);
        }
        rc = _memcpy16_s_uchk(xex + 16, 16 * 2, xex, 16);
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
        static uint16_t xa[1100];
        static uint16_t xb[1100];
        static const size_t xbig[] = {100, 127, 128, 255, 256, 257, 511, 512, 1000};
        size_t xsz, xso, xdo, xk, xn;
        int xdir;

        for (xdir = 0; xdir < 2; xdir++) {
            uint16_t *xfrom = xdir ? xb : xa;
            uint16_t *xto = xdir ? xa : xb;
            for (xn = 0; xn < 71 + sizeof(xbig) / sizeof(xbig[0]); xn++) {
                xsz = xn < 71 ? xn : xbig[xn - 71];
                for (xso = 0; xso < 6; xso++) {
                    for (xdo = 0; xdo < 6; xdo++) {
                        for (xk = 0; xk < 1100; xk++) {
                            xfrom[xk] = (uint16_t)(xk * 7 + 3);
                            xto[xk] = 0xEE;
                        }
                        rc = memcpy16_s(xto + xdo, (xsz + 1) * 2, xfrom + xso,
                                  xsz);
                        ERR(EOK);
                        for (xk = 0; xk < 1100; xk++) {
                            uint16_t xexp = (xk >= xdo && xk < xdo + xsz)
                                           ? xfrom[xso + xk - xdo]
                                           : (uint16_t)0xEE;
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
