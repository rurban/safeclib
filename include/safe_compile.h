/*------------------------------------------------------------------
 * safe_compile.h - diagnose_if compile-time checks
 *
 * Copyright (c) 2018 by Reini Urban
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

#ifndef __SAFE_COMPILE_H__
#define __SAFE_COMPILE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*constraint_handler_t)(const char *restrict /* msg */,
                                     void *restrict /* ptr */,
                                     errno_t /* error */);

/**
 * Since clang-5 most constraints can be caught at compile-time.
 *
 * For pointer sizes we need BOS/__builtin_object_size(),
 * for diagnostics __attribute__((diagnose_if()))
 * gcc violations might be caught with _Static_assert, but this is limited.
 */

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

/* clang-5+ BOS checks */
#define BOS_UNKNOWN ((size_t)-1)
#define _BOS_KNOWN(dest) ((size_t)BOS(dest) != BOS_UNKNOWN)
#ifdef HAVE___BUILTIN_OBJECT_SIZE
#if defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE > 1
#define BOS(dest) __builtin_object_size((dest), 1)
#else
#define BOS(dest) __builtin_object_size((dest), 0)
#endif
#define BOSW(dest) (BOS(dest) / sizeof(wchar_t))
#define _BOS_CHK(dest, dmax) (_BOS_KNOWN(dest) && (size_t)(dmax) != BOS(dest))
#define _BOS_OVR(dest, dmax) (_BOS_KNOWN(dest) && (size_t)(dmax) > BOS(dest))
#define _BOS_ZERO(dest, dmax)                                                  \
    ((_BOS_KNOWN(dest) && BOS(dest) == 0) || (dmax) == 0)
#define _BOS_NULL(dest) ((_BOS_KNOWN(dest) && BOS(dest) == 0) || !(dest))
#define _BOSW_CHK(dest, dmax) (_BOS_KNOWN(dest) && (size_t)(dmax) != BOSW(dest))
#define _BOSW_OVR(dest, dmax) (_BOS_KNOWN(dest) && (size_t)(dmax) > BOSW(dest))
#define _BOS_CHK_N(dest, dmax) ((size_t)(dmax) != BOS(dest))
#define _BOS_OVR_N(dest, dmax) ((size_t)(dmax) > BOS(dest))
#define _BOSW_CHK_N(dest, dmax) ((size_t)(dmax) != BOSW(dest))
#define _BOSW_OVR_N(dest, dmax) ((size_t)(dmax) > BOSW(dest))
#else
#define BOS(dest) BOS_UNKNOWN
#define BOSW(dest) BOS_UNKNOWN
#define _BOS_CHK(dest) 0
#define _BOSW_CHK(dest) 0
#define _BOS_ZERO(dest) 0
#define _BOS_NULL(dest) 0
#define _BOS_OVR_N(dest, dmax) 0
#endif

#ifndef TEST_BOS
#define bos_chk_err "error"
#else
#define bos_chk_err "warning"
#endif

#define _XSTR(s) _STR(s)
#define _STR(s) #s

/* diagnose_if compile-time check since clang-5, gcc not yet */
#if __has_attribute(diagnose_if) && defined(HAVE___BUILTIN_OBJECT_SIZE)
#ifdef HAVE_WARN_DMAX
#ifdef HAVE_ERROR_DMAX
#define bos_chk_sev "error"
#else
#define bos_chk_sev "warning"
#endif
#define BOS_CHK(dest)                                                          \
    __attribute__(                                                             \
        (diagnose_if(_BOS_OVR(dest, dmax), "dest overflow", bos_chk_err)))     \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(dest), "empty dest", bos_chk_err)))         \
            __attribute__((diagnose_if(_BOS_ZERO(dest, dmax),                  \
                                       "empty dest or dmax", bos_chk_err)))    \
                __attribute__((diagnose_if(_BOS_CHK(dest, dmax), "wrong dmax", \
                                           bos_chk_sev)))
