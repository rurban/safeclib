# CMake toolchain file for a freestanding avr-gcc build.
# cmake -B build-avr -DCMAKE_TOOLCHAIN_FILE=build-aux/avr-gcc.cmake \
#   -DENABLE_MINIMAL=ON
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# must match the simavr/avr-sim-test.sh "-m" device name
set(AVR_MCU atmega328 CACHE STRING "AVR device for -mmcu")

find_program(AVR_GCC NAMES avr-gcc REQUIRED)
find_program(AVR_AR NAMES avr-ar REQUIRED)
set(CMAKE_C_COMPILER ${AVR_GCC})
set(CMAKE_AR ${AVR_AR} CACHE FILEPATH "avr archiver")

# libsimavr-dev's avr/avr_mcu_section.h, used by tests/test_minimal.c to
# tag the ELF for the simavr simulator; harmless if not installed (only
# the ENABLE_MINIMAL test target includes it, under __AVR__)
find_path(SIMAVR_INCLUDE_DIR NAMES avr/avr_mcu_section.h
    PATHS /usr/include/simavr)
if(SIMAVR_INCLUDE_DIR)
    set(CMAKE_C_FLAGS_INIT "-mmcu=${AVR_MCU} -Os -I${SIMAVR_INCLUDE_DIR}")
else()
    set(CMAKE_C_FLAGS_INIT "-mmcu=${AVR_MCU} -Os")
endif()
set(CMAKE_EXE_LINKER_FLAGS_INIT "-mmcu=${AVR_MCU}")

# avr-gcc accepts -fstack-protector-strong at compile time (cc1 just
# warns "not supported for this target" and silently skips it per
# function), but the link driver's spec file still tries to pull in
# -lssp/-lssp_nonshared, which this target's sysroot doesn't ship.
# Preempt CMakeLists.txt's check_c_compiler_flag() probe, which only
# exercises a trivial program and doesn't trip over the missing libs.
set(HAVE_STACK_PROTECTOR_STRONG 0 CACHE INTERNAL "")
