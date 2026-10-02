/*------------------------------------------------------------------
 * test_vu8snprintf_s
 * File 'extu8/vu8snprintf_s.c'
 * Lines executed:100.00% of 2
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <stdarg.h>

#define LEN (128)

static char8_t str1[LEN];
int vtu8nprintf_s(char8_t *restrict dest, rsize_t dmax,
                 const char *restrict fmt, ...);
int test_vu8snprintf_s(void);

int vtu8nprintf_s(char8_t *restrict dest, rsize_t dmax,
                  const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vu8snprintf_s(dest, dmax, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vu8snprintf_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/

    rc = vtu8nprintf_s(NULL, LEN, "%s", "x");
    NEGERR(ESNULLP)

    rc = vtu8nprintf_s(str1, LEN, NULL);
    NEGERR(ESNULLP)

    rc = vtu8nprintf_s(str1, 0, "%s", "x");
    NEGERR(ESZEROL)

    /*--------------------------------------------------*/

    rc = vtu8nprintf_s(str1, LEN, "caf\xC3\xA9 %d", 42);
    ERR(8)
    EXPSTR((char *)str1, "caf\xC3\xA9 42")

    /*--------------------------------------------------*/

    strcpy((char *)str1, "aaaaaaaaaa");
    rc = vtu8nprintf_s(str1, 5, "%s", "keep it simple");
    ERR(-ESNOSPC);
    EXPSTR((char *)str1, "")

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vu8snprintf_s()); }