#define BOSW_CHK(dest)                                                         \
    __attribute__(                                                             \
        (diagnose_if(_BOSW_OVR(dest, dmax), "dest overflow", bos_chk_err)))    \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(dest), "empty dest", bos_chk_err)))         \
            __attribute__((diagnose_if(_BOS_ZERO(dest, dmax),                  \
                                       "empty dest or dmax", bos_chk_err)))    \
                __attribute__((diagnose_if(_BOSW_CHK(dest, dmax),              \
                                           "wrong dmax", bos_chk_sev)))
#define BOS_CHK2(buf, bufsize)                                                 \
    __attribute__((diagnose_if(_BOS_OVR(buf, bufsize), _XSTR(buf) " overflow", \
                               bos_chk_err)))                                  \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))   \
            __attribute__((diagnose_if(                                        \
                _BOS_ZERO(buf, bufsize),                                       \
                "empty " _XSTR(buf) " or " _XSTR(bufsize), bos_chk_err)))      \
                __attribute__(                                                 \
                    (diagnose_if(_BOS_CHK(buf, bufsize),                       \
                                 "wrong " _XSTR(bufsize), bos_chk_sev)))
#define BOSW_CHK2(buf, bufsize)                                                \
    __attribute__((diagnose_if(_BOSW_OVR(buf, bufsize),                        \
                               _XSTR(buf) " overflow", bos_chk_err)))          \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))   \
            __attribute__((diagnose_if(                                        \
                _BOS_ZERO(buf, bufsize),                                       \
                "empty " _XSTR(buf) " or " _XSTR(bufsize), bos_chk_err)))      \
                __attribute__(                                                 \
                    (diagnose_if(_BOSW_CHK(buf, bufsize),                      \
                                 "wrong " _XSTR(bufsize), bos_chk_sev)))
/* allow slen=0, even with dest=NULL or dest overflow */
#define BOS_CHK_BUTZERO(dest, slen)                                            \
    __attribute__(                                                             \
        (diagnose_if(slen && (_BOS_OVR(dest, dmax) || _BOS_NULL(dest) ||       \
                              _BOS_ZERO(dest, dmax)),                          \
                     "dest overflow or empty", bos_chk_err)))                  \
        __attribute__(                                                         \
            (diagnose_if(_BOS_CHK(dest, dmax), "wrong dmax", bos_chk_sev)))
#define BOSW_CHK_BUTZERO(dest, slen)                                           \
    __attribute__(                                                             \
        (diagnose_if(slen && (_BOSW_OVR(dest, dmax) || _BOS_NULL(dest) ||      \
                              _BOS_ZERO(dest, dmax)),                          \
                     "dest overflow or empty", bos_chk_err)))                  \
        __attribute__(                                                         \
            (diagnose_if(_BOSW_CHK(dest, dmax), "wrong dmax", bos_chk_sev)))
#else /* !HAVE_WARN_DMAX */
#define BOS_CHK(dest)                                                          \
    __attribute__(                                                             \
        (diagnose_if(_BOS_OVR(dest, dmax), "dest overflow", bos_chk_err)))     \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(dest), "empty dest", bos_chk_err)))         \
            __attribute__((diagnose_if(_BOS_ZERO(dest, dmax),                  \
                                       "empty dest or dmax", bos_chk_err)))
/* allow slen=0, even with dest=NULL or dest overflow */
#define BOS_CHK_BUTZERO(dest, slen)                                            \
    __attribute__(                                                             \
        (diagnose_if(slen && (_BOS_OVR(dest, dmax) || _BOS_NULL(dest) ||       \
                              _BOS_ZERO(dest, dmax)),                          \
                     "dest overflow or empty", bos_chk_err)))
