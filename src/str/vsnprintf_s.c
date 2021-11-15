/*------------------------------------------------------------------
 * vsnprintf_s.c
 *
 * 2014-2019 Marco Paland
 * November 2021, Reini Urban
 *
 * Copyright (c) 2014-2019, PALANDesign Hannover, Germany
 * Copyright (c) 2021 by Reini Urban
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

///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2019, PALANDesign Hannover, Germany
//
// based on https://github.com/mpaland/printf/blob/master/printf.c
// converted by Reini Urban Nov 2021 for the _s variants.
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for
//        speed on embedded systems with a very limited
//        resources. These routines are thread safe and reentrant! Use
//        this instead of the bloated standard/newlib printf cause
//        these use malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////

/* safelib extensions:
 * added support for _s
 * %lc, %ls widechar conversions
 */

#include <stdbool.h>
#include <stdint.h>

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

// optionally define this globally (e.g. gcc -DPRINTF_INCLUDE_CONFIG_H ...) to include the
// printf_config.h header file
// default: undefined
#ifdef PRINTF_INCLUDE_CONFIG_H
#include "printf_config.h"
#endif

// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_NTOA_BUFFER_SIZE
#define PRINTF_NTOA_BUFFER_SIZE 32U
#endif

// 'ftoa' conversion buffer size, this must be big enough to hold one converted
// float number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_FTOA_BUFFER_SIZE
#define PRINTF_FTOA_BUFFER_SIZE 32U
#endif

// support for the floating point type (%f)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
#define PRINTF_SUPPORT_FLOAT
#endif

// support for exponential floating point notation (%e/%g)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_SUPPORT_EXPONENTIAL
#endif

// define the default floating point precision
// default: 6 digits
#ifndef PRINTF_DEFAULT_FLOAT_PRECISION
#define PRINTF_DEFAULT_FLOAT_PRECISION 6U
#endif

// define the largest float suitable to print with %f
// default: 1e9
#ifndef PRINTF_MAX_FLOAT
#define PRINTF_MAX_FLOAT 1e9
#endif

// support for the long long types (%llu or %p)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_SUPPORT_LONG_LONG
#endif

// support for the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
#define PRINTF_SUPPORT_PTRDIFF_T
#endif

///////////////////////////////////////////////////////////////////////////////

// internal flag definitions
#define FLAGS_ZEROPAD (1U << 0U)
#define FLAGS_LEFT (1U << 1U)
#define FLAGS_PLUS (1U << 2U)
#define FLAGS_SPACE (1U << 3U)
#define FLAGS_HASH (1U << 4U)
#define FLAGS_UPPERCASE (1U << 5U)
#define FLAGS_CHAR (1U << 6U)
#define FLAGS_SHORT (1U << 7U)
#define FLAGS_LONG (1U << 8U)
#define FLAGS_LONG_LONG (1U << 9U)
#define FLAGS_PRECISION (1U << 10U)
#define FLAGS_ADAPT_EXP (1U << 11U)

// import float.h for DBL_MAX
#if defined(PRINTF_SUPPORT_FLOAT)
#include <float.h>
#endif

// internal secure strlen
// \return The length of the string (excluding the terminating 0) limited by
// 'maxsize'
static inline unsigned int safec_strnlen_s(const char *str, size_t maxsize)
{
    const char *s;
    for (s = str; *s && maxsize--; ++s)
        ;
    return (unsigned int)(s - str);
}

// internal test if char is a digit (0-9)
// \return true if char is a digit
static inline bool safec_is_digit(char ch) { return (ch >= '0') && (ch <= '9'); }

// internal ASCII string to unsigned int conversion
static unsigned int safec_atoi(const char **str)
{
    unsigned int i = 0U;
    while (safec_is_digit(**str)) {
        i = i * 10U + (unsigned int)(*((*str)++) - '0');
    }
    return i;
}

// output the specified string in reverse, taking care of any zero-padding
static size_t safec_out_rev(out_fct_type out, char *buffer, size_t idx,
                            size_t maxlen, const char *buf, size_t len,
                            unsigned int width, unsigned int flags)
{
    const size_t start_idx = idx;

    // pad spaces up to given width
    if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
        for (size_t i = len; i < width; i++) {
            int rc = out(' ', buffer, idx++, maxlen);
            if (unlikely(rc < 0))
                return rc;
        }
    }

    // reverse string
    while (len) {
        int rc = out(buf[--len], buffer, idx++, maxlen);
        if (unlikely(rc < 0))
            return rc;
    }

    // append pad spaces up to given width
    if (flags & FLAGS_LEFT) {
        while (idx - start_idx < width) {
            int rc = out(' ', buffer, idx++, maxlen);
            if (unlikely(rc < 0))
                return rc;
        }
    }

    return idx;
}

