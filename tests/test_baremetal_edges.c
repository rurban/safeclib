/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 * @brief C11 Annex K Bare-Metal Edge Case Test Suite
 * @ingroup safeclib
 */

/*
 * Copyright (C) 2026 Wayne Michael Thornton <wmthornton-dev@outlook.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <safe_lib.h>
#include <safe_str_lib.h>
#include <safe_mem_lib.h>

#ifndef ESNOSPC
#define ESNOSPC 406
#endif
#ifndef ESLEMAX
#define ESLEMAX 403
#endif
#ifndef ESNULLP
#define ESNULLP 400
#endif
#ifndef ESOVRLP
#define ESOVRLP 402
#endif
#ifndef ESZEROL
#define ESZEROL 401
#endif

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

static void check_test(const char* test_name, errno_t actual, errno_t expected) {
    tests_run++;
    if (actual == expected) {
        tests_passed++;
    } else {
        tests_failed++;
        printf("[FAIL] %s: Expected errno %d, got %d\n", test_name, expected, actual);
    }
}

static void check_int(const char* test_name, int actual, int expected) {
    tests_run++;
    if (actual == expected) {
        tests_passed++;
    } else {
        tests_failed++;
        printf("[FAIL] %s: Expected value %d, got %d\n", test_name, expected, actual);
    }
}

static void test_string_copy_concat(void) {
    char dest[10];
    errno_t rc;

    printf("Running String Copy & Concatenation Suite...\n");

    rc = strcpy_s(dest, sizeof(dest), "RTEMS");
    check_test("strcpy_s valid", rc, EOK);
    check_test("strcpy_s content", strcmp(dest, "RTEMS"), 0);

    rc = strcpy_s(dest, sizeof(dest), "This is way too long for a 10-byte buffer!");
    check_test("strcpy_s overflow caught", rc, ESNOSPC);
    check_test("strcpy_s dest zeroed on error", dest[0], '\0');

    rc = strcpy_s(NULL, 10, "test");
    check_test("strcpy_s NULL dest caught", rc, ESNULLP);

    rc = strncpy_s(dest, sizeof(dest), "SPARC-ERC32-Target", 5);
    check_test("strncpy_s valid truncation", rc, EOK);
    check_test("strncpy_s content", strcmp(dest, "SPARC"), 0);

    strcpy_s(dest, sizeof(dest), "RTEMS ");
    rc = strcat_s(dest, sizeof(dest), "7");
    check_test("strcat_s valid", rc, EOK);
    check_test("strcat_s content", strcmp(dest, "RTEMS 7"), 0);

    rc = strcat_s(dest, sizeof(dest), " - Extra Long String");
    check_test("strcat_s overflow caught", rc, ESNOSPC);
    check_test("strcat_s dest zeroed on error", dest[0], '\0');

    /* --- NEW STRING EDGE CASE TESTS --- */
    rc = strcpy_s(dest, sizeof(dest), NULL);
    check_test("strcpy_s NULL src caught", rc, ESNULLP);
    check_test("strcpy_s dest zeroed on NULL src", dest[0], '\0');

    rc = strcpy_s(dest, 0, "test");
    check_test("strcpy_s zero dmax caught", rc, ESZEROL);

    rc = strcat_s(NULL, sizeof(dest), "test");
    check_test("strcat_s NULL dest caught", rc, ESNULLP);

    rc = strcat_s(dest, sizeof(dest), NULL);
    check_test("strcat_s NULL src caught", rc, ESNULLP);
    check_test("strcat_s dest zeroed on NULL src", dest[0], '\0');
}

static void test_string_length(void) {
    rsize_t len = 0;
    printf("Running String Length Suite...\n");
    len = strnlen_s("RTEMS", 100);
    check_int("strnlen_s normal string", (int)len, 5);
    len = strnlen_s("RTEMS", 3);
    check_int("strnlen_s max limit enforced", (int)len, 3);
    len = strnlen_s(NULL, 100);
    check_int("strnlen_s NULL handled safely", (int)len, 0);
}

