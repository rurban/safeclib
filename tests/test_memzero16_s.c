/*------------------------------------------------------------------
 * test_memzero16_s
 * File 'extmem/memzero16_s.c'
 * Lines executed:88.89% of 9
 *
 *
 *=
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN (512)

int main(void) {
    errno_t rc;
    uint32_t i;
    uint32_t len;

    uint16_t mem1[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = memzero16_s(NULL, LEN);
    ERR(ESNULLP)
    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or len * 2")
    rc = memzero16_s(mem1, 0);
    ERR(ESZEROL)
    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = memzero16_s(mem1, RSIZE_MAX_MEM16 + 1);
    ERR(ESLEMAX)
#endif
    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    len = 1;
    rc = memzero16_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i = 0; i < len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    len = 2;
    rc = memzero16_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i = 0; i < len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    len = 3;
    rc = memzero16_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i = 0; i < len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }

    len = LEN;
    rc = memzero16_s(mem1, len);
    ERR(EOK)
    /* verify mem1 was zeroed */
    for (i = 0; i < len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d \n", __LINE__, i, mem1[i]);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}
