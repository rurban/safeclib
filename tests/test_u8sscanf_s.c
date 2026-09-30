/*------------------------------------------------------------------
 * test_u8sscanf_s
 * File 'extu8/u8sscanf_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t str1[LEN];
static char str2[LEN];
int test_u8sscanf_s(void);

int test_u8sscanf_s(void) {
    errno_t rc;
    int num = 0;
    int errs = 0;

    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = u8sscanf_s(str1, NULL, NULL);
    GCC_DIAG_RESTORE
    ERREOF(ESNULLP);

    str2[0] = '\0';
    rc = u8sscanf_s(NULL, "%s", str2);
    ERREOF(ESNULLP);

    /*--------------------------------------------------*/

    strcpy((char *)str1, "      24");
    rc = u8sscanf_s(str1, " %d", &num);
    ERR(1);
    ERRNO(0);
    if (num != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__, num);
        errs++;
    }

    /*--------------------------------------------------*/

    /* multi-byte utf-8 content is byte-compatible with %s */
    strcpy((char *)str1, "caf\xC3\xA9 42");
    GCC_DIAG_IGNORE(-Wformat-extra-args)
    rc = u8sscanf_s(str1, "%s", str2, LEN);
    GCC_DIAG_RESTORE
    ERR(1)
    EXPSTR(str2, "caf\xC3\xA9")

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8sscanf_s()); }
