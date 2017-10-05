/*------------------------------------------------------------------
 * ctime_s.c
 *
 * September 2017, Reini Urban
 *
 * Copyright (c) 2017 by Reini Urban
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
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#include <time.h>
#endif

/*
#if defined(HAVE_CTIME_R)
char *ctime_r(const time_t *, char *);
#endif
*/

/** 
 * @brief
 *    The \c ctime_s function converts the given time since epoch to a
 *    calendar local time and then to a textual representation, as if by
 *    calling <tt>asctime_s(buffer, bufsz, localtime_s(time, &(struct tm){0}))</tt>.
 *    The message is copied into the user-provided \c dest buffer, which is
 *    guaranteed to be null-terminated.
 *
 * @details
 *    No more than dmax-1 bytes are written, the buffer is always
 *    null-terminated. The function does not support localization.
 *    Uses the re-entrant \c ctime_r() if available.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.8.2.2 The ctime_s function (p: 624-625)
 *    http://en.cppreference.com/w/c/chrono/ctime
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest    pointer to a user-provided buffer.
 * @param[in]   dmax    restricted maximum length of dest
 * @param[in]   timer   pointer to a epoch (long, seconds since 1970)
 *
 * @pre dest and timer shall not be a null pointer.
 * @pre dmax shall not be less than 26 and greater than RSIZE_MAX_STR.
 *
 * @return  Zero if the entire message was successfully stored in dest,
 *          non-zero otherwise.
 * @retval  EOK        on success
 * @retval  ESNULLP    when dest or tm is a NULL pointer
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  ESLEMIN    when dmax < 26 or *timer < 0
 * @retval  ESLEMAX    when *timer > 313360441200L, the year 10000,
 *                     resp. LONG_MAX on 32bit systems
 * @retval  ESNOSPC    when dmax is too small for the result buffer
 * @retval  -1         when ctime_r or ctime returned NULL
 *
 * @note
 *    ctime returns a pointer to static data and is not thread-safe. In addition,
 *    it modifies the static tm object which may be shared with gmtime and
 *    localtime. POSIX marks this function obsolete and recommends strftime
 *    instead. The C standard also recommends strftime instead of ctime and
 *    ctime_s because strftime is more flexible and locale-sensitive.  The
 *    behavior of ctime may be undefined for the values of time_t that result
 *    in the string longer than 25 characters (e.g. year 10000).
 *
 * @see
 *    asctime_s()
 */

EXPORT errno_t
ctime_s(char *dest, rsize_t dmax, const time_t *timer)
{
    const char* buf;
    size_t len;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("ctime_s: dest is null",
                   NULL, ESNULLP);
        return ESNULLP;
    }

    if (unlikely(timer == NULL)) {
        invoke_safe_str_constraint_handler("ctime_s: timer is null",
                   NULL, ESNULLP);
        return ESNULLP;
    }

    if (unlikely(dmax < 26)) {
        invoke_safe_str_constraint_handler("ctime_s: dmax is too small",
                   NULL, ESLEMIN);
        return ESLEMIN;
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("ctime_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return ESLEMAX;
    }

    if (unlikely(*timer < 0)) {
        invoke_safe_str_constraint_handler("ctime_s: timer is <0",
                   NULL, ESLEMIN);
        return ESLEMIN;
    }

    /* 32bit have a lower limit: -Werror=type-limits (long) */
    if (unlikely(*timer >= MAX_TIME_T_STR)) { /* year 10000 */
        invoke_safe_str_constraint_handler("ctime_s: timer is too large",
                   NULL, ESLEMAX);
        return ESLEMAX;
    }

#if defined(HAVE_CTIME_R)
    if (dmax >= 120) { /* glibc reserves 114 */
        buf = ctime_r(timer, dest);
        if (!buf) {
# ifdef SAFECLIB_STR_NULL_SLACK
            memset(dest, 0, dmax);
# else
            *dest = '\0';
# endif
            return -1;
        }
    }
    else {
        static char tmp[120];
        buf = ctime_r(timer, (char *)&tmp);
        if (!buf)
            return -1;
        len = strlen(buf);
        if (likely(len < dmax)) {
            strcpy_s(dest, dmax, buf);
            return EOK;
        } else {
            goto esnospc;
        }
    }
#else
    buf = ctime(timer);
    if (0) goto esnospc;
#endif
    if (!buf) {
        return -1;
    }
    len = strlen(buf);

    if (likely(len < dmax)) {
        strcpy_s(dest, dmax, buf);
    } else {
      esnospc:
        invoke_safe_str_constraint_handler("ctime_s: dmax is too small",
                   NULL, ESNOSPC);
        return ESNOSPC;
    }

    return EOK;
}
EXPORT_SYMBOL(ctime_s)
