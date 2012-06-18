/*------------------------------------------------------------------
 * safe_limits.h
 *
 * February 2009, Bo Berry
 *
 * Copyright (c) 2009-2011 by Cisco Systems 
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

#ifndef __SAFE_LIMITS__
#define __SAFE_LIMITS__

/*
 * Abstract header file for portability. 
 */ 


/*
 * ANSI C - number of bits by type  
 */
#ifndef CHAR_BIT
#define CHAR_BIT        ( 8 )
#endif

#ifndef BYTE_BIT
#define BYTE_BIT        ( 8 )
#endif

#ifndef SHORT_BIT
#define SHORT_BIT       ( 16 )
#endif

#ifndef INT_BIT
#define INT_BIT         ( 32 )
#endif

#ifndef LONG_BIT
// #define LONG_BIT        ( 32 )
#define LONG_BIT        ( 64 )
#endif

#ifndef LONG_LONG_BIT
#define LONG_LONG_BIT   ( 64 )
#endif

#ifndef POINTER_BIT
#define POINTER_BIT     ( 64 )
#endif



#ifndef BYTE_MAX 
#define BYTE_MAX        ( 255 )
#endif

#ifndef SCHAR_MAX  
#define SCHAR_MAX       (  127 )
#define SCHAR_MIN       ( -SCHAR_MAX - 1 )
#endif

#ifndef CHAR_MAX  
#define CHAR_MAX        ( 127 )
#define CHAR_MIN        ( -CHAR_MAX - 1 )
#endif

#ifndef UCHAR_MAX 
#define UCHAR_MAX       ( 0xff )
#endif

#ifndef SHRT_MAX
#define SHRT_MAX        (  32767 )
#define SHRT_MIN        ( -SHRT_MAX - 1 )
#endif

#ifndef USHRT_MAX    
#define USHRT_MAX       ( 0xffff )
#endif
 
#ifndef INT_MAX   
#define INT_MAX         (  2147483647 )
#define INT_MIN         ( -INT_MAX - 1 )
#endif

#ifndef UINT_MAX    
#define UINT_MAX        ( 0xffffffff )
#endif

#ifndef LLONG_MAX 
#define LLONG_MAX       (  9223372036854775807 )
#define LLONG_MIN       ( -LLONG_MAX - 1 )
#endif

#ifndef ULLONG_MAX  
#define ULLONG_MAX      ( 0xffffffffffffffff )
#endif


#if LONG_BIT == INT_BIT 

#ifndef LONG_MAX  
#define LONG_MAX        (  INT_MAX)
#define LONG_MIN        ( -LONG_MAX - 1 )
#endif

#ifndef ULONG_MAX
#define ULONG_MAX       ( UINT_MAX )
#endif

#else

#ifndef LONG_MAX
#define LONG_MAX        (  LLONG_MAX)
#define LONG_MIN        ( -LONG_MAX - 1 )
#endif

#ifndef ULONG_MAX
#define ULONG_MAX       ( ULLONG_MAX )
#endif

#endif


#ifndef INT8_MIN  
#define INT8_MIN        ( SCHAR_MIN )
#endif

#ifndef INT8_MAX 
#define INT8_MAX        ( SCHAR_MAX )
#endif

#ifndef UINT8_MAX 
#define UINT8_MAX       ( UCHAR_MAX )
#endif

#ifndef INT16_MIN  
#define INT16_MIN       ( SHRT_MIN )
#endif

#ifndef INT16_MAX 
#define INT16_MAX       ( SHRT_MAX )
#endif

#ifndef UINT16_MAX 
#define UINT16_MAX      ( USHRT_MAX )
#endif

#ifndef INT32_MIN  
#define INT32_MIN       ( INT_MIN )
#endif

#ifndef INT32_MAX    
#define INT32_MAX       ( INT_MAX )
#endif

#ifndef UINT32_MAX    
#define UINT32_MAX      ( UINT_MAX )
#endif

#ifndef INT64_MIN      
#define INT64_MIN       ( LLONG_MIN )
#endif 

#ifndef INT64_MAX    
#define INT64_MAX       ( LLONG_MAX )
#endif 

#ifndef UINT64_MAX    
#define UINT64_MAX      ( ULLONG_MAX )
#endif 


#endif /* __SAFE_LIMITS__ */

