/*------------------------------------------------------------------
 * test_memcpy_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 1024 )

static uint8_t  mem1[LEN+2];
static uint8_t  mem2[LEN+2];

int test_memcpy_s (void)
{
    errno_t rc;
    uint32_t i;
    rsize_t len;

/*--------------------------------------------------*/

    rc = memcpy_s(NULL, LEN, mem2, LEN);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, 0, mem2, LEN);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, RSIZE_MAX_MEM+1, mem2, LEN);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, LEN, NULL, LEN);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, 10, mem2, 0);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, LEN, mem2, RSIZE_MAX_MEM+1);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN+1; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    } else {

    for (i=0; i<len; i++) {
        if (mem1[i] != mem2[i]) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

    if (mem1[i] != 33) {
        printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, len, mem1[len], mem2[len]);
    }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN+1; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, (len+1) );
    if (rc == EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);

    } else {

    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

    if (mem1[i] != 33) {
        printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, len, mem1[len], mem2[len]);
    }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN+2; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN/2;
    rc = memcpy_s(mem1, len, mem2, LEN);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);

    } else {

    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

    if (mem1[len] != 33) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, len, mem1[len], mem2[len]);
    }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN+2; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, 0);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);

    } else {

    /* verify mem1 was zeroed */
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

    if (mem1[len] != 33) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, len, mem1[len], mem2[len]);
    }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, RSIZE_MAX_MEM+1);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    } else {

    /* verify mem1 was zeroed */
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

    if (mem1[len] == 0) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
    }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }

    /* same ptr - no move */
    rc = memcpy_s(mem1, LEN, mem1, LEN);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }
    for (i=0; i<LEN; i++) { mem2[i] = 65; }

    /* overlap */
    len = 100;
    rc = memcpy_s(&mem1[0], len, &mem1[10], len);
    if (rc != ESOVRLP) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    } else {

    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

    if (mem1[len] == 0) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
    }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }
    for (i=0; i<LEN; i++) { mem2[i] = 65; }

    /* overlap */
    len = 100;
    rc = memcpy_s(&mem1[10], len, &mem1[0], len);
    if (rc != ESOVRLP) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    } else {

    for (i=10; i<len+10; i++) {
        if (mem1[i] != 0) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 35; }
    for (i=0; i<LEN; i++) { mem2[i] = 55; }

    len = 5;
    rc = memcpy_s(mem1, len, mem2, len);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    } else {

    for (i=0; i<len; i++) {
        if (mem1[i] != 55) {
            printf("%d - %d m1=%d\n",
                 __LINE__, i, mem1[i]);
        }
    }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 35; }
    for (i=0; i<LEN; i++) { mem2[i] = 55; }

    rc = memcpy_s(mem1, LEN, mem2, LEN/2);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    } else {

    for (i=0; i<LEN/2; i++) {
        if (mem1[i] != 55) {
            printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
    }
    }

/*--------------------------------------------------*/
/*--------------------------------------------------*/

    return (0);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_memcpy_s());
}
#endif
