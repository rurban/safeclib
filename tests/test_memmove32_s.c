/*------------------------------------------------------------------
 * test_memmove32_s
 * File 'extmem/memmove32_s.c'
 * Lines executed:100.00% of 26
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 1024 )

int main()
{
    errno_t rc;
    uint32_t i;
    uint32_t len;

    uint32_t  mem1[LEN];
    uint32_t  mem2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = memmove32_s(NULL, LEN, mem2, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = memmove32_s(mem1, 0, mem2, LEN);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = memmove32_s(mem1, RSIZE_MAX_MEM16+1, mem2, LEN);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    rc = memmove32_s(mem1, LEN, NULL, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = memmove32_s(mem1, 10, mem2, 0);
#ifdef HAVE_C11
    ERR(EOK)
#else
    ERR(ESZEROL)
#endif

/*--------------------------------------------------*/

    rc = memmove32_s(mem1, LEN, mem2, RSIZE_MAX_MEM16+1);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* a valid move */
    len = LEN;
    rc = memmove32_s(mem1, len, mem2, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != mem2[i]) {
            debug_printf("%d m1=%d  m2=%d  \n",
                 i, mem1[i], mem2[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* length error */
    len = LEN/2;
    rc = memmove32_s(mem1, len, mem2, LEN);
    ERR(ESNOSPC)
    /* verify mem1 was zeroed */
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* invalid length - zero dest */
    len = LEN;
    rc = memmove32_s(mem1, len, mem2, 0);
#ifdef HAVE_C11
    ERR(EOK)
#else
    ERR(ESZEROL)
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* invalid length - zero dest */
    len = LEN;
    rc = memmove32_s(mem1, len, mem2, RSIZE_MAX_MEM+1);
    ERR(ESLEMAX)
    /* verify mem1 was zeroed */
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* same ptr - no move */
    rc = memmove32_s(mem1, LEN, mem1, LEN);
    ERR(EOK)
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move */
    len = 20;
    rc = memmove32_s(&mem1[0], len, &mem1[10], len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != 35) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move */
    len = 20;
    rc = memmove32_s(&mem1[10], len, &mem1[0], len);
    ERR(EOK)
    for (i=0; i<10; i++) {
        if (mem1[i] != 25) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    return (errs);
}
