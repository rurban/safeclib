/*------------------------------------------------------------------
 * test_memset32_s
 * File 'extmem/memset32_s.c'
 * Lines executed:100.00% of 20
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#define LEN      ( 256 )

int main()
{
    errno_t rc;
    uint32_t len;
    uint32_t i;
    uint32_t  value;

    rsize_t  MAX = LEN*4;
    uint32_t mem1[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    value = 34;

    rc = memset32_s(NULL, MAX, value, LEN);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    value = 34;

    /* first check dest, then n */
    rc = memset32_s(NULL, MAX, value, 0);
    ERR(ESNULLP);

    /* check n first, then args 2-3 */
    rc = memset32_s(mem1, MAX, value, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);

    rc = memset32_s(mem1, 0, value, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);

    rc = memset32_s(mem1, MAX, 256, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memset32_s(mem1, RSIZE_MAX_MEM+1, value, LEN);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = memset32_s(mem1, LEN, value, RSIZE_MAX_MEM32+1);
    ERR(ESLEMAX); /* and set all */
    EXPMEM(mem1, 0, LEN, value, 4);
#endif

/*--------------------------------------------------*/

    rc = memset32_s(mem1, MAX, value, LEN+1);
    ERR(ESNOSPC) /* and set all */
    EXPMEM(mem1, 0, LEN, value, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 1;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 4);
    EXPMEM(mem1, len, LEN, 99, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 2;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 4);
    EXPMEM(mem1, len, LEN, 99, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 12;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 4);
    EXPMEM(mem1, len, LEN, 99, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 31;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 4);
    EXPMEM(mem1, len, LEN, 99, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 133;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 4);
    EXPMEM(mem1, len, LEN, 99, 4);

/*--------------------------------------------------*/

    return (errs);
}
