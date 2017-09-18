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

/* conflicting API */
#ifndef MINGW_HAS_SECURE_API

#define LEN   10
/* needs to be sorted! */
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

    rc = qsort_s(NULL, LEN, sizeof(array[0]), comp, NULL);
    ERR(ESNULLP);

    rc = qsort_s(array, LEN, sizeof(array[0]), NULL, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = qsort_s(array, RSIZE_MAX_MEM+1, sizeof(array[0]), comp, NULL);
    ERR(ESLEMAX);

    rc = qsort_s(array, LEN, RSIZE_MAX_MEM+1, comp, NULL);
    ERR(ESLEMAX);

/*--------------------------------------------------*/

    /* allow empty array */
    rc = qsort_s(NULL, 0, 0, comp, NULL);
    ERR(EOK);

/*--------------------------------------------------*/

    /* sorted */
    rc = qsort_s(array, LEN, sizeof(array[0]), comp, NULL);
    ERR(EOK);
    for (ind = 0; ind < LEN; ++ind) {
        INDCMP(!= array[ind].iv);
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

#else
int main (void)
{
    printf("skipped under windows sec_api: conflicting API\n");
    return 0;
}
#endif
