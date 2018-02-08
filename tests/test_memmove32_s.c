/*------------------------------------------------------------------
 * test_memmove32_s
 * File 'extmem/memmove32_s.c'
 * Lines executed:100.00% of 23
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#define LEN   ( 1024 )
#define MAX   ( LEN * 4 )

int main()
{
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint32_t  mem1[LEN];
    uint32_t  mem2[LEN];
    rsize_t count = LEN;
    int errs = 0;

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }

    rc = memmove32_s(NULL, MAX, mem2, count);
    ERR(ESNULLP);
/*--------------------------------------------------*/

    rc = memmove32_s(mem1, 0, mem2, count);
    ERR(ESZEROL); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memmove32_s(mem1, RSIZE_MAX_MEM+1, mem2, count);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);
#endif
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = memmove32_s(mem1, MAX, NULL, count);
    ERR(ESNULLP); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 4);
/*--------------------------------------------------*/

    for (i=0; i<10; i++) { mem1[i] = 33; }
    rc = memmove32_s(mem1, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, 2);

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memmove32_s(mem1, MAX, mem2, RSIZE_MAX_MEM32+1);
    ERR(ESLEMAX); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 2);
#endif
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 1;
    rc = memmove32_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 4);
    EXPMEM(mem1, len, LEN-len, 33, 4);

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 2;
    rc = memmove32_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 4);
    EXPMEM(mem1, len, LEN-len, 33, 4);
    
    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* a valid move */
    len = LEN;
    rc = memmove32_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* count*4 greater than dmax */
    rc = memmove32_s(mem1, MAX, mem2, count+1);
    ERR(ESNOSPC); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* empty count */
    rc = memmove32_s(mem1, MAX, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* same ptr - no move */
    rc = memmove32_s(mem1, MAX, mem1, count);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++)     { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move + */
    len = 20;
    rc = memmove32_s(&mem1[0], LEN, &mem1[10], len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 35, 4);
    EXPMEM(mem1, len, LEN, 35, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++)     { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move - */
    len = 20;
    rc = memmove32_s(&mem1[10], (LEN-10)*2, &mem1[0], len);
    ERR(EOK);
    EXPMEM(mem1, 0, 10, 25, 4);
    EXPMEM(mem1, len, LEN, 35, 4);

/*--------------------------------------------------*/

    return (errs);
}
