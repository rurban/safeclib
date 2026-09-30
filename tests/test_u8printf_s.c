/*------------------------------------------------------------------
 * test_u8printf_s
 * File 'extu8/u8printf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <unistd.h>

#define LEN (128)

static char str1[LEN];
int test_u8printf_s(void);

int test_u8printf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;

    /*--------------------------------------------------*/

    rc = u8printf_s(NULL);
    NEGERR(ESNULLP);

    /*--------------------------------------------------*/

    GCC_DIAG_IGNORE(-Wformat-zero-length)
    rc = u8printf_s("");
    GCC_DIAG_RESTORE
    NEGERR(EOK)

    /*--------------------------------------------------*/

    str1[0] = '\0';
    GCC_DIAG_IGNORE(-Wformat)
    rc = u8printf_s("%s%n\n", str1, &ind);
    GCC_DIAG_RESTORE
    NEGERR(EINVAL)

    /*--------------------------------------------------*/

    rc = u8printf_s("caf\xC3\xA9\n");
    ERR(6)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8printf_s()); }
