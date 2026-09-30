/*------------------------------------------------------------------
 * test_vu8scanf_s
 * File 'extu8/vu8scanf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <stdarg.h>
#include <unistd.h>

#define LEN (128)
#define TMP "tmpvu8scanfstdin"

static char str2[LEN];
static FILE *stream = NULL;
void stuff_vu8stdin(const char *dest);
int vtu8scanf_s(const char *restrict fmt, ...);
int test_vu8scanf_s(void);

void stuff_vu8stdin(const char *dest) {
    stream = fopen(TMP, "w+");
    fprintf(stream, "%s\n", dest);
    fclose(stream);
    stream = freopen(TMP, "r", stdin);
}

int vtu8scanf_s(const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vu8scanf_s(fmt, ap);
    va_end(ap);
    return rc;
}

int test_vu8scanf_s(void) {
    errno_t rc;
    int i1;
    int errs = 0;

    /*--------------------------------------------------*/

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtu8scanf_s(NULL, NULL);
    GCC_DIAG_RESTORE
    ERREOF(ESNULLP);

    /*--------------------------------------------------*/

    stuff_vu8stdin("      24");
    rc = vtu8scanf_s(" %d", &i1);
    if (rc != 1) {
        printf("flapping tests - abort\n");
        unlink(TMP);
        return errs;
    }
    ERR(1);
    ERRNO(0);
    if (i1 != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, i1);
        errs++;
    }

    /*--------------------------------------------------*/

    stuff_vu8stdin("caf\xC3\xA9");
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = vtu8scanf_s("%s", str2, LEN);
    GCC_DIAG_RESTORE
    if (rc != 1) {
        printf("flapping tests - abort\n");
        unlink(TMP);
        return errs;
    }
    ERR(1);
    EXPSTR(str2, "caf\xC3\xA9")

    /*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vu8scanf_s()); }
