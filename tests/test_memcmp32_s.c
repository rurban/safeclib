/*------------------------------------------------------------------
 * test_memcmp32_s
 * File 'extmem/memcmp32_s.c'
 * Lines executed:100.00% of 36
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 128 )

int main()
{
    errno_t rc;
    uint32_t len;
    int32_t  ind;
    uint32_t i;

    uint32_t  mem1[LEN];
    uint32_t  mem2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = memcmp32_s(NULL, LEN, mem2, LEN, &ind);
    ERR(ESNULLP);

    rc = memcmp32_s(mem1, LEN, NULL, LEN, &ind);
    ERR(ESNULLP);

    rc = memcmp32_s(mem1, LEN, mem2, LEN, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = memcmp32_s(mem1, 0, mem2, LEN, &ind);
    ERR(ESZEROL);

    rc = memcmp32_s(mem1, LEN, mem2, 0, &ind);
    ERR(ESZEROL);

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memcmp32_s(mem1, RSIZE_MAX_MEM+1, mem2, LEN, &ind);
    ERR(ESLEMAX);
    rc = memcmp32_s(mem1, LEN, mem2, RSIZE_MAX_MEM+1, &ind);
    ERR(ESLEMAX);
#endif

/*--------------------------------------------------*/

    rc = memcmp32_s(mem1, LEN-1, mem2, LEN, &ind);
    ERR(ESNOSPC);

/*--------------------------------------------------*/

    /* short cut */
    rc = memcmp32_s(mem1, LEN, mem1, LEN, &ind);
    ERR(EOK);
    INDZERO();

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = i; }
    for (i=0; i<LEN; i++) { mem2[i] = i; }

    len = 1;
    rc = memcmp32_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    INDZERO();
    ind = memcmp(mem1, mem2, len*4);
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc );
        errs++;
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = i; }
    for (i=0; i<LEN; i++) { mem2[i] = i; }

    len = 2;
    rc = memcmp32_s(mem1, len, mem2, len, &ind);
    ERR(EOK);
    INDZERO();
    ind = memcmp(mem1, mem2, len*4);
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc );
        errs++;
    }
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = i; }
    for (i=0; i<LEN; i++) { mem2[i] = i; }

    rc = memcmp32_s(mem1, LEN, mem2, LEN, &ind);
    ERR(EOK);
    INDZERO();
    ind = memcmp(mem1, mem2, LEN*4);
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc );
        errs++;
    }
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = i; }
    for (i=0; i<LEN; i++) { mem2[i] = i; }

    for (i=10; i<LEN-10; i++) { mem2[i] = 5; }

    rc = memcmp32_s(mem1, LEN, mem2, LEN, &ind);
    ERR(EOK);
    INDCMP(<= 0);

/*--------------------------------------------------*/

    return (errs);
}