#define BOSW_CHK_BUTZERO(dest, slen)                                           \
    __attribute__(                                                             \
        (diagnose_if(slen && (_BOSW_OVR(dest, dmax) || _BOS_NULL(dest) ||      \
                              _BOS_ZERO(dest, dmax)),                          \
                     "dest overflow or empty", bos_chk_err)))
#define BOSW_CHK(dest)                                                         \
    __attribute__(                                                             \
        (diagnose_if(_BOSW_OVR(dest, dmax), "dest overflow", bos_chk_err)))    \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(dest), "empty dest", bos_chk_err)))         \
            __attribute__((diagnose_if(_BOS_ZERO(dest, dmax),                  \
                                       "empty dest or dmax", bos_chk_err)))
#define BOS_CHK2(buf, bufsize)                                                 \
    __attribute__((diagnose_if(_BOS_OVR(buf, bufsize), _XSTR(buf) " overflow", \
                               bos_chk_err)))                                  \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))   \
            __attribute__((diagnose_if(                                        \
                _BOS_ZERO(buf, bufsize),                                       \
                "empty " _XSTR(buf) " or " _XSTR(bufsize), bos_chk_err)))
#define BOSW_CHK2(buf, bufsize)                                                \
    __attribute__((diagnose_if(_BOSW_OVR(buf, bufsize),                        \
                               _XSTR(buf) " overflow", bos_chk_err)))          \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))   \
            __attribute__((diagnose_if(                                        \
                _BOS_ZERO(buf, bufsize),                                       \
                "empty " _XSTR(buf) " or " _XSTR(bufsize), bos_chk_err)))
#endif

/* independent on WARN_DMAX, pure overflow/null/zero checks */
/* no zero or null checks */
#define BOS_OVR2(buf, bufsize)                                                 \
    __attribute__((diagnose_if(_BOS_OVR(buf, bufsize), _XSTR(buf) " overflow", \
                               bos_chk_err)))                                  \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))
#define BOS_OVR_BUTNULL(dest)                                                  \
    __attribute__(                                                             \
        (diagnose_if(_BOS_OVR(dest, dmax), "dest overflow", bos_chk_err)))
#define BOS_OVR2_BUTNULL(buf, bufsize)                                         \
    __attribute__((diagnose_if(_BOS_OVR(buf, bufsize), _XSTR(buf) " overflow", \
                               bos_chk_err)))
/* allow bufsize=0, even with buf=NULL */
#define BOS_OVR2_BUTZERO(buf, bufsize)                                         \
    __attribute__(                                                             \
        (diagnose_if(bufsize && (_BOS_OVR(buf, bufsize) || _BOS_NULL(buf)),    \
                     _XSTR(buf) " overflow or empty", bos_chk_err)))
#define BOSW_OVR_BUTNULL(dest)                                                 \
    __attribute__(                                                             \
        (diagnose_if(_BOSW_OVR(dest, dmax), "dest overflow", bos_chk_err)))
#define BOSW_OVR2_BUTZERO(buf, bufsize)                                        \
    __attribute__(                                                             \
        (diagnose_if(bufsize && (_BOSW_OVR(buf, bufsize) || _BOS_NULL(buf)),   \
                     _XSTR(buf) " overflow or empty", bos_chk_err)))
#define BOSW_OVR2_BUTNULL(buf, bufsize)                                        \
    __attribute__((diagnose_if(_BOSW_OVR(buf, bufsize),                        \
                               _XSTR(buf) " overflow", bos_chk_err)))
#define BOS_NULL(buf)                                                          \
    __attribute__(                                                             \
        (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))
#define BOS_FMT(fmt)                                                           \
    __attribute__((diagnose_if(_BOS_NULL(fmt), "empty fmt", bos_chk_err)))
/* with zero check */
#define BOS_OVR2Z(buf, bufsize)                                                \
    __attribute__((diagnose_if(_BOS_OVR(buf, bufsize), _XSTR(buf) " overflow", \
                               bos_chk_err)))                                  \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))   \
            __attribute__((diagnose_if(                                        \
                _BOS_ZERO(buf, bufsize),                                       \
                "empty " _XSTR(buf) " or " _XSTR(bufsize), bos_chk_err)))
