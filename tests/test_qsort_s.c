/*------------------------------------------------------------------
 * test_qsort_s
 * File 'misc/qsort_s.c'
 * Lines executed:89.23% of 130
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <stdlib.h>

#ifdef HAVE_QSORT_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

/* conflicting API, void return. skip only shared */
#if defined(MINGW_HAS_SECURE_API) && !defined(DISABLE_DLLIMPORT)
int main(void) {
    printf("skipped under windows sec_api: conflicting API\n");
    return 0;
}
#else
int test_qsort_s(void);

#define LEN 10
struct items {
    int iv;
    const char *sv;
} array[] = {{1, "one"},  {3, "three"}, {8, "eight"}, {4, "four"},
             {0, "zero"}, {5, "five"},  {2, "two"},   {6, "six"},
             {9, "nine"}, {7, "seven"}, {10, "ten"}};

static int comp(const void *ptr1, const void *ptr2, void *ctx) {
    struct items *i1 = (struct items *)ptr1;
    struct items *i2 = (struct items *)ptr2;
    if (ctx) {
        errno = -1;
        return 0; /* shortcut just for testing */
    }
    return i1->iv > i2->iv ? 1 : i1->iv == i2->iv ? 0 : -1;
}

int test_qsort_s(void) {
    errno_t rc;
    int errs = 0;
    int ind;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty buf or bufsize")
    rc = qsort_s(NULL, LEN, sizeof(array[0]), comp, NULL);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    if (!use_msvcrt) {
        ERR(ESNULLP);
    } /* msvcrt returns void */

    EXPECT_BOS("empty compar")
    rc = qsort_s(array, LEN, sizeof(array[0]), NULL, NULL);
    if (!use_msvcrt) {
        ERR(ESNULLP);
    } /* msvcrt returns void */

    /*--------------------------------------------------*/

    if (!use_msvcrt) {
        EXPECT_BOS("base overflow")
        rc = qsort_s(array, RSIZE_MAX_MEM + 1, sizeof(array[0]), comp, NULL);
#ifdef HAVE___BUILTIN_OBJECT_SIZE
        ERR(ESNOSPC);
#else
        ERR(ESLEMAX);
#endif

        EXPECT_BOS("base overflow")
        rc = qsort_s(array, LEN, RSIZE_MAX_MEM + 1, comp, NULL);
#ifdef HAVE___BUILTIN_OBJECT_SIZE
        ERR(ESNOSPC);
#else
        ERR(ESLEMAX);
#endif
    }
#endif

    /*--------------------------------------------------*/

    /* allow empty array with safec, msvcrt does not! */
    if (!use_msvcrt) {
        rc = qsort_s(NULL, 0, 0, comp, NULL);
        ERR(EOK);
    }

    /*--------------------------------------------------*/

    /* sorted */
    rc = qsort_s(array, LEN, sizeof(array[0]), comp, NULL);
    if (!use_msvcrt) {
        ERR(EOK);
    }
    for (ind = 0; ind <= LEN; ++ind) {
        if (ind != array[ind].iv) {
            /* TODO check why msvcrt fails here */
            printf("%s %u  %s  got=%d expected=%d \n", __FUNCTION__, __LINE__,
                   use_msvcrt ? "Todo" : "Error", array[ind].iv, ind);
            if (!use_msvcrt)
                errs++;
        }
    }

    /*--------------------------------------------------*/

    /* sort many sizes and patterns, to exercise all heap rebalancing paths */
    if (!use_msvcrt) {
        static struct items big[300];
        unsigned seed = 12345;
        size_t n, j;
        int pattern;

        for (pattern = 0; pattern < 4; pattern++) {
            for (n = 1; n <= 300; n += (n < 140 ? 1 : 29)) {
                long sum = 0, sum1 = 0;
                for (j = 0; j < n; j++) {
                    seed = seed * 1103515245u + 12345u;
                    switch (pattern) {
                    case 0: /* random */
                        big[j].iv = (int)((seed >> 8) % 1000);
                        break;
                    case 1: /* reversed */
                        big[j].iv = (int)(n - j);
                        break;
                    case 2: /* few distinct values */
                        big[j].iv = (int)((seed >> 8) % 4);
                        break;
                    default: /* organ pipe */
                        big[j].iv = (int)(j < n / 2 ? j : n - j);
                        break;
                    }
                    big[j].sv = NULL;
                    sum += big[j].iv;
                }
                rc = qsort_s(big, n, sizeof(big[0]), comp, NULL);
                ERR(EOK);
                for (j = 0; j < n; j++) {
                    sum1 += big[j].iv;
                    if (j && big[j - 1].iv > big[j].iv) {
                        debug_printf("%s %u  pattern %d n=%u not sorted at %u\n",
                                     __FUNCTION__, __LINE__, pattern,
                                     (unsigned)n, (unsigned)j);
                        errs++;
                        break;
                    }
                }
                if (sum != sum1) {
                    debug_printf("%s %u  pattern %d n=%u elements lost\n",
                                 __FUNCTION__, __LINE__, pattern, (unsigned)n);
                    errs++;
                }
            }
        }
    }

    /*--------------------------------------------------*/

    /* unknown base size: nmemb or size > RSIZE_MAX_MEM */
    if (!use_msvcrt) {
        static volatile size_t hsz = 32;
        char *hp = (char *)malloc(hsz);

        if (!hp) {
            return errs + 1;
        }
        rc = qsort_s(hp, RSIZE_MAX_MEM + 1, 1, comp, NULL);
        ERR(ESLEMAX);
        rc = qsort_s(hp, 1, RSIZE_MAX_MEM + 1, comp, NULL);
        ERR(ESLEMAX);
        free(hp);
    }

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_qsort_s()); }
#endif

#endif /* sec_api */
