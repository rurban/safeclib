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
 *    sprintf_s(char *buffer, rsize_t bufsize, const char *fmt, ...)
 *
 * DESCRIPTION
 *    The sprintf_s function composes a string with same test that 
 *    would be printed if format was used on printf. Instead of being 
 *    printed, the content is stored in buffer. On success, the total 
 *    number of characters written is returned. On failure, a negative 
 *    number is returned. 
 *
 *
 * INPUT PARAMETERS
 *    buffer      storage location for output buffer.
 *
 *    bufsize     maximum number of characters to store in buffer.
 *
 *    fmt         format-control string.
 *
 *    ...         optional arguments
 *
 * RETURN VALUE
 *    On success the total number of characters written is returned. 
 *
 *    On failure a negative number is returned. 
 */

errno_t sprintf_s(char *buffer, rsize_t bufsize, const char *fmt, ...)
{
    va_list ap;
    ssize_t ret = -1;
    if ((int)bufsize <= 0) 
    {
        return ret;
    }

    va_start(ap, fmt);

    ret = vsnprintf(buffer, bufsize, fmt, ap);
    if (ret >= (ssize_t)bufsize)
        ret = -1;

    va_end(ap);

    return ret;
}
