/*------------------------------------------------------------------
 * test_vu8sscanf_s
 * File 'extu8/vu8sscanf_s.c'
 * Lines executed:85.71% of 28
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <stdarg.h>

#define LEN (128)

static char8_t str1[LEN];
static char str2[LEN];
int vtu8sscanf_s(const char8_t *restrict dest, const char *restrict fmt, ...);
int test_vu8sscanf_s(void);

int vtu8sscanf_s(const char8_t *restrict dest, const char *restrict fmt,
                 ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vu8sscanf_s(dest, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vu8sscanf_s(void) {
    errno_t rc;
    int num = 0;
    int errs = 0;

    /*--------------------------------------------------*/

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtu8sscanf_s(str1, NULL, NULL);
    GCC_DIAG_RESTORE
    ERREOF(ESNULLP);

    str2[0] = '\0';
    rc = vtu8sscanf_s(NULL, "%s", str2);
    ERREOF(ESNULLP);

    /*--------------------------------------------------*/

    strcpy((char *)str1, "      24");
    rc = vtu8sscanf_s(str1, " %d", &num);
    ERR(1);
    ERRNO(0);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy((char *)str1, "caf\xC3\xA9 42");
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtu8sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1)
    EXPSTR(str2, "caf\xC3\xA9")

    /*--------------------------------------------------*/

    /* malformed UTF-8 must be rejected, not silently scanned */
    strcpy((char *)str1, "\x80zzz");
    rc = vtu8sscanf_s(str1, " %d", &num);
    ERREOF(EILSEQ);

    strcpy((char *)str1, "abc\xC3");
    rc = vtu8sscanf_s(str1, " %d", &num);
    ERREOF(EILSEQ);

    strcpy((char *)str1, "\xC0\xAF");
    rc = vtu8sscanf_s(str1, " %d", &num);
    ERREOF(EILSEQ);

    /*--------------------------------------------------*/

    /* %s followed by another real conversion used to misconsume the
     * orphaned destination-size argument as that conversion's
     * destination pointer and segfault; must now scan both safely. */
    strcpy((char *)str1, "caf 42");
    rc = vtu8sscanf_s(str1, "%s %d", str2, LEN, &num);
    ERR(2);
    ERRNO(0);
    EXPSTR(str2, "caf");
    if (num != 42) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /* a destination too small for the input must be rejected, not
     * overflowed */
    strcpy((char *)str1, "toolong");
    rc = vtu8sscanf_s(str1, "%s", str2, (rsize_t)2);
    ERREOF(ESNOSPC);

    /* a zero destination size is a constraint violation, not UB */
    strcpy((char *)str1, "x");
    rc = vtu8sscanf_s(str1, "%s", str2, (rsize_t)0);
    ERREOF(ESZEROL);

    /* %c with an exact 1-element destination still works */
    strcpy((char *)str1, "Z");
    {
        char c1 = 0;
        rc = vtu8sscanf_s(str1, "%c", &c1, (rsize_t)1);
        ERR(1);
        ERRNO(0);
        if (c1 != 'Z') {
            debug_printf("%s %u wrong arg: %c\n", __FUNCTION__, __LINE__, c1);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_vu8sscanf_s()); }
