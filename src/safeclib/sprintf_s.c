/* Need restrict */
#include "config.h"
#include "safe_str_lib.h"
#include "safe_str_constraint.h"
#include <stdarg.h>

/* TODO: error when fmt contains %n, or encoding errors occur.
 */

/** 
 * @brief
 *    The sprintf_s function composes a string with same test that 
 *    would be printed if format was used on printf. Instead of being 
 *    printed, the content is stored in dest.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.6 The sprintf_s function (p: 595-596)
 *    http://en.cppreference.com/w/c/string/byte/sprintf
 *
 * @param[out] dest  storage location for output buffer.
 * @param[in]  dmax  maximum number of characters to store in buffer.
 * @param[in]  fmt   format-control string.
 * @param[in]  ...   optional arguments
 *
 * @pre Neither dest nor fmt shall be a null pointer.
 * @pre dmax shall not be greater than RSIZE_MAX_STR.
 * @pre dmax shall not equal zero.
 * @pre dmax shall be greater than strnlen_s(dest, dmax).
 * @pre fmt  shall not contain the conversion specifier %n.
 * @pre None of the arguments corresponding to %s is a null pointer.
 * @pre No encoding error shall occur.
 *
 * @return  On success the total number of characters written is returned. 
 * @return  On failure a negative number is returned.
 * @return  If the buffer dest is too small for the formatted text,
 *          including the terminating null, then the buffer is set to an
 *          empty string by placing a null character at dest[0], and the
 *          invalid parameter handler is invoked. Unlike _snprintf,
 *          sprintf_s guarantees that the buffer will be null-terminated
 *          unless the buffer size is zero.
 * @retval  ESNULLP when dest/fmt is NULL pointer
 * @retval  ESZEROL when dmax = 0
 * @retval  ESLEMAX when dmax > RSIZE_MAX_STR
 * @retval  ESNOSPC when return value exceeds dmax
 *
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
