/*------------------------------------------------------------------
 * gets_s.c
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
#endif

/* i386-mingw32-gcc */
#ifndef HAVE_STRNLEN
#define strnlen(s, smax) strlen(s)
#endif

/**
 * @def gets_s(dest,dmax)
 * @brief
 *    The \c gets_s function reads characters from stdin until a newline is
 *    found or end-of-file occurs. Writes only at most dmax characters into
 *    the array pointed to by str, and always writes the terminating null
 *    character.

 * @details
 *    Note that C11 allows only writing dmax-1 character. We need to work
 *    with the system fgets() passing it dmax+1.
 *    In any case, gets_s first finishes reading and discarding the characters
 *    from stdin until new-line character, end-of-file condition, or read
 *    error before calling the constraint handler.
 *    With SAFECLIB_STR_NULL_SLACK the rest of dmax is cleared with NULL
 *    bytes, without all elements following the terminating null character (if
 *    any) written by gets_s in the array of dmax characters pointed to by
 *    dest take unspeciﬁed values when gets_s returns.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.4.1 The gets_s function (p: 602-603)
 *    http://en.cppreference.com/w/c/io/gets
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out]  dest      character string to be written.
 *                        If the resulting concatenated string is less
 *                        than dmax, the remaining slack space is nulled.
 * @param[in]   dmax      restricted maximum length of the resulting dest,
 *                        including the null. it may temp. write dmax+1, but
 *                        always return max dmax.
 *
 * @pre  dest shall not be a null pointer
 * @pre  dmax shall not equal zero
 * @pre  dmax shall not be greater than RSIZE_MAX_STR and size of dest
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  If there is a runtime-constraint violation, then if dest is
 *          not a null pointer and dmax is greater than zero and not
 *          greater than RSIZE_MAX_STR, then gets_s nulls dest.
 * @retval  int >0     when on successful operation, all the characters from src
 *                     were appended to dest and the result in dest is null
 *                     terminated.
 * @retval  errno=ESNULLP    when dest is a NULL pointer
 * @retval  errno=ESZEROL    when dmax = 0
 * @retval  errno=ESLEMAX    when dmax > RSIZE_MAX_STR
 * @retval  errno=EOVERFLOW  when dmax > size of dest
 * @retval  errno=ESUNTERM   endline or eof not encountered after storing
 *                           dmax-1 characters to dest.
 *
 * @see
 *    scanf_s()
 */

#ifdef FOR_DOXYGEN
char *gets_s(char *restrict dest, rsize_t dmax)
#else
EXPORT char *_gets_s_chk(char *restrict dest, rsize_t dmax,
                         const size_t destbos)
#endif
{
    char *ret;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("gets_s: dest is null", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return NULL;
    }
    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("gets_s: dmax is 0", NULL, ESZEROL);
        errno = ESZEROL;
        return NULL;
    }
    if (destbos == BOS_UNKNOWN) {
        if (unlikely(dmax > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("gets_s: dmax exceeds max", dest,
                                               ESLEMAX);
            errno = ESLEMAX;
            return NULL;
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        if (unlikely(dmax > destbos)) {
            if (unlikely(dmax > RSIZE_MAX_STR)) {
                invoke_safe_str_constraint_handler("gets_s: dmax exceeds max",
                                                   dest, ESLEMAX);
                errno = ESLEMAX;
                return NULL;
            } else {
                invoke_safe_str_constraint_handler("gets_s: dmax exceeds dest",
                                                   dest, EOVERFLOW);
                errno = EOVERFLOW;
                return NULL;
            }
        }
    }

    errno = 0;
    ret = fgets(dest, dmax + 1, stdin);

    if (likely(ret)) {
        rsize_t len = (rsize_t)strnlen(dest, dmax);
        if (len > 0 && dest[len - 1] == '\n') {
            dest[len - 1] = 0;
        } else if (len > (rsize_t)(dmax - 1)) {
            ret = NULL;
            goto nospc;
        } else if (feof(stdin)) /* dead code: feof returns NULL */
            ;
        else if (len == (rsize_t)(dmax - 1) && dest[len] == '\0') {
            ret = NULL;
            goto nospc;
        }
    } else {
        if (!feof(stdin) && errno == 0) { /* closed? */
        nospc:
            handle_error(dest, dmax, "gets_s: length exceeds dmax", ESNOSPC);
            errno = ESNOSPC;
#ifdef SAFECLIB_STR_NULL_SLACK
            memset(dest, 0, dmax);
#endif
        }
    }

    return ret;
}
