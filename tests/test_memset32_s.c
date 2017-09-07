/*------------------------------------------------------------------
 * test_memset32_s
 *
 *
 *=
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN      ( 256 )

int main()
{
    errno_t rc;
    uint32_t len;

    uint32_t i;
    uint32_t  value;

    rsize_t MAX = LEN*4;
    uint32_t  mem1[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    value = 34;

    rc = memset32_s(NULL, MAX, value, LEN);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    value = 34;

    rc = memset32_s(mem1, MAX, value, 0);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = memset32_s(mem1, RSIZE_MAX_MEM+1, value, LEN);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    rc = memset32_s(mem1, LEN, value, RSIZE_MAX_MEM32+1);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    rc = memset32_s(mem1, LEN, value, LEN+1);
    ERR(ESNOSPC)

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 1;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d  \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 2;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d  \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 12;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d  \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 31;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d  \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 133;
    value = 34;

    rc = memset32_s(mem1, MAX, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d  \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    return (errs);
}
