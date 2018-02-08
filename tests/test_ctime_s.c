/*------------------------------------------------------------------
 * test_ctime_s
 * File 'os/ctime_s.c'
 * Lines executed:67.50% of 40
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"

#if defined(HAVE_CTIME_S) || defined(HAVE_ASCTIME_S)
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN   ( 128 )

static char   str1[LEN];

int test_ctime_s (void)
{
    errno_t rc;
    int errs = 0;
    int have_wine = 0;
    time_t timer;

    timer = time(NULL);

/*--------------------------------------------------*/
    /* even static we will use the native forceinline ctime_s */
#if defined(_WIN32) && (HAVE_NATIVE)
    use_msvcrt = true;
#endif
    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    rc = ctime_s(NULL, 0, &timer);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP,EINVAL);
#endif

    rc = ctime_s(str1, LEN, NULL);
    ERR_MSVC(ESNULLP,EINVAL);

/*--------------------------------------------------*/

    rc = ctime_s(str1, 25, &timer);
    ERR_MSVC(ESLEMIN,EINVAL);

#ifndef HAVE_CT_BOS_OVR
    rc = ctime_s(str1, RSIZE_MAX_STR+1, &timer);
    ERR_MSVC(ESLEMAX,0);
#endif

/*--------------------------------------------------*/

    timer = 0;
    rc = ctime_s(str1, LEN, &timer);
    if (use_msvcrt && rc == EINVAL)
        have_wine = 1;
    else {
        ERR(EOK);
    }

    timer = -1;
    rc = ctime_s(str1, LEN, &timer);
    ERR_MSVC(ESLEMIN,EINVAL);

    {
        struct tm *tm = gmtime(&timer);
        if (!tm) {
            printf("gmtime() failed\n");
            return errs+1;
        }
        memset(tm, 0, sizeof(struct tm));
#if SIZEOF_TIME_T < 8
        /* year 10000, ie 313392063599L would overflow on 32bit */
        timer = MAX_TIME_T_STR;
#else
        tm->tm_year = 10000;
        timer = mktime(tm);
        debug_printf("year 10000 = %ld\n", timer);
        if (timer != -1)
            timer++;
#endif
    }

    /* eg. 313360441200L */
    rc = ctime_s(str1, LEN, &timer);
    if (timer == -1) {
        ERR_MSVC(ESLEMIN, EINVAL);
    } else {
        ERR_MSVC(ESLEMAX, 0);
    }
/*
#ifndef __MINGW32__
    ERR(ESLEMAX);
#else
    ERR(0);
#endif
*/

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_ctime_s());
}
#endif
