/*------------------------------------------------------------------
 * safe_str_constraint.h
 *
 * October 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 Cisco Systems
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

#ifndef __SAFE_STR_CONSTRAINT_H__
#define __SAFE_STR_CONSTRAINT_H__

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

/*
 * Function used by the libraries to invoke the registered
 * runtime-constraint handler. Always needed.
 */
EXTERN void invoke_safe_str_constraint_handler(const char *restrict msg,
                                               void *restrict ptr,
                                               errno_t error);

/* Disable the constraint handlers with --disable-constraint-handler */
#ifdef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
#define invoke_safe_str_constraint_handler(msg, ptr, error)
#define handle_str_bos_chk_warn(func, dest, dmax, destbos)
#define handle_str_src_bos_chk_warn(func, dest, smax, srcbos, srcname, smaxname)
#endif

/*
 * Safe C Lib internal string routine to consolidate error handling.
 * With SAFECLIB_STR_NULL_SLACK clear the dest buffer to eliminate
 * partial copy.
 */
static inline void handle_error(char *restrict dest, const rsize_t dmax,
                                const char *restrict err_msg,
                                errno_t err_code) {
#ifdef SAFECLIB_STR_NULL_SLACK
    /* null string to eliminate partial copy */
    memset((void *)dest, 0, dmax);
    /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#else
    (void)dmax;
    *dest = '\0';
#endif

    invoke_safe_str_constraint_handler(err_msg, (void *)dest, err_code);
    return;
}

#ifndef SAFECLIB_DISABLE_WCHAR
/*
 * Safe C Lib internal string routine to consolidate error handling.
 * With SAFECLIB_STR_NULL_SLACK clear the dest wide buffer to eliminate
 * partial copy.
 */
static inline void handle_werror(wchar_t *restrict dest, const rsize_t dmax,
                                 const char *err_msg, errno_t err_code) {
#ifdef SAFECLIB_STR_NULL_SLACK
    /* null string to eliminate partial copy */
    memset((void *)dest, 0, dmax * sizeof(wchar_t));
    /*while (dmax) { *dest = '\0'; dmax--; dest++; }*/
#else
    (void)dmax;
    *dest = L'\0';
#endif

    invoke_safe_str_constraint_handler(err_msg, (void *)dest, err_code);
    return;
}
#endif

/*
 * Safe C Lib internal string handlers to handle overflows with compile-time
 * known dest and dmax sizes.
 */
int handle_str_bos_overload(const char *restrict msg, char *restrict dest,
                            const rsize_t dmax);

#ifndef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
/*
 * Safe C Lib internal string handler to handle deviating compile-time known
 * dest and dmax sizes, when dest != sizeof(dest).
 */
void handle_str_bos_chk_warn(const char *restrict func, char *restrict dest,
                             const rsize_t dmax, const size_t destbos);

void handle_str_src_bos_chk_warn(const char *restrict func, char *restrict dest,
                                 const rsize_t smax, const size_t srcbos,
                                 const char *srcname, const char *smaxname);
#endif /* SAFECLIB_DISABLE_CONSTRAINT_HANDLER */

#endif /* __SAFE_STR_CONSTRAINT_H__ */
