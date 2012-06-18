/*------------------------------------------------------------------
 * runtime_environment.c -- Misc functions
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
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

#include <stdio.h>
#include <stdlib.h>

#include "safe_types.h"
#include "runtime_environment.h"


/* simple stub */ 
static uint32_t interrupt_sema_count = 0;


/**
 * NAME
 *    lock_interrupts
 *
 * DESCRIPTION
 *    Abstract the interrupt lock.
 *
 * INPUT PARAMETERS
 *    context - a void pointer 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURNS
 *    none
 *
 */
void
lock_interrupts (void *context)
{
    interrupt_sema_count++;
    return;
}


/**
 * NAME
 *    unlock_interrupts
 *
 * DESCRIPTION
 *    Abstract the interrupt lock.
 *
 * INPUT PARAMETERS
 *    context - a void pointer 
 *
 * OUTPUT PARAMETERS
 *    none
 * 
 * RETURNS
 *    none
 *
 */
void
unlock_interrupts(void *context)
{
    interrupt_sema_count--;
    return;
}


