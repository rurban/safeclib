# CMake toolchain file for a freestanding sdcc STM8 build.
# cmake -B build-stm8 -DCMAKE_TOOLCHAIN_FILE=build-aux/cmake/sdcc-stm8.cmake \
#   -DENABLE_MINIMAL=ON
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR stm8)

find_program(SDCC NAMES sdcc sdcc-sdcc REQUIRED)
# Fedora prefixes the tools with sdcc-
find_program(SDCCAR_EXECUTABLE NAMES sdar sdcc-sdar sdcclib REQUIRED)
set(CMAKE_C_COMPILER ${SDCC})
# --stack-auto: the constraint handlers are called via function pointers
set(CMAKE_C_FLAGS_INIT "-mstm8 --std-c11 --stack-auto")
