/*------------------------------------------------------------------
 * test_timingsafe_bcmp
 * File 'timingsafe_bcmp.c'
 * Lines executed:61.11% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN (1024)

static uint8_t mem1[LEN];
static uint8_t mem2[LEN];
int test_timingsafe_bcmp(void);

int test_timingsafe_bcmp(void) {
    int ind;
#ifdef HAVE_BCMP
    int std_ind;
#endif
    int len;
    errno_t rc = 0;
    int errs = 0;

    /*--------------------------------------------------*/

    /* no short cut */
    ind = timingsafe_bcmp(mem1, mem1, LEN);
    INDZERO();

    /*--------------------------------------------------*/

    len = 1;
    memset(mem1, 55, len);
    memset(mem2, 55, len);

    ind = timingsafe_bcmp(mem1, mem2, len);
    INDZERO();
#ifdef HAVE_BCMP
    std_ind = bcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
#endif

    /*--------------------------------------------------*/

    len = 2;
    memset(mem1, 55, LEN);
    memset(mem2, 55, LEN);

    ind = timingsafe_bcmp(&mem1[1], &mem2[1], len);
    INDZERO();
#ifdef HAVE_BCMP
    std_ind = bcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
#endif
    /*--------------------------------------------------*/

    len = 32;
    memset(mem1, 55, len);
    memset(mem2, 55, len);

    ind = timingsafe_bcmp(mem1, mem2, len);
    INDZERO();
#ifdef HAVE_BCMP
    std_ind = bcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  Std=%d \n", __FUNCTION__, __LINE__, ind,
                     std_ind);
        errs++;
    }
#endif
    /*--------------------------------------------------*/

    len = 32;
    memset(mem1, 55, len);
    memset(mem2, 55, len);
    memset(&mem2[(len / 2)], 35, len);

    ind = timingsafe_bcmp(mem1, mem2, len);
    if (!ind) {
        debug_printf("%s %u  Ind=%d\n", __FUNCTION__, __LINE__, ind);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 32;
    memset(mem1, 55, len);
    memset(mem2, 55, len);
    memset(&mem2[(len / 2)], 65, len);

    ind = timingsafe_bcmp(mem1, mem2, len);
    if (!ind) {
        debug_printf("%s %u  Ind=%d\n", __FUNCTION__, __LINE__, ind);
        errs++;
    }
    /*--------------------------------------------------*/

    len = 3;
    memset(mem1, 55, len);
    memset(mem2, 55, len);

    ind = timingsafe_bcmp(mem1, mem2, len);
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d \n", __FUNCTION__, __LINE__, ind);
        errs++;
    }
    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
int main(void) { return (test_timingsafe_bcmp()); }
#endif