// internal itoa format
static size_t safec_ntoa_format(out_fct_type out, const char *funcname,
				char *buffer, size_t idx,
                                size_t maxlen, char *buf, size_t len, bool negative,
                                unsigned int base, unsigned int prec,
                                unsigned int width, unsigned int flags)
{
    // pad leading zeros
    if (!(flags & FLAGS_LEFT)) {
        if (width && (flags & FLAGS_ZEROPAD) &&
            (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
            width--;
        }
        while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = '0';
        }
        while ((flags & FLAGS_ZEROPAD) && (len < width) &&
               (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = '0';
        }
    }

    // handle hash
    if (flags & FLAGS_HASH) {
        if (!(flags & FLAGS_PRECISION) && len &&
            ((len == prec) || (len == width))) {
            len--;
            if (len && (base == 16U)) {
                len--;
            }
        }
        if ((base == 16U) && !(flags & FLAGS_UPPERCASE) &&
            (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'x';
        } else if ((base == 16U) && (flags & FLAGS_UPPERCASE) &&
                   (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'X';
        } else if ((base == 2U) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'b';
        }
        if (len < PRINTF_NTOA_BUFFER_SIZE) {
            buf[len++] = '0';
        }
    }

    if (len < PRINTF_NTOA_BUFFER_SIZE) {
        if (negative) {
            buf[len++] = '-';
        } else if (flags & FLAGS_PLUS) {
            buf[len++] = '+'; // ignore the space if the '+' exists
        } else if (flags & FLAGS_SPACE) {
            buf[len++] = ' ';
        }
    }
    if (width > 2147483614) {
        char msg[80];
        snprintf(msg, sizeof msg, "%s: width exceeds max", funcname);
        invoke_safe_str_constraint_handler(msg, buf, ESLEMAX);
        return -ESLEMAX;
    }

    return safec_out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

// internal itoa for 'long' type
static size_t safec_ntoa_long(out_fct_type out,  const char *funcname,
			      char *buffer, size_t idx,
                              size_t maxlen, unsigned long value, bool negative,
                              unsigned long base, unsigned int prec,
                              unsigned int width, unsigned int flags)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    size_t len = 0U;

    // no hash for 0 values
    if (!value) {
        flags &= ~FLAGS_HASH;
    }

    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] =
                digit < 10 ? '0' + digit
                           : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
    }

    return safec_ntoa_format(out, funcname, buffer, idx, maxlen, buf, len, negative,
                             (unsigned int)base, prec, width, flags);
}

// internal itoa for 'long long' type
#if defined(PRINTF_SUPPORT_LONG_LONG)
static size_t safec_ntoa_long_long(out_fct_type out,  const char *funcname,
				   char *buffer, size_t idx,
                                   size_t maxlen, unsigned long long value,
                                   bool negative, unsigned long long base,
                                   unsigned int prec, unsigned int width,
                                   unsigned int flags)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    size_t len = 0U;

    // no hash for 0 values
    if (!value) {
        flags &= ~FLAGS_HASH;
    }

    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] =
                digit < 10 ? '0' + digit
                           : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
    }

    return safec_ntoa_format(out, funcname, buffer, idx, maxlen, buf, len, negative,
                             (unsigned int)base, prec, width, flags);
}
#endif // PRINTF_SUPPORT_LONG_LONG

#if defined(PRINTF_SUPPORT_FLOAT)

#if defined(PRINTF_SUPPORT_EXPONENTIAL)
// forward declaration so that safec_ftoa can switch to exp notation for values >
// PRINTF_MAX_FLOAT
static size_t safec_etoa(out_fct_type out,  const char *funcname,
			 char *buffer, size_t idx, size_t maxlen,
                         double value, unsigned int prec, unsigned int width,
                         unsigned int flags);
#endif

