# tests/CMakeLists.txt - Safe C Library Tests
#
# Feb 2026, Reini Urban with some kimi AI
#
# Copyright (c) 2026 Reini Urban
# All rights reserved.

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_BINARY_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra)
  add_compile_options(-g)
endif()
if (HAS_PEDANTIC)
  add_compile_options(-Wno-pedantic)
endif()

set(BUILD_SHARED_LIBS FALSE)
# Common test files
set(SAFEC_TEST_COMMON
    test_private.h
)

set(SAFEC_TEST_MSVCRT
    test_msvcrt.h
    test_msvcrt.c
)

# Unicode data files needed for some tests
set(UNIDATA
    CaseFolding.txt
    DerivedGeneralCategory.txt
)

# Download Unicode data if not present
foreach(datafile ${UNIDATA})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${datafile})
        file(DOWNLOAD
            "https://www.unicode.org/Public/UCD/latest/ucd/${datafile}"
            ${CMAKE_CURRENT_SOURCE_DIR}/${datafile}
            SHOW_PROGRESS
        )
    endif()
endforeach()

# Core tests (always built)
set(SAFEC_TESTS
    t_memcpy_s
    t_memmove_s
    t_memset_s
    t_sprintf_s
    t_sprintf_s_exc
    t_strcat_s
    t_strcpy_s
    t_strncat_s
    t_strncpy_s
    t_strnlen_s
    t_strtok_s
    t_vsprintf_s
    t_sscanf_s
    t_strerror_s
    t_strerrorlen_s
    t_fprintf_s
    t_vprintf_s
    t_printf_s
    t_vfprintf_s
    t_tmpfile_s
    t_gets_s
    t_fopen_s
    t_freopen_s
    t_asctime_s
    t_ctime_s
    t_gmtime_s
    t_localtime_s
    t_getenv_s
    t_bsearch_s
    t_qsort_s
)

# Debug/unstable tests
if(ENABLE_DEBUG)
    list(APPEND SAFEC_TESTS
        t_scanf_s
        t_fscanf_s
    )
endif()

# Wchar tests
set(SAFEC_WCHAR_TESTS
    t_mbstowcs_s
    t_mbsrtowcs_s
    t_wcsrtombs_s
    t_wcstombs_s
    t_wcrtomb_s
    t_wctomb_s
    t_wcsnlen_s
    t_wcscpy_s
    t_wcsncpy_s
    t_wcscat_s
    t_wcsncat_s
    t_wmemcpy_s
    t_wmemmove_s
    t_wcstok_s
    t_vswprintf_s
    t_swprintf_s
    t_fwprintf_s
    t_vfwprintf_s
    t_swscanf_s
    t_vswscanf_s
    t_vwscanf_s
    t_fwscanf_s
    t_vfwscanf_s
    t_wscanf_s
)

if(ENABLE_WCHAR)
    list(APPEND SAFEC_TESTS ${SAFEC_WCHAR_TESTS})

    if(ENABLE_DEBUG)
        list(APPEND SAFEC_TESTS
            t_wprintf_s
            t_vwprintf_s
        )
    endif()
endif()

# Extension tests
set(SAFEC_EXT_TESTS
    t_memcpy32_s
    t_memcpy16_s
    t_memcmp_s
    t_memcmp32_s
    t_memcmp16_s
    t_memmove32_s
    t_memmove16_s
    t_memset32_s
    t_memset16_s
    t_memzero_s
    t_memzero32_s
    t_memzero16_s
    t_stpcpy_s
    t_stpncpy_s
    t_strcasecmp_s
    t_strcasestr_s
    t_strcmp_s
    t_strcmpfld_s
    t_strcpyfldin_s
    t_strcpyfldout_s
    t_strcpyfld_s
    t_strcspn_s
    t_strfirstchar_s
    t_strfirstdiff_s
    t_strfirstsame_s
    t_strisalphanumeric_s
    t_strisascii_s
    t_strisdigit_s
    t_strishex_s
    t_strislowercase_s
    t_strismixed_s
    t_strispassword_s
    t_strisuppercase_s
    t_strlastchar_s
    t_strlastdiff_s
    t_strlastsame_s
    t_strljustify_s
    t_strnatcmp_s
    t_strnterminate_s
    t_strpbrk_s
    t_strprefix_s
    t_strremovews_s
    t_strspn_s
    t_strchr_s
    t_memchr_s
    t_strrchr_s
    t_memrchr_s
    t_strstr_s
    t_strtolowercase_s
    t_strtouppercase_s
    t_strset_s
    t_strnset_s
    t_strzero_s
    t_strcoll_s
    t_timingsafe_memcmp
    t_timingsafe_bcmp
    t_memccpy_s
)

