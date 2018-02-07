/*------------------------------------------------------------------
 * strerror_s.h
 *
 * October 2017, Reini Urban
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

static const char* errmsgs_s[] = {
    "null ptr",       	     /* ESNULLP */
    "length is zero",         /* ESZEROL */
    "length is below min",    /* ESLEMIN */
    "length exceeds RSIZE_MAX",/* ESLEMAX */
    "overlap undefined",      /* ESOVRLP */
    "empty string",           /* ESEMPTY */
    "not enough space",       /* ESNOSPC */
    "unterminated string",    /* ESUNTERM */
    "no difference",          /* ESNODIFF */
    "not found",              /* ESNOTFND */
    "wrong length",           /* ESLEWRNG */
};

static const int len_errmsgs_s[] = {
    sizeof "null ptr",       	     /* ESNULLP */
    sizeof "length is zero",         /* ESZEROL */
    sizeof "length is below min",    /* ESLEMIN */
    sizeof "length exceeds RSIZE_MAX",/* ESLEMAX */
    sizeof "overlap undefined",      /* ESOVRLP */
    sizeof "empty string",           /* ESEMPTY */
    sizeof "not enough space",       /* ESNOSPC */
    sizeof "unterminated string",    /* ESUNTERM */
    sizeof "no difference",          /* ESNODIFF */
    sizeof "not found",              /* ESNOTFND */
    sizeof "wrong length",           /* ESLEWRNG */
};
