/*------------------------------------------------------------------
 * test_memcpy_s
 * File 'mem/memcpy_s.c'
 * Lines executed:100.00% of 31
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"
#if defined(TEST_MSVCRT) && defined(HAVE_MEMCPY_S)
#undef memcpy_s
#endif

#ifdef HAVE_MEMCPY_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (1024)

static uint8_t mem1[LEN + 2];
static uint8_t mem2[LEN + 2];
int test_memcpy_s(void);
EXTERN void handle_mem_bos_chk_warn(const char *restrict func,
                                   void *restrict dest, const rsize_t dmax,
                                   const size_t destbos);

static int hcalls, hcalls2;
static errno_t herr;
static void count_handler(const char *msg, void *ptr, errno_t error) {
    (void)msg;
    (void)ptr;
    hcalls++;
    herr = error;
}
static void count_handler2(const char *msg, void *ptr, errno_t error) {
    (void)msg;
    (void)ptr;
    hcalls2++;
    herr = error;
}

int test_memcpy_s(void) {
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
    rc = memcpy_s(NULL, LEN, mem2, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow or empty") EXPECT_BOS("slen overflow >dmax")
    rc = memcpy_s(mem1, 0, mem2, LEN);
    ERR_MSVC(ESZEROL, ERANGE); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow or empty")
    rc = memcpy_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN);
    ERR_MSVC(ESLEMAX, 0); /* and implementation defined */
    if (!use_msvcrt)
        EXPMEM(mem1, 0, LEN, 33, 1);
    else
        EXPMEM(mem1, 0, LEN, 44, 1);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    EXPECT_BOS("dest overflow or empty")
    rc = memcpy_s(mem1, LEN + 3, mem2, LEN);
    ERR(EOVERFLOW); /* limited to destbos */
#ifdef _WIN32
    debug_printf("errno EOVERFLOW=%s\n", strerror(EOVERFLOW));
    debug_printf("errno 132=%s\n", strerror(132));