if(ENABLE_EXTENSIONS)
    list(APPEND SAFEC_TESTS ${SAFEC_EXT_TESTS})
endif()

# Wchar extension tests
set(SAFEC_WCHAR_EXT_TESTS
    t_wmemcmp_s
    t_wcscmp_s
    t_wcsncmp_s
    t_wcsicmp_s
    t_wcsnatcmp_s
    t_wcsstr_s
    t_wcsset_s
    t_wcsnset_s
    t_wcscoll_s
    t_wcslwr_s
    t_wcsupr_s
    t_towfc_s
    t_towlower
    t_towupper
    t_wcsfc_s
    t_wcsnorm_s
)

if(ENABLE_WCHAR AND ENABLE_EXTENSIONS)
    list(APPEND SAFEC_TESTS ${SAFEC_WCHAR_EXT_TESTS})
endif()

# Unsafe tests
if(ENABLE_UNSAFE)
    list(APPEND SAFEC_TESTS t_tmpnam_s)

    if(ENABLE_WCHAR)
        list(APPEND SAFEC_TESTS
            t_snprintf_s
            t_vsnprintf_s
            t_snwprintf_s
            t_vsnwprintf_s
        )
    endif()
endif()

# Performance tests
set(SAFEC_PERF_TESTS
    p_memcpy_s
    p_strcpy_s
)

if(NOT HAVE_MINGW)
    list(APPEND SAFEC_PERF_TESTS p_memset_s)
endif()

if(ENABLE_EXTENSIONS)
    list(APPEND SAFEC_PERF_TESTS p_memcpy32_s)
endif()