#define BOSW_OVR2(buf, bufsize)                                                \
    __attribute__((diagnose_if(_BOSW_OVR(buf, bufsize),                        \
                               _XSTR(buf) " overflow", bos_chk_err)))          \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))
#define BOSW_OVR2Z(buf, bufsize)                                               \
    __attribute__((diagnose_if(_BOSW_OVR(buf, bufsize),                        \
                               _XSTR(buf) " overflow", bos_chk_err)))          \
        __attribute__(                                                         \
            (diagnose_if(_BOS_NULL(buf), "empty " _XSTR(buf), bos_chk_err)))   \
            __attribute__((diagnose_if(                                        \
                _BOS_ZERO(buf, bufsize),                                       \
                "empty " _XSTR(buf) " or " _XSTR(bufsize), bos_chk_err)))
#define VAL_OVR2(value, vmax)                                                  \
    __attribute__(                                                             \
        (diagnose_if((value) > (vmax), _XSTR(value) " overflow >" _XSTR(vmax), \
                     bos_chk_err)))
#define VAL_OVR2_BUTZERO(value, vmax)                                          \
    __attribute__(                                                             \
        (diagnose_if((value) && (value) > (vmax),                              \
                     _XSTR(value) " overflow >" _XSTR(vmax), bos_chk_err)))
#define BOS_ATTR(chk, msg) __attribute__((diagnose_if(chk, msg, bos_chk_err)))
#else
#define BOS_CHK(dest)
#define BOSW_CHK(dest)
#define BOS_CHK2(buf, bufsize)
#define BOSW_CHK2(buf, bufsize)
#define BOS_OVR_BUTNULL(dest)
#define BOSW_OVR_BUTNULL(dest)
#define BOS_CHK_BUTZERO(dest, slen)
#define BOSW_CHK_BUTZERO(dest, slen)
#define BOS_OVR2(buf, bufsize)
#define BOSW_OVR2(buf, bufsize)
#define BOS_OVR2Z(buf, bufsize)
#define BOSW_OVR2Z(buf, bufsize)
#define BOS_OVR2_BUTZERO(buf, bufsize)
#define BOSW_OVR2_BUTZERO(buf, bufsize)
#define BOS_OVR2_BUTNULL(buf, bufsize)
#define BOSW_OVR2_BUTNULL(buf, bufsize)
#if __has_attribute(diagnose_if)
/* independent on BOS for pointer sizes, pure value checks */
#define VAL_OVR2(value, vmax)                                                  \
    __attribute__(                                                             \
        (diagnose_if((value) > (vmax), _XSTR(value) " overflow >" _XSTR(vmax), \
                     bos_chk_err)))
#define VAL_OVR2_BUTZERO(value, vmax)                                          \
    __attribute__(                                                             \
        (diagnose_if((value) && (value) > (vmax),                              \
                     _XSTR(value) " overflow >" _XSTR(vmax), bos_chk_err)))
#define BOS_NULL(buf)                                                          \
    __attribute__((diagnose_if(!buf, "empty " _XSTR(buf), bos_chk_err)))
#define BOS_FMT(fmt)                                                           \
    __attribute__((diagnose_if(!fmt, "empty fmt", bos_chk_err)))
#define BOS_ATTR(chk, msg) __attribute__((diagnose_if(chk, msg, bos_chk_err)))
#else
#define VAL_OVR2(value, vmax)
#define VAL_OVR2_BUTZERO(value, vmax)
#define BOS_NULL(buf)
#define BOS_NULLPTR(buf)
#define BOS_FMT(fmt)
#define BOS_ATTR(chk, msg)
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif /* __SAFE_COMPILE_H__ */
