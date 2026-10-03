#!/bin/sh
# Run the cmake ENABLE_MINIMAL tests/test_minimal.c in the ucsim STM8 simulator.
# usage: build-aux/sdcc-stm8-test.sh build-dir
# test_minimal.c writes 0x5a + errors to test_result, then the test_marker
# sentinel. Nothing else writes the marker.
dir=${1:-build-stm8}
sim=$(command -v sstm8 || command -v ucsim_stm8 || command -v sdcc-ucsim_stm8)
if [ -z "$sim" ]; then
    echo "no ucsim stm8 simulator found"
    exit 1
fi
addr=$(awk '/ _test_marker /{print $1; exit}' "$dir/t_minimal.map")
resaddr=$(awk '/ _test_result /{print $1; exit}' "$dir/t_minimal.map")
if [ -z "$addr" ] || [ -z "$resaddr" ]; then
    echo "no _test_marker in $dir/t_minimal.map"
    exit 1
fi
addr=$(printf '0x%x' "0x$addr")
resaddr=$(printf '0x%x' "0x$resaddr")
out=$(printf 'break rom w %s 2\nrun\nstep\ndump rom %s %s\ndump rom %s %s\nquit\n' \
             "$addr" "$resaddr" "$resaddr" "$addr" "$addr" |
      timeout 60 "$sim" "$dir/t_minimal.ihx" 2>&1)
result=$(printf '%s\n' "$out" | awk -v a="$resaddr" 'tolower($1) ~ "^0x0*"substr(a,3)"$" {print $2}' | tail -1)
marker=$(printf '%s\n' "$out" | awk -v a="$addr" 'tolower($1) ~ "^0x0*"substr(a,3)"$" {print $2}' | tail -1)
echo "test_result=0x$result test_marker=0x$marker"
test "$marker" = "c3" && test "$result" = "5a"
