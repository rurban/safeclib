/*------------------------------------------------------------------
 * test_memcpy16_s
 * File 'extmem/memcpy16_s.c'
 * Lines executed:100.00% of 28
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#define LEN   ( 1024 )
#define MAX   ( LEN * 2 )

int main()
{
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint16_t  mem1[LEN];
    uint16_t  mem2[LEN];
    rsize_t count = LEN;
    int errs = 0;

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = memcpy16_s(NULL, MAX, mem2, count);
    ERR(ESNULLP);
/*--------------------------------------------------*/

    rc = memcpy16_s(mem1, 0, mem2, count);
    ERR(ESZEROL); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memcpy16_s(mem1, RSIZE_MAX_MEM+1, mem2, count);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = memcpy16_s(mem1, MAX, NULL, count);
    ERR(ESNULLP); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);
/*--------------------------------------------------*/

    for (i=0; i<10; i++) { mem1[i] = 33; }

    /* check n=0 first */
    rc = memcpy16_s(mem1, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);

    rc = memcpy16_s(NULL, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);

    rc = memcpy16_s(mem1, 0, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);

    rc = memcpy16_s(mem1, 10, NULL, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memcpy16_s(mem1, MAX, mem2, RSIZE_MAX_MEM16+1);
    ERR(ESLEMAX); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);
#endif
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 1;
    rc = memcpy16_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 2);
    EXPMEM(mem1, len, LEN-len, 33, 2);

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 2;
    rc = memcpy16_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 2);
    EXPMEM(mem1, len, LEN-len, 33, 2);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* count*2 greater than dmax */
    rc = memcpy16_s(mem1, MAX, mem2, count+1);
    ERR(ESNOSPC); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }
    for (i=0; i<LEN; i++) { mem2[i] = 65; }

    /* same ptr - no move */
    rc = memcpy16_s(mem1, MAX, mem1, count);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 55, 2);
    
/*--------------------------------------------------*/

    for (i=0; i<100; i++)  { mem1[i] = 25; }
    for (i=10; i<100; i++) { mem1[i] = 35; }

    /* overlap + */
    rc = memcpy16_s(&mem1[0], MAX, &mem1[10], 100);
    ERR(ESOVRLP); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);

/*--------------------------------------------------*/

    for (i=0; i<100; i++)  { mem1[i] = 25; }
    for (i=10; i<100; i++) { mem1[i] = 45; }

    /* overlap - */
    rc = memcpy16_s(&mem1[10], (LEN-10)*2, &mem1[0], 100);
    ERR(ESOVRLP); /* and cleared */
    EXPMEM(mem1, 10, (LEN-10)*2, 0, 2);
    EXPMEM(mem1, 0, 10, 25, 2);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    rc = memcpy16_s(mem1, MAX, mem2, count-10);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, count-10, 44, 2);
    EXPMEM(mem1, count-10, LEN, 33, 2);

/*--------------------------------------------------*/

    return (errs);
}
