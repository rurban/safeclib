/*------------------------------------------------------------------
 * test_vfu8scanf_s
 * File 'extu8/vfu8scanf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <stdarg.h>
#include <unistd.h>

#define LEN (128)
#define TMP "tmpvu8scanf"

static char str2[LEN];
static FILE *stream = NULL;
void stuff_vu8stream(const char *dest);
int vtfu8scanf_s(FILE *restrict f, const char *restrict fmt, ...);
int test_vfu8scanf_s(void);

void stuff_vu8stream(const char *dest) {
    if (!stream)
        stream = fopen(TMP, "w+");
    else
        rewind(stream);
    fprintf(stream, "%s\n", dest);
    rewind(stream);
}

int vtfu8scanf_s(FILE *restrict f, const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vfu8scanf_s(f, fmt, ap);
    va_end(ap);
    return rc;
}

int test_vfu8scanf_s(void) {
    errno_t rc;
    int i1;
    int errs = 0;

    /*--------------------------------------------------*/

    rc = vtfu8scanf_s(NULL, "%s", str2);
    ERREOF(ESNULLP);

    stuff_vu8stream("1");
    rc = vtfu8scanf_s(stream, NULL);
    ERREOF(ESNULLP);

    /*--------------------------------------------------*/

    stuff_vu8stream("      24");
    rc = vtfu8scanf_s(stream, " %d", &i1);
    ERR(1);
    ERRNO(0);
    if (i1 != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, i1);
        errs++;
    }

    /*--------------------------------------------------*/

    fclose(stream);
    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vfu8scanf_s()); }
