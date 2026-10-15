/*------------------------------------------------------------------
 * test_memmove_s
 * File 'mem/memmove_s.c'
 * Lines executed:100.00% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"
#if defined(TEST_MSVCRT) && defined(HAVE_MEMMOVE_S)
#undef memmove_s
EXTERN errno_t memmove_s(void *dest, rsize_t dmax, const void *src,
                         rsize_t slen);
#endif

#ifdef HAVE_MEMMOVE_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (1024)

static uint8_t mem1[LEN + 1];
static uint8_t mem2[LEN];
int test_memmove_s(void);
/* unguarded primitive, not used by any public function */
extern void mem_prim_move8(uint8_t *dest, const uint8_t *src, uint32_t len);

int test_memmove_s(void) {
    errno_t rc;
    uint32_t i;
    rsize_t len;
    int errs = 0;

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }
    print_msvcrt(use_msvcrt);

    /* with clang-5 compile-time checks these errors */
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("dest overflow or empty")
    rc = memmove_s(NULL, LEN, mem2, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow or empty") EXPECT_BOS("slen overflow >dmax")
    rc = memmove_s(mem1, 0, mem2, LEN);
    ERR_MSVC(ESZEROL, ERANGE); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow or empty")
    rc = memmove_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN);
    ERR_MSVC(ESLEMAX, 0); /* and implementation defined */
    if (!use_msvcrt)
        EXPMEM(mem1, 0, LEN, 33, 1);
    else
        EXPMEM(mem1, 0, LEN, 44, 1);
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    /* check n=0 first */
    rc = memmove_s(mem1, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memmove_s(NULL, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memmove_s(mem1, 0, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memmove_s(mem1, 10, NULL, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("src overflow or empty")
    rc = memmove_s(mem1, LEN, NULL, LEN);
    ERR_MSVC(ESNULLP, EINVAL); /* and cleared */
    if (!use_msvcrt) {
        EXPMEM(mem1, 0, LEN, 0, 1); /* broken with msvcrt! */
    }

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    EXPECT_BOS("src overflow or empty") EXPECT_BOS("slen overflow >dmax")
    rc = memmove_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1);
    ERR_MSVC(ESLEMAX, ERANGE); /* and cleared */
    if (!use_msvcrt) {
        EXPMEM(mem1, 0, LEN, 0, 1);
    }
#endif
    /*--------------------------------------------------*/

    for (i = 0; i < LEN + 1; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = LEN;
    rc = memmove_s(mem1, len, mem2, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, 44, 1);
    EXPMEM(mem1, len, LEN + 1, 33, 1);

    for (i = 0; i < LEN + 1; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = 1;
    rc = memmove_s(mem1, len, mem2, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, 44, 1);
    EXPMEM(mem1, len, LEN + 1, 33, 1);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN + 1; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* length error */
    len = LEN / 2;
    rc = memmove_s(mem1, len, mem2, LEN);
    ERR_MSVC(ESNOSPC, ERANGE); /* and cleared */

    if (!use_msvcrt) {
        EXPMEM(mem1, 0, len, 0, 1); /* broken with msvcrt */
    }
    if (mem1[len] != 33) {
        debug_printf("%d - %d m1=%d  m2=%d  \n", __LINE__, (int)len, mem1[len],
                     mem2[len]);
        errs++;
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* invalid length, untouched dest */
    len = LEN;
    rc = memmove_s(mem1, len, mem2, 0);
    ERR(EOK);
    /* verify mem1[0] was not zeroed */
    if (mem1[0] != 33) {
        debug_printf("%d - %d m1=%d  m2=%d  \n", __LINE__, 0, (int)mem1[0],
                     (int)mem2[0]);
        errs++;
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* invalid length - zero dest */
#ifndef HAVE_CT_BOS_OVR
    len = LEN;
    EXPECT_BOS("src overflow or empty")
    rc = memmove_s(mem1, len, mem2, RSIZE_MAX_MEM + 1);
    ERR_MSVC(ESLEMAX, ERANGE);

    if (!use_msvcrt) {
        /* verify mem1 was zeroed */
        for (i = 0; i < len; i++) {
            if (mem1[i] != 0) {
                debug_printf("%d - %" PRIu32 " m1=%d \n", __LINE__, i, mem1[i]);
                errs++;
            }
        }
    }
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /* same ptr - no move */
    rc = memmove_s(mem1, LEN, mem1, LEN);
    ERR(EOK)

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 25;
    }
    for (i = 10; i < LEN - 10; i++) {
        mem1[i] = 35;
    }

    /* overlap move */
    len = 20;
    rc = memmove_s(&mem1[0], len, &mem1[10], len);
    ERR(EOK)
    else {
        for (i = 0; i < len; i++) {
            if (mem1[i] != 35) {
                debug_printf("%d - %" PRIu32 " m1=%d \n", __LINE__, i, mem1[i]);
                errs++;
            }
        }
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 25;
    }
    for (i = 10; i < LEN - 10; i++) {
        mem1[i] = 35;
    }

    /* overlap move */
    len = 20;
    rc = memmove_s(&mem1[10], len, &mem1[0], len);
    ERR(EOK)

    for (i = 0; i < LEN; i++) {
        mem2[i] = 25;
    }
    for (i = 10; i < LEN - 10; i++) {
        mem2[i] = 35;
    }

    for (i = 0; i < 10; i++) {
        if (mem1[i] != 25) {
            debug_printf("%d - %" PRIu32 " m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* unknown dest and src sizes, hidden from the compiler */
    if (!use_msvcrt) {
        static volatile size_t hsz = 64;
        uint8_t *hp = (uint8_t *)malloc(hsz);
        uint8_t *hs = (uint8_t *)malloc(hsz);

        if (!hp || !hs) {
            free(hp);
            free(hs);
            return errs + 1;
        }
        memset(hs, 7, hsz);
        memset(hp, 9, hsz);

        rc = memmove_s(hp, RSIZE_MAX_MEM + 1, hs, 8);
        ERR(ESLEMAX);
        EXPMEM(hp, 0, 8, 9, 1);

        rc = memmove_s(hp, 64, hs, 16);
        ERR(EOK);
        EXPMEM(hp, 0, 16, 7, 1);
        EXPMEM(hp, 16, 64, 9, 1);

        /* known too small src, unknown dest */
#if !defined(HAVE_CT_BOS_OVR) && defined(HAVE___BUILTIN_OBJECT_SIZE)
        {
            uint8_t small[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            rc = memmove_s(hp, 64, small, 16);
            ERR(EOVERFLOW);
            EXPMEM(hp, 0, 16, 7, 1);
        }
#endif
        free(hp);
        free(hs);
    }

    /*--------------------------------------------------*/
    /* all sizes and alignments, overlapping forward and backward, through
       the mem primitives */
    if (!use_msvcrt) {
        static uint8_t buf[2048];
        static uint8_t ref[2048];
        static const size_t big[] = {100, 127, 128, 255, 256, 257, 511, 512,
                                     1000};
        size_t sz, so, d_o, k, n;

        for (n = 0; n < 71 + sizeof(big) / sizeof(big[0]); n++) {
            sz = n < 71 ? n : big[n - 71];
            for (so = 0; so < 10; so++) {
                for (d_o = 0; d_o < 10; d_o++) {
                    for (k = 0; k < sizeof(buf); k++) {
                        buf[k] = (uint8_t)(k * 13 + 5);
                    }
                    memcpy(ref, buf, sizeof(buf));
                    memmove(ref + 16 + d_o, ref + 16 + so, sz);
                    rc = memmove_s(buf + 16 + d_o, sz + 1, buf + 16 + so, sz);
                    ERR(EOK);
                    if (memcmp(buf, ref, sizeof(buf)) != 0) {
                        debug_printf("%s %u  size %lu so %lu do %lu wrong\n",
                                     __FUNCTION__, __LINE__, (unsigned long)sz,
                                     (unsigned long)so, (unsigned long)d_o);
                        errs++;
                    }
                }
            }
        }

        /* separate buffers, both directions */
        for (n = 0; n < 71; n++) {
            for (so = 0; so < 8; so++) {
                for (d_o = 0; d_o < 8; d_o++) {
                    for (k = 0; k < 1024; k++) {
                        buf[k] = (uint8_t)(k * 13 + 5);
                        buf[1024 + k] = 0xEE;
                    }
                    rc = memmove_s(buf + 1024 + 8 + d_o, n + 1, buf + 8 + so,
                                   n);
                    ERR(EOK);
                    if (memcmp(buf + 1024 + 8 + d_o, buf + 8 + so, n) != 0) {
                        debug_printf("%s %u  fwd size %lu so %lu do %lu\n",
                                     __FUNCTION__, __LINE__, (unsigned long)n,
                                     (unsigned long)so, (unsigned long)d_o);
                        errs++;
                    }
                    rc = memmove_s(buf + 8 + d_o, n + 1, buf + 1024 + 8 + so,
                                   n);
                    ERR(EOK);
                    if (memcmp(buf + 8 + d_o, buf + 1024 + 8 + so, n) != 0) {
                        debug_printf("%s %u  bwd size %lu so %lu do %lu\n",
                                     __FUNCTION__, __LINE__, (unsigned long)n,
                                     (unsigned long)so, (unsigned long)d_o);
                        errs++;
                    }
                }
            }
        }
    }

    /*--------------------------------------------------*/
    /* the unguarded uint8_t primitive, overlapping forward and backward */
    {
        static uint8_t pbuf[2048];
        static uint8_t pref[2048];
        static const size_t pbig[] = {100, 127, 128, 255, 256, 257, 511, 512,
                                      1000};
        size_t psz, pso, pdo, pk, pn;

        for (pn = 0; pn < 71 + sizeof(pbig) / sizeof(pbig[0]); pn++) {
            psz = pn < 71 ? pn : pbig[pn - 71];
            for (pso = 0; pso < 4; pso++) {
                for (pdo = 0; pdo < 4; pdo++) {
                    for (pk = 0; pk < sizeof(pbuf); pk++) {
                        pbuf[pk] = (uint8_t)(pk * 13 + 5);
                    }
                    memcpy(pref, pbuf, sizeof(pbuf));
                    memmove(pref + 16 + pdo, pref + 16 + pso, psz);
                    mem_prim_move8(pbuf + 16 + pdo, pbuf + 16 + pso,
                                   (uint32_t)psz);
                    if (memcmp(pbuf, pref, sizeof(pbuf)) != 0) {
                        debug_printf("%s %u  size %lu so %lu do %lu wrong\n",
                                     __FUNCTION__, __LINE__,
                                     (unsigned long)psz, (unsigned long)pso,
                                     (unsigned long)pdo);
                        errs++;
                    }
                }
            }
        }
    }

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_memmove_s()); }
#endif
