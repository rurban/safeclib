#!/bin/sh
# Run the cmake ENABLE_MINIMAL tests/test_minimal.c in the ucsim STM8 simulator.
# usage: build-aux/sdcc-stm8-test.sh build-dir
# test_minimal.c writes 0x5a + errors to test_result at its end.
dir=${1:-build-stm8}
sim=$(command -v sstm8 || command -v ucsim_stm8 || command -v sdcc-ucsim_stm8)
if [ -z "$sim" ]; then
    echo "no ucsim stm8 simulator found"
    exit 1
fi
addr=$(awk '/ _test_result /{print $1; exit}' "$dir/t_minimal.map")
if [ -z "$addr" ]; then
    echo "no _test_result in $dir/t_minimal.map"
    exit 1
fi
addr=$(printf '0x%x' "0x$addr")
# the 1st write is the bss clearing, the 2nd the result
result=$(printf 'break rom w %s 2\nrun\nstep\ndump rom %s %s\nquit\n' \
                "$addr" "$addr" "$addr" |
         timeout 60 "$sim" "$dir/t_minimal.ihx" 2>&1 |
         awk -v a="$addr" 'tolower($1) ~ "^0x0*"substr(a,3)"$" {print $2}' | tail -1)
echo "test_result=0x$result"
test "$result" = "5a"