# Helper function to add a test executable
function(add_safec_test test_name)
    string(REGEX REPLACE "^t_" "test_" source_file "${test_name}")
    set(src_file "${CMAKE_CURRENT_SOURCE_DIR}/${source_file}.c")

    # Check for special source requirements
    set(sources ${src_file})

    # Add common headers
    list(APPEND sources ${SAFEC_TEST_COMMON})

    # Determine if we need msvcrt support
    set(need_msvcrt FALSE)
    set(need_expmem FALSE)
    set(need_math FALSE)

    # Check content for special requirements
    if(test_name MATCHES "^t_")
        # Most t_* tests need msvcrt
        if(NOT test_name MATCHES "t_strcspn_s|t_strnlen_s|t_strerrorlen_s|t_strspn_s|t_strpbrk_s|t_strfirstchar_s|t_strfirstdiff_s|t_strfirstsame_s|t_strisalphanumeric_s|t_strisascii_s|t_strisdigit_s|t_strishex_s|t_strislowercase_s|t_strismixed_s|t_strispassword_s|t_strisuppercase_s|t_strlastchar_s|t_strlastdiff_s|t_strlastsame_s|t_strljustify_s|t_strnatcmp_s|t_strnterminate_s|t_strprefix_s|t_strremovews_s|t_strchr_s|t_memchr_s|t_strrchr_s|t_memrchr_s|t_strstr_s|t_strtolowercase_s|t_strtouppercase_s|t_strzero_s|t_strset_s|t_strnset_s|t_strcoll_s|t_timingsafe_memcmp|t_timingsafe_bcmp|t_wmemcmp_s|t_wcscmp_s|t_wcsncmp_s|t_wcsicmp_s|t_wcsnatcmp_s|t_wcsstr_s|t_wcsset_s|t_wcsnset_s|t_wcscoll_s|t_wcslwr_s|t_wcsupr_s|t_towfc_s|t_towlower|t_towupper|t_wcsfc_s|t_wcsnorm_s|t_wcsnlen_s|t_vswscanf_s|t_vwscanf_s|t_gmtime_s|t_localtime_s|t_vprintf_s")
            set(need_msvcrt TRUE)
        endif()

        # Check for expmem
        if(test_name MATCHES "t_memcpy|t_memmove|t_memset|t_memzero|t_wmemcpy|t_wmemmove|t_memccpy")
            set(need_expmem TRUE)
        endif()

        # Check for math library
        if(test_name STREQUAL "t_sprintf_s_exc")
            set(need_math TRUE)
        endif()
    endif()

    if(test_name MATCHES "^p_")
        message(WARNING "${test_name} not a test")
        list(APPEND sources perf_private.h)
    endif()

    if(need_msvcrt)
        list(APPEND sources ${SAFEC_TEST_MSVCRT})
    endif()

    if(need_expmem)
        list(APPEND sources test_expmem.h)
    endif()

    # message(STATUS "add_executable(${test_name} ${sources})")
    add_executable(${test_name} ${sources})

    # Link to library
    if(BUILD_SHARED_LIBS)
        target_link_libraries(${test_name} safec_shared)
    else()
        target_link_libraries(${test_name} safec_static)
    endif()

    # Special link flags
    if(need_math)
        target_link_libraries(${test_name} m)
    endif()

    # Special case for p_memset_s on Windows
    if(test_name STREQUAL "p_memset_s" AND HAVE_MINGW)
        # Need extra object - handled by linking the library normally
    endif()

    # Add to CTest
    add_test(NAME ${test_name} COMMAND ${test_name})

    # Set test properties
    set_tests_properties(${test_name} PROPERTIES
        TIMEOUT 60
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()

# Helper function to add performance test
function(add_safec_perf perf_name)
    string(REGEX REPLACE "^p_" "perf_" source_file "${perf_name}")
    set(src_file "${CMAKE_CURRENT_SOURCE_DIR}/${source_file}.c")
    set(sources ${src_file} ${SAFEC_TEST_COMMON} perf_private.h)

    # message(STATUS "add_executable(${test_name} ${sources})")
    add_executable(${perf_name} ${sources})

    if(BUILD_SHARED_LIBS)
        target_link_libraries(${perf_name} safec_shared)
    else()
        target_link_libraries(${perf_name} safec_static)
    endif()

    # Performance tests are not added to CTest by default
    set_target_properties(${perf_name} PROPERTIES
        EXCLUDE_FROM_ALL TRUE
    )
endfunction()

# Build all test executables
foreach(test ${SAFEC_TESTS})
    add_safec_test(${test})
endforeach()

# Build performance tests
foreach(perf ${SAFEC_PERF_TESTS})
    add_safec_perf(${perf})
endforeach()

# Custom target for performance tests
add_custom_target(perfs)
foreach(perf ${SAFEC_PERF_TESTS})
    add_dependencies(perfs ${perf})
endforeach()

# BOS (Built-in Object Size) tests
if(HAVE_ATTRIBUTE_DIAGNOSE_IF)
    # Create BOS test variants
    set(BOS_TESTS)
    foreach(test ${SAFEC_TESTS})
        set(bos_test "${test}_bos")
        add_executable(${bos_test} EXCLUDE_FROM_ALL)

        # Get source files from original target
        get_target_property(sources ${test} SOURCES)
        target_sources(${bos_test} PRIVATE ${sources})

        # Add BOS define
        target_compile_definitions(${bos_test} PRIVATE TEST_BOS)

        if(BUILD_SHARED_LIBS)
            target_link_libraries(${bos_test} safec_shared)
        else()
            target_link_libraries(${bos_test} safec_static)
        endif()

        list(APPEND BOS_TESTS ${bos_test})
    endforeach()

    add_custom_target(tests-bos
        COMMAND ${CMAKE_COMMAND} --build . --target ${BOS_TESTS} 2>&1 | tee tests-bos.log || true
        COMMAND ${PERL_EXECUTABLE} -n ${CMAKE_CURRENT_SOURCE_DIR}/tests-bos.pl tests-bos.log || true
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running BOS tests"
    )
endif()

# Valgrind integration
if(VALGRIND_EXECUTABLE AND ENABLE_VALGRIND)
    foreach(test ${SAFEC_TESTS})
        set(valgrind_test "valgrind_${test}")
        add_test(NAME ${valgrind_test}
            #--suppressions=${CMAKE_CURRENT_SOURCE_DIR}/safeclib.supp
            COMMAND ${VALGRIND_EXECUTABLE}
                --leak-check=full
                --error-exitcode=1
                $<TARGET_FILE:${test}>
        )
        set_tests_properties(${valgrind_test} PROPERTIES
            TIMEOUT 120
        )
    endforeach()

    if(NOT APPLE)
        foreach(test ${SAFEC_TESTS})
            set(helgrind_test "helgrind_${test}")
            add_test(NAME ${helgrind_test}
                COMMAND ${VALGRIND_EXECUTABLE}
                    --tool=helgrind
                    --error-exitcode=1
                    $<TARGET_FILE:${test}>
            )
            set_tests_properties(${helgrind_test} PROPERTIES
                TIMEOUT 120
            )

            set(drd_test "drd_${test}")
            add_test(NAME ${drd_test}
                COMMAND ${VALGRIND_EXECUTABLE}
                    --tool=drd
                    --error-exitcode=1
                    $<TARGET_FILE:${test}>
            )
            set_tests_properties(${drd_test} PROPERTIES
                TIMEOUT 120
            )
        endforeach()
    endif()
endif()

# Wine tests for MinGW cross-compilation
if(WINE_EXECUTABLE AND HAVE_MINGW_CROSS)
    foreach(test ${SAFEC_TESTS})
        add_test(NAME wine_${test}
            COMMAND ${WINE_EXECUTABLE} $<TARGET_FILE:${test}>
        )
    endforeach()
endif()

# GCOV target
if(ENABLE_GCOV)
    find_program(GCOV_EXECUTABLE gcov)
    if(GCOV_EXECUTABLE)
      string(REPLACE ";" " " TESTS_LIST "${SAFEC_TESTS}")
        add_custom_target(gcov
            COMMAND ${CMAKE_COMMAND} -E remove -f gcov.log
            COMMAND ${CMAKE_COMMAND} -E touch gcov.log
            COMMAND sh -c "for f in ${TESTS_LIST}\; do
                c=\\\"\\$$(echo \\$$f | sed -e 's,t_,test_,' -e 's,_bos,,' ).c\\\" \;
                echo ${GCOV_EXECUTABLE} \\$$c >> gcov.log \;
                test -e \\$$c && ${GCOV_EXECUTABLE} \\$$c >> gcov.log \;
            done"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running gcov on test sources"
            VERBATIM
        )
    endif()
endif()

# Clean targets
set_property(DIRECTORY APPEND PROPERTY
    ADDITIONAL_MAKE_CLEAN_FILES
    "*~" "*.lo" "*.o" "*.i" "*.gcno" "*.gcda" "*.gcov"
    "t_*.log" "t_*.trs" "*.exe"
    "test-fc.pl" "test-upr.pl"
    "tests-bos.log" "tests-bos.err"
)

if(HAVE_DSYMUTIL)
    set_property(DIRECTORY APPEND PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES "*.dSYM"
    )
endif()

# Print test summary
message(STATUS "Tests configured:")
message(STATUS "  Core tests: ${SAFEC_TESTS}")
message(STATUS "  Performance tests: ${SAFEC_PERF_TESTS}")
if(ENABLE_WCHAR)
    message(STATUS "  Wchar support: enabled")
endif()
if(ENABLE_EXTENSIONS)
    message(STATUS "  Extensions: enabled")
endif()
if(ENABLE_UNSAFE)
    message(STATUS "  Unsafe functions: enabled")
endif()
if(HAVE_ATTRIBUTE_DIAGNOSE_IF)
    message(STATUS "  BOS tests: available via 'tests-bos' target")
endif()
if(VALGRIND_EXECUTABLE AND ENABLE_VALGRIND)
    message(STATUS "  Valgrind tests: enabled")
endif()

