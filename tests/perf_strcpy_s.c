/*
 * perf_strcpy_s.c
 *
 * gcc-7 darwin:
 *   objsize --disable-constraint-handler
 *         strcpy_s.o 1464
 *   default:
 *         strcpy_s.o 1972  +34.7%, 508 bytes
 *
 *   Speed overhead: 91% --disable-constraint-handler
 *                   91% default
 *
 * clang-7 darwin:
 *   --disable-constraint-handler
 *         strcpy_s.o 1132
 *   with:
 *         strcpy_s.o 1688  +33.1%, 420 bytes
 *
 *   Speed overhead: 89% --disable-constraint-handler
 *                   90% default
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include "perf_private.h"

#if defined(TEST_MSVCRT) && defined(HAVE_STRCPY_S)
#undef strcpy_s
#endif

#define LEN (1024)

char str1[LEN];
char str2[LEN];

static double timing_loop(uint32_t len, uint32_t loops) {
    uint32_t i;
    size_t errors = 0;

    uint64_t clock_start;
    uint64_t clock_end;

    clock_t sl_clock_diff;
    clock_t sd_clock_diff;

    double sl_clock_dur;
    double sd_clock_dur;
    double percent;

    for (i = 0; i < LEN - 1; i++) {
        str1[i] = 33;
    }
    for (i = 0; i < LEN - 1; i++) {
        str2[i] = 44;
    }
    str1[LEN - 1] = '\0';
    str2[LEN - 1] = '\0';

    /*printf("\n Timing %d byte copy, %u loops \n", len, loops); */

    /*
     * Safe C Lib Routine
     */
    clock_start = timer_start();
    for (i = 0; i < loops; i++) {
        volatile errno_t rc;
        rc = strcpy_s(str1, LEN, str2);
        errors += rc;
    }
    clock_end = timer_end();

    /*
     * Note that background stuff continues to run, i.e. interrupts, so
     * we need to compute average time per loop
     */
    sl_clock_diff = (clock_end - clock_start) / loops;

    /*
     * Standard C Routine
     */
    clock_start = timer_start();
    for (i = 0; i < loops; i++) {
        volatile char *rc;
        rc = strcpy(str1, str2);
        errors += *rc;
    }
    clock_end = timer_end();

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
    printf("%u  %u  strcpy_s %1.6f  strcpy %1.6f  diff %1.6f  %2.2f %%\n",
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
    printf("avg: %2.2f%%\n", avg / 8.0);

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
    printf("avg: %2.2f%%\n", avg / 16.0);

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
