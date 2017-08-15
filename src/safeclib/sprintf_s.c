#include "safe_str_lib.h"
#include "safe_str_constraint.h"
#include <stdarg.h>


/** 
 * NAME
 *    sprintf_s
 *
 * SYNOPSIS
 *    #include "safe_str_lib.h"
 *    errno_t 
 *    sprintf_s(char * restrict dest, rsize_t dmax, const char * restrict fmt, ...)
 *
 * DESCRIPTION
 *    The sprintf_s function composes a string with same test that 
 *    would be printed if format was used on printf. Instead of being 
 *    printed, the content is stored in dest.
 * 
 *    If the buffer dest is too small for the formatted text,
 *    including the terminating null, then the buffer is set to an
 *    empty string by placing a null character at dest[0], and the
 *    invalid parameter handler is invoked. Unlike _snprintf,
 *    sprintf_s guarantees that the buffer will be null-terminated
 *    unless the buffer size is zero.
 *
 * INPUT PARAMETERS
 *    dest        storage location for output buffer.
 *
 *    dmax        maximum number of characters to store in buffer.
 *
 *    fmt         format-control string.
 *
 *    ...         optional arguments
 *
 * RETURN VALUE
 *    On success the total number of characters written is returned. 
 *
 *    On failure a negative number is returned. 
 *     ESNULLP    NULL pointer
 *     ESZEROL    zero length
 *     ESLEMAX    length exceeds max limit
 */

int sprintf_s(char * restrict dest, rsize_t dmax, const char * restrict fmt, ...)
{
    va_list ap;
    int ret = -1;
    if (dmax > RSIZE_MAX_STR) {
        invoke_safe_str_constraint_handler("sprintf_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return RCNEGATE(ESLEMAX);
    }

    if (dest == NULL) {
        invoke_safe_str_constraint_handler("sprintf_s: dest is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (fmt == NULL) {
        invoke_safe_str_constraint_handler("sprintf_s: fmt is null",
                   NULL, ESNULLP);
        return RCNEGATE(ESNULLP);
    }

    if (dmax == 0) {
        invoke_safe_str_constraint_handler("sprintf_s: dmax is 0",
                   NULL, ESZEROL);
        return RCNEGATE(ESZEROL);
    }

    va_start(ap, fmt);

    ret = vsnprintf(dest, (size_t)dmax, fmt, ap);

    if (ret >= (int)dmax) {
        invoke_safe_str_constraint_handler("sprintf_s: len exceeds dmax",
                   NULL, ESNOSPC);
        *dest = 0;
        ret = RCNEGATE(ESNOSPC);
    }

    va_end(ap);

    return ret;
}
