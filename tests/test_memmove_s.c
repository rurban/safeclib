/*------------------------------------------------------------------
 * test_memmove_s
 *
 *
 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "safe_types.h"
#include "safe_mem_lib.h"


#define debug_printf  printf


#define LEN   ( 1024 )


int main()
{
    errno_t rc;
    uint32_t i;
    uint32_t len;

    uint8_t  mem1[LEN];
    uint8_t  mem2[LEN];

/*--------------------------------------------------*/

    rc = memmove_s(NULL, LEN, mem2, LEN);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memmove_s(mem1, 0, mem2, LEN);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memmove_s(mem1, RSIZE_MAX_MEM+1, mem2, LEN);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memmove_s(mem1, LEN, NULL, LEN);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memmove_s(mem1, 10, mem2, 0);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memmove_s(mem1, LEN, mem2, RSIZE_MAX_MEM+1);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* a valid move */  
    len = LEN; 
    rc = memmove_s(mem1, len, mem2, LEN);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    for (i=0; i<len; i++) {
        if (mem1[i] != mem2[i]) {
            printf("%d m1=%d  m2=%d  \n",
                 i, mem1[i], mem2[i]);  
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* length error */ 
    len = LEN/2; 
    rc = memmove_s(mem1, len, mem2, LEN);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    /* verify mem1 was zeroed */ 
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* invalid length - zero dest */ 
    len = LEN; 
    rc = memmove_s(mem1, len, mem2, 0);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    /* verify mem1 was zeroed */ 
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* invalid length - zero dest */ 
    len = LEN; 
    rc = memmove_s(mem1, len, mem2, RSIZE_MAX_MEM+1);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }

    /* verify mem1 was zeroed */ 
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* same ptr - no move */
    rc = memmove_s(mem1, LEN, mem1, LEN);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move */ 
    len = 20; 
    rc = memmove_s(&mem1[0], len, &mem1[10], len);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }
 
    for (i=0; i<len; i++) {
        if (mem1[i] != 35) {
            printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move */ 
    len = 20; 
    rc = memmove_s(&mem1[10], len, &mem1[0], len);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    for (i=0; i<LEN; i++) { mem2[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem2[i] = 35; }

    for (i=0; i<10; i++) {
        if (mem1[i] != 25) {
            printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
        }
    }

/*--------------------------------------------------*/
/*--------------------------------------------------*/

    return (0);
}

