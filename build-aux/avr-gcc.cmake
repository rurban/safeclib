# CMake toolchain file for a freestanding avr-gcc build.
# cmake -B build-avr -DCMAKE_TOOLCHAIN_FILE=build-aux/avr-gcc.cmake \
#   -DENABLE_MINIMAL=ON
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# must match the simavr/avr-sim-test.sh "-m" device name. atmega328's
# 2K SRAM is too small for the u8+extensions ENABLE_MINIMAL variant;
# atmega2560's 8K fits both, and keeps the plain and u8 builds on the
# same simulated core.
set(AVR_MCU atmega2560 CACHE STRING "AVR device for -mmcu")

find_program(AVR_GCC NAMES avr-gcc REQUIRED)
find_program(AVR_AR NAMES avr-ar REQUIRED)
set(CMAKE_C_COMPILER ${AVR_GCC})
set(CMAKE_AR ${AVR_AR} CACHE FILEPATH "avr archiver")

# -g: avr-sim-test.sh reads tests/test_minimal.c's result via avr-gdb,
# which needs real debug info to resolve symbol addresses/types over
# the simavr gdbserver remote protocol.
set(CMAKE_C_FLAGS_INIT "-mmcu=${AVR_MCU} -Os -g")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-mmcu=${AVR_MCU}")

# avr-gcc accepts -fstack-protector-strong at compile time (cc1 just
# warns "not supported for this target" and silently skips it per
# function), but the link driver's spec file still tries to pull in
# -lssp/-lssp_nonshared, which this target's sysroot doesn't ship.
# Preempt CMakeLists.txt's check_c_compiler_flag() probe, which only
# exercises a trivial program and doesn't trip over the missing libs.
set(HAVE_STACK_PROTECTOR_STRONG 0 CACHE INTERNAL "")
