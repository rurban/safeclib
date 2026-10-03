# CMake toolchain file for a freestanding avr-gcc build.
# cmake -B build-avr -DCMAKE_TOOLCHAIN_FILE=build-aux/avr-gcc.cmake \
#   -DENABLE_MINIMAL=ON
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# must match the simulavr/avr-sim-test.sh "-d" device name
set(AVR_MCU atmega328 CACHE STRING "AVR device for -mmcu")

find_program(AVR_GCC NAMES avr-gcc REQUIRED)
find_program(AVR_AR NAMES avr-ar REQUIRED)
set(CMAKE_C_COMPILER ${AVR_GCC})
set(CMAKE_AR ${AVR_AR} CACHE FILEPATH "avr archiver")
set(CMAKE_C_FLAGS_INIT "-mmcu=${AVR_MCU} -Os")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-mmcu=${AVR_MCU}")
