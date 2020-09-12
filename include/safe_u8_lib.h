/*------------------------------------------------------------------
 * safe_u8_lib.h -- Safe C Library UTF-8 String APIs
 *
 * September 2020, Reini Urban
 *
 * Copyright (c) 2020 by Reini Urban
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

#ifndef __SAFE_U8_LIB_H__
#define __SAFE_U8_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "safe_config.h"
#include "safe_lib_errno.h"
#include "safe_types.h"
#include "safe_compile.h"
#include "safe_str_lib.h"

/* char8i_t[] is a NFC normalized UTF8 string, disallowing mixed strings as with Unicode identifiers, and
   forbidding changes of Bidirectional Text.
   See http://unicode.org/reports/tr36/ and http://perl11.org/blog/foldcase.html
   They make up safer strings, and are easier to compare with, as you can skip the normalization step.
 */
typedef char8_t char8i_t;

/* utf-8 string concatenate */
EXTERN errno_t _u8cat_s_chk(char8_t *restrict dest, rsize_t dmax,
                            const char8_t *restrict src, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define u8cat_s(dest, dmax, src) _u8cat_s_chk(dest, dmax, src, BOS(dest))

/* utf-8 string copy */
EXTERN errno_t _u8cpy_s_chk(char8_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define u8cpy_s(dest, dmax, src) _u8cpy_s_chk(dest, dmax, src, BOS(dest))

/* utf-8 string identifier copy. dest normalized, normalizing src */
EXTERN errno_t _u8icpy_s_chk(char8i_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define u8icpy_s(dest, dmax, src) _u8icpy_s_chk(dest, dmax, src, BOS(dest))

/* utf-8 string identifier concatenate. dest normalized, normalizing src */
EXTERN errno_t _u8icat_s_chk(char8i_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src, const size_t destbos)
    BOS_CHK(dest) BOS_NULL(src);
#define u8icat_s(dest, dmax, src) _u8icat_s_chk(dest, dmax, src, BOS(dest))

/* fitted utf-8 string concatenate */
EXTERN errno_t _u8ncat_s_chk(char8_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src, rsize_t slen,
                             const size_t destbos, const size_t srcbos)
    BOS_ATTR((slen || dest || dmax) &&
                 (_BOS_NULL(dest) || _BOS_ZERO(dest, dmax)),
             "empty dest or dmax")
        BOS_ATTR((slen || dest || dmax) && _BOS_OVR(dest, dmax),
                 "dest overflow") BOS_OVR2_BUTZERO(src, slen);
#define u8ncat_s(dest, dmax, src, slen)                                       \
    _u8ncat_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* fitted utf-8 string copy */
EXTERN errno_t _u8ncpy_s_chk(char8_t *restrict dest, rsize_t dmax,
                              const char8_t *restrict src, rsize_t slen,
                              const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2_BUTZERO(src, slen);
#define u8ncpy_s(dest, dmax, src, slen)                                       \
    _u8ncpy_s_chk(dest, dmax, src, slen, BOS(dest), BOS(src))

/* utf-8 string length */
EXTERN rsize_t _u8len_s_chk(const char8_t *str, size_t strbos)
    BOS_CHK(str);
#define u8len_s(str) _u8len_s_chk(str, BOS(str))

/* utf-8 string bounded length */
EXTERN rsize_t _u8nlen_s_chk(const char8_t *str, rsize_t smax, size_t strbos)
    BOS_CHK2(str, smax);
#define u8nlen_s(str, smax) _u8nlen_s_chk(str, smax, BOS(str))

/* string tokenizer */
EXTERN char8_t *_u8tok_s_chk(char8_t *restrict dest, rsize_t *restrict dmaxp,
                           const char8_t *restrict delim, char8_t **restrict ptr,
                           const size_t destbos) BOS_OVR2_BUTNULL(dest, *dmaxp)
    BOS_ATTR(dest &&_BOS_NULL(dmaxp), "empty dmax") BOS_NULL(delim)
        BOS_NULL(ptr);
#define u8tok_s(dest, dmaxp, delim, ptr)                                      \
    _u8tok_s_chk(dest, dmaxp, delim, ptr, BOS(dest))

/* secure stdio */

/* safe u8sprintf_s */
EXTERN int _u8sprintf_s_chk(char8_t *restrict dest, const rsize_t dmax,
                           const size_t destbos, const char *restrict fmt, ...)
   __attribute_format__(printf,4,5) BOS_CHK(dest) BOS_FMT(fmt);
#define u8sprintf_s(dest, dmax, fmt, ...)                         \
  _u8sprintf_s_chk(dest, dmax, BOS(dest), fmt, __VA_ARGS__)

EXTERN int _vu8sprintf_s_chk(char8_t *restrict dest, rsize_t dmax,
                           const size_t destbos, const char *restrict fmt,
                           va_list ap) BOS_CHK(dest) BOS_FMT(fmt);
#define vu8sprintf_s(dest, dmax, fmt, ap)                                        \
    _vu8sprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)

/* truncating, no ESNOSPC */
EXTERN int _u8snprintf_s_chk(char8_t *restrict dest, rsize_t dmax,
                            const size_t destbos, const char *restrict fmt, ...)
  __attribute_format__(printf,4,5) BOS_CHK(dest) BOS_FMT(fmt);
#define u8snprintf_s(dest, dmax, ...)                                            \
    _u8snprintf_s_chk(dest, dmax, BOS(dest), __VA_ARGS__)

EXTERN int _vu8snprintf_s_chk(char8_t *restrict dest, rsize_t dmax,
                            const size_t destbos, const char *restrict fmt,
                            va_list ap) BOS_CHK(dest) BOS_FMT(fmt);
#define vu8snprintf_s(dest, dmax, fmt, ap)                                       \
    _vu8snprintf_s_chk(dest, dmax, BOS(dest), fmt, ap)

/* Note: there is no __vsscanf_chk yet. Unchecked */
EXTERN int u8sscanf_s(const char8_t *restrict buffer, const char *restrict fmt, ...)
    __attribute_format__(scanf,2,3) BOS_NULL(buffer) BOS_FMT(fmt);

EXTERN int fu8scanf_s(FILE *restrict stream, const char *restrict fmt, ...)
  __attribute_format__(scanf,2,3) BOS_NULL(stream) BOS_FMT(fmt);

EXTERN int u8scanf_s(const char *restrict fmt, ...)
  __attribute_format__(scanf,1,2) BOS_FMT(fmt);

EXTERN int vu8scanf_s(const char *restrict fmt, va_list ap) BOS_FMT(fmt);

EXTERN int vfu8scanf_s(FILE *restrict stream, const char *restrict fmt,
                       va_list ap) BOS_NULL(stream) BOS_FMT(fmt);

EXTERN int vu8sscanf_s(const char8_t *restrict dest, const char *restrict fmt,
                       va_list ap) BOS_NULL(dest) BOS_FMT(fmt);

EXTERN int u8printf_s(const char *restrict fmt, ...)
  __attribute_format__(printf,1,2) BOS_FMT(fmt);

EXTERN int fu8printf_s(FILE *restrict stream, const char *restrict fmt, ...)
  __attribute_format__(printf,2,3) BOS_FMT(fmt);

EXTERN int vu8printf_s(const char *restrict fmt, va_list ap) BOS_FMT(fmt);

EXTERN int vfu8printf_s(FILE *restrict stream, const char *restrict fmt,
                        va_list arg) BOS_FMT(fmt);

EXTERN errno_t _u8error_s_chk(char8_t *dest, rsize_t dmax, errno_t errnum,
                              const size_t destbos) BOS_CHK(dest);
#define u8error_s(dest, dmax, errnum)                                         \
    _u8error_s_chk(dest, dmax, errnum, BOS(dest))

EXTERN size_t u8errorlen_s(errno_t errnum);

#ifndef SAFECLIB_DISABLE_EXTENSIONS

/* utf-8 string compare, normalizing */
EXTERN errno_t _u8cmp_s_chk(const char8_t *dest, rsize_t dmax, const char8_t *src,
                            int *resultp, const size_t destbos,
                            const size_t srcbos) BOS_CHK(dest) BOS_NULL(src)
    BOS_NULL(resultp);
#define u8cmp_s(dest, dmax, src, resultp)                                     \
    _u8cmp_s_chk(dest, dmax, src, resultp, BOS(dest), BOS(src))

/* utf-8 string compare, normalized (both being valid identifier-like) */
EXTERN errno_t _u8icmp_s_chk(const char8i_t *dest, rsize_t dmax, const char8i_t *src,
                             int *resultp, const size_t destbos,
                             const size_t srcbos) BOS_CHK(dest) BOS_NULL(src)
    BOSW_CHK2(src, smax) BOS_NULL(resultp);
#define u8icmp_s(dest, dmax, src, resultp)                                     \
    _u8icmp_s_chk(dest, dmax, src, resultp, BOS(dest), BOS(src))

/* utf-8 string compare fold-cased, normalizing */
EXTERN errno_t _u8fccmp_s_chk(const char8_t *dest, rsize_t dmax,
                                 const char8_t *src, int *resultp,
                                 const size_t destbos) BOS_CHK(dest)
    BOS_NULL(src) BOS_NULL(resultp);
#define u8fccmp_s(dest, dmax, src, resultp)                                 \
    _u8fccmp_s_chk(dest, dmax, src, resultp, BOS(dest))

/* natural order string compare */
EXTERN errno_t _u8natcmp_s_chk(const char8_t *dest, rsize_t dmax, const char8_t *src,
                                const int fold_case, int *resultp,
                                const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_NULL(src) BOS_NULL(resultp);
#define u8natcmp_s(dest, dmax, src, resultp)                                  \
    _u8natcmp_s_chk(dest, dmax, src, 0, resultp, BOS(dest), BOS(src))
#define u8natfccmp_s(dest, dmax, src, resultp)                              \
    _u8natfccmp_s_chk(dest, dmax, src, 1, resultp, BOS(dest), BOS(src))

/* find a substring - fold-cased, normalizing */
EXTERN errno_t _u8fcu8_s_chk(char8_t *dest, rsize_t dmax, const char8_t *src,
                                 rsize_t slen, char8_t **substring,
                                 const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(substring);
#define u8fcu8_s(dest, dmax, src, slen, substring)                         \
    _u8fcu8_s_chk(dest, dmax, src, slen, substring, BOS(dest), BOS(src))

/* find a substring - fold-cased, both normalized */
EXTERN errno_t _u8ifcu8_s_chk(char8i_t *dest, rsize_t dmax, const char8i_t *src,
                              rsize_t slen, char8i_t **substring,
                              const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(substring);
#define u8ifcu8_s(dest, dmax, src, slen, substring)                         \
    _u8ifcu8_s_chk(dest, dmax, src, slen, substring, BOS(dest), BOS(src))

/* computes excluded prefix length */
EXTERN errno_t _u8cspn_s_chk(const char8_t *dest, rsize_t dmax, const char8_t *src,
                              rsize_t slen, rsize_t *countp,
                              const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2(src, slen) BOS_NULL(countp);
#define u8cspn_s(dest, dmax, src, slen, countp)                               \
    _u8cspn_s_chk(dest, dmax, src, slen, countp, BOS(dest), BOS(src))

/* get pointer to first occurrence from set of char8_t */
EXTERN errno_t _u8pbrk_s_chk(char8_t *dest, rsize_t dmax, char8_t *src, rsize_t slen,
                              char8_t **firstp, const size_t destbos,
                              const size_t srcbos) BOS_CHK(dest)
    BOS_OVR2(src, slen) BOS_NULL(firstp);
#define u8pbrk_s(dest, dmax, src, slen, firstp)                               \
    _u8pbrk_s_chk(dest, dmax, src, slen, firstp, BOS(dest), BOS(src))

/* computes inclusive prefix length */
EXTERN errno_t _u8spn_s_chk(const char8_t *dest, rsize_t dmax, const char8_t *src,
                             rsize_t slen, rsize_t *countp,
                             const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_CHK2(src, slen) BOS_NULL(countp);
#define u8spn_s(dest, dmax, src, slen, countp)                                \
    _u8spn_s_chk(dest, dmax, src, slen, countp, BOS(dest), BOS(src))

/* find a substring, normalizing */
EXTERN errno_t _u8u8_s_chk(char8_t *dest, rsize_t dmax, const char8_t *src,
                           rsize_t slen, char8_t **substringp,
                           const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2(src, slen) BOS_NULL(substringp);
#define u8u8_s(dest, dmax, src, slen, substringp)                            \
    _u8u8_s_chk(dest, dmax, src, slen, substringp, BOS(dest), BOS(src))

/* find a substring, all normalized */
EXTERN errno_t _u8iu8i_s_chk(char8i_t *dest, rsize_t dmax, const char8i_t *src,
                           rsize_t slen, char8i_t **substringp,
                           const size_t destbos, const size_t srcbos)
    BOS_CHK(dest) BOS_OVR2(src, slen) BOS_NULL(substringp);
#define u8iu8i_s(dest, dmax, src, slen, substringp)                            \
    _u8iu8i_s_chk(dest, dmax, src, slen, substringp, BOS(dest), BOS(src))

/* find first character, normalizing */
EXTERN errno_t _u8chr_s_chk(const char8_t *restrict dest, rsize_t dmax,
                            const int ch, char8_t **restrict resultp,
                            const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(ch, 255) BOS_NULL(resultp);
#define u8chr_s(dest, dmax, ch, resultp)                                      \
    _u8chr_s_chk(dest, dmax, ch, resultp, BOS(dest))

/* find last character, normalizing */
EXTERN errno_t _u8rchr_s_chk(const char8_t *restrict dest, rsize_t dmax,
                              const int ch, char8_t **restrict resultp,
                              const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(ch, 255) BOS_NULL(resultp) BOS_ATTR(!*dest, "empty *dest");
#define u8rchr_s(dest, dmax, ch, resultp)                                     \
    _u8rchr_s_chk(dest, dmax, ch, resultp, BOS(dest))

/* convert string to lowercase, normalizing */
EXTERN errno_t _u8lwr_s_chk(char8_t *dest, rsize_t dmax,
                            const size_t destbos) BOS_CHK2(dest, dmax);
#define u8lwr_s(dest, dmax)                                           \
    _u8lwr_s_chk(dest, dmax, BOS(dest))

/* convert string to uppercase, normalizing */
EXTERN errno_t _u8upr_s_chk(char8_t *dest, rsize_t dmax,
                            const size_t destbos) BOS_CHK2(dest, dmax);
#define u8upr_s(dest, dmax)                                           \
    _u8upr_s_chk(dest, dmax, BOS(dest))

/* zero an entire string with nulls.
   mingw string_s.h has: _strset_s */
EXTERN errno_t _u8zero_s_chk(char8_t *dest, rsize_t dmax, const size_t destbos)
    BOS_CHK(dest);
#define u8zero_s(dest, dmax) _u8zero_s_chk(dest, dmax, BOS(dest))

EXTERN errno_t _u8coll_s_chk(const char8_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src, int *resultp,
                             const size_t destbos) BOS_CHK(dest) BOS_NULL(src)
    BOS_NULL(resultp);
#define u8coll_s(dest, dmax, src, resultp)                                    \
    _u8coll_s_chk(dest, dmax, src, resultp, BOS(dest))

/* Derived from windows extensions sec_api/string_s.h
   defined(MINGW_HAS_SECURE_API) */

EXTERN errno_t _u8set_s_chk(char8_t *restrict dest, rsize_t dmax, int value,
                            const size_t destbos) BOS_CHK(dest)
    VAL_OVR2(value, 255);
#define u8set_s(dest, dmax, value) _u8set_s_chk(dest, dmax, value, BOS(dest))

EXTERN errno_t _u8nset_s_chk(char8_t *restrict dest, rsize_t dmax, int value,
                              rsize_t n, const size_t destbos) BOS_CHK(dest)
    BOS_OVR2_BUTZERO(dest, n) VAL_OVR2(value, 255) VAL_OVR2_BUTZERO(n, dmax);
#define u8nset_s(dest, dmax, value, n)                                        \
    _u8nset_s_chk(dest, dmax, value, n, BOS(dest))

#endif /* SAFECLIB_DISABLE_EXTENSIONS */

/* multibyte conversions, from - to wchar8_t */
#ifndef SAFECLIB_DISABLE_WCHAR

EXTERN errno_t _u8towcs_s_chk(size_t *restrict retvalp, wchar_t *restrict dest,
                              rsize_t dmax, const char8_t *restrict src,
                              rsize_t len, const size_t destbos)
    BOS_NULL(retvalp)
        BOS_ATTR(!_BOS_NULL(dest) && _BOS_ZERO(dest, dmax), "empty dmax")
            BOS_ATTR(!_BOS_NULL(dest) && _BOSW_OVR(dest, dmax), "dest overflow")
                BOS_ATTR(!_BOS_NULL(dest) && (void *)dest == (void *)src,
                         "dest overlap") BOS_CHK2(src, len);
#define u8towcs_s(retvalp, dest, dmax, src, len)                              \
    _u8towcs_s_chk(retvalp, dest, dmax, src, len, BOS(dest))

EXTERN errno_t _u8rtowcs_s_chk(size_t *restrict retvalp,
                               wchar_t *restrict dest, rsize_t dmax,
                               const char8_t **restrict srcp, rsize_t len,
                               mbstate_t *restrict ps, const size_t destbos)
    BOS_NULL(retvalp) BOS_NULL(srcp) BOS_NULL(ps)
        BOS_ATTR(!_BOS_NULL(dest) && _BOS_ZERO(dest, dmax), "empty dmax")
            BOS_ATTR(!_BOS_NULL(dest) && _BOSW_OVR(dest, dmax), "dest overflow")
                BOS_ATTR(!_BOS_NULL(dest) && (char8_t *)dest == *srcp,
                         "dest overlap") BOS_CHK2(*srcp, len)
                    BOS_ATTR(dmax &&len > dmax, "len overflow >dmax");
#define u8rtowcs_s(retvalp, dest, dmax, srcp, len, ps)                        \
    _u8rtowcs_s_chk(retvalp, dest, dmax, srcp, len, ps, BOS(dest))

EXTERN errno_t _wcstou8_s_chk(size_t *restrict retvalp, char8_t *restrict dest,
                               rsize_t dmax, const char *restrict src,
                               rsize_t len, const size_t destbos)
    BOS_NULL(retvalp) BOS_CHK(dest) BOSW_CHK2(src, len)
        BOS_ATTR(dmax &&len > dmax, "len overflow >dmax");
#define wcstou8_s(retvalp, dest, dmax, src, len)                              \
    _wcstou8_s_chk(retvalp, dest, dmax, src, len, BOS(dest))

EXTERN errno_t _wcsrtou8_s_chk(size_t *restrict retvalp, char8_t *restrict dest,
                                rsize_t dmax, const char **restrict srcp,
                                rsize_t len, mbstate_t *restrict ps,
                                const size_t destbos) BOS_NULL(retvalp)
    BOS_NULL(ps) BOS_ATTR(!_BOS_NULL(dest) && !dmax, "empty dmax")
        BOS_ATTR(!_BOS_NULL(dest) && _BOS_OVR(dest, dmax), "dest overflow")
            BOS_ATTR(!_BOS_NULL(dest) && (void *)dest == (void *)srcp,
                     "dest overlap") BOS_NULL(srcp) BOSW_CHK2(*srcp, len)
                BOS_ATTR(dmax &&len > dmax, "len overflow >dmax");
#define wcsrtou8_s(retvalp, dest, dmax, srcp, len, ps)                        \
    _wcsrtou8_s_chk(retvalp, dest, dmax, srcp, len, ps, BOS(dest))

EXTERN errno_t _wcrtou8_s_chk(size_t *restrict retvalp, char8_t *restrict dest,
                              rsize_t dmax, char wc, mbstate_t *restrict ps,
                              const size_t destbos) BOS_NULL(retvalp)
    BOS_CHK(dest) BOS_NULL(ps) VAL_OVR2(wc, 0x10ffff);
#define wcrtou8_s(retvalp, dest, dmax, wc, ps)                                 \
    _wcrtou8_s_chk(retvalp, dest, dmax, wc, ps, BOS(dest))

EXTERN errno_t _wctou8_s_chk(int *restrict retvalp, char8_t *restrict dest,
                             rsize_t dmax, char wc, const size_t destbos)
    BOS_NULL(retvalp)
        BOS_ATTR(!_BOS_NULL(dest) &&
                     (!dmax || dmax > RSIZE_MAX_STR || _BOS_OVR(dest, dmax)),
                 "dest overflow or empty") VAL_OVR2(wc, 0x10ffff);
#define wctou8_s(retvalp, dest, dmax, wc)                                      \
    _wctou8_s_chk(retvalp, dest, dmax, wc, BOS(dest))

#endif /* SAFECLIB_DISABLE_WCHAR */

/* search wide substring */
EXTERN errno_t _u8str_s_chk(char8_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src, rsize_t slen,
                             char8_t **restrict substringp,
                             const size_t destbos, const size_t srcbos)
    BOSW_CHK(dest) BOSW_OVR2(src, slen) BOS_NULL(substringp);
#define u8str_s(dest, dmax, src, slen, substringp)                            \
    _u8str_s_chk(dest, dmax, src, slen, substringp, BOS(dest), BOS(src))

/* TODO stricter security checks for identifiers: char8i_t
   mixed scripts, bidi, ...
*/
  
/* full foldcase + NFD normalization */
EXTERN errno_t _u8fc_s_chk(char8_t *restrict dest, rsize_t dmax,
                            const char8_t *restrict src, rsize_t *restrict lenp,
                            const size_t destbos) BOSW_CHK(dest) BOS_NULL(src);
#define u8fc_s(dest, dmax, src, lenp)                                         \
    _u8fc_s_chk(dest, dmax, src, lenp, BOS(dest))

/* Normalize to FCD/pre-NFKD */
EXTERN errno_t _u8norm_decompose_s_chk(char8_t *restrict dest, rsize_t dmax,
                                       const char8_t *restrict src,
                                       rsize_t *restrict lenp,
                                       const bool iscompat,
                                       const size_t destbos) BOSW_CHK(dest)
    BOS_NULL(src);
#define u8norm_decompose_s(dest, dmax, src, lenp, iscompat)                   \
    _u8norm_decompose_s_chk(dest, dmax, src, lenp, iscompat, BOS(dest))

/* Normalize to NCD/NFKD */
EXTERN errno_t _u8norm_reorder_s_chk(char8_t *restrict dest, rsize_t dmax,
                                     const char8_t *restrict src,
                                     const rsize_t len, const size_t destbos)
    BOSW_CHK(dest) BOSW_OVR2(src, len);
#define u8norm_reorder_s(dest, dmax, src, len)                                \
    _u8norm_reorder_s_chk(dest, dmax, src, len, BOS(dest))

/* Normalize to NFC/NFKC */
EXTERN errno_t _u8norm_compose_s_chk(char8_t *restrict dest, rsize_t dmax,
                                     const char8_t *restrict src,
                                     rsize_t *restrict lenp, bool iscontig,
                                     const size_t destbos) BOSW_CHK(dest)
    BOS_NULL(src) BOS_NULL(lenp);
#define u8norm_compose_s(dest, dmax, src, lenp, iscontig)                     \
    _u8norm_compose_s_chk(dest, dmax, src, lenp, iscontig, BOS(dest))

#ifdef SAFECLIB_DISABLE_WCHAR
enum wcsnorm_mode {
    WCSNORM_NFD = 0,
    WCSNORM_NFC = 1,  /* default */
    WCSNORM_FCD = 2,  /* not reordered */
    WCSNORM_FCC = 3,  /* contiguous composition only */
    WCSNORM_NFKD = 4, /* compat. OPTIONAL with --enable-norm-compat */
    WCSNORM_NFKC = 5  /* compat. OPTIONAL with --enable-norm-compat */
};
typedef enum wcsnorm_mode wcsnorm_mode_t;
#endif /* SAFECLIB_DISABLE_WCHAR */

/* Normalize to NFC (default), NFD nfc=0.
   experim. nfc>1: FCD, FCC */
EXTERN errno_t _u8norm_s_chk(char8_t *restrict dest, rsize_t dmax,
                             const char8_t *restrict src,
                             const wcsnorm_mode_t mode, rsize_t *restrict lenp,
                             const size_t destbos) BOSW_CHK(dest)
    BOS_NULL(src);
#define u8norm_s(dest, dmax, src, mode, lenp)                                 \
    _u8norm_s_chk(dest, dmax, src, mode, lenp, BOS(dest))

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_U8_LIB_H__ */
