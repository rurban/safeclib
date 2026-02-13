# CMakeLists.txt - Safe C Library
#
# 2026  Reini Urban <rurban@cpan.org>
#
# Copyright (c) 2026 Reini Urban
# All rights reserved.
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.10)
project(safeclib
    VERSION 3.9.2
    DESCRIPTION "Safe C Library"
    HOMEPAGE_URL "http://github.com/rurban/safeclib/"
    LANGUAGES C
)

# Library version info (current:revision:age)
# https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
set(SAFEC_SO_CURRENT 3)
set(SAFEC_SO_REVISION 9)
set(SAFEC_SO_AGE 0)
math(EXPR SAFEC_SO_MAJOR "${SAFEC_SO_CURRENT} - ${SAFEC_SO_AGE}")
set(SAFEC_SO_VERSION "${SAFEC_SO_MAJOR}.${SAFEC_SO_AGE}.${SAFEC_SO_REVISION}")

# Include GNUInstallDirs for standard install paths
include(GNUInstallDirs)

# Options
option(ENABLE_GCOV "Enable test coverage (gcov)" OFF)
option(ENABLE_DEBUG "Compile with debugging symbols" OFF)
option(ENABLE_EXTENSIONS "Enable additional functions not in C11 spec" ON)
option(ENABLE_FLOAT "Enable float support in printf functions" ON)
option(ENABLE_FLOAT_EXP "Enable exponential float notation (%e/%g)" ON)
option(ENABLE_LONG_LONG "Enable long long types (%llu/%p)" ON)
option(ENABLE_LONG_DOUBLE "Enable long double types (%Lf/%Le/%Lg/%La)" ON)
option(ENABLE_PRINTF_PTRDIFF "Enable ptrdiff_t type (%t) in printf" ON)
option(ENABLE_NULLSLACK "Null out remaining part of string buffer" ON)
option(ENABLE_CONSTRAINT_HANDLER "Enable C11 invoke_safe_{str,mem}_constraint_handler" ON)
option(ENABLE_UNSAFE "Include unsafe std C11 functions (tmpnam_s)" OFF)
option(ENABLE_NORM_COMPAT "Enable NFKC and NFKD modes for wcsnorm" OFF)
option(ENABLE_WARN_DMAX "Enable dmax checks against __builtin_object_size" OFF)
option(ENABLE_ERROR_DMAX "Make WARN_DMAX fatal" OFF)
option(ENABLE_DOC "Enable documentation" ON)
option(ENABLE_DEBUG_BUILD "Enable build debugging output" OFF)
option(ENABLE_HARDENING "Enable hardening flags" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
option(ENABLE_VALGRIND "Add tests with valgrind" OFF)
# TODO enable-memmax=262144 enable-strmax=2056

# Set default handler (ignore, abort, or empty for runtime selection)
set(DEFAULT_HANDLER "" CACHE STRING "Compile-time default handler (ignore/abort)")

# Size limits
set(RSIZE_MAX_MEM "(256UL << 20)" CACHE STRING "Maximum object size for mem* functions (default 256MB)")
set(RSIZE_MAX_STR "(4UL << 10)" CACHE STRING "Maximum object size for str* functions (default 4KB)")

# Check for version file
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.tarball-version")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.tarball-version" PACKAGE_VERSION_RAW)
    string(STRIP "${PACKAGE_VERSION_RAW}" PACKAGE_VERSION)
else()
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --always
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE PACKAGE_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
    else()
        set(PACKAGE_VERSION "${PROJECT_VERSION}")
    endif()
endif()

# Compiler detection
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(COMPILER_GNU TRUE)
else()
    set(COMPILER_GNU FALSE)
endif()

if(CMAKE_CXX_COMPILER_ID)
    set(HAVE_CXX TRUE)
else()
    set(HAVE_CXX FALSE)
endif()

# C99/C11 support
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckStructHasMember)
include(CheckCSourceCompiles)
include(CheckCXXSourceCompiles)
include(TestBigEndian)

# Check C99 support
check_c_compiler_flag("-std=c99" HAVE_C99_FLAG)
if(HAVE_C99_FLAG)
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(SAFECLIB_HAVE_C99 1)
else()
    set(SAFECLIB_HAVE_C99 0)
    message(WARNING "Compiler may not support C99")
endif()

# Check C11 support
check_c_compiler_flag("-std=c11" HAVE_C11_FLAG)
if(HAVE_C11_FLAG)
    set(HAVE_C11 1)
else()
    set(HAVE_C11 0)
endif()


