/*------------------------------------------------------------------
 * test_memmove_s
 * File 'mem/memmove_s.c'
 * Lines executed:100.00% of 25
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#ifdef HAVE_MEMMOVE_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN   ( 1024 )

static uint8_t  mem1[LEN+1];
static uint8_t  mem2[LEN];

int test_memmove_s (void)
{
    errno_t rc;
    uint32_t i;
    rsize_t len;
#ifndef __KERNEL__
    int ind;
#endif
    int errs = 0;

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }
    print_msvcrt(use_msvcrt);

    rc = memmove_s(NULL, LEN, mem2, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    rc = memmove_s(mem1, 0, mem2, LEN);
    ERR_MSVC(ESZEROL, ERANGE); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

/*--------------------------------------------------*/

    /* with clang-7 compile-time checks this errors */
#ifndef HAVE_CT_BOS_OVR
    rc = memmove_s(mem1, RSIZE_MAX_MEM+1, mem2, LEN);
    ERR_MSVC(ESLEMAX, 0); /* and implementation defined */
    if (!use_msvcrt)
        EXPMEM(mem1, 0, LEN, 33, 1);
    else
        EXPMEM(mem1, 0, LEN, 44, 1);
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }

    /* check n=0 first */
    rc = memmove_s(mem1, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memmove_s(NULL, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memmove_s(mem1, 0, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

    rc = memmove_s(mem1, 10, NULL, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 1);

/*--------------------------------------------------*/

    rc = memmove_s(mem1, LEN, NULL, LEN);
    ERR_MSVC(ESNULLP, EINVAL); /* and cleared */
    if (!use_msvcrt) {
        EXPMEM(mem1, 0, LEN, 0, 1); /* broken with msvcrt! */
    }

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = memmove_s(mem1, LEN, mem2, RSIZE_MAX_MEM+1);
    ERR_MSVC(ESLEMAX, ERANGE);  /* and cleared */
    if (!use_msvcrt) {
        EXPMEM(mem1, 0, LEN, 0, 1);
    }
#endif
/*--------------------------------------------------*/

    for (i=0; i<LEN+1; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++)   { mem2[i] = 44; }

    len = LEN;
    rc = memmove_s(mem1, len, mem2, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, 44, 1);
    EXPMEM(mem1, len, LEN+1, 33, 1);

    for (i=0; i<LEN+1; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++)   { mem2[i] = 44; }

    len = 1;
    rc = memmove_s(mem1, len, mem2, len);
    ERR(EOK);
    EXPMEM(mem1, 0, len, 44, 1);
    EXPMEM(mem1, len, LEN+1, 33, 1);

/*--------------------------------------------------*/

    for (i=0; i<LEN+1; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++)   { mem2[i] = 44; }

    /* length error */
    len = LEN/2;
    rc = memmove_s(mem1, len, mem2, LEN);
    ERR_MSVC(ESNOSPC, ERANGE); /* and cleared */

    if (!use_msvcrt) {
        EXPMEM(mem1, 0, len, 0, 1); /* broken with msvcrt */
    }
    if (mem1[len] != 33) {
        debug_printf("%d - %d m1=%d  m2=%d  \n",
                     __LINE__, (int)len, mem1[len], mem2[len]);
        errs++;
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* invalid length, untouched dest */
    len = LEN;
    rc = memmove_s(mem1, len, mem2, 0);
    ERR(EOK);
    /* verify mem1[0] was not zeroed */
    if (mem1[0] != 33) {
        debug_printf("%d - %d m1=%d  m2=%d  \n",
                     __LINE__, 0, (int)mem1[0], (int)mem2[0]);
        errs++;
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* invalid length - zero dest */
#ifndef HAVE_CT_BOS_OVR
    len = LEN;
    rc = memmove_s(mem1, len, mem2, RSIZE_MAX_MEM+1);
    ERR_MSVC(ESLEMAX, ERANGE);

    if (!use_msvcrt) {
        /* verify mem1 was zeroed */
        for (i=0; i<len; i++) {
            if (mem1[i] != 0) {
                debug_printf("%d - %d m1=%d \n",
                             __LINE__, i, mem1[i]);
                errs++;
            }
        }
    }
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* same ptr - no move */
    rc = memmove_s(mem1, LEN, mem1, LEN);
    ERR(EOK)

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move */
    len = 20;
    rc = memmove_s(&mem1[0], len, &mem1[10], len);
    ERR(EOK)
    else {
        for (i=0; i<len; i++) {
            if (mem1[i] != 35) {
                debug_printf("%d - %d m1=%d \n",
                             __LINE__, i, mem1[i]);
                errs++;
            }
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move */
    len = 20;
    rc = memmove_s(&mem1[10], len, &mem1[0], len);
    ERR(EOK)

    for (i=0; i<LEN; i++) { mem2[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem2[i] = 35; }

    for (i=0; i<10; i++) {
        if (mem1[i] != 25) {
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
    return (test_memmove_s());
}
#endif
