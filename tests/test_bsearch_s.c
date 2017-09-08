/*------------------------------------------------------------------
 * test_bsearch_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <stdlib.h>

#define LEN   10
/* needs to be sorted! */
struct items {
    int iv;
    const char *sv;
} array[] = {
    { 0, "zero" },
    { 1, "one" },
    { 2, "two" },
    { 3, "three" },
    { 4, "four" },
    { 5, "five" },
    { 6, "six" },
    { 7, "seven" },
    { 8, "eight" },
    { 9, "nine" },
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

int test_bsearch_s (void)
{
    int errs = 0;
    int i;
    struct items key;
    struct items *item;

/*--------------------------------------------------*/

    item = bsearch_s(NULL, array, LEN, sizeof(array[0]), comp, NULL);
    ERRNO(ESNULLP);
    PTRNULL(item);

    item = bsearch_s(&key, NULL, LEN, sizeof(array[0]), comp, NULL);
    ERRNO(ESNULLP);
    PTRNULL(item);

    item = bsearch_s(&key, array, LEN, sizeof(array[0]), NULL, NULL);
    ERRNO(ESNULLP);
    PTRNULL(item);

/*--------------------------------------------------*/

    item = bsearch_s(&key, array, RSIZE_MAX_MEM+1, sizeof(array[0]), comp, NULL);
    ERRNO(ESLEMAX);
    PTRNULL(item);

    item = bsearch_s(&key, array, LEN, RSIZE_MAX_MEM+1, comp, NULL);
    ERRNO(ESLEMAX);
    PTRNULL(item);

/*--------------------------------------------------*/

    /* allow empty array */
    key.iv = 0;
    item = (struct items *) bsearch_s(&key, NULL, 0, 0, comp, NULL);
    PTRNULL(item);
    ERRNO(0);

/*--------------------------------------------------*/

    /* find em */
    for (i = 0; i < LEN; ++i) {
        key.iv = array[i].iv;
        item = (struct items *) bsearch_s(&key, array, LEN, sizeof(array[0]), comp, NULL);
        PTRNN(item);
        if (item != &array[i]) {
            PTRNULL(item);
        }
        ERRNO(0);
    }

    /* cannot find em */
    key.iv = -1;
    item = (struct items *) bsearch_s(&key, array, LEN, sizeof(array[0]), comp, NULL);
    PTRNULL(item);
    ERRNO(0);

    key.iv = 11;
    item = (struct items *) bsearch_s(&key, array, LEN, sizeof(array[0]), comp, NULL);
    PTRNULL(item);
    ERRNO(0);

    /* special case for 3rd compar arg ctx */
    key.iv = 1;
    item = (struct items *) bsearch_s(&key, array, LEN, sizeof(array[0]), comp, array);
    PTRNN(item);
    ERRNO(-1);

    key.iv = 10;
    item = (struct items *) bsearch_s(&key, array, LEN, sizeof(array[0]), comp, array);
    PTRNN(item);
    ERRNO(-1);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_bsearch_s());
}
#endif
