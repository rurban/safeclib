/*------------------------------------------------------------------
 * test_asctime_s
 * File 'os/asctime_s.c'
 * Lines executed:90.91% of 55
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"

#define LEN   ( 128 )

static char   str1[LEN];

int test_asctime_s (void)
{
    errno_t rc;
    int errs = 0;
    int old;
    int ind;
    struct tm *tm;
    const time_t timet = 20000000;
    char *str2;

    tm = gmtime(&timet);
    
/*--------------------------------------------------*/

    rc = asctime_s(NULL, 0, tm);
    ERR(ESNULLP);

    rc = asctime_s(str1, LEN, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = asctime_s(str1, 25, tm);
    ERR(ESLEMIN);

    rc = asctime_s(str1, RSIZE_MAX_STR+1, tm);
    ERR(ESLEMAX);

/*--------------------------------------------------*/

#define TM_RANGE(memb, mmin, mmax) \
    old = tm->tm_##memb;                \
    tm->tm_##memb = mmin-1;             \
    rc = asctime_s(str1, LEN, tm);	\
    ERR(ESLEMIN);                       \
                                        \
    tm->tm_##memb = mmax+1;             \
    rc = asctime_s(str1, LEN, tm);	\
    ERR(ESLEMAX);                       \
                                        \
    tm->tm_##memb = mmin;               \
    rc = asctime_s(str1, LEN, tm);	\
    ERR(EOK);                           \
    tm->tm_##memb = mmax;               \
    rc = asctime_s(str1, LEN, tm);	\
    tm->tm_##memb = old;                \
    ERR(EOK)


    TM_RANGE(sec,  0, 60);
    TM_RANGE(min,  0, 59);
    TM_RANGE(hour, 0, 23);
    TM_RANGE(mday, 1, 31);
    TM_RANGE(mon,  0, 11);
    TM_RANGE(year, 0, 8099);
    TM_RANGE(wday, 0, 6);
    TM_RANGE(yday, 0, 365);
    TM_RANGE(isdst,0, 1);

    /* stack buffer branch */
    tm = gmtime(&timet);
    tm->tm_year = 0;
    rc = asctime_s(str1, 40, tm);
    ERR(EOK);

    /* darwin asctime_r overflows >= 8100 */
    for (ind=8090; ind<=8200; ind++) {
        tm->tm_year = ind;
        rc = asctime_s(str1, 40, tm);
        if (rc == -1) {
            ERR(EOK);
            debug_printf("year %d => %d ", ind, rc);
        }
    }
    
/*--------------------------------------------------*/

    tm = gmtime(&timet);
    rc = asctime_s(str1, 40, tm);
    str2 = asctime(tm);
    EXPSTR(str1, str2);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_asctime_s());
}
#endif
