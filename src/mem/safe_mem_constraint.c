/*------------------------------------------------------------------
 * safe_mem_constraint.c
 *
 * October 2008, Bo Berry
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 *
 * Copyright (c) 2008-2012 Cisco Systems
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

#include "safe_mem_constraint.h"

static constraint_handler_t mem_handler = NULL;

/**
 * @brief
 *    The set_mem_constraint_handler_s function sets the runtime-constraint
 *    handler to be handler.
 * @details
 *    The runtime-constraint handler is the function to
 *    be called when a library function detects a runtime-constraint
 *    order:
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
set_mem_constraint_handler_s (constraint_handler_t handler)
{
    constraint_handler_t prev_handler = mem_handler;
    if (NULL == handler) {
        mem_handler = sl_default_handler;
    } else {
        mem_handler = handler;
    }
    return prev_handler;
}
#ifdef __KERNEL__
EXPORT_SYMBOL(set_mem_constraint_handler_s);
#endif /* __KERNEL__ */


/**
 * @brief
 *    Invokes the currently set constraint handler or the default.
 *
 * @param *msg    Pointer to the message describing the error
 * @param *ptr    Pointer to aassociated data. Can be NULL.
 * @param error  The error code encountered.
 *
 */
EXPORT void
invoke_safe_mem_constraint_handler (const char *msg,
                                    void *ptr,
                                    errno_t error)
{
    if (NULL != mem_handler) {
        mem_handler(msg, ptr, error);
    } else {
        sl_default_handler(msg, ptr, error);
    }
}
#ifdef __KERNEL__
EXPORT_SYMBOL(invoke_safe_mem_constraint_handler);
#endif /* __KERNEL__ */