# Check warnings
set(COMMON_WARNINGS
    -Wall
    -Wextra
    -pedantic
    -fno-strict-aliasing
    #-Wpedantic
    -Wshadow
    -Wcast-align
    -Wunused
    -Wpointer-arith
    #-Wcast-qual
    -Wmissing-declarations
    -Wmissing-prototypes
    -Wstrict-prototypes
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wformat=2
    -Wmissing-format-attribute
    -Wnull-dereference
    -Wwrite-strings
    -Wstrict-overflow=2
    -Wredundant-decls
    -Wnested-externs
    -Winline
    -Wundef
    -Wstrict-aliasing=2
    -Werror=return-type
    -Werror=implicit-function-declaration
    -Werror=implicit-int
    -Werror=pointer-arith
    -Werror=strict-prototypes
)

# Check each flag and add supported ones
set(WARN_CFLAGS)
foreach(flag ${COMMON_WARNINGS})
    # Create a valid CMake variable name from the flag
    string(REPLACE "-" "_" flag_var ${flag})
    string(REPLACE "=" "_" flag_var ${flag_var})
    string(REPLACE "+" "_" flag_var ${flag_var})
    string(TOUPPER "HAS${flag_var}" flag_check)

    check_c_compiler_flag(${flag} ${flag_check})
    if(${flag_check})
        list(APPEND WARN_CFLAGS ${flag})
    endif()
endforeach()

# Apply all supported warnings
add_compile_options(${WARN_CFLAGS})
message(STATUS "WARN_CFLAGS: ${WARN_CFLAGS}")

