/*------------------------------------------------------------------
 * perf_private.h - Internal benchmarking tools
 *
 * 2020,2022  Reini Urban
 *
 * Copyright (c) 2017, 2020, 2022 Reini Urban
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#ifndef __PERF_PRIVATE_H__
#define __PERF_PRIVATE_H__

#include "config.h"
#include "safe_config.h"

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

static inline clock_t rdtsc();
static inline uint64_t timer_start();
static inline uint64_t timer_end();

static inline clock_t rdtsc() {
#ifndef ASM_INLINE
#define NO_CYCLE_COUNTER
    return clock();
#elif defined __x86_64__
    uint64_t a, d;
    ASM_INLINE volatile("rdtsc" : "=a"(a), "=d"(d));
    return (clock_t)(a | (d << 32));
#elif defined(__i386__)
    clock_t x;
    ASM_INLINE volatile("rdtsc" : "=A"(x));
    return x;
#elif defined(__ARM_ARCH) && (__ARM_ARCH >= 7) && (SIZEOF_SIZE_T == 4)
  // V7 is the earliest arch that has a standard cyclecount (some say 6)
  uint32_t pmccntr;
  uint32_t pmuseren;
  uint32_t pmcntenset;
  // Read the user mode perf monitor counter access permissions.
  ASM_INLINE volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
  if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
    ASM_INLINE volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
    if (pmcntenset & 0x80000000ul) {  // Is it counting?
      ASM_INLINE volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
      // The counter is set up to count every 64th cycle
      return (int64_t)(pmccntr) * 64;  // Should optimize to << 6
    }
  }
  return (uint64_t)rdtsc();
#elif defined(__aarch64__) && (SIZEOF_SIZE_T == 8)
  uint64_t pmccntr;
  uint64_t pmuseren = 1UL;
  // Read the user mode perf monitor counter access permissions.
  //ASM_INLINE volatile("mrs cntv_ctl_el0,  %0" : "=r" (pmuseren));
  if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
    ASM_INLINE volatile("mrs %0, cntvct_el0" : "=r" (pmccntr));
    return (uint64_t)(pmccntr) * 64;  // Should optimize to << 6
  }
  return (uint64_t)rdtsc();
#elif defined(__powerpc64__) || defined(__ppc64__)
    uint64_t tb;
    ASM_INLINE volatile (\
      "mfspr %0, 268"
      : "=r" (tb));
    return tb;
#elif defined(__powerpc__) || defined(__ppc__)
    // This returns a time-base, which is not always precisely a cycle-count.
    uint32_t tbu, tbl, tmp;
    ASM_INLINE volatile (\
      "0:\n"
      "mftbu %0\n"
      "mftbl %1\n"
      "mftbu %2\n"
      "cmpw %0, %2\n"
      "bne- 0b"
      : "=r" (tbu), "=r" (tbl), "=r" (tmp));
    return (((uint64_t) tbu << 32) | tbl);
#elif defined(__sparc__)
    uint64_t tick;
    ASM_INLINE(".byte 0x83, 0x41, 0x00, 0x00");
    ASM_INLINE("mov   %%g1, %0" : "=r" (tick));
    return tick;
#elif defined(__ia64__)
    uint64_t itc;
    ASM_INLINE("mov %0 = ar.itc" : "=r" (itc));
    return itc;
#else
#define NO_CYCLE_COUNTER
    return clock();
#endif
}

// see https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf
// 3.2.1 The Improved Benchmarking Method
static inline uint64_t timer_start()
{
#ifndef ASM_INLINE
    return (uint64_t)rdtsc();
#elif defined (__i386__) || (defined(__x86_64__) && SIZEOF_SIZE_T == 4)
  uint32_t cycles_high, cycles_low;
  ASM_INLINE volatile
      ("cpuid\n\t"
       "rdtsc\n\t"
       "mov %%edx, %0\n\t"
       "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
       "%eax", "%ebx", "%ecx", "%edx");
    return ((uint64_t)cycles_high << 32) | cycles_low;
#elif defined __x86_64__
  uint32_t cycles_high, cycles_low;
  ASM_INLINE volatile
      ("cpuid\n\t"
       "rdtsc\n\t"
       "mov %%edx, %0\n\t"
       "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
       "%rax", "%rbx", "%rcx", "%rdx");
  return ((uint64_t)cycles_high << 32) | cycles_low;
#else
  return (uint64_t)rdtsc();
#endif
}

static inline uint64_t timer_end()
{
#ifndef ASM_INLINE
    return (uint64_t)rdtsc();
#elif defined (__i386__) || (defined(__x86_64__) && defined (HAVE_BIT32))
  uint32_t cycles_high, cycles_low;
 ASM_INLINE volatile
      ("rdtscp\n\t"
       "mov %%edx, %0\n\t"
       "mov %%eax, %1\n\t"
       "cpuid\n\t": "=r" (cycles_high), "=r" (cycles_low)::
       "%eax", "%ebx", "%ecx", "%edx");
    return ((uint64_t)cycles_high << 32) | cycles_low;
#elif defined __x86_64__
  uint32_t cycles_high, cycles_low;
  ASM_INLINE volatile
      ("rdtscp\n\t"
       "mov %%edx, %0\n\t"
       "mov %%eax, %1\n\t"
       "cpuid\n\t": "=r" (cycles_high), "=r" (cycles_low)::
       "%rax", "%rbx", "%rcx", "%rdx");
  return ((uint64_t)cycles_high << 32) | cycles_low;
#else
  return (uint64_t)rdtsc();
#endif
}

#endif /* __PERF_PRIVATE_H__ */
