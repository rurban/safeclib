/*
 * perf_memset_s.c
 * with the 64bit version.
 *
 * gcc-7 darwin:
 *   objsize --disable-constraint-handler
 *         memset_s.o  760
 *   default:
 *         memset_s.o 1464  +92%,  704 bytes
 *
 *   Speed overhead: 0 - 16%
 *
 * clang-7 darwin:
 *   objsize --disable-constraint-handler
 *         memset_s.o   784
 *   default:
 *         memset_s.o  1228  +56%,  444 bytes
 *
 *   Speed overhead: -1 - 2%
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#ifndef __KERNEL__
#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif
#elif !defined(__x86_64__) && !defined(__i386__)
#error Not supported in Linux kernel space
#endif

#if defined(TEST_MSVCRT) && defined(HAVE_MEMSET_S)
#undef memset_s
#endif

#define LEN (1024 * 10)

uint8_t mem1[LEN];
uint8_t mem2[LEN];

static inline clock_t rdtsc();
static double timing_loop(uint32_t len, uint32_t loops);
int main(void);

static inline clock_t rdtsc() {
#ifdef __x86_64__
    uint64_t a, d;
    __asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
    return (clock_t)(a | (d << 32));
#elif defined(__i386__)
    clock_t x;
    __asm__ volatile("rdtsc" : "=A"(x));
    return x;
#else
#define NO_CYCLE_COUNTER
    return clock();
#endif
}

static double timing_loop(uint32_t len, uint32_t loops) {
    uint32_t i;
    size_t errors = 0;

    clock_t clock_start;
    clock_t clock_end;

    clock_t sl_clock_diff;
    clock_t sd_clock_diff;

    double sl_clock_dur;
    double sd_clock_dur;
    double percent;

    for (i = 0; i < LEN; i++) {
        mem1[i] = 33;
    }
    for (i = 0; i < LEN; i++) {
        mem2[i] = 44;
    }

    /*printf("\n Timing %d byte copy, %u loops \n", len, loops); */

    /*
     * Safe C Lib Routine
     */
    clock_start = rdtsc();
    for (i = 0; i < loops; i++) {
        volatile errno_t rc;
        rc = memset_s(mem1, len, 0, len);
        errors += rc;
    }
    clock_end = rdtsc();

    /*
     * Note that background stuff continues to run, i.e. interrupts, so
     * we need to compute average time per loop
     */
    sl_clock_diff = (clock_end - clock_start) / loops;

    /*
     * Standard C Routine
     */
    clock_start = rdtsc();
    for (i = 0; i < loops; i++) {
        volatile char *rc;
        rc = (char *)memset(mem1, 0, len);
        errors += *rc;
    }
    clock_end = rdtsc();

    /*
     * Note that background stuff continues to run, i.e. interrupts, so
     * we need to compute average time per loop
     */
    sd_clock_diff = (clock_end - clock_start) / loops;

    /* convert to seconds */
    sl_clock_dur = ((double)(sl_clock_diff) / CLOCKS_PER_SEC);
    sd_clock_dur = ((double)(sd_clock_diff) / CLOCKS_PER_SEC);
    percent = 100 * (sl_clock_dur - sd_clock_dur) / sl_clock_dur;

    /* just to disable optimizing away the inner loop */
    /* fprintf(stderr, "errors %lu\n", errors); */
    printf("%u  %u  memset_s %1.6f  memset %1.6f  diff %1.6f  %2.2f %%\n",
           loops, len, sl_clock_dur, sd_clock_dur,
           (sl_clock_dur - sd_clock_dur), percent);

    return percent;
}

int main(void) {
    double avg = 0.0;
    printf("\n");

    avg += timing_loop(1024, 400);
    avg += timing_loop(1024 * 2, 400);
    avg += timing_loop(1024 * 3, 400);
    avg += timing_loop(1024 * 4, 400);
    avg += timing_loop(1024 * 5, 400);
    avg += timing_loop(1024 * 6, 400);
    avg += timing_loop(1024 * 7, 400);
    avg += timing_loop(1024 * 8, 400);
    printf("avg: %2.2f %%\n", avg / 8.0);

    /*--------------------------------------------------*/

    printf("\n");

    avg += timing_loop(1024, 600);
    avg += timing_loop(1024 * 2, 600);
    avg += timing_loop(1024 * 3, 600);
    avg += timing_loop(1024 * 4, 600);
    avg += timing_loop(1024 * 5, 600);
    avg += timing_loop(1024 * 6, 600);
    avg += timing_loop(1024 * 7, 600);
    avg += timing_loop(1024 * 8, 600);
    printf("avg: %2.2f %%\n", avg / 16.0);

    /*--------------------------------------------------*/

    printf("\n");

    avg += timing_loop(1024, 800);
    avg += timing_loop(1024 * 2, 800);
    avg += timing_loop(1024 * 3, 800);
    avg += timing_loop(1024 * 4, 800);
    avg += timing_loop(1024 * 5, 800);
    avg += timing_loop(1024 * 6, 800);
    avg += timing_loop(1024 * 7, 800);
    avg += timing_loop(1024 * 8, 800);

    printf("\nsum: %2.2f %%\n", avg / 24.0);
    /*--------------------------------------------------*/

    return (0);
}
