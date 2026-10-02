/*------------------------------------------------------------------
 * test_u8ncpy_s
 * File 'extu8/u8ncpy_s.c'
 * Lines executed:57.47% of 87
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_u8_lib.h"

#define LEN (128)

static char8_t dest[LEN];
int test_u8ncpy_s(void);

int test_u8ncpy_s(void) {
    errno_t rc;
    int errs = 0;

    /*--------------------------------------------------*/
    /* basic copy */

    rc = u8ncpy_s(dest, LEN, (char8_t *)"hello", 5);
    ERR(EOK)
    EXPSTR((char *)dest, "hello")

    /*--------------------------------------------------*/
    /* valid multi-byte UTF-8 is copied untouched */

    {
        char8_t src[] = "h\xC3\xA9llo";
        rc = u8ncpy_s(dest, LEN, src, sizeof(src) - 1);
        ERR(EOK)
        EXPSTR((char *)dest, "h\xC3\xA9llo")
    }

    /*--------------------------------------------------*/
    /* overlong 2-byte encoding of NUL (0xC0 0x80) is illegal UTF-8 */

    {
        char8_t src[] = {(char8_t)0xC0, (char8_t)0x80, 0};
        rc = u8ncpy_s(dest, LEN, src, 2);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* encoded surrogate half (0xED 0xA0 0x80 = U+D800) is illegal */

    {
        char8_t src[] = {(char8_t)0xED, (char8_t)0xA0, (char8_t)0x80, 0};
        rc = u8ncpy_s(dest, LEN, src, 3);
        ERR(EILSEQ)
    }

    /*--------------------------------------------------*/
    /* slen truncation must stop at a character boundary, not split a
       multi-byte sequence: "a" + e-acute (0xC3 0xA9) + "b", slen=2
       cannot fit the 2-byte e-acute after 'a', so only "a" is copied,
       never a dangling lead byte 0xC3. */

    {
        char8_t src[] = "a\xC3\xA9" "b";
        memset(dest, 'X', sizeof(dest));
        rc = u8ncpy_s(dest, LEN, src, 2);
        ERR(EOK)
        EXPSTR((char *)dest, "a")
        if ((unsigned char)dest[1] != 0) {
            debug_printf("%s %u  dangling byte after truncation: %02X\n",
                         __FUNCTION__, __LINE__, (unsigned char)dest[1]);
            errs++;
        }
    }

    /*--------------------------------------------------*/
    /* slen exactly fits the multi-byte character */

    {
        char8_t src[] = "a\xC3\xA9" "b";
        rc = u8ncpy_s(dest, LEN, src, 3);
        ERR(EOK)
        EXPSTR((char *)dest, "a\xC3\xA9")
    }

    /*--------------------------------------------------*/

    return (errs);
}

int main(void) { return (test_u8ncpy_s()); }
