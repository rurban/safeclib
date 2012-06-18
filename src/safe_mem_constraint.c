/*------------------------------------------------------------------
 * safe_mem_constraint.c
 *
 * October 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 Cisco Systems
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

#include "safe_lib.h"
#include "safe_mem_constraint.h"


/*
 * CONFIGURE: This file is conditionally compiled for the platform specific
 * constraint handler.  
 */ 


/** 
 * NAME
 *    invoke_safe_mem_constraint_handler
 *
 * SYNOPSIS 
 *    #include "safe_mem_constraint.h" 
 *    void 
 *    invoke_safe_mem_constraint_handler(const char *msg,
 *                                void *ptr, 
 *                                errno_t error)
 * 
 * DESCRIPTION
 *    This handler simply prints the constraint to stderr. The platform
 *    could dictate that this be a syslog, abort or otherwise.
 *    The platform constraint could also take different actions
 *    based upon the error code.
 *
 * INPUT PARAMETERS
 *   *msg            Pointer to the message describing the error 
 *
 *   *ptr            Pointer to aassociated data.  Can be NULL. 
 *
 *    error          The error code encountered. 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void 
invoke_safe_mem_constraint_handler (const char *msg,
                                 void *ptr, 
                                 errno_t error)
{
#if defined YOUR_OS 
/* 
 * CONFIGURE: Create your platform specific constraint handler. 
 */ 
    if (msg) {
        fprintf(stderr, 
                "%%SAFE_MEM CONSTRAINT: %s, error code=%u \n",
                msg, error);
    } else {
        fprintf(stderr, 
                "%%SAFE_MEM CONSTRAINT: Null message, error code=%u \n",
                error);  
    } 

#elif defined MAC_OS_X 
    /* 
     * For Mac testing, write the constraint to stderr. 
     */ 
    if (msg) {
        fprintf(stderr, 
                "%%SAFE_MEM CONSTRAINT: %s, error code=%u \n",
                msg, error);
    } else {
        fprintf(stderr, 
                "%%SAFE_MEM CONSTRAINT: Null message, error code=%u \n",
                error);
    } 

#else
#error Safe mem lib environment not defined. 
#endif 
    return;
}