// internal ftoa for fixed decimal floating point
static size_t safec_ftoa(out_fct_type out,  const char *funcname,
			 char *buffer, size_t idx, size_t maxlen,
                         double value, unsigned int prec, unsigned int width,
                         unsigned int flags)
{
    char buf[PRINTF_FTOA_BUFFER_SIZE];
    size_t len = 0U;
    double tmp;
    double diff = 0.0;
    unsigned long frac;
    int whole;
    bool negative;

    // powers of 10
    static const double pow10[] = {1,         10,        100,     1000,
                                   10000,     100000,    1000000, 10000000,
                                   100000000, 1000000000};

    // test for special values
    if (value != value)
        return safec_out_rev(out, buffer, idx, maxlen, "nan", 3, width, flags);
    if (value < -DBL_MAX)
        return safec_out_rev(out, buffer, idx, maxlen, "fni-", 4, width, flags);
    if (value > DBL_MAX)
        return safec_out_rev(out, buffer, idx, maxlen,
                        (flags & FLAGS_PLUS) ? "fni+" : "fni",
                        (flags & FLAGS_PLUS) ? 4U : 3U, width, flags);

    // test for very large values
    // standard printf behavior is to print EVERY whole number digit -- which
    // could be 100s of characters overflowing your buffers == bad
    if ((value > PRINTF_MAX_FLOAT) || (value < -PRINTF_MAX_FLOAT)) {
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
        return safec_etoa(out, funcname, buffer, idx, maxlen, value, prec, width, flags);
#else
        return 0U;
#endif
    }

    // test for negative
    negative = false;
    if (value < 0) {
        negative = true;
        value = 0 - value;
    }

    // set default precision, if not set explicitly
    if (!(flags & FLAGS_PRECISION)) {
        prec = PRINTF_DEFAULT_FLOAT_PRECISION;
    }
    // limit precision to 9, cause a prec >= 10 can lead to overflow errors
    while ((len < PRINTF_FTOA_BUFFER_SIZE) && (prec > 9U)) {
        buf[len++] = '0';
        prec--;
    }

    whole = (int)value;
    tmp = (value - whole) * pow10[prec];
    frac = (unsigned long)tmp;
    diff = tmp - frac;

    if (diff > 0.5) {
        ++frac;
        // handle rollover, e.g. case 0.99 with prec 1 is 1.0
        if (frac >= pow10[prec]) {
            frac = 0;
            ++whole;
        }
    } else if (diff < 0.5) {
    } else if ((frac == 0U) || (frac & 1U)) {
        // if halfway, round up if odd OR if last digit is 0
        ++frac;
    }

    if (prec == 0U) {
        diff = value - (double)whole;
        if ((!(diff < 0.5) || (diff > 0.5)) && (whole & 1)) {
            // exactly 0.5 and ODD, then round up
            // 1.5 -> 2, but 2.5 -> 2
            ++whole;
        }
    } else {
        unsigned int count = prec;
        // now do fractional part, as an unsigned number
        while (len < PRINTF_FTOA_BUFFER_SIZE) {
            --count;
            buf[len++] = (char)(48U + (frac % 10U));
            if (!(frac /= 10U)) {
                break;
            }
        }
        // add extra 0s
        while ((len < PRINTF_FTOA_BUFFER_SIZE) && (count-- > 0U)) {
            buf[len++] = '0';
        }
        if (len < PRINTF_FTOA_BUFFER_SIZE) {
            // add decimal
            buf[len++] = '.';
        }
    }

    // do whole part, number is reversed
    while (len < PRINTF_FTOA_BUFFER_SIZE) {
        buf[len++] = (char)(48 + (whole % 10));
        if (!(whole /= 10)) {
            break;
        }
    }

    // pad leading zeros
    if (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD)) {
        if (width && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
            width--;
        }
        while ((len < width) && (len < PRINTF_FTOA_BUFFER_SIZE)) {
            buf[len++] = '0';
        }
    }

    if (len < PRINTF_FTOA_BUFFER_SIZE) {
        if (negative) {
            buf[len++] = '-';
        } else if (flags & FLAGS_PLUS) {
            buf[len++] = '+'; // ignore the space if the '+' exists
        } else if (flags & FLAGS_SPACE) {
            buf[len++] = ' ';
        }
    }

    return safec_out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

