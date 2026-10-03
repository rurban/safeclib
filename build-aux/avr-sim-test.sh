#!/bin/sh
# Run the cmake ENABLE_MINIMAL tests/test_minimal.c in the simulavr AVR
# simulator. test_minimal.c writes 0x5a + errors to simulavr's documented
# debug-output port (0x20) right before returning from main; "-W 0x20,-"
# pipes that single byte to stdout. "-T exit" stops the simulation at the
# auto-generated label right after main() returns.
# usage: build-aux/avr-sim-test.sh build-dir [mcu]
dir=${1:-build-avr}
mcu=${2:-atmega328}
sim=$(command -v simulavr)
if [ -z "$sim" ]; then
    echo "no simulavr found"
    exit 1
fi
elf="$dir/t_minimal"
if [ ! -e "$elf" ]; then
    echo "no $elf"
    exit 1
fi
result=$(timeout 30 "$sim" -d "$mcu" -f "$elf" -W 0x20,- -T exit 2>/dev/null |
         od -An -tu1 | tr -d ' \n')
echo "test_result=$result (want 90)"
test "$result" = "90"
