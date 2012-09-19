/*------------------------------------------------------------------
 * Safe C Library
 *
 * Copyright (c) 2009 Cisco Systems
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


Intro

The ISO TR24731 Bounds Checking Interface documents indicate that the
key motivation for the new specification is to help mitigate the ever
increasing security attacks, specifically the buffer overrun.

The rationale document says ""Buffer overrun attacks continue to be a
security problem. Roughly 10% of vulnerability reports cataloged by
CERT from 01/01/2005 to 07/01/2005 involved buffer overflows. Preventing
buffer overruns is the primary, but not the only, motivation for this
technical report.""

The rationale document continues ""that these only mitigate, that is
lessen, security problems. When used properly, these functions decrease
the danger buffer overrun attacks. Source code may remain vulnerable
due to other bugs and security issues. The highest level of security
is achieved by building in layers of security utilizing multiple
strategies.""


The rationale document lists the following key points for TR24731:
  -Guard against overflowing a buffer
  -Do not produce unterminated strings
  -Do not unexpectedly truncate strings
  -Provide a library useful to existing code
  -Preserve the null terminated string datatype
  -Only require local edits to programs
  -Library based solution
  -Support compile-time checking
  -Make failures obvious
  -Zero buffers, null strings
  -Runtime-constraint handler mechanism
  -Support re-entrant code
  -Consistent naming scheme
  -Have a uniform pattern for the function parameters and return type
  -Deference to existing technology

and the following can be added...

  -provide a library of functions with like behavior
  -provide a library of functions that promote and increase code safety and security
  -provide a library of functions that are efficient



Safe C Library Design Considerations

This library only implements a subset of the functions defined in the
specification.  Included in the library are extensions to the specification
to provide a complementary set of functions with like behavior.


Use of errno

The TR24731 specification says an implementation may set errno for
the functions deﬁned in the technical report, but is not required to.
This library does not set errno.  The library does use errno return
codes as required by functional APIs.  Specific Safe C String and
Safe C Memory errno codes are defined in the safe_errno.h file.


Runtime-constraints

Per the spec, the library verifies that the calling program does not
violate the function's runtime-constraints. If a runtime-constraint
is violated, the library calls the currently registered runtime-constraint
handler.

Per the spec, multiple runtime-constraint violations in the same call
to a library function result in only one call to the runtime-constraint
handler. The first violation encountered invokes the runtime-constraint
handler.

The runtime-constraint handler might not return. If the handler does
return, the library function whose runtime-constraint was violated
returns an indication of failure as given by the function’s return.


rsize_t

The specification defines a new type.  This type, rsize_t, is conditionally
defined in the safe_lib.h header file.


RSIZE_MAX Macro

The specification defines the macro RSIZE_MAX which expands to a value
of type rsize_t. The specification uses RSIZE_MAX for both the string
functions and the memory functions. This implementation defines two
macros: RSIZE_MAX_STR and RSIZE_MAX_MEM.  RSIZE_MAX_STR defines the
range limit for the safe string functions.  RSIZE_MAX_MEM defines the
range limit for the safe memory functions.  The point is that string
limits can and should be different from memory limits.


Configurations

Optional features and system dependent features can be configured by
macros in the Safe C library header files.  These can be found by
a grep of the files:  grep CONFIGURE *.h.


Header Files

The specification states the variopus functions would be added to
existing Standard C header files: stdio.h, string.h, etc.  This
implementation separates the memory related functions into the
safe_mem_lib.h header and the string related functions into the
safe_str_lib.h header.  The make file builds a single library
safe_lib.a in the ../lib directory.

It is possible to split the make such that a separate safe_mem_lib.a
and safe_str_lib.a are built.  It is also possible to integrate the
prototypes into the Standard C header files, but that may require
changes to your development tool chain.


References

[1] Programming languages, their environments and system software
    interfaces, Extensions to the C Library, Part I: Bounds-checking
    interfaces, ISO/IEC TR 24731-1.

[2] Rationale for TR 24731 Extensions to the C Library Part I:
    Bounds-checking interfaces, ISO/IEC JTC1 SC22 WG14 N1225.

[3] The Open Group Base Specifications Issue 7
    http://pubs.opengroup.org/onlinepubs/9699919799/functions/contents.html

[4] CERT C Secure Coding Standard
    https://www.securecoding.cert.org/confluence/display/seccode/CERT+C+Secure+Coding+Standard
