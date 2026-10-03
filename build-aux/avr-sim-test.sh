#!/bin/sh
# Run the cmake ENABLE_MINIMAL tests/test_minimal.c in the simavr AVR
# simulator (https://github.com/buserror/simavr, Debian/Ubuntu package
# "simavr"). simavr's avr-gdb remote stub does *not* actually execute
# the simulated core: 'continue' to a breakpoint reports a stop at the
# right PC without ever running the intervening instructions, and
# 'stepi' just walks the PC forward one word at a time without
# decoding/executing it either (verified against gdb-avr 13/15 +
# simavr 1.6: "step through call main" never enters main, and memory
# or register reads after a real run come back zeroed). So the AVR
# path can't drive or observe the test via avr-gdb like the sdcc stm8
# ucsim job does with native ucsim commands.
#
# Instead, run simavr without -g and let tests/test_minimal.c report
# test_result/test_marker as hex text over the real UART0 peripheral:
# simavr's avr_uart model genuinely runs the core, and with -v it
# echoes transmitted bytes to simavr's own stdout.
# usage: build-aux/avr-sim-test.sh build-dir [mcu]
dir=${1:-build-avr}
mcu=${2:-atmega2560}
sim=$(command -v simavr)
if [ -z "$sim" ]; then
    echo "no simavr found"
    exit 1
fi
elf="$dir/t_minimal"
if [ ! -e "$elf" ]; then
    echo "no $elf"
    exit 1
fi

out=$(timeout 20 "$sim" -m "$mcu" -v "$elf" 2>&1)
rc=$?
echo "$out"
result=$(printf '%s\n' "$out" | sed -n 's/.*R=\([0-9a-f][0-9a-f]\) M=\([0-9a-f][0-9a-f]\).*/\1/p' | head -1)
marker=$(printf '%s\n' "$out" | sed -n 's/.*R=\([0-9a-f][0-9a-f]\) M=\([0-9a-f][0-9a-f]\).*/\2/p' | head -1)
echo "test_result=0x$result test_marker=0x$marker (want 0x5a 0xc3)"
if [ -z "$result" ] || [ -z "$marker" ]; then
    if [ "$rc" != "124" ]; then
        echo "simavr exited early with status $rc"
    fi
fi
test "$result" = "5a" && test "$marker" = "c3"
