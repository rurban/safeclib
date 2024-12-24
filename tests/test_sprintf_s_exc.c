/*------------------------------------------------------------------
 * test_sprintf_s_exc
 * File 'str/sprintf_s_exc.c'
 * Lines executed:100.00% of 5
 *
 *------------------------------------------------------------------
 */

#if defined(HAVE_FENV_H) && defined(HAVE_FEENABLEEXCEPT) && defined(HAVE_XMMINTRIN_H) \
    && defined(__SSE__)

#define _GNU_SOURCE
#include <fenv.h>
#ifdef __SSE__
#include <xmmintrin.h>
#endif

#include "test_private.h"
#include "safe_str_lib.h"
#if defined(TEST_MSVCRT) && defined(HAVE_SPRINTF_S)
#ifdef SAFECLIB_HAVE_C99
#undef sprintf_s
EXTERN int sprintf_s(char *restrict dest, rsize_t dmax,
                     const char *restrict fmt, ...);
#endif
#endif
#include "test_msvcrt.h"

int test_sprintf_s_exc(void);

int test_sprintf_s_exc(void) {
    int errs = 0;

    /*--------------------------------------------------*/

    {
        /* div-by-zero bug GH #144 */
        static char s[200];
        sprintf_s(s, 200, "  %e", 0.0);

        feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#ifdef __SSE__
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON); 
#endif
        sprintf_s(s, 200, "  %e", 0.0);
    }
        
    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_sprintf_s_exc()); }
#endif

#else // HAVE_FENV_H && HAVE_FEENABLEEXCEPT && HAVE_XMMINTRIN_H && __SSE__

#include <stdio.h>
int main(void) {
    puts("test_sprintf_s_exc ignored");
    return 0;
}

#endif
