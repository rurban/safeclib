/*------------------------------------------------------------------
 * test_memcmp16_s
 * File 'extmem/memcmp16_s.c'
 * Lines executed:100.00% of 57
 *
 *------------------------------------------------------------------
 * fails: ubuntu-aarch64-linux-native and aarch64-fedora-docker,
 * but passes {debian,centos,fedora}-aarch64-linux
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN (128)

static volatile size_t vrmm = RSIZE_MAX_MEM16 + 1;

int main(void) {
    errno_t rc;

    uint32_t len;
    int ind;
    int std_ind;
    uint32_t i;

    uint16_t mem1[LEN];
    uint16_t mem2[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = memcmp16_s(NULL, LEN, mem2, LEN, &ind);
    ERR(ESNULLP);

    EXPECT_BOS("empty src")
    rc = memcmp16_s(mem1, LEN, NULL, LEN, &ind);
    ERR(ESNULLP);

    EXPECT_BOS("empty diff")
    rc = memcmp16_s(mem1, LEN, mem2, LEN, NULL);
    ERR(ESNULLP);

    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dlen * 2") EXPECT_BOS("slen overflow >dlen")
    rc = memcmp16_s(mem1, 0, mem2, LEN, &ind);
    ERR(ESZEROL);

    EXPECT_BOS("empty src or slen * 2")
    rc = memcmp16_s(mem1, LEN, mem2, 0, &ind);
    ERR(ESZEROL);

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = memcmp16_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN, &ind);
    ERR(ESLEMAX);

    EXPECT_BOS("src overflow") EXPECT_BOS("slen overflow >dlen")
    rc = memcmp16_s(mem1, LEN, mem2, RSIZE_MAX_MEM16 + 1, &ind);
    ERR(ESLEMAX);

    if (_BOS_KNOWN(mem2)) {
        EXPECT_BOS("src overflow") EXPECT_BOS("slen overflow >dlen")
        rc = memcmp16_s(mem1, LEN, mem2, LEN + 1, &ind);
        ERR(EOVERFLOW);
    }

    /*--------------------------------------------------*/

    EXPECT_BOS("slen overflow >dlen")
    rc = memcmp16_s(mem1, LEN - 1, mem2, LEN, &ind);
    ERR(ESNOSPC);
#endif

    /*--------------------------------------------------*/
    /* undetected at compile-time, but still doing
       run-time object_size checks */

    len = RSIZE_MAX_MEM16 + 1;
    rc = memcmp16_s(mem1, LEN, mem2, len, &ind);
    ERR(ESLEMAX);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    len = LEN + 1;
    rc = memcmp16_s(mem1, LEN, mem2, len, &ind);
    ERR(EOVERFLOW);
#endif

    /*--------------------------------------------------*/

    /* short cut */
    rc = memcmp16_s(mem1, LEN, mem1, LEN, &ind);
    ERR(EOK);
    INDZERO();

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = i;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = i;
    }

    len = 1;
    rc = memcmp16_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    INDZERO();
    std_ind = memcmp(mem1, mem2, len * 2);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  std_ind=%d rc=%u \n", __FUNCTION__,
                     __LINE__, ind, std_ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = i;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = i;
    }

    len = 2;
    rc = memcmp16_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    INDZERO();
    std_ind = memcmp(mem1, mem2, len * 2);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  std_ind=%d rc=%u \n", __FUNCTION__,
                     __LINE__, ind, std_ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = i;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = i;
    }

    rc = memcmp16_s(mem1, LEN, mem2, LEN, &ind);
    ERR(EOK);
    INDZERO();
    std_ind = memcmp(mem1, mem2, LEN * 2);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  std_ind=%d rc=%u \n", __FUNCTION__,
                     __LINE__, ind, std_ind, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = i;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = i;
    }

    for (i = 10; i < LEN - 10; i++) {
        mem2[i] = 5;
    }

    rc = memcmp16_s(mem1, LEN, mem2, LEN, &ind);
    ERR(EOK);
    INDCMP(== 0);

#define sgn(i) ((i) > 0 ? 1 : ((i) < 0 ? -1 : 0))

    /* ANSI C may return just the signum here: 1 instead of 5 */
    std_ind = memcmp(mem1, mem2, LEN * 2);
    if (ind != std_ind && sgn(ind) != std_ind) {
        debug_printf("%s %u  Ind=%d  std_ind=%d rc=%u \n", __FUNCTION__,
                     __LINE__, ind, std_ind, rc);
        errs++;
    }

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* unknown dest and src sizes, hidden from the compiler */
    {
        static volatile size_t xhsz = 64 * sizeof(uint16_t);
        uint16_t *xhp = (uint16_t *)malloc(xhsz);
        uint16_t *xhs = (uint16_t *)malloc(xhsz);
        int xdiff = 5;

        if (!xhp || !xhs) {
            free(xhp);
            free(xhs);
            return errs + 1;
        }
        memset(xhp, 1, xhsz);
        memset(xhs, 1, xhsz);

        /* unknown dest too large */
        rc = memcmp16_s(xhp, RSIZE_MAX_MEM16 + 1, xhs, 8, &xdiff);
        ERR(ESLEMAX);
        if (xdiff != -1) {
            debug_printf("%s %u  diff=%d\n", __FUNCTION__, __LINE__, xdiff);
            errs++;
        }

        /* unknown src too large, known dest */
        rc = memcmp16_s(mem1, LEN, xhs, vrmm, &xdiff);
        ERR(ESLEMAX);

        /* both unknown */
        rc = memcmp16_s(xhp, 64, xhs, 64, &xdiff);
        ERR(EOK);
        if (xdiff != 0) {
            debug_printf("%s %u  diff=%d\n", __FUNCTION__, __LINE__, xdiff);
            errs++;
        }
        xhs[63] = 2;
        rc = memcmp16_s(xhp, 64, xhs, 64, &xdiff);
        ERR(EOK);
        if (xdiff == 0) {
            debug_printf("%s %u  diff=%d\n", __FUNCTION__, __LINE__, xdiff);
            errs++;
        }

        /* unknown dest, dlen in elements below max (but dmax in bytes
           above it) */
        {
            static volatile size_t xmid = RSIZE_MAX_MEM16 / 2 + 1;
            xdiff = 5;
            rc = memcmp16_s(xhp, xmid, xhs, 1, &xdiff);
            ERR(EOK);
        }

        /* known dest, dlen larger than dest but not larger than max */
        {
            static volatile size_t xbig = LEN + 1;
            rc = memcmp16_s(mem1, xbig, mem2, 1, &xdiff);
            ERR(EOVERFLOW);
        }
        free(xhp);
        free(xhs);
    }

    return (errs);
}
