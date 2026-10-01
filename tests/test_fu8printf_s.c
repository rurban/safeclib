/*------------------------------------------------------------------
 * test_fu8printf_s
 * File 'extu8/fu8printf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <unistd.h>
#if defined(HAVE_FTRUNCATE) && defined(HAVE_FILENO)
#include <sys/stat.h>
#endif

#define TMP "tmpu8fp"
#define LEN (128)

static FILE *out;
static char str[LEN];
int errs = 0;
int test_fu8printf_s(void);

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

int test_fu8printf_s(void) {
    errno_t rc;
    int ind;

    out = fopen(TMP, "w+");

    /*--------------------------------------------------*/

    rc = fu8printf_s(NULL, "%s", (char *)NULL);
    NEGERR(ESNULLP);

#ifndef HAVE_CT_BOS_OVR
    rc = fu8printf_s(out, NULL);
    NEGERR(ESNULLP);
#endif

    /*--------------------------------------------------*/

    str[0] = '\0';
    rc = fu8printf_s(out, "%s %n", str, &ind);
    NEGERR(EINVAL);

    /*--------------------------------------------------*/

    /* multi-byte utf-8 payload writes like any other byte data */
    strcpy(str, "caf\xC3\xA9");
    rc = fu8printf_s(out, "%s", str);
    ERR(strlen(str))
    u8_cmp_and_reset(out, "caf\xC3\xA9");

    /*--------------------------------------------------*/

    fclose(out);
    unlink(TMP);

    return (errs);
}

int main(void) { return (test_fu8printf_s()); }
