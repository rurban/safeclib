/*------------------------------------------------------------------
 * test_memset_s
 * File 'memset_s.c'
 * Lines executed:100.00% of 22
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#ifdef HAVE_MEMSET_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN   ( 256 )
#define MAX   RSIZE_MAX_MEM

static uint8_t mem1[LEN];

int test_memset_s (void)
{
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint8_t value;
    int errs = 0;

/*--------------------------------------------------*/

#if defined HAVE_MEMSET_S && defined HAVE_C11 && defined WANT_C11
    use_msvcrt = true;
#else
    use_msvcrt = false;
#endif
    value = 34;
    rc = memset_s(NULL, LEN, value, LEN);
    if ( rc == ESNULLP ) {
        if (use_msvcrt)
            printf("safec overriding libc\n");
        use_msvcrt = false;
    } else {
        if (!use_msvcrt)
            printf("libc overriding safec\n");
        use_msvcrt = true;
    }
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    value = 34;

    /* first check dest, then n */
    rc = memset_s(NULL, LEN, value, 0);
    ERR_MSVC(ESNULLP, EINVAL);

    /* check n first, then args 2-3 */
    rc = memset_s(mem1, LEN, value, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memset_s(mem1, 0, value, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memset_s(mem1, LEN, 256, 0);
    ERR(EOK); /* still untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

/*--------------------------------------------------*/

    rc = memset_s(mem1, LEN, 256, LEN); /* XXX This should be caught at compile-time */
    ERR_MSVC(ESLEMAX, 0); /* no native overflow check on darwin! */
    if (use_msvcrt)
        EXPMEM(mem1, 0, LEN, 0, 1);

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = memset_s(mem1, MAX+1, value, LEN);
    ERR_MSVC(ESLEMAX, 0); /* and implementation defined */
    if (use_msvcrt)
        EXPMEM(mem1, 0, LEN, value, 1);
#endif
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = memset_s(mem1, LEN, value, MAX+1); /* TODO This should be caught at compile-time! */
    ERR_MSVC(ESLEMAX, EOVERFLOW); /* and set all */
    EXPMEM(mem1, 0, LEN, value, 1);

/*--------------------------------------------------*/

    rc = memset_s(mem1, LEN, value, LEN+1); /* TODO This should be caught at compile-time! */
    ERR_MSVC(ESNOSPC, EOVERFLOW); /* and set all */
    EXPMEM(mem1, 0, LEN, value, 1);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 1;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 2;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 12;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 31;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 133;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, value, 1);
    EXPMEM(mem1, len, LEN, 99, 1);

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
