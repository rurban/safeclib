/*------------------------------------------------------------------
 * strtok_s.c
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
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
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

#if defined(_WIN32) && defined(HAVE_STRTOK_S)
#else

/**
 * @def strtok_s(dest,dmaxp,delim,ptr)
 * @brief
 *    A sequence of calls to the strtok_s function breaks the string
 *    pointed to by dest into a sequence of tokens, each of which is
 *    delimited by a character from the string pointed to by delim. The
 *    fourth argument points to a caller-provided char pointer into
 *    which the strtok_s function stores information necessary for
 *    it to continue scanning the same string.
 * @details
 *    The first call in a sequence has a non-null first argument and
 *    dmax points to an object whose value is the number of elements
 *    in the character array pointed to by the first argument. The
 *    first call stores an initial value in the object pointed to by
 *    ptr and updates the value pointed to by dmax to reﬂect the
 *    number of elements that remain in relation to ptr. Subsequent
 *    calls in the sequence have a null first argument and the objects
 *    pointed to by dmax and ptr are required to have the values
 *    stored by the previous call in the sequence, which are then
 *    updated. The separator string pointed to by delim may be different
 *    from call to call.
 *
 *    The first call in the sequence searches the string pointed to
 *    by dest for the first character that is not contained in the
 *    current separator string pointed to by delim. If no such character
 *    is found, then there are no tokens in the string pointed to
 *    by dest and the strtok_s function returns a null pointer. If
 *    such a character is found, it is the start of the first token.
 *
 *    The strtok_s function then searches from there for the first
 *    character in dest that is contained in the current separator
 *    string. If no such character is found, the current token
 *    extends to the end of the string pointed to by dest, and
 *    subsequent searches in the same string for a token return
 *    a null pointer. If such a character is found, it is
 *    overwritten by a null character, which terminates the
 *    current token.
 *
 *    In all cases, the strtok_s function stores sufﬁcient information
 *    in the pointer pointed to by ptr so that subsequent calls,
 *    with a null pointer for dest and the unmodiﬁed pointer value
 *    for ptr, shall start searching just past the element overwritten
 *    by a null character (if any).
 *
 *    delim uses a STRTOK_DELIM_MAX_LEN of 16.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.3.1 The strtok_s function (p: 620-621)
 *    http://en.cppreference.com/w/c/string/byte/strtok
 *    * ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest    pointer to string to tokenize
 * @param[out]  dmaxp   pointer to restricted maximum length of dest string
 * @param[in]   delim   pointer to delimiter string (len < 255)
 * @param[out]  ptr     returned pointer to token
 *
 * @pre  delim shall not be a null pointer.
 * @pre  ptr shall not be a null pointer.
 * @pre  dmax shall not be a null pointer.
 * @pre  *dmax shall not be 0.
 * @pre  If dest is a null pointer, then *ptr shall not be a null pointer.
 * @pre  dest must not be unterminated.
 * @pre  The value of *dmax shall not be greater than RSIZE_MAX_STR and size of
 *       dest. The end of the token found shall occur within the first *dmax
 *       characters of dest for the first call, and shall occur within the
 *       first *dmax characters of where searching resumes on subsequent
 *       calls.
 * @pre  delim must not be longer than STRTOK_DELIM_MAX_LEN (default: 16).
 *
 * @note The mingw MINGW_HAS_SECURE_API declares it without the dmax
 *       argument and without restrict. Skip it there.
 *       <tt>char* strtok_s (char *str, const char *delim, char **ctx)</tt>
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  The strtok_s function returns a pointer to the first character
 *          of a token; or a null pointer if there is no token or there
 *          is a runtime-constraint violation.
 *          Each call modifies dest by substituting a NULL character for the
 *          first delimiter that occurs after the returned token.
 *          If there is a runtime-constraint violation, the strtok_s function
 *          does not indirect through the dest/delim pointers, and does not
 *          store a value in the object pointed to by ptr.
 *
 * errno is set to:
 *          ESNULLP     when dest/delim/ptr is NULL pointer
 *          ESZEROL     when *dmax = 0
 *          ESLEMAX     when *dmax > RSIZE_MAX_STR
 *          EOVERFLOW   when *dmax > size of dest
 *          ESLEWRNG    when *dmax != size of dest and --enable-error-dmax
 *          ESUNTERM    when unterminated string
 * C11 just returns EINVAL
 *
 * @remarks
 * Example to demonstrate usage of strtok_s() to tokenize a string
 * @code{.c}
 *   // Approach1: sequential strtok_s() calls
 *   str1 = ",.:*one,two;three,;four*.*.five-six***"; // String to tokenize
 *   len = 38;
 *   str2 = ",.;*"; // String of delimiters
 *
 *   p2tok = strtok_s(str1, &len, str2, &p2str);
 *   // token: one, remaining: two;three,;four*.*.five-six***, len: 30
 *
 *   p2tok = strtok_s(NULL, &len, str2, &p2str);
 *   // token: two, remaining: three,;four*.*.five-six***, len: 26
 *
 *   p2tok = strtok_s(NULL, &len, str2, &p2str);
 *   // token: three,  remaining: ;four*.*.five-six***, len: 20
 *
 *   p2tok = strtok_s(NULL, &len, str2, &p2str);
 *   // token: four, remaining .*.five-six***, len: 14
 *
 *   p2tok = strtok_s(NULL, &len, str2, &p2str);
 *   // token: five-six, remaining: **, len: 2
 *
 *    p2tok = strtok_s(NULL, &len, str2, &p2str);
 *    // token: (null), remaining: **, len: 0
 *
 *    // Approach2: Use of while loop with same entry data as used above
 *    p2tok = str1;
 *    while (p2tok && len)
 *    {
 *      p2tok = strtok_s(NULL, &len, str2, &p2str);
 *      printf("  token --   remaining --  len=0 \n", p2tok, p2str, (int)len );
 *    }
 * @endcode
 */
