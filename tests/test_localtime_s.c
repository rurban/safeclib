/*------------------------------------------------------------------
 * test_localtime_s
 * File 'os/localtime_s.c'
 * Lines executed:100.00% of 20
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"

/* conflicting API */
#ifndef HAVE_MINGW64
int test_localtime_s (void);

int test_localtime_s (void)
{
    int errs = 0;
    time_t timer;
    struct tm tm;
    struct tm *tmptr;

    timer = time(NULL);

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    tmptr = localtime_s(&timer, NULL);
    ERRNO(ESNULLP);
    PTRNULL(tmptr);

    EXPECT_BOS("empty timer")
    tmptr = localtime_s(NULL, &tm);
    ERRNO(ESNULLP);
    PTRNULL(tmptr);
#endif
/*--------------------------------------------------*/

    timer = 0;
    tmptr = localtime_s(&timer, &tm);
    ERRNO(0);
    PTRNN(tmptr);

    timer = -1;
    tmptr = localtime_s(&timer, &tm);
    ERRNO(EOVERFLOW);
    PTRNULL(tmptr);

    {
        struct tm *tmp = localtime(&timer);
        memset(tmp, 0, sizeof(struct tm));
#if SIZEOF_TIME_T < 8
        /* year 10000, ie 313392063599L would overflow on 32bit */
        timer = MAX_TIME_T_STR;
#else
        tmp->tm_year = 10000;
        timer = mktime(tmp);
        debug_printf("year 10000 = %ld\n", timer);
        timer++;
#endif
    }

    /* eg. 313392063599L */
    tmptr = localtime_s(&timer, &tm);
    ERRNO(EOVERFLOW);
    PTRNULL(tmptr);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_localtime_s());
}
#endif
#else
int main (void)
{
    printf("skipped under windows sec_api: reversed arguments\n");
    return 0;
}
#endif
