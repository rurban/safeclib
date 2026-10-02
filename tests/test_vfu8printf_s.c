/*------------------------------------------------------------------
 * test_vfu8printf_s
 * File 'extu8/vfu8printf_s.c'
 * Lines executed:100.00% of 2
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <stdarg.h>
#include <unistd.h>
#if defined(HAVE_FTRUNCATE) && defined(HAVE_FILENO)
#include <sys/stat.h>
#endif

#define TMP "tmpvu8fp"
#define LEN (128)

static FILE *out;
static char str[LEN];
int errs = 0;
int vtfu8printf_s(FILE *restrict stream, const char *restrict fmt, ...);
int test_vfu8printf_s(void);

int vtfu8printf_s(FILE *restrict stream, const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vfu8printf_s(stream, fmt, ap);
    va_end(ap);
    return rc;
}

static size_t u8_cmp_and_reset(FILE *out, const char *s) {
    size_t nread;
    size_t pos = ftell(out);
    fflush(out);
    *str = '\0';
    rewind(out);
    nread = fread(str, 1, pos, out);
    EXPSTR(str, s)
    rewind(out);
#if defined(HAVE_FTRUNCATE) && defined(HAVE_FILENO)
    nread = ftruncate(fileno(out), 0L);
#endif
    (void)nread;
    return pos;
}

int test_vfu8printf_s(void) {
    errno_t rc;
    int ind;

    out = fopen(TMP, "w+");

    /*--------------------------------------------------*/

    rc = vtfu8printf_s(NULL, "%s", (char *)NULL);
    NEGERR(ESNULLP);

    rc = vtfu8printf_s(out, NULL);
    NEGERR(ESNULLP);

    /*--------------------------------------------------*/

    str[0] = '\0';
    rc = vtfu8printf_s(out, "%s %n", str, &ind);
    NEGERR(EINVAL);

    /*--------------------------------------------------*/

    strcpy(str, "caf\xC3\xA9");
    rc = vtfu8printf_s(out, "%s", str);
    ERR(strlen(str))
    u8_cmp_and_reset(out, "caf\xC3\xA9");

    /*--------------------------------------------------*/

    fclose(out);
    unlink(TMP);

    return (errs);
}

int main(void) { return (test_vfu8printf_s()); }
