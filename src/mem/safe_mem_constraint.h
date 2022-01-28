/*------------------------------------------------------------------
 * safe_mem_constraint.h
 *
 * October 2008, Bo Berry
 *
 * Copyright (c) 2008, 2009 by Cisco Systems, Inc.
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

#ifndef __SAFE_MEM_CONSTRAINT_H__
#define __SAFE_MEM_CONSTRAINT_H__

#ifdef FOR_DOXYGEN
#include "safe_mem_lib.h"
#else
#include "safeclib_private.h"
#endif

/*
 * Function used by the libraries to invoke the registered
 * runtime-constraint handler. Always needed.
 */
EXTERN void invoke_safe_mem_constraint_handler(const char *msg, void *ptr,
                                               errno_t error);

/* Disable the constraint handlers with --disable-constraint-handler */
#ifdef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
#define invoke_safe_mem_constraint_handler(msg, ptr, error)
#define handle_mem_bos_chk_warn(func, dest, dmax, destbos)
#endif

static inline void handle_mem_error(void *restrict dest, rsize_t dmax,
                                    const char *err_msg, errno_t err_code) {
    memset(dest, 0, dmax);
    invoke_safe_mem_constraint_handler(err_msg, dest, err_code);
    return;
}

#ifndef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
void handle_mem_bos_chk_warn(const char *restrict func, void *restrict dest,
                             const rsize_t dmax, const size_t destbos);
#endif /* SAFECLIB_DISABLE_CONSTRAINT_HANDLER */

#endif /* __SAFE_MEM_CONSTRAINT_H__ */
