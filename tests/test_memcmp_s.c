/*------------------------------------------------------------------
 * test_memcmp_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 128 )

static uint8_t  mem1[LEN];
static uint8_t  mem2[LEN];

int test_memcmp_s (void)
{
    errno_t rc;
    int32_t  ind;
    int32_t  std_ind;
    uint32_t len;

/*--------------------------------------------------*/

    rc = memcmp_s(NULL, LEN, mem2, LEN, &ind);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = memcmp_s(mem1, LEN, NULL, LEN, &ind);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = memcmp_s(mem1, LEN, mem2, LEN, NULL);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = memcmp_s(mem1, 0, mem2, LEN, &ind);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = memcmp_s(mem1, LEN, mem2, 0, &ind);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = memcmp_s(mem1, LEN, mem2, RSIZE_MAX_MEM+1, &ind);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    rc = memcmp_s(mem1, RSIZE_MAX_MEM+1, mem2, LEN, &ind);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

/*--------------------------------------------------*/

    len = 1;

    memset(mem1, 55, len);
    memset(mem2, 55, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    len = 2;

    memset(mem1, 55, LEN);
    memset(mem2, 55, LEN);

    rc = memcmp_s(&mem1[1], len, &mem2[1], len, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    len = 32;

    memset(mem1, 55, len);
    memset(mem2, 55, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

    std_ind = memcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    len = 32;

    memset(mem1, 55, len);
    memset(mem2, 55, len);
    memset( &mem2[(len/2)], 35, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

    if (ind < 1) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    len = 32;

    memset(mem1, 55, len);
    memset(mem2, 55, len);
    memset( &mem2[(len/2)], 65, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

    if (ind > -1) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    len = 3;

    memset(mem1, 55, len);
    memset(mem2, 55, len);

    rc = memcmp_s(mem1, len, mem2, len, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc);
    }

    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc);
    }

/*--------------------------------------------------*/

    return (0);
}

#ifndef __KERNEL__
int main (void)
{
    return (test_memcmp_s());
}
#endif
