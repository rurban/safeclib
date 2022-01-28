/*------------------------------------------------------------------
 * mem_primitives_lib.h - Unguarded Memory Copy Routines
 *
 * October 2008, Bo Berry
 * December 2018, Reini Urban
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
 * Copyright (c) 2018,2019 Reini Urban
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

#ifndef __MEM_PRIMITIVES_LIB_H__
#define __MEM_PRIMITIVES_LIB_H__

#ifndef FOR_DOXYGEN
#include "safeclib_private.h"
#endif

#ifndef __KERNEL__
#if defined(HAVE_XMMINTRIN_H)
/* targeting SSE2 x86/x86-64 or compats */
#include <xmmintrin.h>
#if defined(__x86_64__) || (defined(__i386__) && defined(__SSE2__)) ||         \
    defined(_ARCH_PWR)
/* have mfence */
#define HAVE_X86_XMM
#else
/* only sfence */
#define HAVE_COMPAT_XMM
#endif
#elif defined(HAVE_EMMINTRIN_H)
#include <emmintrin.h>
#if defined(__x86_64__) || (defined(__i386__) && defined(__SSE2__)) ||         \
    defined(_ARCH_PWR)
/* have mfence */
#define HAVE_X86_XMM
#else
/* only sfence */
#define HAVE_COMPAT_XMM
#endif
#elif defined(_MSC_VER) && defined(HAVE_INTRIN_H)
/* Microsoft C/C++-compatible compiler */
#include <intrin.h>
/* have mfence */
#if defined(__x86_64__) || defined(__i686__)
#define HAVE_X86_XMM
#else /* arm __dmb(type) */
#define HAVE_ARM_DMB
#endif
#elif defined(__GNUC__) &&                                                     \
    (defined(__x86_64__) || (defined(__i386__) && defined(__SSE2__))) &&       \
    defined(HAVE_X86INTRIN_H)
/* GCC-compatible, targeting x86/x86-64 (in emmintrin.h) */
#define HAVE_X86_X86
#include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON__) && defined(HAVE_ARM_NEON_H)
/* GCC-compatible, targeting ARM with NEON */
#define HAVE_ARM_NEON
#include <arm_neon.h>
#elif defined(__GNUC__) && defined(__IWMMXT__) && defined(HAVE_MMINTRIN_H)
/* GCC-compatible, targeting ARM with WMMX */
#define HAVE_ARM_WMMX
#include <mmintrin.h>
#elif (defined(__GNUC__) || defined(__xlC__)) &&                               \
    (defined(__VEC__) || defined(__ALTIVEC__)) && defined(HAVE_ALTIVEC_H)
/* XLC or GCC-compatible, targeting PowerPC with VMX/VSX */
#define HAVE_PPC_ALTIVEC
#include <altivec.h>
#elif defined(__GNUC__) && defined(__SPE__) && defined(HAVE_SPE_H)
/* GCC-compatible, targeting PowerPC with SPE */
#define HAVE_PPC_SPE
#include <spe.h>
#elif defined(HAVE_MBARRIER_H)
/* Solaris */
#define HAVE_SOLARIS_MBARRIER
#include <mbarrier.h>
#endif
#endif

#ifndef __WORDSIZE
/* e.g. kernel. from <asm/types.h> */
#ifdef BITS_PER_LONG
#define __WORDSIZE BITS_PER_LONG
#else
#define __WORDSIZE (8 * SIZEOF_SIZE_T)
#endif
#endif

/* asm volatile ("mfence" ::: "memory") might have been overkill. No need to
   sync CPU's, just need to spill registers and observe compiler
   store/load order. glibc explicit_bzero() still just uses a simple
   COMPILER_BARRIER. But meltdown/spectre seem to require mfence, or even
   __sync_synchronize with smb. MEMORY_BARRIER should be the same as mb() write
   memory barrier in linux asm/system.h
 */
