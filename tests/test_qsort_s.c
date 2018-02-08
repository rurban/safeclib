/*------------------------------------------------------------------
 * test_qsort_s
 * File 'misc/qsort_s.c'
 * Lines executed:90.32% of 124
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <stdlib.h>

#ifdef HAVE_QSORT_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

/* conflicting API, void return. skip only shared */
#if defined(MINGW_HAS_SECURE_API) && !defined(DISABLE_DLLIMPORT)
int main (void)
{
    printf("skipped under windows sec_api: conflicting API\n");
    return 0;
}
#else

#define LEN   10
struct items {
    int iv;
    const char *sv;
} array[] = {
    { 1, "one" },
    { 3, "three" },
    { 8, "eight" },
    { 4, "four" },
    { 0, "zero" },
    { 5, "five" },
    { 2, "two" },
    { 6, "six" },
    { 9, "nine" },
    { 7, "seven" },
    { 10, "ten" }
};

static int
comp (const void *ptr1, const void *ptr2, void *ctx)
{
    struct items *i1 = (struct items *) ptr1;
    struct items *i2 = (struct items *) ptr2;
    if (ctx) {
        errno = -1;
        return 0; /* shortcut just for testing */
    }
    return i1->iv > i2->iv ? 1 : i1->iv == i2->iv ? 0 : -1;
}

int test_qsort_s (void)
{
    errno_t rc;
    int errs = 0;
    int ind;

/*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);

    rc = qsort_s(NULL, LEN, sizeof(array[0]), comp, NULL);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    if (!use_msvcrt) {
        ERR(ESNULLP);
    } /* msvcrt returns void */

    rc = qsort_s(array, LEN, sizeof(array[0]), NULL, NULL);
    if (!use_msvcrt) {
        ERR(ESNULLP);
    } /* msvcrt returns void */

/*--------------------------------------------------*/

    if (!use_msvcrt) {
#ifndef HAVE_CT_BOS_OVR
        rc = qsort_s(array, RSIZE_MAX_MEM+1, sizeof(array[0]), comp, NULL);
        ERR(ESLEMAX);

        rc = qsort_s(array, LEN, RSIZE_MAX_MEM+1, comp, NULL);
        ERR(ESLEMAX);
#endif
    }

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
            printf("%s %u  %s  got=%d expected=%d \n",
                   __FUNCTION__, __LINE__,
                   use_msvcrt ? "Todo" : "Error",
                   array[ind].iv, ind);
            if (!use_msvcrt)
                errs++;
        }
    }

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_qsort_s());
}
#endif

#endif /* sec_api */
