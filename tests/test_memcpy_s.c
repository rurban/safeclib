/*------------------------------------------------------------------
 * test_memcpy_s
 * File 'mem/memcpy_s.c'
 * Lines executed:86.36% of 22
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
            debug_printf("%d - %d m1=%d  m2=%d  \n", __LINE__, i, mem1[i],
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

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_memcpy_s()); }
#endif
