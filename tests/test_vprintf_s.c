/*------------------------------------------------------------------
 * test_vprintf_s
 * File 'vprintf_s.c'
 * Lines executed:80.00% of 15
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
int vtprintf_s(const char *restrict fmt, ...);
int test_vprintf_s(void);

int vtprintf_s(const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vprintf_s(fmt, ap);
    va_end(ap);
    return rc;
}

int test_vprintf_s(void) {
    errno_t rc;
    int32_t ind;
    int errs = 0;

    /*--------------------------------------------------*/

    rc = vtprintf_s(NULL, NULL);
    NEGERR(ESNULLP)

    /*--------------------------------------------------*/

    rc = vtprintf_s("");
    NEGERR(EOK)

    /* TODO
    rc = vtprintf_s("%s", NULL);
    NEGERR(ESNULLP)
    */

    /*--------------------------------------------------*/

    str1[0] = '\0';
    rc = vtprintf_s("%s%n\n", str1, &ind);
    NEGERR(EINVAL)

    /*--------------------------------------------------*/

    rc = vtprintf_s("%s%%n\n", str1);
    if (rc < 0) {
        printf("Failed to open stdout for write: %s\n", strerror(errno));
        return errs;
    }
    ERR(3)

    rc = vtprintf_s("%%n\n");
    ERR(3);

    /*--------------------------------------------------*/

    strcpy(str1, "12");
    strcpy(str2, "34");

    rc = vtprintf_s("%s%s", str1, str2);
    ERR(4)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vprintf_s()); }
