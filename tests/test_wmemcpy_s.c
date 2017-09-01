/*------------------------------------------------------------------
 * test_wmemcpy_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 1024 )

int main()
{
    errno_t rc;
    uint32_t len;
    uint32_t i;

    wchar_t  mem1[LEN];
    wchar_t  mem2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = wmemcpy_s(NULL, LEN, mem2, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = wmemcpy_s(mem1, 0, mem2, LEN);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = wmemcpy_s(mem1, RSIZE_MAX_MEM16+1, mem2, LEN);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    rc = wmemcpy_s(mem1, LEN, NULL, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = wmemcpy_s(mem1, 10, mem2, 0);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = wmemcpy_s(mem1, LEN, mem2, RSIZE_MAX_MEM16+1);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 1;
    rc = wmemcpy_s(mem1, len, mem2, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != mem2[i]) {
            debug_printf("%d - %d m1=%lc  m2=%lc  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 2;
    rc = wmemcpy_s(mem1, len, mem2, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != mem2[i]) {
            debug_printf("%d - %d m1=%lc  m2=%lc  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* slen greater than dmax */
    rc = wmemcpy_s(mem1, LEN/2, mem2, LEN);
    ERR(ESNOSPC)
    /* verify mem1 was zeroed */
    for (i=0; i<LEN/2; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%lc  m2=%lc  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    rc = wmemcpy_s(mem1, LEN, mem2, 0);
    ERR(ESZEROL)
    /* verify mem1 was zeroed */
    for (i=0; i<LEN; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%lc  m2=%lc  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    rc = wmemcpy_s(mem1, LEN, mem2, RSIZE_MAX_MEM16+1);
    ERR(ESLEMAX)
    /* verify mem1 was zeroed */
    for (i=0; i<LEN; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%lc  m2=%lc  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }
    for (i=0; i<LEN; i++) { mem2[i] = 65; }

    /* same ptr - no move */
    rc = wmemcpy_s(mem1, LEN, mem1, LEN);
    ERR(EOK)
/*--------------------------------------------------*/

    for (i=0; i<100; i++) { mem1[i] = 25; }
    for (i=10; i<100; i++) { mem1[i] = 35; }

    /* overlap */
    rc = wmemcpy_s(&mem1[0], 100, &mem1[10], 100);
    ERR(ESOVRLP)
    /* verify mem1 was zeroed */
    for (i=0; i<100; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%lc  \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<100; i++) { mem1[i] = 25; }
    for (i=10; i<100; i++) { mem1[i] = 45; }

    /* overlap error */
    rc = wmemcpy_s(&mem1[10], 100, &mem1[0], 100);
    ERR(ESOVRLP)
    /* verify mem1 was zeroed */
    for (i=10; i<100; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%lc  \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    rc = wmemcpy_s(mem1, LEN, mem2, LEN/2);
    ERR(EOK)
    for (i=10; i<LEN/2; i++) {
        if (mem1[i] != 44) {
            debug_printf("%d - %d m1=%lc  m2=%lc  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    return (errs);
}