#ifdef ASM_INLINE
#define ASM_VOLATILE ASM_INLINE volatile
#define COMPILER_BARRIER                                                       \
    ASM_VOLATILE("" ::: "memory") /* the insecure fallback */
#else
/* warning no inline asm */
#define ASM_VOLATILE
#define COMPILER_BARRIER
#endif

#if defined __KERNEL__
#define MEMORY_BARRIER mb()
#elif defined(HAVE_X86_XMM) || defined(HAVE_X86_INTRIN) ||                     \
    defined(HAVE_X86_X86) || defined(HAVE__MM_MFENCE)
#define MEMORY_BARRIER _mm_mfence()
#elif defined(HAVE_ARM_DMB) && defined(_MSC_VER)
#define MEMORY_BARRIER __dmb(_ARM_BARRIER_OSH)
#elif defined(HAVE_MBARRIER_H) && (defined(sun) || defined(__sun))
/* Solaris 12 (membar) */
#define MEMORY_BARRIER __machine_rw_barrier()
#elif defined(__GNUC__) && defined(ASM_INLINE) &&                              \
    (defined(HAVE_PPC_ALTIVEC) || defined(HAVE_PPC_SPE))
#define MEMORY_BARRIER ASM_VOLATILE("lwsync" ::: "memory")
#elif defined(__GNUC__) && defined(ASM_INLINE) &&                              \
    (defined(__x86_64__) || defined(__SSE2__))
#define MEMORY_BARRIER ASM_VOLATILE("mfence" ::: "memory")
#elif defined(__GNUC__) && defined(ASM_INLINE) && defined(__i386__)
#define MEMORY_BARRIER ASM_VOLATILE("lock; addl $0,0(%%esp)" ::: "memory")
#elif defined(ASM_INLINE) && (defined(HAVE_ARM_NEON) || defined(HAVE_ARM_NEON))
#define MEMORY_BARRIER ASM_VOLATILE("dmb; dsb; isb" ::: "memory")
#elif (defined(__GNUC__) && __GNUC__ >= 4) || defined(HAVE___SYNC_SYNCHRONIZE)
#define MEMORY_BARRIER __sync_synchronize()
/* new gcc-5 memory_barrier insn for most archs:
   x86, mips, nios2, riscv, rs6000, s390, ia64, avr, alpha,
   arc, tilepro, xtensa, ..., but not in the arm-linux-gnueabihf-gcc 8.3 cross
   compiler */
/* #define MEMORY_BARRIER ASM_VOLATILE("memory_barrier" ::: "memory") */
#elif defined(HAVE_COMPAT_XMM) || defined(HAVE__MM_SFENCE)
/* x86-compat headers (e.g. rs6000, arm, ...) have no mfence */
#define MEMORY_BARRIER _mm_sfence()
#else
#define MEMORY_BARRIER COMPILER_BARRIER
#endif

/*
 * These are prototypes for _unguarded_ memory routines.  The caller must
 * validate all parameters prior to invocation.  Useful for diagnostics
 * and system initialization processing.
 */

/* moves (handles overlap) memory  */
extern void mem_prim_move(void *dest, const void *src, uint32_t len);

/* uint8_t moves (handles overlap) memory */
extern void mem_prim_move8(uint8_t *dest, const uint8_t *src, uint32_t len);

/* uint16_t moves (handles overlap) memory */
extern void mem_prim_move16(uint16_t *dest, const uint16_t *src, uint32_t len);

/* uint32_t moves (handles overlap) memory */
extern void mem_prim_move32(uint32_t *dest, const uint32_t *src, uint32_t len);

/* set bytes */
extern void mem_prim_set(void *dest, uint32_t len, uint8_t value);

/* set uint16_t's */
extern void mem_prim_set16(uint16_t *dest, uint32_t len, uint16_t value);

/* set uint32_t's */
extern void mem_prim_set32(uint32_t *dest, uint32_t len, uint32_t value);

#endif /* __MEM_PRIMITIVES_LIB_H__ */
