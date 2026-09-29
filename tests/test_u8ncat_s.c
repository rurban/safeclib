/*------------------------------------------------------------------
 * test_u8ncat_s
 * File 'extu8/u8ncat_s.c'
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t dest[LEN];
int test_u8ncat_s(void);

int test_u8ncat_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/
    /* basic append, slen bigger than src's actual content but not
       bigger than its known object size */

    strcpy((char *)dest, "hello ");
    rc = u8ncat_s(dest, LEN, (char8_t *)"world", 6);
    ERR(EOK)
    EXPSTR((char *)dest, "hello world")

    /*--------------------------------------------------*/
    /* slen truncates the source before its NUL */

    strcpy((char *)dest, "hello ");
    rc = u8ncat_s(dest, LEN, (char8_t *)"world!!!", 5);
    ERR(EOK)
    EXPSTR((char *)dest, "hello world")

    /*--------------------------------------------------*/
    /* slen truncation must stop at a UTF-8 character boundary, not
       split a multi-byte sequence: "a" + e-acute (0xC3 0xA9) + "b" */

    {
        char8_t src[] = "a\xC3\xA9"
                        "b";
        strcpy((char *)dest, "x");
        rc = u8ncat_s(dest, LEN, src, 2);
        ERR(EOK)
        EXPSTR((char *)dest, "xa")
        if ((unsigned char)dest[2] != 0) {
            debug_printf("%s %u  dangling byte after truncation: %02X\n",
                         __FUNCTION__, __LINE__, (unsigned char)dest[2]);
            errs++;
        }
    }

    /*--------------------------------------------------*/
    /* illegal UTF-8 (overlong encoding of NUL) */

    {
        char8_t src[] = {(char8_t)0xC0, (char8_t)0x80, 0};
        strcpy((char *)dest, "x");
        rc = u8ncat_s(dest, LEN, src, 2);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* dest not big enough */

    strcpy((char *)dest, "hello");
    rc = u8ncat_s(dest, 7, (char8_t *)"world", 6);
    ERR(ESNOSPC)

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8ncat_s()); }
