/*------------------------------------------------------------------
 * test_memchr_s
 * File 'memchr_s.c'
 * Lines executed:100.00% of 24
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 128 )

static uint8_t mem[LEN];

int test_memchr_s (void)
{
    errno_t rc;
    int  ch;
    void *sub;
    void *std_sub;

    size_t len;
    int errs = 0;

/*--------------------------------------------------*/

    ch = 0;
    rc = memchr_s(NULL, LEN, ch, &sub);
    ERR(ESNULLP);
    SUBNULL();

    rc = memchr_s(mem, LEN, ch, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = memchr_s(mem, 0, ch, &sub);
    ERR(ESZEROL)
    SUBNULL();

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memchr_s(mem, RSIZE_MAX_STR+1, ch, &sub);
    ERR(ESLEMAX)
    SUBNULL();

    rc = memchr_s(mem, LEN, 256, &sub);
    ERR(ESLEMAX)
    SUBNULL();
#endif

/*--------------------------------------------------*/

    *mem = '\0';
    rc = memchr_s(mem, LEN, 0, &sub);
    ERR(EOK);
    PTREQ(sub, mem);

/*--------------------------------------------------*/

    len = 20;
              /* 012345678901234567890 */
    memcpy(mem, "keep it all together", len);

    rc = memchr_s(mem, LEN, 0, &sub);
    ERR(EOK);
    PTREQ(sub, &mem[len]);

/*--------------------------------------------------*/

    /* at beginning */
    rc = memchr_s(mem, LEN, 'k', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[0]);

    /* in the middle - left */
    rc = memchr_s(mem, LEN, 'e', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[1]);

    /* in the middle - right */
    rc = memchr_s(mem, LEN, 'o', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[13]);

/*--------------------------------------------------*/

    rc = memchr_s(mem, len, 'h', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[17]);

    rc = memchr_s(mem, len, 'r', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[19]);

/*--------------------------------------------------*/

    rc = memchr_s(mem, 3, 'i', &sub);
    ERR(ESNOTFND)
    SUBNULL();

    rc = memchr_s(mem, LEN, 'i', &sub);
    ERR(EOK)

/*--------------------------------------------------*/

    rc = memchr_s(mem, len, 1, &sub);
    ERR(ESNOTFND);
    SUBNULL();

    rc = memchr_s(mem, LEN, 'e', &sub);
    ERR(EOK)
    PTREQ(sub, &mem[1]);
    /* compare to legacy */
    std_sub = memchr(mem, 'e', LEN);
    PTREQ(sub, std_sub);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_memchr_s());
}
#endif
