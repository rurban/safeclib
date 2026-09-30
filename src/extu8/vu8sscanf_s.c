/*------------------------------------------------------------------
 * vu8sscanf_s.c
 *
 * September 2026
 *
 * Copyright (c) 2026 by Reini Urban
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
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#endif

/**
 * @brief
 *    The vu8sscanf_s function reads a formatted utf-8 string, and
 *    writes to a list of arguments.
 *
 * @param[in]   dest   pointer to a NUL-terminated utf-8 string to read from
 * @param[in]   fmt    format-control string.
 * @param[out]  ap     arguments to write to
 *
 * @pre Neither \c dest nor \c fmt shall be a null pointer.
 * @pre \c fmt shall not contain the conversion specifier \c %n
 *
 * @return Number of receiving arguments successfully assigned, or \c EOF
 *         if read failure occurs before the first receiving argument
 *         was assigned or if there is a runtime constraint
 *         violation.
 *
 * @retval  > 0  on success, the number of arguments assigned
 * @retval  EOF  on error
 *
 * @see
 *    u8sscanf_s(), vsscanf_s()
 */
EXPORT int vu8sscanf_s(const char8_t *restrict dest, const char *restrict fmt,
                       va_list ap) {
    return vsscanf_s((const char *)dest, fmt, ap);
}
