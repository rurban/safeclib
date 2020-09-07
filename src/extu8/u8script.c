/*------------------------------------------------------------------
 * u8script.c
 *
 * September 2020, Reini Urban (WIP)
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

#ifdef FOR_DOXYGEN
#include "safe_u8_lib.h"
#else
#include "safeclib_private.h"
#include <assert.h>
#endif

#include "u8scripts.h"

// TODO static hash of scripts in u8i string

// return the const script ptr of the codepoint
const char* _u8_get_script(uint32_t cp)
{
  // binary search over _u8_script_ranges[]
  return "";
}

// resets the current set of scripts of each u8i string
int _u8_init_script(void)
{
  return 0;
}

// check if the codepoint fits to the current set of scripts
int _u8_chk_script(char8_t *restrict str, rsize_t len)
{
  return 0;
}

// throw the invalid mixed string error
int _u8_err_script(char8_t *restrict str, const char *script, uint32_t cp)
{
  return 0;
}

// check if the string needs to be normalized or not.
// it needs if any decomposed sequence is found.
int _u8_is_decomposed(char8_t *restrict str, rsize_t len)
{
  return 0;
}