#if defined(PRINTF_SUPPORT_EXPONENTIAL)
// internal ftoa variant for exponential floating-point type, contributed by
// Martijn Jasperse <m.jasperse@gmail.com>
static size_t safec_etoa(out_fct_type out, const char *funcname,
			 char *buffer, size_t idx, size_t maxlen,
                         double value, unsigned int prec, unsigned int width,
                         unsigned int flags)
{
    union {
        uint64_t U;
        double F;
    } conv;
    int exp2, expval;
    unsigned int minwidth, fwidth;
    bool negative;

    // check for NaN and special values
    if ((value != value) || (value > DBL_MAX) || (value < -DBL_MAX)) {
        return safec_ftoa(out, funcname, buffer, idx, maxlen, value, prec, width, flags);
    }

    // determine the sign
    negative = value < 0;
    if (negative) {
        value = -value;
    }

    // default precision
    if (!(flags & FLAGS_PRECISION)) {
        prec = PRINTF_DEFAULT_FLOAT_PRECISION;
    }

    // determine the decimal exponent
    // based on the algorithm by David Gay
    // (https://www.ampl.com/netlib/fp/dtoa.c)
    conv.F = value;
    exp2 = (int)((conv.U >> 52U) & 0x07FFU) - 1023; // effectively log2
    conv.U = (conv.U & ((1ULL << 52U) - 1U)) |
             (1023ULL << 52U); // drop the exponent so conv.F is now in [1,2)
    // now approximate log10 from the log2 integer part and an expansion of ln
    // around 1.5
    expval = (int)(0.1760912590558 + exp2 * 0.301029995663981 +
                       (conv.F - 1.5) * 0.289529654602168);
    // now we want to compute 10^expval but we want to be sure it won't overflow
    exp2 = (int)(expval * 3.321928094887362 + 0.5);
    {
        const double z = expval * 2.302585092994046 - exp2 * 0.6931471805599453;
        const double z2 = z * z;
        conv.U = (uint64_t)(exp2 + 1023) << 52U;
        // compute exp(z) using continued fractions, see
        // https://en.wikipedia.org/wiki/Exponential_function#Continued_fractions_for_ex
        conv.F *= 1 + 2 * z / (2 - z + (z2 / (6 + (z2 / (10 + z2 / 14)))));
        // correct for rounding errors
        if (value < conv.F) {
            expval--;
            conv.F /= 10;
        }
    }

    // the exponent format is "%+03d" and largest value is "307", so set aside
    // 4-5 characters
    minwidth = ((expval < 100) && (expval > -100)) ? 4U : 5U;

    // in "%g" mode, "prec" is the number of *significant figures* not decimals
    if (flags & FLAGS_ADAPT_EXP) {
        // do we want to fall-back to "%f" mode?
        if ((value >= 1e-4) && (value < 1e6)) {
            if ((int)prec > expval) {
                prec = (unsigned)((int)prec - expval - 1);
            } else {
                prec = 0;
            }
            flags |= FLAGS_PRECISION; // make sure safec_ftoa respects precision
            // no characters in exponent
            minwidth = 0U;
            expval = 0;
        } else {
            // we use one sigfig for the whole part
            if ((prec > 0) && (flags & FLAGS_PRECISION)) {
                --prec;
            }
        }
    }

    // will everything fit?
    fwidth = width;
    if (width > minwidth) {
        // we didn't fall-back so subtract the characters required for the
        // exponent
        fwidth -= minwidth;
    } else {
        // not enough characters, so go back to default sizing
        fwidth = 0U;
    }
    if ((flags & FLAGS_LEFT) && minwidth) {
        // if we're padding on the right, DON'T pad the floating part
        fwidth = 0U;
    }

    // rescale the float value
    if (expval) {
        value /= conv.F;
    }

    // output the floating part
    {
        const size_t start_idx = idx;
        idx = safec_ftoa(out, funcname, buffer, idx, maxlen,
                         negative ? -value : value, prec, fwidth,
                         flags & ~FLAGS_ADAPT_EXP);

        // output the exponent part
        if (minwidth) {
            // output the exponential symbol
            int rc = out((flags & FLAGS_UPPERCASE) ? 'E' : 'e', buffer, idx++, maxlen);
            if (unlikely(rc < 0))
                return rc;
            // output the exponent value
            idx = safec_ntoa_long(out, funcname, buffer, idx, maxlen,
                             (expval < 0) ? -expval : expval, expval < 0, 10, 0,
                             minwidth - 1, FLAGS_ZEROPAD | FLAGS_PLUS);
            // might need to right-pad spaces
            if (flags & FLAGS_LEFT) {
                while (idx - start_idx < width) {
                    out(' ', buffer, idx++, maxlen);
                    if (unlikely(rc < 0))
                        return rc;
                }
            }
        }
    }
    return idx;
}
#endif // PRINTF_SUPPORT_EXPONENTIAL
#endif // PRINTF_SUPPORT_FLOAT