#ifdef FOR_DOXYGEN
char *strtok_s(char *restrict dest, rsize_t *restrict dmaxp,
               const char *restrict delim, char **restrict ptr)
#else
EXPORT char *_strtok_s_chk(char *restrict dest, rsize_t *restrict dmaxp,
                           const char *restrict delim, char **restrict ptr,
                           const size_t destbos)
#endif
{
    const char *pt;
    char *ptoken;
    rsize_t dlen;
    rsize_t slen;
    rsize_t dmax;
    const char *orig_dest = dest;

    if (unlikely(dmaxp == NULL)) {
        invoke_safe_str_constraint_handler("strtok_s: dmax is NULL", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }
    if (unlikely(*dmaxp == 0)) {
        invoke_safe_str_constraint_handler("strtok_s: *dmaxp is 0", dest,
                                           ESZEROL);
        errno = ESZEROL;
        return (NULL);
    }
    dmax = *dmaxp;
    if (unlikely(delim == NULL)) {
        invoke_safe_str_constraint_handler("strtok_s: delim is null", dest,
                                           ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }
    if (unlikely(ptr == NULL)) {
        invoke_safe_str_constraint_handler("strtok_s: ptr is null", dest,
                                           ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }

    /* if the source was NULL, use the tokenizer context */
    if (dest == NULL) {
        dest = *ptr;
        if (unlikely(dest == NULL)) {
            invoke_safe_str_constraint_handler("strtok_s: dest/*ptr is null",
                                               NULL, ESNULLP);
            errno = ESNULLP;
            return (NULL);
        }
    }

    /* on dest == NULL, destbos is known and 0. skip that */
    if (destbos == BOS_UNKNOWN || !orig_dest) {
        if (unlikely(dmax > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("strtok_s: *dmaxp exceeds max",
                                               dest, ESLEMAX);
            errno = ESLEMAX;
            return (NULL);
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        if (unlikely(dmax > destbos)) {
            invoke_safe_str_constraint_handler("strtok_s: *dmaxp exceeds dest",
                                               dest, EOVERFLOW);
            errno = EOVERFLOW;
            return (NULL);
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(dmax != destbos)) {
            handle_str_bos_chk_warn("strtok_s", dest, dmax, destbos);
#ifdef HAVE_ERROR_DMAX
            errno = ESLEWRNG;
            return (NULL);
#endif
        }
#endif
    }

    /*
     * scan dest for a delimiter
     */
    dlen = *dmaxp;
    ptoken = NULL;
    errno = 0;
    while (*dest != '\0' && !ptoken) {

        if (unlikely(dlen == 0)) {
            *ptr = NULL;
            invoke_safe_str_constraint_handler("strtok_s: dest is unterminated",
                                               dest, ESUNTERM);
            errno = ESUNTERM;
            return (NULL);
        }

        /*
         * must scan the entire delimiter list
         * ISO should have included a delimiter string limit!!
         */
        slen = STRTOK_DELIM_MAX_LEN;
        pt = delim;
        while (*pt != '\0') {

            if (unlikely(slen == 0)) {
                *ptr = NULL;
                *dmaxp = 0;
                *dest = '\0';
                invoke_safe_str_constraint_handler(
                    "strtok_s: delim is unterminated", dest, ESUNTERM);
                errno = ESUNTERM;
                return (NULL);
            }
            slen--;

            if (*dest == *pt) {
                ptoken = NULL;
                break;
            } else {
                pt++;
                ptoken = dest;
            }
        }
        dest++;
        dlen--;
    }

    /*
     * if the beginning of a token was not found, then no
     * need to continue the scan.
     */
    if (ptoken == NULL) {
        *dmaxp = dlen;
        return (ptoken);
    }

    /*
     * Now we need to locate the end of the token
     */
    while (*dest != '\0') {

        if (unlikely(dlen == 0)) {
            *ptr = NULL;
            *dmaxp = 0;
            *dest = '\0';
            invoke_safe_str_constraint_handler("strtok_s: dest is unterminated",
                                               dest, ESUNTERM);
            errno = ESUNTERM;
            return (NULL);
        }

        slen = STRTOK_DELIM_MAX_LEN;
        pt = delim;
        while (*pt != '\0') {

            if (unlikely(slen == 0)) {
                *ptr = NULL;
                *dmaxp = 0;
                *dest = '\0';
                invoke_safe_str_constraint_handler(
                    "strtok_s: delim is unterminated", dest, ESUNTERM);
                errno = ESUNTERM;
                return (NULL);
            }
            slen--;

            if (*dest == *pt) {
                /*
                 * found a delimiter, set to null
                 * and return context ptr to next char
                 */
                *dest = '\0';
                *ptr = (dest + 1); /* return pointer for next scan */
                *dmaxp = dlen - 1; /* account for the nulled delimiter */
                return (ptoken);
            } else {
                /*
                 * simply scanning through the delimiter string
                 */
                pt++;
            }
        }
        dest++;
        dlen--;
    }

    *dmaxp = dlen;
    return (ptoken);
}

#endif
