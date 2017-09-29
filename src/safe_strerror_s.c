/*------------------------------------------------------------------
 * safe_strerror_s.c
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

#include "safeclib_private.h"
#include <string.h>

static const char errmsgs[] =
    "null ptr\0"		/* ESNULLP */
    "length is zero\0"		/* ESZEROL */
    "length is below min\0"	/* ESLEMIN */
    "length exceeds RSIZE_MAX\0"/* ESLEMAX */
    "overlap undefined\0"	/* ESOVRLP */
    "empty string\0"		/* ESEMPTY */
    "not enough space\0"	/* ESNOSPC */
    "unterminated string\0"	/* ESUNTERM */
    "no difference\0"		/* ESNODIFF */
    "not found\0"		/* ESNOTFND */
    "illegal %n\0"              /* ESILFMTN */
    "\0Unknown error";

static char errbuf[80];

const char *_strerror(int errcode)
{
    char *s;
    if (unlikely(errcode < ESNULLP))
        return strerror(errcode);
    if (unlikely(errcode > ESILFMTN))
        return "Unknown error";
    errcode -= 400;
    for (s=(char*)errmsgs; errcode && *s; errcode--, s++)
        for (; *s; s++)
            ;
    if (!*s) s++;
    return s;
}

const char *_safec_strerror(const char* prefix, int errcode)
{
    strcpy(errbuf, prefix);
    strcat(errbuf, _strerror(errcode));
    return errbuf;
}