// internal vsnprintf, extended for _s.
// already checked in the caller
// mingw has a _vsnprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,va_list _ArgList);
int safec_vsnprintf_s(out_fct_type out, const char* funcname,
                      char *buffer, const size_t bufsize, const char *format, va_list va)
{
    size_t idx = 0U;
    unsigned int flags, width, precision, n;
    int rc = 0;

    while (*format) {
        if ((long)idx < 0)
            return idx;
        // format specifier?  %[flags][width][.precision][length]
        if (*format != '%') {
            // no
            rc = out(*format, buffer, idx++, bufsize);
            if (unlikely(rc < 0))
                return rc;
            format++;
            continue;
        } else {
            // yes, evaluate it
            format++;
        }

        // evaluate flags
        flags = 0U;
        do {
            switch (*format) {
            case '0':
                flags |= FLAGS_ZEROPAD;
                format++;
                n = 1U;
                break;
            case '-':
                flags |= FLAGS_LEFT;
                format++;
                n = 1U;
                break;
            case '+':
                flags |= FLAGS_PLUS;
                format++;
                n = 1U;
                break;
            case ' ':
                flags |= FLAGS_SPACE;
                format++;
                n = 1U;
                break;
            case '#':
                flags |= FLAGS_HASH;
                format++;
                n = 1U;
                break;
            default:
                n = 0U;
                break;
            }
        } while (n);

        // evaluate width field
        width = 0U;
        if (safec_is_digit(*format)) {
            width = safec_atoi(&format);
        } else if (*format == '*') {
            const int w = va_arg(va, int);
            if (w < 0) {
                flags |= FLAGS_LEFT; // reverse padding
                width = (unsigned int)-w;
            } else {
                width = (unsigned int)w;
            }
            format++;
        }

        // evaluate precision field
        precision = 0U;
        if (*format == '.') {
            flags |= FLAGS_PRECISION;
            format++;
            if (safec_is_digit(*format)) {
                precision = safec_atoi(&format);
            } else if (*format == '*') {
                const int prec = (int)va_arg(va, int);
                precision = prec > 0 ? (unsigned int)prec : 0U;
                format++;
            }
        }

        // evaluate length field
        switch (*format) {
        case 'l':
            flags |= FLAGS_LONG;
            format++;
            if (*format == 'l') {
                flags |= FLAGS_LONG_LONG;
                format++;
            }
            break;
        case 'h':
            flags |= FLAGS_SHORT;
            format++;
            if (*format == 'h') {
                flags |= FLAGS_CHAR;
                format++;
            }
            break;
#if defined(PRINTF_SUPPORT_PTRDIFF_T)
        case 't':
            flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG
                                                        : FLAGS_LONG_LONG);
            format++;
            break;
#endif
        case 'j':
            flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG
                                                       : FLAGS_LONG_LONG);
            format++;
            break;
        case 'z':
            flags |=
                (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
            format++;
            break;
        default:
            break;
        }

        // evaluate specifier
        switch (*format) {
        case 'd':
        case 'i':
        case 'u':
        case 'x':
        case 'X':
        case 'o':
        case 'b': {
            // set the base
            unsigned int base;
            if (*format == 'x' || *format == 'X') {
                base = 16U;
            } else if (*format == 'o') {
                base = 8U;
            } else if (*format == 'b') {
                base = 2U;
            } else {
                base = 10U;
                flags &= ~FLAGS_HASH; // no hash for dec format
            }
            // uppercase
            if (*format == 'X') {
                flags |= FLAGS_UPPERCASE;
            }

            // no plus or space flag for u, x, X, o, b
            if ((*format != 'i') && (*format != 'd')) {
                flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
            }

            // ignore '0' flag when precision is given
            if (flags & FLAGS_PRECISION) {
                flags &= ~FLAGS_ZEROPAD;
            }

            // convert the integer
            if ((*format == 'i') || (*format == 'd')) {
                // signed
                if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                    const long long value = va_arg(va, long long);
                    idx = safec_ntoa_long_long(
                        out, funcname, buffer, idx, bufsize,
                        (unsigned long long)(value > 0 ? value : 0 - value),
                        value < 0, base, precision, width, flags);
#endif
                } else if (flags & FLAGS_LONG) {
                    const long value = va_arg(va, long);
                    idx = safec_ntoa_long(
                        out, funcname, buffer, idx, bufsize,
                        (unsigned long)(value > 0 ? value : 0 - value),
                        value < 0, base, precision, width, flags);
                } else {
                    const int value =
                        (flags & FLAGS_CHAR)    ? (char)va_arg(va, int)
                        : (flags & FLAGS_SHORT) ? (short int)va_arg(va, int)
                                                : va_arg(va, int);
                    idx = safec_ntoa_long(
                        out, funcname, buffer, idx, bufsize,
                        (unsigned int)(value > 0 ? value : 0 - value),
                        value < 0, base, precision, width, flags);
                }
            } else {
                // unsigned
                if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                    idx = safec_ntoa_long_long(
                        out, funcname, buffer, idx, bufsize,
                        va_arg(va, unsigned long long), false, base, precision,
                        width, flags);
#endif
                } else if (flags & FLAGS_LONG) {
                    idx = safec_ntoa_long(out, funcname, buffer, idx, bufsize,
                                          va_arg(va, unsigned long), false,
                                          base, precision, width, flags);
                } else {
                    const unsigned int value =
                        (flags & FLAGS_CHAR)
                            ? (unsigned char)va_arg(va, unsigned int)
                        : (flags & FLAGS_SHORT)
                            ? (unsigned short int)va_arg(va, unsigned int)
                            : va_arg(va, unsigned int);
                    idx = safec_ntoa_long(out, funcname, buffer, idx, bufsize,
                                          value, false, base, precision, width,
                                          flags);
                }
            }
            format++;
            break;
        }
