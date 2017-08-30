/*------------------------------------------------------------------
 * test_memzero_s
 *
 *
 *=
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 512 )

int main()
{
    errno_t rc;
    uint32_t i;
    uint32_t len;

    uint8_t mem1[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = memzero_s(NULL, LEN);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = memzero_s(mem1, 0);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = memzero_s(mem1, RSIZE_MAX_MEM+1);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }

    len = 1;
    rc = memzero_s(mem1, len);
    ERR(EOK)
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

    len = 2;
    rc = memzero_s(mem1, len);
    ERR(EOK)
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

    len = 3;
    rc = memzero_s(mem1, len);
    ERR(EOK)
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

    len = LEN;
    rc = memzero_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i] );
            errs++;
        }
    }

/*--------------------------------------------------*/

    return (errs);
}
