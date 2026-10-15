/*------------------------------------------------------------------
 * test_memset_s
 * File 'memset_s.c'
 * Lines executed:100.00% of 21
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"
#if defined(TEST_MSVCRT) && defined(HAVE_MEMSET_S)
#undef memset_s
#endif

#ifdef HAVE_MEMSET_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (256)
#define MAX RSIZE_MAX_MEM

static uint8_t mem1[LEN];
int test_memset_s(void);

int test_memset_s(void) {
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint8_t value;
    int errs = 0;

    /*--------------------------------------------------*/

#if defined HAVE_MEMSET_S
#if defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1)
    /* no compile-time checks for memset_s, since it's overridden from libc */
#undef EXPECT_BOS
#define EXPECT_BOS(s)
#endif
    use_msvcrt = true;
#else
    use_msvcrt = false;
#endif
    value = 34;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = memset_s(NULL, LEN, value, 0);
    if (rc == ESNULLP) {
        if (use_msvcrt)
            printf("safec overriding libc\n");
        use_msvcrt = false;
    } else {
        if (!use_msvcrt)
            printf("libc overriding safec\n");
        use_msvcrt = true;
    }
    ERR_MSVC(ESNULLP, EINVAL);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    value = 34;

    /* first check dest, then n */
    EXPECT_BOS("empty dest")
    rc = memset_s(NULL, LEN, value, 0);
    ERR_MSVC(ESNULLP, EINVAL);

    EXPECT_BOS("value overflow >255")
    rc = memset_s(mem1, LEN, 256,
                  LEN);   /* This should be caught at compile-time */
    ERR_MSVC(ESLEMAX, 0); /* no native overflow check on darwin! */
    if (use_msvcrt)
        EXPMEM(mem1, 0, LEN, 0, 1);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    EXPECT_BOS("dest overflow or empty")
    rc = memset_s(mem1, MAX + 1, value, LEN);
    ERR_MSVC(ESLEMAX, 0); /* and implementation defined */
    if (use_msvcrt)
        EXPMEM(mem1, 0, LEN, value, 1);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    EXPECT_BOS("n overflow >dest/dmax")
    rc = memset_s(mem1, LEN, value, MAX + 1);
    ERR_MSVC(ESLEMAX, EOVERFLOW); /* and set all */
    EXPMEM(mem1, 0, LEN, value, 1);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("n overflow >dest/dmax")
    rc = memset_s(mem1, LEN, value, LEN + 1);
    ERR_MSVC(ESNOSPC, EOVERFLOW); /* and set all */
    EXPMEM(mem1, 0, LEN, value, 1);
#endif
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    value = 34;

    /* check n first, then args 2-3 */
    rc = memset_s(mem1, LEN, value, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memset_s(mem1, 0, value, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("value overflow >255")
    rc = memset_s(mem1, LEN, 256, 0); /* should error! */
    ERR(EOK);                         /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);
#endif
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 1;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 2;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 12;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 31;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 133;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

    /*--------------------------------------------------*/

    /*--------------------------------------------------*/
    /* unknown dest size, hidden from the compiler */
    if (!use_msvcrt) {
        static volatile size_t hsz = 64;
        uint8_t *hp = (uint8_t *)malloc(hsz);

        if (!hp) {
            return errs + 1;
        }
        memset(hp, 1, hsz);
        rc = memset_s(hp, MAX + 1, 2, 8);
        ERR(ESLEMAX);
        EXPMEM(hp, 0, 8, 1, 1);

        rc = memset_s(hp, 64, 2, 8);
        ERR(EOK);
        EXPMEM(hp, 0, 8, 2, 1);
        EXPMEM(hp, 8, 64, 1, 1);
        free(hp);
    }

    /*--------------------------------------------------*/
    /* the unchecked worker, called directly */
    {
        uint8_t ex[16];
        memset(ex, 1, sizeof(ex));
        rc = _memset_s_uchk(ex + 2, 5, 12);
        ERR(EOK);
        EXPMEM(ex, 0, 2, 1, 1);
        EXPMEM(ex, 2, 14, 5, 1);
        EXPMEM(ex, 14, 16, 1, 1);
    }

    /*--------------------------------------------------*/
    /* all sizes and alignments through the mem primitives */
    {
        static uint8_t buf[1100];
        static const size_t big[] = {100, 127, 128, 255, 256,
                                     257, 511, 512, 1000};
        size_t sz, off, n, k;

        for (n = 0; n < 71 + sizeof(big) / sizeof(big[0]); n++) {
            sz = n < 71 ? n : big[n - 71];
            for (off = 0; off < 10; off++) {
                memset(buf, 0xEE, sizeof(buf));
                rc = memset_s(buf + off, sz + 1, 0x5A, sz);
                ERR(EOK);
                for (k = 0; k < sizeof(buf); k++) {
                    uint8_t exp = (k >= off && k < off + sz) ? 0x5A : 0xEE;
                    if (buf[k] != exp) {
                        debug_printf("%s %u  size %lu off %lu wrong at %lu\n",
                                     __FUNCTION__, __LINE__, (unsigned long)sz,
                                     (unsigned long)off, (unsigned long)k);
                        errs++;
                        break;
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
int main(void) { return (test_memset_s()); }
#endif