#if defined(PRINTF_SUPPORT_FLOAT)
        case 'f':
        case 'F':
            if (*format == 'F')
                flags |= FLAGS_UPPERCASE;
            idx = safec_ftoa(out, funcname, buffer, idx, bufsize,
                             va_arg(va, double), precision, width, flags);
            format++;
            break;
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if ((*format == 'g') || (*format == 'G'))
                flags |= FLAGS_ADAPT_EXP;
            if ((*format == 'E') || (*format == 'G'))
                flags |= FLAGS_UPPERCASE;
            idx = safec_etoa(out, funcname, buffer, idx, bufsize,
                             va_arg(va, double), precision, width, flags);
            format++;
            break;
#endif // PRINTF_SUPPORT_EXPONENTIAL
#endif // PRINTF_SUPPORT_FLOAT
        case 'c': {
            unsigned int l = 1U;
            char wstr[4];
            if (flags & FLAGS_LONG) {
#ifndef SAFECLIB_DISABLE_WCHAR
                int len = wctomb(wstr, va_arg(va, int));
                if (len <= 0) {
		    char msg[80];
		    snprintf(msg, sizeof msg, "%s: wctomb for %%lc arg failed", funcname);
                    invoke_safe_str_constraint_handler(msg, buffer, RCNEGATE(-len));
                    return len;
                }
#else
		char msg[80];
		snprintf(msg, sizeof msg, "%s: unsupported %%lc arg", funcname);
                invoke_safe_str_constraint_handler(msg, buffer, RCNEGATE(1));
                return -1;
#endif
            }
            // pre padding
            if (!(flags & FLAGS_LEFT)) {
                while (l++ < width) {
                    rc = out(' ', buffer, idx++, bufsize);
                    if (unlikely(rc < 0))
                        return rc;
                }
            }
            // char output
            if (flags & FLAGS_LONG) {
                char *p = &wstr[0];
                while (*p != 0) {
                    rc = out(*(p++), buffer, idx++, bufsize);
                    if (unlikely(rc < 0))
                        return rc;
                }
            } else {
                rc = out((char)va_arg(va, int), buffer, idx++, bufsize);
                if (unlikely(rc < 0))
                    return rc;
            }
            // post padding
            if (flags & FLAGS_LEFT) {
                while (l++ < width) {
                    rc = out(' ', buffer, idx++, bufsize);
                    if (unlikely(rc < 0))
                        return rc;
                }
            }
            format++;
            break;
        }

        case 's': {
            char *p;
            unsigned int l;
            if (flags & FLAGS_LONG) {
#ifndef SAFECLIB_DISABLE_WCHAR
                size_t len;
                errno_t err;
                const wchar_t *lp = va_arg(va, wchar_t *);
                if (!lp) {
		    char msg[80];
		    snprintf(msg, sizeof msg, "%s: %%ls arg is null", funcname);
                    invoke_safe_str_constraint_handler(msg, buffer, ESNULLP);
                    return -(ESNULLP);
                }
                l = wcsnlen_s(lp, precision ? precision : (size_t)-1);
                p = (char*)malloc(l + 1);
                if (!p) {
		    char msg[80];
		    snprintf(msg, sizeof msg, "%s: malloc %%ls arg failed", funcname);
                    invoke_safe_str_constraint_handler(msg, buffer, 1);
                    return -1;
                }
                err = wcstombs_s(&len, p, l, lp, l);
                if (err != EOK) {
		    char msg[80];
		    snprintf(msg, sizeof msg, "%s: wcstombs_s for %%ls arg failed", funcname);
                    invoke_safe_str_constraint_handler(msg, buffer, RCNEGATE(err));
                    return err; 
                }
#else
		{
		    char msg[80];
		    snprintf(msg, sizeof msg, "%s: unsupported %%ls arg", funcname);
		    invoke_safe_str_constraint_handler(msg, buffer, RCNEGATE(1));
		}
                return -1;
#endif
            } else {
                p = va_arg(va, char *);
                if (!p) {
		    char msg[80];
		    snprintf(msg, sizeof msg, "%s: %%s arg is null", funcname);
                    invoke_safe_str_constraint_handler(msg, buffer, ESNULLP);
                    return -(ESNULLP);
                }
                l = safec_strnlen_s(p, precision ? precision : (size_t)-1);
            }
            if (l + idx > bufsize) {
		char msg[80];
		snprintf(msg, sizeof msg, "%s: %%s arg exceeds dmax", funcname);
                invoke_safe_str_constraint_handler(msg, buffer, ESNOSPC);
                if (flags & FLAGS_LONG)
                    free(p);
                return -(ESNOSPC);
            }
            // pre padding
            if (flags & FLAGS_PRECISION) {
                l = (l < precision ? l : precision);
            }
            if (!(flags & FLAGS_LEFT)) {
                while (l++ < width) {
                    rc = out(' ', buffer, idx++, bufsize);
                    if (unlikely(rc < 0)) {
                        if (flags & FLAGS_LONG)
                            free(p);
                        return rc;
                    }
                }
            }
            // string output
            while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--)) {
                rc = out(*(p++), buffer, idx++, bufsize);
                if (unlikely(rc < 0)) { //eg.  EBADF write to closed file
                    if (flags & FLAGS_LONG)
                        free(p);
                    return rc;
                }
            }
            if (flags & FLAGS_LONG)
                free(p);
            // post padding
            if (flags & FLAGS_LEFT) {
                while (l++ < width) {
                    rc = out(' ', buffer, idx++, bufsize);
                    if (unlikely(rc < 0))
                        return rc;
                }
            }
            format++;
            break;
        }

        case 'p': {
            uintptr_t arg = (uintptr_t)va_arg(va, void *);
            width = sizeof(void *) * 2U;
            flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
#if defined(PRINTF_SUPPORT_LONG_LONG)
            if (sizeof(uintptr_t) == sizeof(long long)) {
                idx = safec_ntoa_long_long(out, funcname, buffer, idx, bufsize,
                                      arg, false, 16U,
                                      precision, width, flags);
            } else {
#endif
                idx = safec_ntoa_long(out, funcname, buffer, idx, bufsize,
                                 (unsigned long)arg,
                                 false, 16U, precision, width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
            }
#endif
            format++;
            break;
        }

        case '%':
            rc = out('%', buffer, idx++, bufsize);
            if (unlikely(rc < 0))
                return rc;
            format++;
            break;

        case 'n':
            {
                char msg[80];
                snprintf(msg, sizeof msg, "%s: illegal %%n", funcname);
                invoke_safe_str_constraint_handler(msg, NULL, EINVAL);
                return -1;
            }
        default:
            // illegal % format-specifier
#ifndef ENABLE_ILLEGAL_FORMATSPECIFIER
            {
                char msg[80];
                snprintf(msg, sizeof msg, "%s: illegal %%%c format-specifier", funcname, *format);
                invoke_safe_str_constraint_handler(msg, NULL, EINVAL);
            }
            return -1;
#else
            {
                rc = out(*format, buffer, idx++, bufsize);
                if (unlikely(rc < 0))
                    return rc;
            }
            format++;
#endif
            break;
        }
    }

    // termination
    rc = out((char)0, buffer, idx < bufsize ? idx : bufsize - 1U, bufsize);
    if (unlikely(rc < 0))
        return rc;

    // return written chars without terminating \0
    return (int)idx;
}

