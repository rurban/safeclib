/*------------------------------------------------------------------
 * test_memset16_s
 * File 'extmem/memset16_s.c'
 * Lines executed:100.00% of 21
 *
 *
 *=
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN      ( 256 )
#define MAXMEM   RSIZE_MAX_MEM16

int main()
{
    errno_t rc;
    uint32_t len;
    uint32_t i;
    uint16_t value;

    rsize_t MAX = LEN*2;
    uint16_t mem1[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    value = 34;

    rc = memset16_s(NULL, MAX, value, LEN);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    value = 34;

    rc = memset16_s(mem1, MAX, value, 0);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = memset16_s(mem1, RSIZE_MAX_MEM+1, value, LEN);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    rc = memset16_s(mem1, LEN, value, RSIZE_MAX_MEM16+1);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    rc = memset16_s(mem1, LEN, value, LEN+1);
    ERR(ESNOSPC)

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 1;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 2;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 12;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 31;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 133;
    value = 34;

    rc = memset16_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    return (errs);
}
