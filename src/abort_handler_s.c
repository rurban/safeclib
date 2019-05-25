/*------------------------------------------------------------------
 * abort_handler_s.c
 *
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 *
 * Copyright (c) 2012 Cisco Systems
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

/**
 * @brief
 *    This function writes a message on the standard error stream in
 *    an implementation-defined format. The message shall include the
 *    string pointed to by msg. The abort_handler_s function then calls
 *    the abort function.
 *
 * @remark SPECIFIED IN
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in] msg    Pointer to the message describing the error
 * @param[in] ptr    Pointer to aassociated data.  Can be NULL.
 * @param[in] error  The error code encountered.
 *
 * @see
 * ignore_handler_s()
 */

EXPORT void abort_handler_s(const char *restrict msg, void *restrict ptr,
                            errno_t error) {
    (void)ptr;
    slprintf("ABORT CONSTRAINT HANDLER: (%u) %s\n", error,
             (msg) ? msg : "Null message");
    slabort();
}
#ifdef __KERNEL__
EXPORT_SYMBOL(abort_handler_s);
#endif /* __KERNEL__ */
