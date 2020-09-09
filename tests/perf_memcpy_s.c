/*
 * perf_memcpy_s.c
 * first numbers with old 32bit copy.
 *
 * gcc-7 darwin:
 *   objsize --disable-constraint-handler
 *         memcpy_s.o  952
 *   default:
 *         memcpy_s.o 1908  +100%,  956 bytes
 *
 *   Speed overhead: 88% --disable-constraint-handler
 *                   88% default
 * gcc-8 linux: 53% overhead
 *
 * clang-7 darwin:
 *   objsize --disable-constraint-handler
 *         memcpy_s.o   944
 *   default:
 *         memcpy_s.o  1564  +66%,  620 bytes
 *
 *   Speed overhead: 51% --disable-constraint-handler
 *                   56% default
 *
 * clang-7         5-20% -march=native --disable-constraint-handler
 * clang-4         5-20% -march=native --disable-constraint-handler
 * clang-3.9         87% -march=native --disable-constraint-handler
 * clang-3.8         86% -march=native --disable-constraint-handler
 * clang-3.7         84% -march=native --disable-constraint-handler
 * clang-3.4         89% -march=native --disable-constraint-handler
 * clang-3.3         88% -march=native --disable-constraint-handler
 * Apple/cc          87% -march=native --disable-constraint-handler
 * gcc-8             89% -march=native -Wa,-q --disable-constraint-handler
 * gcc-7             89% -march=native -Wa,-q --disable-constraint-handler
 * gcc-5             88% -march=native -Wa,-q --disable-constraint-handler
 * gcc-4.9           86% -march=native -Wa,-q --disable-constraint-handler
 * gcc-4.8           89% -march=native -Wa,-q --disable-constraint-handler
 * gcc-4.6           89% -march=native -Wa,-q --disable-constraint-handler
 * gcc-4.3           86% -march=native -Wa,-q --disable-constraint-handler
 *
 * clang-7 -Ofast    -2% 64bit -march=native --disable-constraint-handler
 * clang-7 -O2   -2 - 5% 64bit -march=native --disable-constraint-handler
 * gcc-7             77% 64bit -march=native -Wa,-q
 * --disable-constraint-handler
 */

#include "test_private.h"
#include "safe_mem_lib.h"
#include "perf_private.h"

#if defined(TEST_MSVCRT) && defined(HAVE_MEMCPY_S)
#undef memcpy_s
#endif

#define LEN (1024 * 10)

uint8_t mem1[LEN];
uint8_t mem2[LEN];

//static double timing_loop(uint32_t len, uint32_t loops);
//int main(void);

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
    clock_start = timer_start();
    for (i = 0; i < loops; i++) {
        volatile errno_t rc;
        rc = memcpy_s(mem1, len, mem2, len);
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
        rc = (char *)memcpy(mem1, mem2, len);
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
    printf("%u  %u  memcpy_s %1.6f  memcpy %1.6f  diff %1.6f  %2.2f %%\n",
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
