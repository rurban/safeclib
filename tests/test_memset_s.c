/*------------------------------------------------------------------
 * test_memset_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 256 )

static uint8_t mem1[LEN];

int test_memset_s (void)
{
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint8_t value;
    int errs = 0;

/*--------------------------------------------------*/

    value = 34;
    rc = memset_s(NULL, LEN, value, LEN);
#if !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
    if (rc != ESNULLP)
#else
    if (rc != EINVAL)
#endif
    {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    value = 34;

    /* no error with C11 */
    rc = memset_s(mem1, LEN, value, 0);
#if !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
    if (rc != ESZEROL)
#else
    if (rc != 0)
#endif
    {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 1;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 2;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 12;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 31;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 133;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_memset_s());
}
#endif
