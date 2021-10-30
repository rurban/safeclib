/*------------------------------------------------------------------
 * asctime_s.c
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
#if (defined(TEST_MSVCRT) && defined(HAVE_ASCTIME_S)) ||                       \
    (defined(MINGW_HAS_SECURE_API) && __MINGW64_VERSION_MAJOR < 7)
#else

/*#if defined(HAVE_ASCTIME_R)
char *asctime_r(const struct tm * __restrict, char * __restrict);
#endif
*/

/**
 * @def asctime_s(dest,dmax,tm)
 * @brief
 *    The \c asctime_s function converts the given calendar time \c tm to a
 *    textual representation of the following fixed 25-character form:
 *    \c "Www Mmm dd hh:mm:ss yyyy\n", as with asctime. The message is copied
 * into user-provided \c dest buffer, which is guaranteed to be
 * null-terminated.
 *
 * @details
 *    No more than dmax-1 bytes are written, the buffer is always
 *    null-terminated.
 *    Uses the re-entrant \c asctime_r() if available.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.8.2.1 The asctime_s function (p: 624-625)
 *    http://en.cppreference.com/w/c/chrono/asctime
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest    pointer to a user-provided buffer.
 * @param[in]   dmax    restricted maximum length of dest
 * @param[in]   tm      pointer to a tm object specifying the time to print
 *
 * @pre dest and tm shall not be a null pointer.
 * @pre dmax shall not be less than 26 and greater than RSIZE_MAX_STR.
 * @pre not all members of *tm are within their normal ranges,
 *      the year indicated by tm->tm_year is between 0-8099.
 *
 * @return  Zero if the entire message was successfully stored in dest,
 *          non-zero otherwise.
 * @retval  EOK        on success
 * @retval  ESNULLP    when dest or tm is a NULL pointer
 * @retval  ESLEMIN    when dmax < 26 or a tm member is too small
 * @retval  ESLEMAX    when dmax > RSIZE_MAX_STR or a tm member is too large
 * @retval  EOVERFLOW  when dmax > size of dest (optionally, when the compiler
 *                     knows the object_size statically)
 * @retval  ESLEWRNG   when dmax != size of dest and --enable-error-dmax
 * @retval  ESNOSPC    when dmax is too small for the result buffer
 * @retval  -1         when asctime_r or asctime returned NULL
 *
 * @note
 *    This function returns a pointer to static data and is not
 *    thread-safe. POSIX marks this function obsolete and recommends strftime
 *    instead. The C standard also recommends strftime instead of asctime and
 *    asctime_s because strftime is more flexible and locale-sensitive.  POSIX
 *    limits undefined behaviors only to when the output string would be
 *    longer than 25 characters, when tm->tm_wday or tm->tm_mon are
 *    not within the expected ranges, or when tm->tm_year exceeds
 *    INT_MAX-1990.  Some implementations handle tm->tm_mday==0 as
 *    meaning the last day of the preceding month.
 *
 * @see
 *    ctime_s()
 */

#ifdef FOR_DOXYGEN
errno_t asctime_s(char *dest, rsize_t dmax, const struct tm *tm)
#else
EXPORT errno_t _asctime_s_chk(char *dest, rsize_t dmax, const struct tm *tm,
                              const size_t destbos)
#endif
{
    const char *buf;
    size_t len;

    CHK_DEST_NULL("asctime_s")
    if (unlikely(dmax < 26)) {
        invoke_safe_str_constraint_handler("asctime_s: dmax is too small", NULL,
                                           ESLEMIN);
        return ESLEMIN;
    }
    if (destbos == BOS_UNKNOWN) {
        CHK_DMAX_MAX("asctime_s", RSIZE_MAX_STR)
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        CHK_DEST_OVR("asctime_s", destbos)
        if (unlikely(destbos < 26)) {
            invoke_safe_str_constraint_handler("ctime_s: dmax is too small",
                                               dest, ESLEMIN);
            return ESLEMIN;
        }
    }

    if (unlikely(tm == NULL)) {
        invoke_safe_str_constraint_handler("asctime_s: tm is null", NULL,
                                           ESNULLP);
        return ESNULLP;
    }

    if (tm->tm_year < 0 || tm->tm_mon < 0 || tm->tm_yday < 0 ||
        tm->tm_mday < 1 || tm->tm_wday < 0 || tm->tm_hour < 0 ||
        tm->tm_min < 0 || tm->tm_sec < 0 || tm->tm_isdst < 0
#ifdef HAVE_TM_GMTOFF
        || tm->tm_gmtoff < -1036800 /* 12*86400 */
#endif
    ) {
        invoke_safe_str_constraint_handler(
            "asctime_s: a tm member is too small", NULL, ESLEMIN);
        return ESLEMIN;
    }

    if (tm->tm_year > 8099 || tm->tm_mon > 11 || tm->tm_yday > 365 ||
        tm->tm_mday > 31 || tm->tm_wday > 6 || tm->tm_hour > 23 ||
        tm->tm_min > 59 || tm->tm_sec > 60 || tm->tm_isdst > 1
#ifdef HAVE_TM_GMTOFF
        || tm->tm_gmtoff > 1036800 /* 12*86400 */
#endif
    ) {
        /* does EOVERFLOW in asctime() */
        invoke_safe_str_constraint_handler(
            "asctime_s: a tm member is too large", NULL, ESLEMAX);
        return ESLEMAX;
    }

#if defined(HAVE_ASCTIME_R)
    if (dmax >= 120) { /* glibc reserves 114 */
        buf = asctime_r(tm, dest);
        if (!buf) {
#ifdef SAFECLIB_STR_NULL_SLACK
            memset(dest, 0, dmax);
#else
            *dest = '\0';
#endif
            return -1;
        }
    } else {
        static char tmp[120];
        buf = asctime_r(tm, (char *)&tmp);
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
    buf = asctime(tm);
    if (0)
        goto esnospc;
#endif
    if (!buf)
        return -1;
    len = strlen(buf);

    if (likely(len < dmax)) {
        strcpy_s(dest, dmax, buf);
    } else {
    esnospc:
        invoke_safe_str_constraint_handler("asctime_s: dmax is too small", dest,
                                           ESNOSPC);
        return ESNOSPC;
    }

    return EOK;
}

#endif /* MINGW_HAS_SECURE_API */
