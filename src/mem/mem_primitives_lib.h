/*------------------------------------------------------------------
 * mem_primitives_lib.h - Unguarded Memory Copy Routines
 *
 * October 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
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

#ifndef __WORDSIZE
/* e.g. kernel. from <asm/types.h> */
# ifdef BITS_PER_LONG
#  define __WORDSIZE BITS_PER_LONG
# else
#  define __WORDSIZE (8 * SIZEOF_SIZE_T)
# endif
#endif

/*
 * These are prototypes for _unguarded_ memory routines.  The caller must
 * validate all parameters prior to invocation.  Useful for diagnostics
 * and system initialization processing.
 */

/* moves (handles overlap) memory  */
extern void
mem_prim_move(void *dest, const void *src, uint32_t len);

/* uint8_t moves (handles overlap) memory */
extern void
mem_prim_move8(uint8_t *dest, const uint8_t *src, uint32_t len);

/* uint16_t moves (handles overlap) memory */
extern void
mem_prim_move16(uint16_t *dest, const uint16_t *src, uint32_t len);

/* uint32_t moves (handles overlap) memory */
extern void
mem_prim_move32(uint32_t *dest, const uint32_t *src, uint32_t len);


/* set bytes */
extern void
mem_prim_set(void *dest, uint32_t len, uint8_t value);

/* set uint16_t's */
extern void
mem_prim_set16(uint16_t *dest, uint32_t len, uint16_t value);

/* set uint32_t's */
extern void
mem_prim_set32(uint32_t *dest, uint32_t len, uint32_t value);

#endif  /* __MEM_PRIMITIVES_LIB_H__ */
