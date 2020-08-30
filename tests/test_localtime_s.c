/*------------------------------------------------------------------
 * test_localtime_s
 * File 'os/localtime_s.c'
 * Lines executed:100.00% of 20
 *
 *------------------------------------------------------------------
 * fails: ubuntu-arm32-linux{,-qemu} (arm32v7), ubuntu-s390x-linux-qemu
 * but passes debian-arm32-linux, {debian,fedora}-s390x-linux
 */

#include "test_private.h"
#include "safe_lib.h"

/* conflicting API */
#ifndef HAVE_MINGW64
int test_localtime_s(void);

int test_localtime_s(void) {
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

/* unreliable on BSD or DARWIN, esp. with TZ!=GMT */
#ifndef BSD_OR_DARWIN_LIKE
    timer = 12; /* This is timezone sensitive, may lead to <0 if 0 */
    tmptr = localtime_s(&timer, &tm);
    ERRNO(0);
    PTRNN(tmptr);
    /* leak needs to be supressed, darwin valgrind bug */
#endif

    timer = -1;
    tmptr = localtime_s(&timer, &tm);
    ERRNO(EOVERFLOW);
    PTRNULL(tmptr);

    tmptr = localtime(&timer); /* leaks */
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
    tmptr = localtime_s(&timer, &tm);
    ERRNO(EOVERFLOW);
    PTRNULL(tmptr);

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_localtime_s()); }
#endif
#else
int main(void) {
    printf("skipped under windows sec_api: reversed arguments\n");
    return 0;
}
#endif
