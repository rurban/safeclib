/*------------------------------------------------------------------
 * test_memcmp_s
 * File 'extmem/memcmp_s.c'
 * Lines executed:100.00% of 59
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN (128)

static uint8_t mem1[LEN];
static uint8_t mem2[LEN];
static volatile size_t vrmm = RSIZE_MAX_MEM + 1;

int test_memcmp_s(void);

int test_memcmp_s(void) {
    errno_t rc;
    int ind;
    int std_ind;
    uint32_t len;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = memcmp_s(NULL, LEN, mem2, LEN, &ind);
    ERR(ESNULLP);

    EXPECT_BOS("empty src")
    rc = memcmp_s(mem1, LEN, NULL, LEN, &ind);
    ERR(ESNULLP);

    EXPECT_BOS("empty diff")
    rc = memcmp_s(mem1, LEN, mem2, LEN, NULL);
    ERR(ESNULLP);

    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dmax") EXPECT_BOS("slen overflow >dmax")
    rc = memcmp_s(mem1, 0, mem2, LEN, &ind);
    ERR(ESZEROL);

    EXPECT_BOS("empty slen")
    rc = memcmp_s(mem1, LEN, mem2, 0, &ind);
    ERR(ESZEROL);

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = memcmp_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN, &ind);
    ERR(ESLEMAX);

    EXPECT_BOS("src overflow") EXPECT_BOS("slen overflow >dmax")
    rc = memcmp_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1, &ind);
    ERR(ESLEMAX);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = memcmp_s(mem1, LEN + 1, mem2, LEN, &ind);
    ERR(EOVERFLOW);

    EXPECT_BOS("src overflow") EXPECT_BOS("slen overflow >dmax")
    rc = memcmp_s(mem1, LEN, mem2, LEN + 1, &ind);
    ERR(EOVERFLOW);
#endif
    /*--------------------------------------------------*/

    EXPECT_BOS("slen overflow >dmax")
    rc = memcmp_s(mem1, LEN - 1, mem2, LEN, &ind);
    ERR(ESNOSPC);

#endif

    /*--------------------------------------------------*/

    /* undetected at compile-time */

    len = RSIZE_MAX_MEM + 1;
    rc = memcmp_s(mem1, LEN, mem2, len, &ind);
    ERR(ESLEMAX);

    /*--------------------------------------------------*/
    /* intel's mpx dynamic pointer bounds checker.
     * requires -fcheck-pointer-bounds -mmpx
     */
#ifdef HAVE_BND_CHK_MALLOC
    {
        void *memd = bnd_chk_malloc(LEN - 1);
        rc = memcmp_s(memd, LEN, mem2, LEN, &ind);
        /* ERR(ESLEMAX); run-time error here */
        rc = memcmp_s(mem1, LEN, memd, LEN, &ind);
        /* ERR(ESLEMAX); run-time error here */
    }
#endif
    /*--------------------------------------------------*/

    /* short cut */
    rc = memcmp_s(mem1, LEN, mem1, LEN, &ind);
    ERR(EOK);
    INDZERO();

    /*--------------------------------------------------*/

    len = 1;

    memset(mem1, 55, len);
    memset(mem2, 55, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    INDZERO();
    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  rc=%u \n", __FUNCTION__, __LINE__, ind,
                     rc);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 2;

    memset(mem1, 55, LEN);
    memset(mem2, 55, LEN);

    rc = memcmp_s(&mem1[1], len, &mem2[1], len, &ind);
    ERR(EOK);
    INDZERO();
    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  rc=%u \n", __FUNCTION__, __LINE__, ind,
                     rc);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 32;

    memset(mem1, 55, len);
    memset(mem2, 55, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    INDZERO();
    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  rc=%u \n", __FUNCTION__, __LINE__, ind,
                     rc);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 32;

    memset(mem1, 55, len);
    memset(mem2, 55, len);
    memset(&mem2[(len / 2)], 35, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    if (ind < 1) {
        debug_printf("%s %u  Ind=%d  rc=%u \n", __FUNCTION__, __LINE__, ind,
                     rc);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 32;

    memset(mem1, 55, len);
    memset(mem2, 55, len);
    memset(&mem2[(len / 2)], 65, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    if (ind > -1) {
        debug_printf("%s %u  Ind=%d  rc=%u \n", __FUNCTION__, __LINE__, ind,
                     rc);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 3;

    memset(mem1, 55, len);
    memset(mem2, 55, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n", __FUNCTION__, __LINE__, ind,
                     rc);
        errs++;
    }
    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* unknown dest and src sizes, hidden from the compiler */
    {
        static volatile size_t xhsz = 64 * sizeof(uint8_t);
        uint8_t *xhp = (uint8_t *)malloc(xhsz);
        uint8_t *xhs = (uint8_t *)malloc(xhsz);
        int xdiff = 5;

        if (!xhp || !xhs) {
            free(xhp);
            free(xhs);
            return errs + 1;
        }
        memset(xhp, 1, xhsz);
        memset(xhs, 1, xhsz);

        /* unknown dest too large */
        rc = memcmp_s(xhp, RSIZE_MAX_MEM + 1, xhs, 8, &xdiff);
        ERR(ESLEMAX);
        if (xdiff != -1) {
            debug_printf("%s %u  diff=%d\n", __FUNCTION__, __LINE__, xdiff);
            errs++;
        }

        /* unknown src too large, known dest */
        rc = memcmp_s(mem1, LEN, xhs, vrmm, &xdiff);
        ERR(ESLEMAX);

        /* both unknown */
        rc = memcmp_s(xhp, 64, xhs, 64, &xdiff);
        ERR(EOK);
        if (xdiff != 0) {
            debug_printf("%s %u  diff=%d\n", __FUNCTION__, __LINE__, xdiff);
            errs++;
        }
        xhs[63] = 2;
        rc = memcmp_s(xhp, 64, xhs, 64, &xdiff);
        ERR(EOK);
        if (xdiff == 0) {
            debug_printf("%s %u  diff=%d\n", __FUNCTION__, __LINE__, xdiff);
            errs++;
        }
        free(xhp);
        free(xhs);
    }

    return (errs);
}

#ifndef __KERNEL__
int main(void) { return (test_memcmp_s()); }
#endif