/**
 * @def vsnprintf_s(dest,dmax,fmt,ap)
 * @brief
 *    The truncating \c vsnprintf_s function composes a string with same test
 *    that would be printed if format was used on \c printf. Instead of being
 *    printed, the content is stored in dest.  At most dmax characters are
 *    written.  It is guaranteed that dest will be null-terminated.
 *
 * @note
 *    POSIX specifies that \c errno is set on error. However, the safeclib
 *    extended \c ES* errors do not set \c errno, only when the underlying
 *    system \c vsnprintf call fails, \c errno is set.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.5.3.12 The vsnprintf_s function (p: 600)
 *    http://en.cppreference.com/w/c/io/vfprintf
 *
 * @param[out]  dest  pointer to string that will be written into.
 * @param[in]   dmax  restricted maximum length of \c dest
 * @param[in]   fmt   format-control string.
 * @param[in]   ap    optional arguments
 *
 * @pre \c fmt shall not be a null pointer.
 * @pre \c dest shall not be a null pointer.
 * @pre \c dmax shall not be zero.
 * @pre \c dmax shall not be greater than \c RSIZE_MAX_STR and the size of
 * dest.
 * @pre \c fmt  shall not contain the conversion specifier \c %n.
 * @pre None of the arguments corresponding to \c %s is a null pointer.
 * @pre No encoding error shall occur.
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_STR.
 *
 * @return  On success the total number of characters written is returned.
 * @return  On failure a negative error number is returned.
 * @return  If the buffer \c dest is too small for the formatted text,
 *          including the terminating null, then the buffer is truncated
 *          and null terminated.
 *
 * @retval  -ESNULLP    when \c dest or fmt is NULL pointer, or
 *                      when a \c %s argument is NULL
 * @retval  -ESZEROL    when \c dmax == 0
 * @retval  -ESLEMAX    when \c dmax > \c RSIZE_MAX_STR
 * @retval  -EOVERFLOW  when \c dmax > size of dest
 * @retval  -EINVAL     when fmt contains %n
 *
 * @see
 *    snprintf_s(), sprintf_s(), vsprintf_s(), vsnwprintf_s()
 */
