/*------------------------------------------------------------------
 * test_timingsafe_memcmp
 * File 'timingsafe_memcmp.c'
 * Lines executed:68.18% of 22
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN (1024)

static uint8_t mem1[LEN];
static uint8_t mem2[LEN];
int test_timingsafe_memcmp(void);

int test_timingsafe_memcmp(void) {
    int ind;
    int std_ind;
    int len;
    errno_t rc = 0;
    int errs = 0;

    /*--------------------------------------------------*/

    /* no short cut */
    ind = timingsafe_memcmp(mem1, mem1, LEN);
    INDZERO();

    /*--------------------------------------------------*/

    len = 1;
    memset(mem1, 55, len);
    memset(mem2, 55, len);

    ind = timingsafe_memcmp(mem1, mem2, len);
    INDZERO();
    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 2;
    memset(mem1, 55, LEN);
    memset(mem2, 55, LEN);

    ind = timingsafe_memcmp(&mem1[1], &mem2[1], len);
    INDZERO();
    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 32;
    memset(mem1, 55, len);
    memset(mem2, 55, len);

    ind = timingsafe_memcmp(mem1, mem2, len);
    INDZERO();
    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 32;
    memset(mem1, 55, len);
    memset(mem2, 55, len);
    memset(&mem2[(len / 2)], 35, len);

    ind = timingsafe_memcmp(mem1, mem2, len);
    if (ind < 1) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 32;
    memset(mem1, 55, len);
    memset(mem2, 55, len);
    memset(&mem2[(len / 2)], 65, len);

    ind = timingsafe_memcmp(mem1, mem2, len);
    if (ind > -1) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 3;
    memset(mem1, 55, len);
    memset(mem2, 55, len);

    ind = timingsafe_memcmp(mem1, mem2, len);
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
int main(void) { return (test_timingsafe_memcmp()); }
#endif