# Check headers
check_include_file("time.h" HAVE_TIME_H)
check_include_file("sys/time.h" HAVE_SYS_TIME_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)
check_include_file("memory.h" HAVE_MEMORY_H)
check_include_file("ctype.h" HAVE_CTYPE_H)
check_include_file("malloc.h" HAVE_MALLOC_H)
check_include_file("string.h" HAVE_STRING_H)
check_include_file("limits.h" HAVE_LIMITS_H)
check_include_file("stddef.h" HAVE_STDDEF_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("float.h" HAVE_FLOAT_H)
check_include_file("math.h" HAVE_MATH_H)
check_include_file("sys/types.h" HAVE_SYS_TYPES_H)
check_include_file("inttypes.h" HAVE_INTTYPES_H)
check_include_file("stdint.h" HAVE_STDINT_H)
check_include_file("errno.h" HAVE_ERRNO_H)
check_include_file("wchar.h" HAVE_WCHAR_H)
check_include_file("langinfo.h" HAVE_LANGINFO_H)
check_include_file("valgrind/valgrind.h" HAVE_VALGRIND_VALGRIND_H)
check_include_file("fenv.h" HAVE_FENV_H)
check_include_file("intrin.h" HAVE_INTRIN_H)
check_include_file("xmmintrin.h" HAVE_XMMINTRIN_H)
check_include_file("emmintrin.h" HAVE_EMMINTRIN_H)
check_include_file("x86intrin.h" HAVE_X86INTRIN_H)
check_include_file("arm_neon.h" HAVE_ARM_NEON_H)
check_include_file("arm_acle.h" HAVE_ARM_ACLE_H)
check_include_file("mmintrin.h" HAVE_MMINTRIN_H)
check_include_file("altivec.h" HAVE_ALTIVEC_H)
check_include_file("spe.h" HAVE_SPE_H)
check_include_file("mbarrier.h" HAVE_MBARRIER_H)
check_include_file("stdbool.h" HAVE_STDBOOL_H)
check_include_file("valgrind/valgrind.h" HAVE_VALGRIND_VALGRIND_H)

# Check types
set(CMAKE_EXTRA_INCLUDE_FILES "stddef.h")
check_type_size("size_t" SIZEOF_SIZE_T)
if (HAVE_TIME_H)
  set(CMAKE_EXTRA_INCLUDE_FILES "time.h")
  check_type_size("time_t" SIZEOF_TIME_T)
endif()
if (HAVE_WCHAR_H)
  set(CMAKE_EXTRA_INCLUDE_FILES "wchar.h")
  check_type_size("wchar_t" SIZEOF_WCHAR_T)
endif()
unset(CMAKE_EXTRA_INCLUDE_FILES)
check_type_size("long long" LONG_LONG)
check_type_size("long double" LONG_DOUBLE)

if(HAVE_STDINT_H)
    check_type_size("int32_t" INT32_T)
    check_type_size("uint8_t" UINT8_T)
    check_type_size("uint16_t" UINT16_T)
    check_type_size("uint32_t" UINT32_T)
    check_type_size("uint64_t" UINT64_T)
    check_type_size("uintptr_t" UINTPTR_T)
endif()

set(CMAKE_C_FLAGS -Werror)
# Check for errno_t
check_c_source_compiles("
#include <errno.h>
int main() { errno_t e = 0; return 0; }
" HAVE_ERRNO_T)
if(NOT HAVE_ERRNO_T)
    set(FALLBACK_ERRNO_T "typedef int errno_t;")
else()
   set(FALLBACK_ERRNO_T "")
endif()

# Check for mbstate_t
check_type_size("mbstate_t" MBSTATE_T LANGUAGE C)
if(NOT HAVE_MBSTATE_T)
    # Define fallback if needed
endif()

# Check struct tm.tm_gmtoff
check_struct_has_member("struct tm" "tm_gmtoff" "time.h" HAVE_TM_GMTOFF)

# Check for __float128
check_c_source_compiles("__float128 x = 1.0Q; int main() { return 0; }" HAVE_FLOAT128)

# Check compiler builtins
include(CheckCSourceCompiles)

set(CMAKE_C_FLAGS -O2)
# __builtin_object_size
check_c_source_compiles("
int main() {
    char buf[10];
    return __builtin_object_size(buf, 0) != (size_t)-1 ? 0 : 1;
}
" HAVE___BUILTIN_OBJECT_SIZE)

# __builtin_ctz
check_c_source_compiles("
int main() {
    return __builtin_ctz(1U);
}
" HAVE___BUILTIN_CTZ)

# __builtin_constant_p
check_c_source_compiles("
int main() {
    return __builtin_constant_p(1) ? 0 : 1;
}
" HAVE___BUILTIN_CONSTANT_P)

# __builtin_isinf
check_c_source_compiles("
#include <math.h>
int main() {
    return __builtin_isinf(1.0) ? 1 : 0;
}
" HAVE___BUILTIN_ISINF)

# __builtin_isinfl
check_c_source_compiles("
#include <math.h>
int main() {
    return __builtin_isinfl(1.0L) ? 1 : 0;
}
" HAVE___BUILTIN_ISINFL)

set(CMAKE_C_FLAGS -Werror)
# Check function attributes
check_c_source_compiles("
void __attribute__((format(printf, 1, 2))) myprintf(const char *fmt, ...);
int main() { return 0; }
" HAVE_ATTRIBUTE_FORMAT)

check_c_source_compiles("
void __attribute__((format(__wprintf__, 1, 2))) myprintf(const char *fmt, ...);
int main() { return 0; }
" HAVE_ATTRIBUTE_FORMAT_WPRINTF)
if(NOT HAVE_ATTRIBUTE_FORMAT_WPRINTF)
  set(HAVE_ATTRIBUTE_FORMAT_WPRINTF 0)
endif()

check_c_source_compiles("
void __attribute__((format(__wscanf__, 1, 2))) myscanf(const char *fmt, ...);
int main() { return 0; }
" HAVE_ATTRIBUTE_FORMAT_WSCANF)
if(NOT HAVE_ATTRIBUTE_FORMAT_WSCANF)
  set(HAVE_ATTRIBUTE_FORMAT_WSCANF 0)
endif()

check_c_source_compiles("
void __attribute__((malloc)) *myalloc(void);
int main() { return 0; }
" HAVE_ATTRIBUTE_MALLOC)

check_c_source_compiles("
void __attribute__((returns_nonnull)) *myalloc(void);
int main() { return 0; }
" HAVE_ATTRIBUTE_RETURNS_NONNULL)

# diagnose_if attribute (Clang 5+)
check_c_source_compiles("
int myfunc(char *ptr) __attribute__((diagnose_if(!ptr, \"empty ptr\", \"error\")));
int myfunc(char *ptr) { return ptr ? 1 : 0; }
int main() {
    char dest[12];
    return myfunc(dest);
}
" HAVE_ATTRIBUTE_DIAGNOSE_IF)

# Check for inline assembly
check_c_source_compiles("
int main() {
    __asm__ __volatile__ (\"\" ::: \"memory\");
    return 0;
}
" HAVE_ASM_INLINE)
set(CMAKE_C_FLAGS "-Wall -Wextra")

# Check functions
check_function_exists(vswprintf HAVE_VSWPRINTF)
check_function_exists(vswscanf HAVE_VSWSCANF)
check_function_exists(mbsrtowcs HAVE_MBSRTOWCS)

# Determine wchar support
if(HAVE_WCHAR_H AND HAVE_VSWPRINTF AND HAVE_MBSRTOWCS AND HAVE_VSWSCANF)
    set(ENABLE_WCHAR_DEFAULT ON)
else()
    set(ENABLE_WCHAR_DEFAULT OFF)
endif()
option(ENABLE_WCHAR "Enable multibyte and wchar support" ${ENABLE_WCHAR_DEFAULT})

# Standard functions
check_function_exists(memcmp HAVE_MEMCMP)
check_function_exists(memset HAVE_MEMSET)
check_function_exists(strcmp HAVE_STRCMP)
check_function_exists(strcasecmp HAVE_STRCASECMP)
check_function_exists(strcasestr HAVE_STRCASESTR)
check_function_exists(strcspn HAVE_STRCSPN)
check_function_exists(strpbrk HAVE_STRPBRK)
check_function_exists(strspn HAVE_STRSPN)
check_function_exists(strnstr HAVE_STRNSTR)
check_function_exists(strnlen HAVE_STRNLEN)
check_function_exists(strrchr HAVE_STRRCHR)
check_function_exists(memrchr HAVE_MEMRCHR)
check_function_exists(strstr HAVE_STRSTR)
check_function_exists(bcmp HAVE_BCMP)
check_function_exists(secure_getenv HAVE_SECURE_GETENV)
check_function_exists(timingsafe_memcmp HAVE_TIMINGSAFE_MEMCMP)
check_function_exists(timingsafe_bcmp HAVE_TIMINGSAFE_BCMP)
check_function_exists(explicit_bzero HAVE_EXPLICIT_BZERO)
check_function_exists(explicit_memset HAVE_EXPLICIT_MEMSET)
check_function_exists(asctime_r HAVE_ASCTIME_R)
check_function_exists(ctime_r HAVE_CTIME_R)
check_function_exists(gmtime_r HAVE_GMTIME_R)
check_function_exists(localtime_r HAVE_LOCALTIME_R)
check_function_exists(memccpy HAVE_MEMCCPY)
check_function_exists(stpcpy HAVE_STPCPY)
check_function_exists(stpncpy HAVE_STPNCPY)
check_function_exists(strerror HAVE_STRERROR)
check_function_exists(fileno HAVE_FILENO)
check_function_exists(ftruncate HAVE_FTRUNCATE)
check_function_exists(isinfl HAVE_ISINFL)
check_function_exists(isinf HAVE_ISINF)

if(ENABLE_WCHAR)
    check_function_exists(wmemchr HAVE_WMEMCHR)
    check_function_exists(wmemcmp HAVE_WMEMCMP)
    check_function_exists(wcscmp HAVE_WCSCMP)
    check_function_exists(wcsstr HAVE_WCSSTR)
    check_function_exists(vsnwprintf HAVE_VSNWPRINTF)
    check_function_exists(mbstowcs HAVE_MBSTOWCS)
    check_function_exists(iswdigit HAVE_ISWDIGIT)
    check_function_exists(iswspace HAVE_ISWSPACE)
    check_function_exists(towlower HAVE_TOWLOWER)
    check_function_exists(towupper HAVE_TOWUPPER)
    check_function_exists(towctrans HAVE_TOWCTRANS)
endif()

# Check for various __*_chk functions
set(CHK_FUNCS
    __memcpy_chk __memmove_chk __memset_chk
    __strcpy_chk __strncpy_chk __strcat_chk __strncat_chk
    __printf_chk __sprintf_chk __snprintf_chk
    __vfprintf_chk __vfwprintf_chk __vsprintf_chk __vsnprintf_chk
    __vsscanf_chk
)
foreach(func ${CHK_FUNCS})
    string(TOUPPER "${func}" FUNC_UPPER)
    string(REPLACE "__" "" FUNC_NAME "${FUNC_UPPER}")
    check_function_exists(${func} HAVE_${FUNC_NAME})
endforeach()

if(ENABLE_WCHAR)
    check_function_exists(__swprintf_chk HAVE_SWPRINTF_CHK)
    check_function_exists(__vfwprintf_chk HAVE_VFWPRINTF_CHK)
    check_function_exists(__vswscanf_chk HAVE_VSWSCANF_CHK)
endif()

# Check for MPX builtins
check_function_exists(__bnd_chk_ptr_bounds HAVE___BND_CHK_PTR_BOUNDS)
check_function_exists(__bnd_set_ptr_bounds HAVE___BND_SET_PTR_BOUNDS)
check_function_exists(__bnd_null_ptr_bounds HAVE___BND_NULL_PTR_BOUNDS)

# Check for ARM barriers
check_c_source_compiles("__dsb(); int main() { __dsb(); return 0; }" HAVE___DSB)
check_c_source_compiles("__isb(); int main() { __isb(); return 0; }" HAVE___ISB)

# More configs
if (ENABLE_NULLSLACK)
  set(INSERT_NULLSLACK "#define SAFECLIB_STR_NULL_SLACK 1")
else()
  set(INSERT_NULLSLACK "#undef SAFECLIB_STR_NULL_SLACK")
endif()
if (ENABLE_CONSTRAINT_HANDLER)
  set(INSERT_CONSTRAINT_HANDLER "#undef SAFECLIB_DISABLE_CONSTRAINT_HANDLER")
else()
  set(INSERT_CONSTRAINT_HANDLER "#define SAFECLIB_DISABLE_CONSTRAINT_HANDLER 1")
endif()
if (ENABLE_EXTENSIONS)
  set(INSERT_EXTS "#undef SAFECLIB_DISABLE_EXTENSIONS")
else()
  set(INSERT_EXTS "#define SAFECLIB_DISABLE_EXTENSIONS 1")
endif()
if (ENABLE_UNSAFE)
  set(INSERT_UNSAFE "#define SAFECLIB_ENABLE_UNSAFE 1")
else()
  set(INSERT_UNSAFE "#undef SAFECLIB_ENABLE_UNSAFE")
endif()
if (DEFAULT_HANDLER)
  set(INSERT_DEFAULT_HANDLER "#define SAFECLIB_DEFAULT_HANDLER ${DEFAULT_HANDLER}_handler_s")
else()
  set(INSERT_DEFAULT_HANDLER "#undef SAFECLIB_DEFAULT_HANDLER")
endif()
if (SAFECLIB_DISABLE_WCHAR)
  set(INSERT_DISABLE_WCHAR "#define SAFECLIB_DISABLE_WCHAR 1")
else()
  set(INSERT_DISABLE_WCHAR "#undef SAFECLIB_DISABLE_WCHAR")
endif()
if (DISABLE_DLLIMPORT)
  set(INSERT_DISABLE_DLLIMPORT "#define DISABLE_DLLIMPORT 1")
else()
  set(INSERT_DISABLE_DLLIMPORT "#undef DISABLE_DLLIMPORT")
endif()
if (SAFECLIB_HAVE_C99)
  set(INSERT_SAFECLIB_HAVE_C99 "#define SAFECLIB_HAVE_C99 1")
else()
  set(INSERT_SAFECLIB_HAVE_C99 "#undef SAFECLIB_HAVE_C99")
endif()
if (HAVE___BUILTIN_OBJECT_SIZE)
  set(INSERT_OBJECT_SIZE "#define HAVE___BUILTIN_OBJECT_SIZE 1")
  if(ENABLE_WARN_DMAX)
    set(INSERT_WARN_DMAX "#define HAVE_WARN_DMAX 1")
  else()
    set(INSERT_WARN_DMAX "#undef HAVE_WARN_DMAX")
  endif()
  if(ENABLE_ERROR_DMAX)
    set(INSERT_WARN_DMAX "#define HAVE_WARN_DMAX 1")
    set(INSERT_ERROR_DMAX "#define HAVE_ERROR_DMAX 1")
  else()
    set(INSERT_ERROR_DMAX "#undef HAVE_ERROR_DMAX")
  endif()
else()
  set(INSERT_OBJECT_SIZE "#undef HAVE___BUILTIN_OBJECT_SIZE")
  set(INSERT_WARN_DMAX "#undef HAVE_WARN_DMAX")
  set(INSERT_ERROR_DMAX "#undef HAVE_ERROR_DMAX")
endif()
if (HAVE___BUILTIN_CONSTANT_P)
  set(INSERT_CONSTANT_P "#define HAVE___BUILTIN_CONSTANT_P 1")
else()
  set(INSERT_CONSTANT_P "#undef HAVE___BUILTIN_CONSTANT_P")
endif()
if(HAVE_SYS_TYPES_H)
  set(INSERT_SYS_TYPES_H "#include <sys/types.h>")
endif()
if(HAVE_INTTYPES_H)
  set(INSERT_INTTYPES_H "#include <inttypes.h>")
endif()
if(HAVE_STDINT_H)
  set(INSERT_STDINT_H "#include <stdint.h>")
endif()
if(HAVE_ERRNO_H)
  set(INSERT_ERRNO_H "#include <errno.h>")
endif()
if(HAVE_STDBOOL_H)
  set(INSERT_BOOL_SUPPORT "#include <stdbool.h>")
else()
  set(INSERT_BOOL_SUPPORT "# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
")
endif()

# Platform detection
if(WIN32 OR MINGW OR MSYS)
    set(HAVE_MINGW TRUE)
    if(NOT CMAKE_HOST_WIN32)
        set(HAVE_MINGW_CROSS TRUE)
    endif()
else()
    set(HAVE_MINGW FALSE)
    set(HAVE_MINGW_CROSS FALSE)
endif()

if(APPLE)
    set(HAVE_DARWIN TRUE)
else()
    set(HAVE_DARWIN FALSE)
endif()

# Find optional tools
find_package(Doxygen)
find_program(POD2MAN_EXECUTABLE pod2man)
find_program(PANDOC_EXECUTABLE pandoc)
if(MINGW_CROSS)
    find_program(WINE_EXECUTABLE wine)
endif()

# Valgrind
find_program(VALGRIND_EXECUTABLE valgrind)
if(VALGRIND_EXECUTABLE AND NOT APPLE)
    set(HAVE_VALGRIND TRUE)
else()
    set(HAVE_VALGRIND FALSE)
endif()

# Compiler flags
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "")
    add_compile_options(-Wall -Wextra)

    if(ENABLE_HARDENING)
        check_c_compiler_flag(-fstack-protector-strong HAVE_STACK_PROTECTOR_STRONG)
        check_c_compiler_flag(-fstack-clash-protection HAVE_STACK_CLASH_PROTECTION)
        check_c_compiler_flag(-fcf-protection HAVE_CF_PROTECTION)

        if(HAVE_STACK_PROTECTOR_STRONG)
            add_compile_options(-fstack-protector-strong)
            add_link_options(-fstack-protector-strong)
        endif()
        if(HAVE_STACK_CLASH_PROTECTION)
            add_compile_options(-fstack-clash-protection)
            add_link_options(-fstack-clash-protection)
        endif()
        if(HAVE_CF_PROTECTION)
            add_compile_options(-fcf-protection)
            add_link_options(-fcf-protection)
        endif()

        add_compile_options(-fno-strict-aliasing -fno-strict-overflow)
        add_compile_options(-fno-delete-null-pointer-checks)
        add_compile_options(-fno-lifetime-dse)
    endif()

    if(MINGW)
        add_compile_options(-Wno-attributes -Wno-discarded-qualifiers)
        check_c_compiler_flag(-msse2 HAVE_SSE2)
        if(HAVE_SSE2)
            add_compile_options(-msse2)
            add_link_options(-msse2)
        endif()
    endif()

    check_c_compiler_flag(-Wrestrict HAVE_WRESTRICT)
endif()

# Debug/Release flags
if(ENABLE_DEBUG)
    if(ENABLE_GCOV)
        set(CMAKE_C_FLAGS_DEBUG "-g")
    else()
        set(CMAKE_C_FLAGS_DEBUG "-g -O2")
    endif()
    set(CMAKE_BUILD_TYPE Debug)
    add_compile_definitions(DEBUG)

    if(APPLE AND NOT CMAKE_CROSSCOMPILING)
        set(BUILD_SHARED_LIBS OFF)
    endif()
else()
    if(ENABLE_GCOV)
        set(CMAKE_C_FLAGS_RELEASE "-g")
    else()
        set(CMAKE_C_FLAGS_RELEASE "-O2")
        if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
          if (HAVE___BUILTIN_OBJECT_SIZE)
            set(CMAKE_C_FLAGS_RELEASE "-O2 -D_FORTIFY_SOURCE=3")
          endif()
        endif()
    endif()
    set(CMAKE_BUILD_TYPE Release)
endif()

# Coverage
if(ENABLE_GCOV)
    add_compile_options(-fprofile-arcs -ftest-coverage)
    add_link_options(-lgcov --coverage)
endif()

# Feature definitions
if(NOT ENABLE_EXTENSIONS)
    set(SAFECLIB_DISABLE_EXTENSIONS 1)
endif()

if(NOT ENABLE_FLOAT)
    add_compile_definitions(PRINTF_DISABLE_SUPPORT_FLOAT)
endif()

if(NOT ENABLE_FLOAT_EXP)
    add_compile_definitions(PRINTF_DISABLE_SUPPORT_EXPONENTIAL)
endif()

if(NOT ENABLE_LONG_LONG)
    add_compile_definitions(PRINTF_DISABLE_SUPPORT_LONG_LONG)
endif()

if(NOT ENABLE_LONG_DOUBLE)
    add_compile_definitions(PRINTF_DISABLE_SUPPORT_LONG_DOUBLE)
endif()

if(NOT ENABLE_PRINTF_PTRDIFF)
    add_compile_definitions(PRINTF_DISABLE_SUPPORT_PTRDIFF_T)
endif()

if(ENABLE_NULLSLACK)
    set(SAFECLIB_STR_NULL_SLACK 1)
else()
    set(SAFECLIB_STR_NULL_SLACK 0)
endif()

if(NOT ENABLE_CONSTRAINT_HANDLER)
    set(SAFECLIB_DISABLE_CONSTRAINT_HANDLER 1)
endif()

if(ENABLE_UNSAFE)
    set(SAFECLIB_ENABLE_UNSAFE 1)
endif()

if(ENABLE_NORM_COMPAT)
    add_compile_definitions(HAVE_NORM_COMPAT)
endif()

if(NOT ENABLE_WCHAR)
    set(SAFECLIB_DISABLE_WCHAR 1)
endif()

# Default handler
if(DEFAULT_HANDLER STREQUAL "ignore")
    set(SAFECLIB_DEFAULT_HANDLER "ignore_handler_s")
elseif(DEFAULT_HANDLER STREQUAL "abort")
    set(SAFECLIB_DEFAULT_HANDLER "abort_handler_s")
else()
    # Runtime selection
    set(SAFECLIB_DEFAULT_HANDLER "")
endif()

# DLL import/export handling
if(BUILD_SHARED_LIBS)
    set(DISABLE_DLLIMPORT 0)
else()
    set(DISABLE_DLLIMPORT 1)
endif()

# Configure files
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/safe_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/safe_config.h"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/safe_lib_errno.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/safe_lib_errno.h"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/safe_types.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/safe_types.h"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmakeconfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
add_compile_definitions(HAVE_CONFIG_H)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/libsafec.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libsafec.pc"
    @ONLY
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Source files
set(SAFEC_SOURCES
    src/safeclib_private.h
    src/abort_handler_s.c
    src/ignore_handler_s.c
    src/mem/safe_mem_constraint.c
    src/mem/memcpy_s.c
    src/mem/memmove_s.c
    src/mem/memset_s.c
    src/mem/mem_primitives_lib.c
    src/str/safe_str_constraint.h
    src/str/safe_str_constraint.c
    src/str/strcat_s.c
    src/str/strcpy_s.c
    src/str/strncat_s.c
    src/str/strncpy_s.c
    src/str/strnlen_s.c
    src/str/strtok_s.c
    src/str/sprintf_s.c
    src/str/vsprintf_s.c
    src/str/snprintf_s.c
    src/str/vsnprintf_s.c
    src/str/strerror_s.c
    src/io/sscanf_s.c
    src/io/fscanf_s.c
    src/io/scanf_s.c
    src/io/vfscanf_s.c
    src/io/vsscanf_s.c
    src/io/vscanf_s.c
    src/io/printf_s.c
    src/io/fprintf_s.c
    src/io/tmpfile_s.c
    src/io/vfprintf_s.c
    src/io/vprintf_s.c
    src/io/fopen_s.c
    src/io/freopen_s.c
    src/io/gets_s.c
    src/misc/bsearch_s.c
    src/misc/qsort_s.c
    src/os/gmtime_s.c
    src/os/localtime_s.c
    src/os/asctime_s.c
    src/os/ctime_s.c
    src/os/getenv_s.c
)

if(ENABLE_EXTENSIONS)
    list(APPEND SAFEC_SOURCES
        src/extmem/memzero_s.c
        src/extmem/memchr_s.c
        src/extmem/memrchr_s.c
        src/extmem/memccpy_s.c
        src/extmem/memcmp_s.c
        src/extmem/memcpy16_s.c
        src/extmem/memcpy32_s.c
        src/extmem/memmove16_s.c
        src/extmem/memmove32_s.c
        src/extmem/memset16_s.c
        src/extmem/memset32_s.c
        src/extmem/memzero16_s.c
        src/extmem/memzero32_s.c
        src/extmem/memcmp16_s.c
        src/extmem/memcmp32_s.c
        src/extmem/memset16_s.c
        src/extmem/memset32_s.c
        src/extmem/memzero16_s.c
        src/extmem/memzero32_s.c
        src/extmem/memzero_s.c
        src/extmem/timingsafe_bcmp.c
        src/extmem/timingsafe_memcmp.c
        src/extstr/stpcpy_s.c
        src/extstr/stpncpy_s.c
        src/extstr/strcasecmp_s.c
        src/extstr/strcasestr_s.c
        src/extstr/strchr_s.c
        src/extstr/strcspn_s.c
        src/extstr/strcmp_s.c
        src/extstr/strcmpfld_s.c
        src/extstr/strcoll_s.c
        src/extstr/strcpyfld_s.c
        src/extstr/strcpyfldin_s.c
        src/extstr/strcpyfldout_s.c
        src/extstr/strfirstchar_s.c
        src/extstr/strfirstdiff_s.c
        src/extstr/strfirstsame_s.c
        src/extstr/strisalphanumeric_s.c
        src/extstr/strisascii_s.c
        src/extstr/strisdigit_s.c
        src/extstr/strishex_s.c
        src/extstr/strislowercase_s.c
        src/extstr/strismixedcase_s.c
        src/extstr/strispassword_s.c
        src/extstr/strisuppercase_s.c
        src/extstr/strlastchar_s.c
        src/extstr/strlastdiff_s.c
        src/extstr/strlastsame_s.c
        src/extstr/strljustify_s.c
        src/extstr/strnterminate_s.c
        src/extstr/strnatcmp_s.c
        src/extstr/strnset_s.c
        src/extstr/strpbrk_s.c
        src/extstr/strprefix_s.c
        src/extstr/strrchr_s.c
        src/extstr/strremovews_s.c
        src/extstr/strset_s.c
        src/extstr/strspn_s.c
        src/extstr/strstr_s.c
        src/extstr/strtolowercase_s.c
        src/extstr/strtouppercase_s.c
        src/extstr/strzero_s.c
    )
endif()

if(ENABLE_WCHAR)
    list(APPEND SAFEC_SOURCES
        src/wchar/mbsrtowcs_s.c
        src/wchar/mbstowcs_s.c
        src/wchar/wcsrtombs_s.c
        src/wchar/wcstombs_s.c
        src/wchar/wcrtomb_s.c
        src/wchar/wctomb_s.c
        src/wchar/wcsnlen_s.c
        src/wchar/wcscpy_s.c
        src/wchar/wcsncpy_s.c
        src/wchar/wcscat_s.c
        src/wchar/wcsncat_s.c
        src/wchar/wmemcpy_s.c
        src/wchar/wmemmove_s.c
        src/wchar/wcstok_s.c
        src/wchar/vswprintf_s.c
        src/wchar/swprintf_s.c
        src/wchar/vfwprintf_s.c
        src/wchar/vwprintf_s.c
        src/wchar/wprintf_s.c
        src/wchar/snwprintf_s.c
        src/wchar/fwprintf_s.c
        src/wchar/swscanf_s.c
        src/wchar/vswscanf_s.c
        src/wchar/wscanf_s.c
        src/wchar/vfwscanf_s.c
        src/wchar/fwscanf_s.c
        src/wchar/vwscanf_s.c
        src/wchar/vsnwprintf_s.c
        src/extwchar/wcsstr_s.c
    )
    if(ENABLE_EXTENSIONS)
      list(APPEND SAFEC_SOURCES
	src/extwchar/wmemcmp_s.c
	src/extwchar/wcscmp_s.c
	src/extwchar/wcsncmp_s.c
	src/extwchar/wcsicmp_s.c
	src/extwchar/wcsnatcmp_s.c
	src/extwchar/wcsset_s.c
	src/extwchar/wcsnset_s.c
	src/extwchar/wcscoll_s.c
	src/extwchar/wcslwr_s.c
	src/extwchar/wcsupr_s.c
	src/extwchar/towfc_s.c
	src/extwchar/towctrans.c
	src/extwchar/wcsfc_s.c
	src/extwchar/hangul.h
	src/extwchar/unw16ifcan.h
	src/extwchar/unw16ifcpt.h
	src/extwchar/unw16ifcmb.h
	src/extwchar/unw16ifcmp.h
	src/extwchar/unw16ifexc.h
	src/extwchar/unwifcan.h
	src/extwchar/unwifcmb.h
	src/extwchar/unwifcmp.h
	src/extwchar/unwifcpt.h
	src/extwchar/unwifexc.h
	src/extwchar/wcsnorm_s.c
        )
    endif()
endif()

if(ENABLE_UNSAFE)
    list(APPEND SAFEC_SOURCES
        src/tmpnam_s.c
    )
endif()

# prepend ${CMAKE_CURRENT_SOURCE_DIR}/ path
list(JOIN SAFEC_SOURCES ";${CMAKE_CURRENT_SOURCE_DIR}/" SAFEC_SOURCES_WITH_PATH)
# message(STATUS "SAFEC_SOURCES: ${SAFEC_SOURCES}")
set(SAFEC_SOURCES_WITH_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SAFEC_SOURCES_WITH_PATH}")
# message(STATUS "SAFEC_SOURCES_WITH_PATH: ${SAFEC_SOURCES_WITH_PATH}")

# Create library
if(BUILD_SHARED_LIBS)
    add_library(safec_shared SHARED ${SAFEC_SOURCES_WITH_PATH})
    set_target_properties(safec_shared PROPERTIES
        OUTPUT_NAME safec
        VERSION ${SAFEC_SO_VERSION}
        SOVERSION ${SAFEC_SO_MAJOR}
        C_VISIBILITY_PRESET hidden
    )
    if(WIN32)
        target_compile_definitions(safec_shared PRIVATE BUILDING_DLL)
    endif()
endif()

if(BUILD_STATIC_LIBS)
    add_library(safec_static STATIC ${SAFEC_SOURCES_WITH_PATH})
    set_target_properties(safec_static PROPERTIES
        OUTPUT_NAME safec
    )
endif()

# Alias target
if(BUILD_SHARED_LIBS)
    add_library(safec ALIAS safec_shared)
elseif(BUILD_STATIC_LIBS)
    add_library(safec ALIAS safec_static)
endif()

# Installation
include(GNUInstallDirs)

if(BUILD_SHARED_LIBS)
    install(TARGETS safec_shared
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

if(BUILD_STATIC_LIBS)
    install(TARGETS safec_static
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

install(FILES
    include/safe_lib.h
    include/safe_mem_lib.h
    include/safe_str_lib.h
    include/safe_types.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/safe_config.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/safe_lib_errno.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libsafec.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# Documentation
if(ENABLE_DOC AND DOXYGEN_FOUND)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
        @ONLY
    )
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# Testing
enable_testing()
add_subdirectory(tests)

# Print configuration summary
message(STATUS "")
message(STATUS "Safe C Library ${PACKAGE_VERSION} configuration:")
message(STATUS "  Version: ${SAFEC_SO_VERSION}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared libs: ${BUILD_SHARED_LIBS}")
message(STATUS "  Static libs: ${BUILD_STATIC_LIBS}")
message(STATUS "  Extensions: ${ENABLE_EXTENSIONS}")
message(STATUS "  Wchar support: ${ENABLE_WCHAR}")
message(STATUS "  Constraint handler: ${ENABLE_CONSTRAINT_HANDLER}")
message(STATUS "  Debug: ${ENABLE_DEBUG}")
message(STATUS "  Gcov: ${ENABLE_GCOV}")
message(STATUS "  Hardening: ${ENABLE_HARDENING}")
message(STATUS "")
