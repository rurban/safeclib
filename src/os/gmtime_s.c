/*------------------------------------------------------------------
 * gmtime_s.c
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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifdef FOR_DOXYGEN
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#include <time.h>
#endif

/* conflicting API */
#ifndef HAVE_MINGW64

/*#if defined(HAVE_GMTIME_R)
struct tm *gmtime_r(const time_t * __restrict, struct tm * __restrict);
#endif
*/

/**
 * @brief
 *    The \c gmtime_s function converts the given time since epoch to a
 *    calendar time, expressed in Coordinated Universal Time (UTC) in the
 *    struct tm format.  The result is copied into the user-provided tm struct.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.8.2.3 The gmtime_s function (p: 626-627)
 *    http://en.cppreference.com/w/c/chrono/gmtime
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   timer  pointer to a epoch (long, seconds since 1970)
 * @param[out]  dest   pointer to a user-provided struct tm.
 *
 * @pre timer and dest shall not be a null pointer.
 *
 * @return  copy of the dest pointer, or null pointer on error
 *          (which may be a runtime constraint violation or a failure
 *          to convert the specified time to UTC).
 *          May set errno to EOVERFLOW when *timer > 313360441200L, the year
 *          10000, resp. LONG_MAX on 32bit systems or < 0, or to ESNULLP
 *          when dest or timer is a NULL pointer.
 *
 * @note
 *    gmtime returns a pointer to static data and is not thread-safe. When
 *    gmtime_r is available it used instead.
 *
 *    POSIX requires that this function sets errno to EOVERFLOW if it fails
 *    because the argument is too large.  POSIX defines a thread-safe
 *    alternative gmtime_r, which is similar to the C11 function gmtime_s,
 *    except that it does not check the validity of its input parameters.
 *
 *    This function is available under windows with a different API, reversed
 *    argument order, and is not available with safeclib.
 *
 * @see
 *    localtime_s(), ctime_s()
 */

EXPORT struct tm *gmtime_s(const time_t *restrict timer, struct tm *restrict dest)
{

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("gmtime_s: dest is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return NULL;
    }

    if (unlikely(timer == NULL)) {
        invoke_safe_str_constraint_handler("gmtime_s: timer is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return NULL;
    }

    if (unlikely(*timer < 0)) {
        invoke_safe_str_constraint_handler("gmtime_s: timer is < 0", NULL,
                                           ESLEMIN);
        errno = EOVERFLOW;
        return NULL;
    }

    if (unlikely(*timer >= MAX_TIME_T_STR)) { /* year 10000 */
        invoke_safe_str_constraint_handler("gmtime_s: timer is too large", NULL,
                                           ESLEMAX);
        errno = EOVERFLOW;
        return NULL;
    }

    errno = 0;
#if defined(HAVE_GMTIME_R)
    dest = gmtime_r(timer, dest);
#else
    dest = gmtime(timer);
#endif

    return dest;
}

#endif /* HAVE_MINGW64 */
