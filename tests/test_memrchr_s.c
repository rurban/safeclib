/*------------------------------------------------------------------
 * test_memrchr_s
 * File 'memrchr_s.c'
 * Lines executed:100.00% of 24
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"
#include <string.h>
#ifdef HAVE_MEMRCHR
#ifndef _ISOC11_SOURCE
extern void* memrchr(const void *, int, size_t);
#endif
#endif

#define LEN   ( 128 )

static uint8_t mem[LEN];

int test_memrchr_s (void)
{
    errno_t rc;
    int  ch;
    void *sub;
#ifdef HAVE_MEMRCHR
    void *std_sub;
#endif
    size_t len;
    int errs = 0;

/*--------------------------------------------------*/

    ch = 0;
    rc = memrchr_s(NULL, LEN, ch, &sub);
    ERR(ESNULLP);
    SUBNULL();

    rc = memrchr_s(mem, LEN, ch, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = memrchr_s(mem, 0, ch, &sub);
    ERR(ESZEROL)
    SUBNULL();

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memrchr_s(mem, RSIZE_MAX_STR+1, ch, &sub);
    ERR(ESLEMAX)
    SUBNULL();

    rc = memrchr_s(mem, LEN, 256, &sub);
    ERR(ESLEMAX)
    SUBNULL();
#endif

/*--------------------------------------------------*/

    mem[LEN-1] = '\0';
    rc = memrchr_s(mem, LEN, 0, &sub);
    ERR(EOK);
    PTREQ(sub, &mem[LEN-1]);

/*--------------------------------------------------*/

    len = 20;
              /* 012345678901234567890 */
    memcpy(mem, "keep it all together", len);

    rc = memrchr_s(mem, len+1, 0, &sub);
    ERR(EOK);
    PTREQ(sub, &mem[len]);

/*--------------------------------------------------*/

    /* at beginning */
    rc = memrchr_s(mem, len, 'k', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[0]);

    /* in the middle - left */
    rc = memrchr_s(mem, 5, 'e', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[2]);

    /* in the middle - right */
    rc = memrchr_s(mem, len, 'o', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[13]);

/*--------------------------------------------------*/

    rc = memrchr_s(mem, len, 'h', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[17]);

    rc = memrchr_s(mem, len, 'r', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[19]);

/*--------------------------------------------------*/

    rc = memrchr_s(mem, 3, 'i', &sub);
    ERR(ESNOTFND)
    SUBNULL();

    rc = memrchr_s(mem, len, 'i', &sub);
    ERR(EOK)

/*--------------------------------------------------*/

    rc = memrchr_s(mem, len, 1, &sub);
    ERR(ESNOTFND);
    SUBNULL();

    rc = memrchr_s(mem, len, 'e', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[18]);
#ifdef HAVE_MEMRCHR
    /* compare to legacy */
    std_sub = (void*)memrchr(mem, 'e', len);
    PTREQ(sub, std_sub);
#endif

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_memrchr_s());
}
#endif
