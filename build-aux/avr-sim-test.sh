#!/bin/sh
# Run the cmake ENABLE_MINIMAL tests/test_minimal.c in the simavr AVR
# simulator (https://github.com/buserror/simavr, Debian/Ubuntu package
# "simavr"). test_minimal.c tags the ELF with AVR_MCU_SIMAVR_CONSOLE(),
# so simavr prints the 0x5a + errors byte it writes to GPIOR0 straight
# to stdout, with no "-W"/"-R" style command-line wiring needed (that's
# the GNU "simulavr" project's CLI, a different, unrelated tool). The
# firmware then does cli(); sleep_cpu();, which simavr detects as
# "sleeping with interrupts off" and exits on its own.
# usage: build-aux/avr-sim-test.sh build-dir [mcu]
dir=${1:-build-avr}
mcu=${2:-atmega328}
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
result=$(timeout 30 "$sim" -m "$mcu" -f "$elf" 2>/dev/null |
         od -An -tu1 | tr -d ' \n')
echo "test_result=$result (want 90)"
test "$result" = "90"