static void test_memory_operations(void) {
    uint8_t src[16] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                        0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00 };
    uint8_t dest[16];
    errno_t rc;

    printf("Running Memory Manipulation Suite...\n");

    rc = memset_s(dest, sizeof(dest), 0xAA, sizeof(dest));
    check_test("memset_s valid", rc, EOK);
    check_int("memset_s byte verification", dest[7], 0xAA);

    rc = memset_s(dest, RSIZE_MAX_MEM + 1, 0xBB, 16);
    check_test("memset_s max size rejection", rc, ESLEMAX);

    rc = memcpy_s(dest, sizeof(dest), src, sizeof(src));
    check_test("memcpy_s valid", rc, EOK);
    check_int("memcpy_s data integrity", dest[3], 0x44);

    rc = memcpy_s(dest, 4, src, 8);
    check_test("memcpy_s overflow caught", rc, ESNOSPC);
    check_int("memcpy_s dest zeroed on error", dest[0], 0x00);

    uint8_t overlap_buf[10] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
    rc = memmove_s(&overlap_buf[2], 8, &overlap_buf[0], 5);
    check_test("memmove_s overlapping copy", rc, EOK);
    check_test("memmove_s overlap integrity", overlap_buf[2] == 'A' && overlap_buf[6] == 'E', 1);

    /* --- NEW MEMORY EDGE CASE TESTS --- */
    rc = memcpy_s(NULL, sizeof(dest), src, sizeof(src));
    check_test("memcpy_s NULL dest caught", rc, ESNULLP);

    rc = memcpy_s(dest, sizeof(dest), NULL, sizeof(src));
    check_test("memcpy_s NULL src caught", rc, ESNULLP);
    check_int("memcpy_s dest zeroed on NULL src", dest[0], 0x00);

    uint8_t copy_overlap[10] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
    rc = memcpy_s(&copy_overlap[2], 8, &copy_overlap[0], 5);
    check_test("memcpy_s overlap rejection", rc, ESOVRLP);
    check_int("memcpy_s dest zeroed on overlap", copy_overlap[2], 0x00);

    rc = memset_s(NULL, sizeof(dest), 0xAA, sizeof(dest));
    check_test("memset_s NULL dest caught", rc, ESNULLP);

    rc = memset_s(dest, 0, 0xAA, sizeof(dest));
    check_test("memset_s zero dmax no-op (C11 standard)", rc, EOK);
}

static void test_tokenization(void) {
    char str[] = "SPARC,ERC32,RTEMS,7";
    rsize_t len = strnlen_s(str, sizeof(str));
    char *next_token = NULL;
    char *token = NULL;
    int token_count = 0;

    printf("Running String Tokenization Suite...\n");

    token = strtok_s(str, &len, ",", &next_token);
    while (token != NULL) {
        token_count++;
        if (token_count == 1) check_int("strtok_s token 1", strcmp(token, "SPARC"), 0);
        else if (token_count == 2) check_int("strtok_s token 2", strcmp(token, "ERC32"), 0);
        else if (token_count == 3) check_int("strtok_s token 3", strcmp(token, "RTEMS"), 0);
        else if (token_count == 4) check_int("strtok_s token 4", strcmp(token, "7"), 0);
        else {
            tests_failed++; tests_run++;
            printf("[FAIL] strtok_s unexpected extra token: %s\n", token);
        }
        token = strtok_s(NULL, &len, ",", &next_token);
    }
    check_int("strtok_s loop termination count", token_count, 4);
    check_int("strtok_s len tracking reached zero", (int)len, 0);
}

static void test_formatted_io(void) {
    char buf[32];
    int chars_written;

    printf("Running Formatted Output Suite...\n");

    chars_written = sprintf_s(buf, sizeof(buf), "RTEMS %d on %s", 7, "ERC32");
    tests_run++;
    if (chars_written == 16 && strcmp(buf, "RTEMS 7 on ERC32") == 0) {
        tests_passed++;
    } else {
        tests_failed++;
        printf("[FAIL] sprintf_s valid formatting failed. Wrote: %d\n", chars_written);
    }

    chars_written = sprintf_s(buf, 10, "This string is way too long for a 10 byte buffer");
    tests_run++;
    if (chars_written < 0 && buf[0] == '\0') {
        tests_passed++;
    } else {
        tests_failed++;
        printf("[FAIL] sprintf_s overflow was NOT caught! Return: %d\n", chars_written);
    }
}

int main(void) {
    printf("\n==================================================\n");
    printf("     Starting C11 Annex K Bare-Metal Test Suite     \n");
    printf("==================================================\n\n");

    test_string_copy_concat();
    test_string_length();
    test_memory_operations();
    test_tokenization();
    test_formatted_io();

    printf("\n==================================================\n");
    printf("                 TEST RESULTS                     \n");
    printf("==================================================\n");
    printf(" Total Tests Run: %d\n", tests_run);
    printf(" Passed:          %d\n", tests_passed);
    printf(" Failed:          %d\n", tests_failed);
    printf("==================================================\n\n");

    if (tests_failed == 0) {
        printf("SUCCESS: All standard C11 Annex K bounds-checking paths validated!\n");
        return EXIT_SUCCESS;
    } else {
        printf("FAILURE: One or more safeclib tests failed. Check log above.\n");
        return EXIT_FAILURE;
    }
}

