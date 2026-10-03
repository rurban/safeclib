/*------------------------------------------------------------------
 * test_minimal.c
 * Freestanding smoke test for the cmake ENABLE_MINIMAL build, e.g. sdcc
 * stm8: no stdio, the result is the exit status, 0 on success.
 *------------------------------------------------------------------
 */

#include "safe_mem_lib.h"
#include "safe_str_lib.h"
#ifdef SAFECLIB_ENABLE_U8
#include "safe_u8_lib.h"
#endif

static char dest[16];
static char src[16] = "hello";
/* for simulators, which cannot see the exit status: 0x5a + errs */
volatile unsigned char test_result;

int main(void) {
    int errs = 0;

    set_str_constraint_handler_s(ignore_handler_s);
    set_mem_constraint_handler_s(ignore_handler_s);
    if (memcpy_s(dest, sizeof(dest), src, 6) != EOK || dest[4] != 'o')
        errs++;
    if (memset_s(dest, sizeof(dest), 0, sizeof(dest)) != EOK || dest[0])
        errs++;
    if (strcpy_s(dest, sizeof(dest), src) != EOK || dest[4] != 'o')
        errs++;
    if (strcat_s(dest, sizeof(dest), src) != EOK || dest[9] != 'o')
        errs++;
    if (strnlen_s(dest, sizeof(dest)) != 10)
        errs++;
    /* constraint violation */
    if (strcpy_s(dest, 4, src) == EOK)
        errs++;
#ifdef SAFECLIB_ENABLE_U8
    if (u8cpy_s((char8_t *)dest, sizeof(dest), (const char8_t *)src) != EOK ||
        u8nlen_s((const char8_t *)dest, sizeof(dest)) != 5)
        errs++;
#endif
    test_result = 0x5a + errs;
    return errs;
}
