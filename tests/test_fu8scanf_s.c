/*------------------------------------------------------------------
 * test_fu8scanf_s
 * File 'extu8/fu8scanf_s.c'
 * Lines executed:100.00% of 5
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"
#include <unistd.h>

#define LEN (128)
#define TMP "tmpu8scanf"

static char str2[LEN];
static FILE *stream = NULL;
void stuff_u8stream(const char *dest);
int test_fu8scanf_s(void);

void stuff_u8stream(const char *dest) {
    if (!stream)
        stream = fopen(TMP, "w+");
    else
        rewind(stream);
    fprintf(stream, "%s\n", dest);
    rewind(stream);
}

int test_fu8scanf_s(void) {
    errno_t rc;
    int i1;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = fu8scanf_s(NULL, "%s", str2);
    ERREOF(ESNULLP);
#endif

    stuff_u8stream("1");
#ifndef HAVE_CT_BOS_OVR
    rc = fu8scanf_s(stream, NULL);
    ERREOF(ESNULLP);
#endif

    /*--------------------------------------------------*/

    stuff_u8stream("      24");
    rc = fu8scanf_s(stream, " %d", &i1);
    ERR(1);
    ERRNO(0);
    if (i1 != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, i1);
        errs++;
    }

    /*--------------------------------------------------*/

    stuff_u8stream("caf\xC3\xA9");
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = fu8scanf_s(stream, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1);
    EXPSTR(str2, "caf\xC3\xA9")

    /*--------------------------------------------------*/

    fclose(stream);
    unlink(TMP);

    return (errs);
}

int main(void) { return (test_fu8scanf_s()); }
