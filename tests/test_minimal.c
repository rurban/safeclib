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
/* simavr's avr-gdb remote stub does not actually execute the
   simulated core: 'continue' to a breakpoint reports a stop at the
   right PC without ever running the intervening instructions, and
   'stepi' just walks the PC forward one word at a time without
   decoding/executing it either (verified against gdb-avr 13/15 +
   simavr 1.6: both "step through call main" and memory/register
   reads after a real run come back as if main() never ran). So the
   AVR path can't drive or observe the test via avr-gdb like the
   sdcc stm8 ucsim job does below with native ucsim commands.
   Instead, report test_result/test_marker over the real UART0
   peripheral: simavr's avr_uart model genuinely runs the core and
   echoes transmitted bytes to its own stdout (with -v), which is
   how avr-sim-test.sh reads the result. */
#include <avr/io.h>
static void avr_uart_init(void) {
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
static void avr_uart_putc(unsigned char c) {
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = c;
}
static void avr_uart_puthex(unsigned char v) {
    static const char hex[] = "0123456789abcdef";
    avr_uart_putc(hex[(v >> 4) & 0xf]);
    avr_uart_putc(hex[v & 0xf]);
}
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
    avr_uart_init();
    avr_uart_putc('R');
    avr_uart_putc('=');
    avr_uart_puthex(test_result);
    avr_uart_putc(' ');
    avr_uart_putc('M');
    avr_uart_putc('=');
    avr_uart_puthex(test_marker);
    avr_uart_putc('\n');
    for (;;) {
        /* avr-sim-test.sh kills the simulator once it has read the
           "R=.. M=.." line from simavr's UART-echo stdout */
    }
#endif
    return errs;
}
