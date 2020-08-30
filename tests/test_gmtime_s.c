/*------------------------------------------------------------------
 * test_gmtime_s
 * File 'os/gmtime_s.c'
 * Lines executed:100.00% of 20
 *
 *------------------------------------------------------------------
 * fails: ubuntu-arm32-linux{,-qemu} (arm32v7), ubuntu-s390x-linux-qemu
 * but passes: debian-arm32-linux, {debian,fedoar}-s390x-linux
 */

#include "test_private.h"
#include "safe_lib.h"

/* conflicting API */
#ifndef HAVE_MINGW64
int test_gmtime_s(void);

int test_gmtime_s(void) {
    int errs = 0;
    time_t timer;
    struct tm tm;
    struct tm *tmptr;

    timer = time(NULL);

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    tmptr = gmtime_s(&timer, NULL);
    ERRNO(ESNULLP);
    PTRNULL(tmptr);

    EXPECT_BOS("empty timer")
    tmptr = gmtime_s(NULL, &tm);
    ERRNO(ESNULLP);
    PTRNULL(tmptr);
#endif
    /*--------------------------------------------------*/

    timer = 0;
    tmptr = gmtime_s(&timer, &tm);
    ERRNO(0);
    PTRNN(tmptr);
#ifdef BSD_OR_DARWIN_LIKE
    /* leak needs to be supressed, darwin valgrind bug */
#endif

    timer = -1;
    tmptr = gmtime_s(&timer, &tm);
    ERRNO(EOVERFLOW);
    PTRNULL(tmptr);

    tmptr = gmtime(&timer);
    /* memset(&tm, 0, sizeof(struct tm)); */
#if SIZEOF_TIME_T < 8
    /* year 10000, ie 313392063599L would overflow on 32bit */
    timer = MAX_TIME_T_STR;
#else
    tmptr->tm_year = 10000;
    timer = mktime(tmptr);
    debug_printf("year 10000 = %ld\n", timer);
    timer++;
#endif
#ifdef BSD_OR_DARWIN_LIKE
    /* leak needs to be supressed, darwin valgrind bug */
#endif

    /* eg. 313392063599L */
    tmptr = gmtime_s(&timer, &tm);
    ERRNO(EOVERFLOW);
    PTRNULL(tmptr);

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_gmtime_s()); }
#endif
#else
int main(void) {
    printf("skipped under windows sec_api: reversed arguments\n");
    return 0;
}
#endif