#endif
    if (!use_msvcrt)
        EXPMEM(mem1, 0, LEN, 33, 1);
    else {
        EXPMEM(mem1, 0, LEN, 44, 1);
    }
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    EXPECT_BOS("src overflow or empty")
    rc = memcpy_s(mem1, LEN, NULL, LEN);
    ERR_MSVC(ESNULLP, EINVAL); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 1);

    EXPECT_BOS("src overflow or empty") EXPECT_BOS("slen overflow >dmax")
    rc = memcpy_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1);
    ERR_MSVC(ESLEMAX, ERANGE); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 1);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN + 1; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = LEN;
    /* BOS(mem1) == LEN+2 */
    EXPECT_BOS("slen overflow >dmax")
    rc = memcpy_s(mem1, LEN, mem2, (LEN + 1));
    ERR_MSVC(ESNOSPC, ERANGE); /* and cleared */
    EXPMEM(mem1, 0, len + 1, 0, 1);

    for (i = 0; i < LEN + 2; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = LEN / 2;
    EXPECT_BOS("src overflow or empty") EXPECT_BOS("slen overflow >dmax")
    rc = memcpy_s(mem1, LEN / 2, mem2, LEN + 3);
    ERR_MSVC(ESNOSPC, ERANGE);

    EXPMEM(mem1, 0, len, 0, 1);
    if (mem1[len] != 33) {
        debug_printf("%d - %d m1=%d  m2=%d  \n", __LINE__, (int)len, mem1[len],
                     mem2[len]);
        errs++;
    }
#endif

#ifndef HAVE_CT_BOS_OVR
    for (i = 0; i < LEN; i++) {
        mem1[i] = 55;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 65;
    }

    /* overlap */
    len = 100;
    EXPECT_BOS("dest overlaps with src")
    rc = memcpy_s(&mem1[0], 100, &mem1[10], len);
    ERR_MSVC(ESOVRLP, 0);

    if (!use_msvcrt) {
        EXPMEM(mem1, 0, len, 0, 1);
        if (mem1[len] == 0) {
            debug_printf("%d - %" PRIu32 " m1=%d  m2=%d  \n", __LINE__, i, mem1[i],
                         mem2[i]);
            errs++;
        }
    }

    for (i = 0; i < LEN; i++) {
        mem1[i] = 55;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 65;
    }

    /* overlap */
    len = 100;
    EXPECT_BOS("dest overlaps with src")
    rc = memcpy_s(&mem1[10], 100, &mem1[0], 100);
    ERR_MSVC(ESOVRLP, 0);

    if (!use_msvcrt) {
        EXPMEM(mem1, 10, len + 10, 0, 1);
    }
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    /* check n=0 first */
    rc = memcpy_s(mem1, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memcpy_s(NULL, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memcpy_s(mem1, 0, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memcpy_s(mem1, 10, NULL, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN + 1; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, len);
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
    rc = memcpy_s(mem1, len, mem2, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, 44, 1);
    EXPMEM(mem1, len, LEN + 1, 33, 1);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 55;
    }

    /* same ptr - no move */
    GCC_PUSH_WARN_RESTRICT
    rc = memcpy_s(mem1, LEN, mem1, LEN);
    GCC_POP_WARN_RESTRICT
    ERR(EOK);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 35;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 55;
    }

    len = 5;
    rc = memcpy_s(mem1, len, mem2, len);
    ERR(EOK)
    else {
        EXPMEM(mem1, 0, len, 55, 1);
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 35;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 55;
    }

    rc = memcpy_s(mem1, LEN, mem2, LEN / 2);
    ERR(EOK)
    else {
        EXPMEM(mem1, 0, LEN / 2, 55, 1);
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

        rc = memcpy_s(hp, RSIZE_MAX_MEM + 1, hs, 8);
        ERR(ESLEMAX);
        EXPMEM(hp, 0, 8, 9, 1);

        rc = memcpy_s(hp, 64, hs, 16);
        ERR(EOK);
        EXPMEM(hp, 0, 16, 7, 1);
        EXPMEM(hp, 16, 64, 9, 1);

        /* known too small src, unknown dest */
#if !defined(HAVE_CT_BOS_OVR) && defined(HAVE___BUILTIN_OBJECT_SIZE)
        {
            uint8_t small[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            rc = memcpy_s(hp, 64, small, 16);
            ERR(EOVERFLOW);
            EXPMEM(hp, 0, 16, 7, 1);
        }
#endif
        free(hp);
        free(hs);
    }

    /*--------------------------------------------------*/
    /* all sizes known: the unchecked worker */
    if (!use_msvcrt) {
        uint8_t ex[32];

        for (i = 0; i < 32; i++) {
            ex[i] = (uint8_t)(i + 1);
        }
        /* partial overlap is still detected: dest cleared. The offset is
           volatile, else the compile-time check rejects the call. */
        {
            static volatile size_t off = 8;
            rc = memcpy_s(ex, 32, ex + off, 16);
        }
        ERR(ESOVRLP);
        EXPMEM(ex, 0, 32, 0, 1);

        /* the unchecked worker called directly */
        for (i = 0; i < 32; i++) {
            ex[i] = (uint8_t)(i + 1);
        }
        rc = _memcpy_s_uchk(ex, 32, ex + 8, 16);
        ERR(ESOVRLP);
        EXPMEM(ex, 0, 32, 0, 1);
        for (i = 0; i < 32; i++) {
            ex[i] = (uint8_t)(i + 1);
        }
        rc = _memcpy_s_uchk(ex + 16, 16, ex, 16);
        ERR(EOK);
        if (ex[16] != 1 || ex[31] != 16 || ex[15] != 16) {
            debug_printf("%s %u  uchk copy wrong\n", __FUNCTION__, __LINE__);
            errs++;
        }

        /* same src and dest is allowed */
        for (i = 0; i < 32; i++) {
            ex[i] = (uint8_t)(i + 1);
        }
        rc = memcpy_s(ex, 32, ex, 16);
        ERR(EOK);
        if (ex[0] != 1 || ex[15] != 16 || ex[31] != 32) {
            debug_printf("%s %u  same-ptr copy changed data\n", __FUNCTION__,
                         __LINE__);
            errs++;
        }
    }

    /*--------------------------------------------------*/
    /* all sizes and alignments through the mem primitives */
    if (!use_msvcrt) {
        static uint8_t sbuf[LEN + 16];
        static uint8_t dbuf[LEN + 16];
        static volatile size_t hdmax = LEN;
        size_t sz, so, d_o;
        int dir;
        static const size_t big[] = {100, 127, 128, 255, 256, 257, 511, 512,
                                     1000};

        for (i = 0; i < sizeof(sbuf); i++) {
            sbuf[i] = (uint8_t)(i * 7 + 3);
        }
        /* dir 0: sbuf -> dbuf, dir 1: dbuf -> sbuf, so that dest is below
           and above src. Both are non-overlapping */
        for (dir = 0; dir < 2; dir++) {
            uint8_t *from = dir ? dbuf : sbuf;
            uint8_t *to = dir ? sbuf : dbuf;
            for (i = 0; i < 71 + sizeof(big) / sizeof(big[0]); i++) {
                sz = i < 71 ? i : big[i - 71];
                for (so = 0; so < 8; so++) {
                    for (d_o = 0; d_o < 8; d_o++) {
                        size_t k;
                        if (dir) {
                            for (k = 0; k < sizeof(dbuf); k++)
                                dbuf[k] = (uint8_t)(k * 7 + 3);
                            memset(sbuf, 0xEE, sizeof(sbuf));
                        } else {
                            memset(dbuf, 0xEE, sizeof(dbuf));
                        }
                        rc = memcpy_s(to + d_o, hdmax, from + so, sz);
                        ERR(EOK);
                        for (k = 0; k < sz; k++) {
                            if (to[d_o + k] != from[so + k]) {
                                debug_printf("%s %u  size %lu so %lu do %lu "
                                             "differs at %lu\n",
                                             __FUNCTION__, __LINE__,
                                             (unsigned long)sz,
                                             (unsigned long)so,
                                             (unsigned long)d_o,
                                             (unsigned long)k);
                                errs++;
                                break;
                            }
                        }
                        if (to[d_o + sz] != 0xEE ||
                            (d_o && to[d_o - 1] != 0xEE)) {
                            debug_printf("%s %u  size %lu so %lu do %lu "
                                         "overflow\n",
                                         __FUNCTION__, __LINE__,
                                         (unsigned long)sz, (unsigned long)so,
                                         (unsigned long)d_o);
                            errs++;
                        }
                    }
                }
            }
        }
    }

    /*--------------------------------------------------*/
    /* constraint handlers */
    {
        constraint_handler_t prev;
        static void *volatile hnull = NULL;

        hcalls = 0;
        prev = set_mem_constraint_handler_s(count_handler);
        rc = memcpy_s(hnull, 8, mem2, 8);
        ERR_MSVC(ESNULLP, EINVAL);
        if (!use_msvcrt && (hcalls != 1 || herr != ESNULLP)) {
            debug_printf("%s %u  global handler calls=%d err=%d\n",
                         __FUNCTION__, __LINE__, hcalls, (int)herr);
            errs++;
        }
        /* the thread-local handler overrides the global one */
        hcalls = 0;
        thrd_set_mem_constraint_handler_s(count_handler2);
        rc = memcpy_s(hnull, 8, mem2, 8);
        ERR_MSVC(ESNULLP, EINVAL);
        if (!use_msvcrt && (hcalls != 0 || hcalls2 != 1 || herr != ESNULLP)) {
            debug_printf("%s %u  thrd handler calls=%d/%d err=%d\n",
                         __FUNCTION__, __LINE__, hcalls, hcalls2, (int)herr);
            errs++;
        }
        /* the dmax != sizeof(dest) warning, only reached with --enable-warn-dmax
           from the CHK macros */
        hcalls = 0;
        handle_mem_bos_chk_warn("memcpy_s", mem1, 8, 4);
        if (hcalls2 != 2 || herr != ESLEWRNG) {
            debug_printf("%s %u  bos warn handler calls=%d err=%d\n",
                         __FUNCTION__, __LINE__, hcalls2, (int)herr);
            errs++;
        }
        /* NULL resets to the default handler, returning the previous one */
        if (thrd_set_mem_constraint_handler_s(NULL) != count_handler2) {
            debug_printf("%s %u  wrong previous thrd handler\n", __FUNCTION__,
                         __LINE__);
            errs++;
        }
        if (set_mem_constraint_handler_s(NULL) != count_handler) {
            debug_printf("%s %u  wrong previous handler\n", __FUNCTION__,
                         __LINE__);
            errs++;
        }
        (void)set_mem_constraint_handler_s(prev);
    }

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_memcpy_s()); }
#endif
