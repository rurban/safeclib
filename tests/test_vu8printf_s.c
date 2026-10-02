/*------------------------------------------------------------------
 * test_vu8printf_s
 * File 'extu8/vu8printf_s.c'
 * Lines executed:100.00% of 2
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <stdarg.h>
#include <unistd.h>

#define LEN (128)

static char str1[LEN];
int vtu8printf_s(const char *restrict fmt, ...);
int test_vu8printf_s(void);

int vtu8printf_s(const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vu8printf_s(fmt, ap);
    va_end(ap);
    return rc;
}

int test_vu8printf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;

    /*--------------------------------------------------*/

    rc = vtu8printf_s(NULL, NULL);
    NEGERR(ESNULLP)

    /*--------------------------------------------------*/

    rc = vtu8printf_s("");
    NEGERR(EOK)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    rc = vtu8printf_s("%s%n\n", str1, &ind);
    NEGERR(EINVAL)

    /*--------------------------------------------------*/

    rc = vtu8printf_s("caf\xC3\xA9\n");
    ERR(6)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vu8printf_s()); }
