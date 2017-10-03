/*------------------------------------------------------------------
 * test_memcpy32_s
 * File 'extmem/memcpy32_s.c'
 * Lines executed:100.00% of 28
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

    uint32_t  mem1[LEN];
    uint32_t  mem2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = memcpy32_s(NULL, LEN, mem2, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = memcpy32_s(mem1, 0, mem2, LEN);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = memcpy32_s(mem1, RSIZE_MAX_MEM32+1, mem2, LEN);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    rc = memcpy32_s(mem1, LEN, NULL, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = memcpy32_s(mem1, 10, mem2, 0);
#ifdef HAVE_C11
    ERR(EOK);
#else
    ERR(ESZEROL)
#endif

/*--------------------------------------------------*/

    rc = memcpy32_s(mem1, LEN, mem2, RSIZE_MAX_MEM32+1);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 1;
    rc = memcpy32_s(mem1, len, mem2, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != mem2[i]) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 2;
    rc = memcpy32_s(mem1, len, mem2, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != mem2[i]) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* slen greater than dmax */
    rc = memcpy32_s(mem1, LEN/2, mem2, LEN);
    ERR(ESNOSPC)
    /* verify mem1 was zeroed */
    for (i=0; i<LEN/2; i++) {
        if (mem1[i] != 0 ) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    rc = memcpy32_s(mem1, LEN, mem2, 0);
#ifdef HAVE_C11
    ERR(EOK);
#else
    ERR(ESZEROL)
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    rc = memcpy32_s(mem1, LEN, mem2, RSIZE_MAX_MEM32+1);
    ERR(ESLEMAX)
    /* verify mem1 was zeroed */
    for (i=0; i<LEN; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=0; i<LEN; i++) { mem2[i] = 35; }

    /* same ptr */
    rc = memcpy32_s(mem1, LEN, mem1, LEN);
    ERR(EOK)
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=0; i<LEN; i++) { mem2[i] = 35; }

    /* overlap */
    rc = memcpy32_s(&mem1[0], 100, &mem1[10], 100);
    ERR(ESOVRLP)
    /* verify mem1 was zeroed */
    for (i=0; i<100; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 65; }
    for (i=0; i<LEN; i++) { mem2[i] = 75; }

    /* overlap */
    rc = memcpy32_s(&mem1[10], 100, &mem1[0], 100);
    ERR(ESOVRLP)
    /* verify mem1 was zeroed */
    for (i=10; i<100; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 65; }
    for (i=0; i<LEN; i++) { mem2[i] = 75; }

    rc = memcpy32_s(mem1, LEN, mem2, LEN/2);
    ERR(EOK)
    for (i=0; i<LEN/2; i++) {
        if (mem1[i] != 75) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

/*--------------------------------------------------*/

    return (errs);
}
