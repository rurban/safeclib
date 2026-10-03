/*------------------------------------------------------------------
 * test_minimal.c
 * Freestanding smoke test for the cmake ENABLE_MINIMAL build, e.g. sdcc
 * stm8 or avr-gcc: no stdio, the result is the exit status, 0 on success.
 *------------------------------------------------------------------
 */

#include <string.h>

#include "safe_mem_lib.h"
#include "safe_str_lib.h"
#ifdef SAFECLIB_ENABLE_U8
#include "safe_u8_lib.h"
#endif

static char dest[16];
static char src[16] = "hello";
/* for simulators, which cannot see the exit status: 0x5a + errs.
   the marker is written last, so the simulator can watch for it. */
volatile unsigned char test_result;
volatile unsigned char test_marker;
#ifdef __AVR__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/avr_mcu_section.h>
/* simavr's documented debug-console convention: bytes written to the
   designated register (any otherwise-unused one, GPIOR0 here) are
   printed to simavr's stdout as-is, no command-line flags needed. */
AVR_MCU(1000000, "atmega2560");
AVR_MCU_SIMAVR_CONSOLE(&GPIOR0);
#endif

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
    /* string printf and scanf */
    {
        int num = 0;
        int rc;
        memset(dest, 0, sizeof(dest));
        rc = sprintf_s(dest, sizeof(dest), "%s %d", "n=", 42);
        if (rc != 5 || dest[4] != '2' || dest[0] != 'n')
            errs++;
        rc = sscanf_s("77", "%d", &num);
        if (rc != 1 || num != 77)
            errs++;
    }
#ifdef SAFECLIB_ENABLE_U8
    if (u8cpy_s((char8_t *)dest, sizeof(dest), (const char8_t *)src) != EOK ||
        u8nlen_s((const char8_t *)dest, sizeof(dest)) != 5)
        errs++;
    {
        int num = 0;
        int rc;
        memset(dest, 0, sizeof(dest));
        rc = u8sprintf_s((char8_t *)dest, sizeof(dest), "%s %d", "n=", 7);
        if (rc != 4 || dest[3] != '7')
            errs++;
        rc = u8sscanf_s((const char8_t *)"55", "%d", &num);
        if (rc != 1 || num != 55)
            errs++;
    }
#endif
    test_result = 0x5a + errs;
    test_marker = 0xc3;
#ifdef __AVR__
    GPIOR0 = (unsigned char)(0x5a + errs);
    /* simavr detects "sleeping with interrupts off" and terminates the
       simulation gracefully right here. */
    cli();
    sleep_cpu();
#endif
    return errs;
}
