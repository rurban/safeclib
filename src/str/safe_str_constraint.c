/*------------------------------------------------------------------
 * safe_str_constraint.c
 *
 * October 2008, Bo Berry
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 *
 * Copyright (c) 2008, 2009, 2012 Cisco Systems
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
#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

static constraint_handler_t str_handler = NULL;

/**
 * @brief
 *    The set_str_constraint_handler_s function sets the runtime-constraint
 *    handler to be handler.
 * @details
 *    The runtime-constraint handler is the function to
 *    be called when a library function detects a runtime-constraint
 *    violation. Only the most recent handler registered with
 *    set_str_constraint_handler_s is called when a runtime-constraint
 *    violation occurs.
 *    When the handler is called, it is passed the following arguments in
 *    the following order:
 *        1.    A pointer to a character string describing the
 *              runtime-constraint violation.
 *        2.    A null pointer or a pointer to an implementation defined
 *              object.
 *        3.    If the function calling the handler has a return type declared
 *              as errno_t, the return value of the function is passed.
 *              Otherwise, a positive value of type errno_t is passed.
 *    The implementation has a default constraint handler that is used if no
 *    calls to the set_constraint_handler_s function have been made. The
 *    behavior of the default handler is implementation-defined, and it may
 *    cause the program to exit or abort.  If the handler argument to
 *    set_constraint_handler_s is a null pointer, the implementation default
 *    handler becomes the current constraint handler.
 *
 * @remark SPECIFIED IN
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @see
 *    set_str_constraint_handler_s()
 */
EXPORT constraint_handler_t
set_str_constraint_handler_s(constraint_handler_t handler) {
    constraint_handler_t prev_handler = str_handler;
    if (NULL == handler) {
        str_handler = sl_default_handler;
    } else {
        str_handler = handler;
    }
    return prev_handler;
}
#ifdef __KERNEL__
EXPORT_SYMBOL(set_str_constraint_handler_s);
#endif

/* enable for decl */
#ifdef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
#undef invoke_safe_str_constraint_handler
#endif

/**
 * @brief
 *    Invokes the currently set constraint handler or the default.
 *    Can be disabled via \c --disable-constraint-handler
 *
 * @param *msg    Pointer to the message describing the error.
 * @param *ptr    Pointer to aassociated data. Can be NULL.
 * @param error   The error code encountered.
 *
 */
EXPORT void invoke_safe_str_constraint_handler(const char *restrict msg,
                                               void *restrict ptr,
                                               errno_t error) {
    if (NULL != str_handler) {
        str_handler(msg, ptr, error);
    } else {
        sl_default_handler(msg, ptr, error);
    }
}
#ifdef __KERNEL__
EXPORT_SYMBOL(invoke_safe_str_constraint_handler);
#endif

/* disable again */
#ifdef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
#define invoke_safe_str_constraint_handler(msg, ptr, error)
#endif

int handle_str_bos_overload(const char *restrict msg, char *restrict dest,
                            const rsize_t dmax) {
    /* clear the min of strlen and dmax(=destbos) */
    size_t len = strnlen_s(dest, dmax);
    errno_t err = EOVERFLOW;
    if (unlikely(len > RSIZE_MAX_STR)) {
        len = 1;
        err = ESLEMAX;
    }
    handle_error(dest, len, msg, err);
    return RCNEGATE(err);
}

#ifndef SAFECLIB_DISABLE_CONSTRAINT_HANDLER

void handle_str_bos_chk_warn(const char *restrict func, char *restrict dest,
                             const rsize_t dmax, const size_t destbos) {
    char msg[128];
    sprintf(msg, "%s: wrong dmax %lu, dest has size %lu", func,
            (unsigned long)dmax, (unsigned long)destbos);
    invoke_safe_str_constraint_handler(msg, (void *)dest, ESLEWRNG);
}

void handle_str_src_bos_chk_warn(const char *restrict func, char *restrict dest,
                                 const rsize_t smax, const size_t srcbos,
                                 const char *srcname, const char *smaxname) {
    char msg[128];
    sprintf(msg, "%s: wrong %s %lu, %s has size %lu", func, smaxname,
            (unsigned long)smax, srcname, (unsigned long)srcbos);
    invoke_safe_str_constraint_handler(msg, (void *)dest, ESLEWRNG);
}

#endif