#ifdef FOR_DOXYGEN
int vsnprintf_s(char *restrict dest, rsize_t dmax,
                const char *restrict fmt, va_list ap)
#else
EXPORT int _vsnprintf_s_chk(char *restrict dest, rsize_t dmax,
                            const size_t destbos, const char *restrict fmt,
                            va_list ap)
#endif
{
    const char *p;
    int ret;

    if (unlikely(dest == NULL || fmt == NULL)) {
        invoke_safe_str_constraint_handler("vsnprintf_s: dest/fmt is null",
                                           dest, ESNULLP);
        return -ESNULLP;
    }
    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("vsnprintf_s: dmax is zero", dest,
                                           ESZEROL);
        return -ESZEROL;
    }
    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("vsnprintf_s: dmax exceeds max",
                                           dest, ESLEMAX);
        return -ESLEMAX;
    }
    if (destbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(dest, dmax);
    } else {
        if (unlikely(dmax > destbos)) {
            return -(handle_str_bos_overload("vsnprintf_s: dmax exceeds dest",
                                             dest, destbos));
        }
    }
    // catch %n early, before it outputs anything
    if (unlikely((p = strnstr(fmt, "%n", RSIZE_MAX_STR)))) {
        /* at the beginning or if inside, not %%n */
        if ((p - fmt == 0) || *(p - 1) != '%') {
            invoke_safe_str_constraint_handler("vsnprintf_s: illegal %n", dest,
                                               EINVAL);
            return -(EINVAL);
        }
    }

    errno = 0;
    ret = safec_vsnprintf_s(safec_out_buffer, "vsnprintf_s", dest, dmax, fmt, ap);

    //if (unlikely(ret < 0)) {
    //    char errstr[128] = "vsnprintf_s: ";
    //    strcat(errstr, strerror(errno));
    //    handle_error(dest, dmax, errstr, -ret);
    //    return ret;
    //}

    /* manual truncation */
    if (ret >= 0) {
#ifdef SAFECLIB_STR_NULL_SLACK
    /* oops, ret would have been written if dmax was ignored */
        if ((rsize_t)ret > dmax) {
            dest[dmax - 1] = '\0';
        } else {
            memset(&dest[ret], 0, dmax - ret);
        }
#else
        dest[dmax - 1] = '\0';
#endif
    } else {
#ifdef SAFECLIB_STR_NULL_SLACK
        memset(dest, 0, dmax);
#else
        *dest = '\0';
#endif
    }
    return ret;
}
