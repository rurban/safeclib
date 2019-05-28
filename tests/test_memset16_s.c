/*------------------------------------------------------------------
 * test_memset16_s
 * File 'extmem/memset16_s.c'
 * Lines executed:93.33% of 15
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#define LEN (256)

int main(void) {
    errno_t rc;
    uint32_t len;
    uint32_t i;
    uint16_t value;

    rsize_t MAX = LEN * 2;
    uint16_t mem1[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

    value = 34;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = memset16_s(NULL, MAX, value, 0);
    ERR(ESNULLP);

    /* first check dest, then n */
    EXPECT_BOS("empty dest") EXPECT_BOS("dest overflow or empty")
    rc = memset16_s(NULL, MAX, value, LEN);
    ERR(ESNULLP);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    value = 34;

    EXPECT_BOS("dest overflow or empty")
    rc = memset16_s(mem1, RSIZE_MAX_MEM + 1, value, LEN);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    EXPECT_BOS("n*2 overflow >dest/dmax")
    rc = memset16_s(mem1, LEN, value, RSIZE_MAX_MEM16 + 1);
    ERR(ESLEMAX); /* and set all */
    EXPMEM(mem1, 0, LEN, value, 2);

    EXPECT_BOS("n*2 overflow >dest/dmax")
    rc = memset16_s(mem1, MAX, value, LEN + 1);
    ERR(ESNOSPC) /* and set all */
    EXPMEM(mem1, 0, LEN, value, 2);
#endif

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    value = 34;

    /* check n first, then args 2-3 */
    rc = memset16_s(mem1, MAX, value, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    rc = memset16_s(mem1, 0, value, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    rc = memset16_s(mem1, MAX, 256, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 1;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 2;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 12;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 31;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 99;
    }

    len = 133;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 2);
    EXPMEM(mem1, len, LEN, 99, 2);

    /*--------------------------------------------------*/

    return (errs);
}
