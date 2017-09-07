/*------------------------------------------------------------------
 * test_ctime_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"

#define LEN   ( 128 )

static char   str1[LEN];

int test_ctime_s (void)
{
    errno_t rc;
    int errs = 0;
    time_t timer;

    timer = time(NULL);
    
/*--------------------------------------------------*/

    rc = ctime_s(NULL, 0, &timer);
    ERR(ESNULLP);

    rc = ctime_s(str1, LEN, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = ctime_s(str1, 25, &timer);
    ERR(ESLEMIN);

    rc = ctime_s(str1, RSIZE_MAX_STR+1, &timer);
    ERR(ESLEMAX);

/*--------------------------------------------------*/

    timer = 0;
    rc = ctime_s(str1, LEN, &timer);
    ERR(EOK);

    timer = -1;
    rc = ctime_s(str1, LEN, &timer);
    ERR(ESLEMIN);

    {
        struct tm *tm = gmtime(&timer);
        memset(tm, 0, sizeof(struct tm));
        tm->tm_year = 10000;
        timer = mktime(tm);
        debug_printf("year 10000 = %ld\n", timer);
        timer++;
    }

    /* eg. 313392063599L */
    rc = ctime_s(str1, LEN, &timer);
    ERR(ESLEMAX);

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
