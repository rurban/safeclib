/*------------------------------------------------------------------
 * test_bsearch_s
 * File 'misc/bsearch_s.c'
 * Lines executed:89.66% of 29
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <stdlib.h>

#ifdef HAVE_BSEARCH_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"
int test_bsearch_s(void);

#define LEN 10
/* needs to be sorted! */
struct items {
    int iv;
    const char *sv;
} array[] = {{0, "zero"},  {1, "one"},  {2, "two"}, {3, "three"},
             {4, "four"},  {5, "five"}, {6, "six"}, {7, "seven"},
             {8, "eight"}, {9, "nine"}, {10, "ten"}};

static int comp(const void *ptr1, const void *ptr2, void *ctx) {
    struct items *i1 = (struct items *)ptr1;
    struct items *i2 = (struct items *)ptr2;
    if (ctx) {
        errno = -1;
        return 0; /* shortcut just for testing */
    }
    return i1->iv > i2->iv ? 1 : i1->iv == i2->iv ? 0 : -1;
}

int test_bsearch_s(void) {
    int errs = 0;
#ifndef HAVE_CT_BOS_OVR
    int have_wine = 0;
#endif
    int i;
    struct items key;
    struct items *item;
    void *vitem;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty buf or bufsize")
    vitem = bsearch_s(NULL, array, LEN, 0, comp, NULL);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERRNO_MSVC(ESNULLP, EINVAL);
    PTRNULL(vitem);

    EXPECT_BOS("empty buf or bufsize")
    vitem = bsearch_s(&key, NULL, LEN, sizeof(array[0]), comp, NULL);
    if (use_msvcrt && errno == EOF) {
        printf("Using wine\n");
        have_wine = 1;
    }
    ERRNO_MSVC(ESNULLP, have_wine ? EOF : EINVAL);
    if (!have_wine)
        PTRNULL(vitem);

    EXPECT_BOS("empty compar")
    vitem = bsearch_s(&key, (const void *)array, LEN, sizeof(array[0]), NULL, NULL);
    ERRNO_MSVC(ESNULLP, EINVAL);
    PTRNULL(vitem);

    /*--------------------------------------------------*/

    EXPECT_BOS("base overflow")
    vitem = bsearch_s(&key, array, RSIZE_MAX_MEM + 1, sizeof(array[0]), comp, NULL);
    ERRNO_MSVC(ESNOSPC, EOF);
    if (!use_msvcrt)
        PTRNULL(vitem);

    EXPECT_BOS("base overflow")
    vitem = bsearch_s(&key, array, LEN, RSIZE_MAX_MEM + 1, comp, NULL);
    ERRNO_MSVC(ESNOSPC, EOF);
    if (!use_msvcrt)
        PTRNULL(vitem);
#endif
    /*--------------------------------------------------*/

    /* allow empty array? msvcrt disallows it */
    key.iv = 0;
    vitem = bsearch_s(&key, NULL, 0, 0, comp, NULL);
    PTRNULL(vitem);
    ERRNO_MSVC(0, EINVAL);

    /*--------------------------------------------------*/

    /* find em */
    for (i = 0; i < LEN; ++i) {
        key.iv = array[i].iv;
        item = (struct items *)bsearch_s(&key, array, LEN, sizeof(array[0]),
                                         comp, NULL);
        PTRNN(item);
        if (item != &array[i] && !use_msvcrt) {
            PTRNULL(item);
        }
        ERRNO(use_msvcrt ? EOF : 0);
    }

    /* cannot find em */
    key.iv = -1;
    item = (struct items *)bsearch_s(&key, array, LEN, sizeof(array[0]), comp,
                                     NULL);
    if (!use_msvcrt)
        PTRNULL(item);
    ERRNO(use_msvcrt ? EOF : 0);

    key.iv = 11;
    item = (struct items *)bsearch_s(&key, array, LEN, sizeof(array[0]), comp,
                                     NULL);
    if (!use_msvcrt)
        PTRNULL(item);
    ERRNO(use_msvcrt ? EOF : 0);

    /* special case for 3rd compar arg ctx */
    key.iv = 1;
    item = (struct items *)bsearch_s(&key, array, LEN, sizeof(array[0]), comp,
                                     array);
    PTRNN(item);
    ERRNO(-1);

    key.iv = 10;
    item = (struct items *)bsearch_s(&key, array, LEN, sizeof(array[0]), comp,
                                     array);
    PTRNN(item);
    ERRNO(-1);

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_bsearch_s()); }
