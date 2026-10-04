/*------------------------------------------------------------------
 * u8tok_s.c
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
 * @def u8tok_s(dest,dmaxp,delim,ptr)
 * @brief
 *    A sequence of calls to the u8tok_s function breaks the utf-8 string
 *    pointed to by dest into a sequence of tokens, each of which is
 *    delimited by a byte from the utf-8 string pointed to by delim. The
 *    fourth argument points to a caller-provided char8_t pointer into
 *    which u8tok_s stores information necessary for it to continue
 *    scanning the same string. Delimiters are matched byte-wise, like
 *    strtok_s(); multi-byte delimiter characters are not supported.
 * @details
 *    The first call in a sequence has a non-null first argument and
 *    dmaxp points to an object whose value is the number of bytes in
 *    the array pointed to by the first argument. The first call stores
 *    an initial value in the object pointed to by ptr and updates the
 *    value pointed to by dmaxp to reflect the number of bytes that
 *    remain in relation to ptr. Subsequent calls in the sequence have a
 *    null first argument and the objects pointed to by dmaxp and ptr
 *    are required to have the values stored by the previous call in the
 *    sequence, which are then updated. The separator string pointed to
 *    by delim may be different from call to call.
 *
 *    delim uses a STRTOK_DELIM_MAX_LEN of 16.
 *
 * @param[in]   dest    pointer to utf-8 string to tokenize
 * @param[out]  dmaxp   pointer to restricted maximum byte-length of dest
 * @param[in]   delim   pointer to utf-8 delimiter string (len < 255)
 * @param[out]  ptr     returned pointer to token
 *
 * @pre  delim shall not be a null pointer.
 * @pre  ptr shall not be a null pointer.
 * @pre  dmaxp shall not be a null pointer.
 * @pre  *dmaxp shall not be 0.
 * @pre  If dest is a null pointer, then *ptr shall not be a null pointer.
 * @pre  dest must not be unterminated.
 * @pre  *dmaxp shall not be greater than RSIZE_MAX_STR and size of dest.
 * @pre  delim must not be longer than STRTOK_DELIM_MAX_LEN (default: 16).
 *
 * @return  The u8tok_s function returns a pointer to the first byte
 *          of a token; or a null pointer if there is no token or there
 *          is a runtime-constraint violation.
 *          Each call modifies dest by substituting a NUL byte for the
 *          first delimiter that occurs after the returned token.
 *
 * errno is set to:
 *          ESNULLP     when dest/delim/ptr is NULL pointer
 *          ESZEROL     when *dmaxp = 0
 *          ESLEMAX     when *dmaxp > RSIZE_MAX_STR
 *          EOVERFLOW   when *dmaxp > size of dest
 *          ESLEWRNG    when *dmaxp != size of dest and --enable-error-dmax
 *          ESUNTERM    when unterminated string
 *
 * @see
 *    strtok_s()
 */
#ifdef FOR_DOXYGEN
char8_t *u8tok_s(char8_t *restrict dest, rsize_t *restrict dmaxp,
                 const char8_t *restrict delim, char8_t **restrict ptr)
#else
EXPORT char8_t *_u8tok_s_chk(char8_t *restrict dest, rsize_t *restrict dmaxp,
                             const char8_t *restrict delim,
                             char8_t **restrict ptr, const size_t destbos)
#endif
{
    const char8_t *pt;
    char8_t *ptoken;
    rsize_t dlen;
    rsize_t slen;
    rsize_t dmax;
    const char8_t *orig_dest = dest;

    if (unlikely(dmaxp == NULL)) {
        invoke_safe_str_constraint_handler("u8tok_s: dmax is NULL", NULL,
                                           ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }
    if (unlikely(*dmaxp == 0)) {
        invoke_safe_str_constraint_handler("u8tok_s: *dmaxp is 0", (void *)dest,
                                           ESZEROL);
        errno = ESZEROL;
        return (NULL);
    }
    dmax = *dmaxp;
    if (unlikely(delim == NULL)) {
        invoke_safe_str_constraint_handler("u8tok_s: delim is null",
                                           (void *)dest, ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }
    if (unlikely(ptr == NULL)) {
        invoke_safe_str_constraint_handler("u8tok_s: ptr is null", (void *)dest,
                                           ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }

    /* if the source was NULL, use the tokenizer context */
    if (dest == NULL) {
        dest = *ptr;
        if (unlikely(dest == NULL)) {
            invoke_safe_str_constraint_handler("u8tok_s: dest/*ptr is null",
                                               NULL, ESNULLP);
            errno = ESNULLP;
            return (NULL);
        }
    }

    /* on dest == NULL, destbos is known and 0. skip that */
    if (destbos == BOS_UNKNOWN || !orig_dest) {
        if (unlikely(dmax > RSIZE_MAX_STR)) {
            invoke_safe_str_constraint_handler("u8tok_s: *dmaxp exceeds max",
                                               (void *)dest, ESLEMAX);
            errno = ESLEMAX;
            return (NULL);
        }
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        if (unlikely(dmax > destbos)) {
            invoke_safe_str_constraint_handler("u8tok_s: *dmaxp exceeds dest",
                                               (void *)dest, EOVERFLOW);
            errno = EOVERFLOW;
            return (NULL);
        }
#ifdef HAVE_WARN_DMAX
        if (unlikely(dmax != destbos)) {
            handle_str_bos_chk_warn("u8tok_s", (char *)dest, dmax, destbos);
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
            invoke_safe_str_constraint_handler("u8tok_s: dest is unterminated",
                                               (void *)dest, ESUNTERM);
            errno = ESUNTERM;
            return (NULL);
        }

        /*
         * must scan the entire delimiter list
         */
        slen = STRTOK_DELIM_MAX_LEN;
        pt = delim;
        while (*pt != '\0') {

            if (unlikely(slen == 0)) {
                *ptr = NULL;
                *dmaxp = 0;
                *dest = '\0';
                invoke_safe_str_constraint_handler(
                    "u8tok_s: delim is unterminated", (void *)dest, ESUNTERM);
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
            invoke_safe_str_constraint_handler("u8tok_s: dest is unterminated",
                                               (void *)dest, ESUNTERM);
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
                    "u8tok_s: delim is unterminated", (void *)dest, ESUNTERM);
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
