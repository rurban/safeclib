/*------------------------------------------------------------------
 * safe_types.h
 *
 * March 2007, Bo Berry
 *
 * Copyright (c) 2007-2011 by Cisco Systems, Inc
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

#ifndef __SAFE_TYPES__
#define __SAFE_TYPES__

#include <stdlib.h>

#include "safe_limits.h"


/*
 * Abstract header file for portability. 
 */ 

#ifndef TRUE
#define TRUE   ( 1 )  
#endif

#ifndef FALSE
#define FALSE  ( 0 )  
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef boolean_t
typedef unsigned char boolean_t;
#endif



#ifndef int8_t
typedef signed char int8_t;
#endif

#ifndef int16_t
typedef short int16_t;
#endif

#ifndef int32_t
typedef int int32_t;
#endif

#ifndef uchar_t
typedef unsigned char uchar_t;
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#ifndef uint64_t
typedef unsigned long long uint64_t;
#endif

#if 0 
#if POINTER_BIT == 64 
#ifndef intptr_t; 
typedef long long intptr_t; 
#endif

#ifndef uintptr_t; 
typedef unsigned long long uintptr_t; 
#endif

#else 

#ifndef intptr_t; 
typedef signed int intptr_t; 
#endif
#ifndef uintptr_t; 
typedef unsigned int uintptr_t; 
#endif

#endif
#endif


#ifndef ushort
typedef unsigned short ushort;
#endif

#ifndef int_t
typedef int int_t;
#endif

#ifndef uint_t
typedef unsigned int uint_t;
#endif

#ifndef ulong
typedef unsigned long ulong;
#endif

#ifndef ulonglong
typedef unsigned long long ullong;
#endif


#endif /* __SAFE_TYPES__ */

