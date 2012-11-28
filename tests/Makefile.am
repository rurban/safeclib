# Makefile.am - Process with automake to produce Makefile.in
#
# 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
#
# Copyright (c) 2012 Cisco Systems
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
#

SAFEC_BUILT_TESTS = \
	t_memcpy32_s          \
	t_memcpy16_s          \
	t_memcpy_s            \
	t_memcmp32_s          \
	t_memcmp16_s          \
	t_memcmp_s            \
	t_memmove32_s         \
	t_memmove16_s         \
	t_memmove_s           \
	t_memset32_s          \
	t_memset16_s          \
	t_memset_s            \
	t_memzero32_s         \
	t_memzero16_s         \
	t_memzero_s           \
	t_strcasecmp_s        \
	t_strcasestr_s        \
	t_strcat_s            \
	t_strcmpfld_s         \
	t_strcmp_s            \
	t_strcpyfldin_s       \
	t_strcpyfldout_s      \
	t_strcpyfld_s         \
	t_strcpy_s            \
	t_strcspn_s           \
	t_strfirstchar_s      \
	t_strfirstdiff_s      \
	t_strfirstsame_s      \
	t_strisalphanumeric_s \
	t_strisascii_s        \
	t_strisdigit_s        \
	t_strishex_s          \
	t_strislowercase_s    \
	t_strismixed_s        \
	t_strispassword_s     \
	t_strisuppercase_s    \
	t_strlastchar_s       \
	t_strlastdiff_s       \
	t_strlastsame_s       \
	t_strljustify_s       \
	t_strncat_s           \
	t_strncpy_s           \
	t_strnlen_s           \
	t_strnterminate_s     \
	t_strpbrk_s           \
	t_strprefix_s         \
	t_strremovews_s       \
	t_strspn_s            \
	t_strstr_s            \
	t_strtok_s            \
	t_strtolowercase_s    \
	t_strtouppercase_s    \
	t_strzero_s

SAFEC_BUILT_PERFS = p_memcpy32_s p_memcpy_s

SAFEC_TEST_COMMON = \
	test_private.h

#AM_LIBADD = @GCOV_LIBS@
# WARNING: tests won't build with -Werror right now because of some
# type-casting issues
#AM_CFLAGS   += -Werror

LDADD = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

TESTS = $(SAFEC_BUILT_TESTS)
check_PROGRAMS = $(SAFEC_BUILT_TESTS)
noinst_PROGRAMS = $(SAFEC_BUILT_PERFS)

p_memcpy32_s_SOURCES          = perf_memcpy32_s.c $(SAFEC_TEST_COMMON)
p_memcpy_s_SOURCES            = perf_memcpy_s.c $(SAFEC_TEST_COMMON)

t_memcpy32_s_SOURCES          = test_memcpy32_s.c $(SAFEC_TEST_COMMON)
t_memcpy16_s_SOURCES          = test_memcpy16_s.c $(SAFEC_TEST_COMMON)
t_memcpy_s_SOURCES            = test_memcpy_s.c $(SAFEC_TEST_COMMON)
t_memcmp32_s_SOURCES          = test_memcmp32_s.c $(SAFEC_TEST_COMMON)
t_memcmp16_s_SOURCES          = test_memcmp16_s.c $(SAFEC_TEST_COMMON)
t_memcmp_s_SOURCES            = test_memcmp_s.c $(SAFEC_TEST_COMMON)
t_memmove32_s_SOURCES         = test_memmove32_s.c $(SAFEC_TEST_COMMON)
t_memmove16_s_SOURCES         = test_memmove16_s.c $(SAFEC_TEST_COMMON)
t_memmove_s_SOURCES           = test_memmove_s.c $(SAFEC_TEST_COMMON)
t_memset32_s_SOURCES          = test_memset32_s.c $(SAFEC_TEST_COMMON)
t_memset16_s_SOURCES          = test_memset16_s.c $(SAFEC_TEST_COMMON)
t_memset_s_SOURCES            = test_memset_s.c $(SAFEC_TEST_COMMON)
t_memzero32_s_SOURCES         = test_memzero32_s.c $(SAFEC_TEST_COMMON)
t_memzero16_s_SOURCES         = test_memzero16_s.c $(SAFEC_TEST_COMMON)
t_memzero_s_SOURCES           = test_memzero_s.c $(SAFEC_TEST_COMMON)
t_strcasecmp_s_SOURCES        = test_strcasecmp_s.c $(SAFEC_TEST_COMMON)
t_strcasestr_s_SOURCES        = test_strcasestr_s.c $(SAFEC_TEST_COMMON)
t_strcat_s_SOURCES            = test_strcat_s.c $(SAFEC_TEST_COMMON)
t_strcmpfld_s_SOURCES         = test_strcmpfld_s.c $(SAFEC_TEST_COMMON)
t_strcmp_s_SOURCES            = test_strcmp_s.c $(SAFEC_TEST_COMMON)
t_strcpyfldin_s_SOURCES       = test_strcpyfldin_s.c $(SAFEC_TEST_COMMON)
t_strcpyfldout_s_SOURCES      = test_strcpyfldout_s.c $(SAFEC_TEST_COMMON)
t_strcpyfld_s_SOURCES         = test_strcpyfld_s.c $(SAFEC_TEST_COMMON)
t_strcpy_s_SOURCES            = test_strcpy_s.c $(SAFEC_TEST_COMMON)
t_strcspn_s_SOURCES           = test_strcspn_s.c $(SAFEC_TEST_COMMON)
t_strfirstchar_s_SOURCES      = test_strfirstchar_s.c $(SAFEC_TEST_COMMON)
t_strfirstdiff_s_SOURCES      = test_strfirstdiff_s.c $(SAFEC_TEST_COMMON)
t_strfirstsame_s_SOURCES      = test_strfirstsame_s.c $(SAFEC_TEST_COMMON)
t_strisalphanumeric_s_SOURCES = test_strisalphanumeric_s.c $(SAFEC_TEST_COMMON)
t_strisascii_s_SOURCES        = test_strisascii_s.c $(SAFEC_TEST_COMMON)
t_strisdigit_s_SOURCES        = test_strisdigit_s.c $(SAFEC_TEST_COMMON)
t_strishex_s_SOURCES          = test_strishex_s.c $(SAFEC_TEST_COMMON)
t_strislowercase_s_SOURCES    = test_strislowercase_s.c $(SAFEC_TEST_COMMON)
t_strismixed_s_SOURCES        = test_strismixed_s.c $(SAFEC_TEST_COMMON)
t_strispassword_s_SOURCES     = test_strispassword_s.c $(SAFEC_TEST_COMMON)
t_strisuppercase_s_SOURCES    = test_strisuppercase_s.c $(SAFEC_TEST_COMMON)
t_strlastchar_s_SOURCES       = test_strlastchar_s.c $(SAFEC_TEST_COMMON)
t_strlastdiff_s_SOURCES       = test_strlastdiff_s.c $(SAFEC_TEST_COMMON)
t_strlastsame_s_SOURCES       = test_strlastsame_s.c $(SAFEC_TEST_COMMON)
t_strljustify_s_SOURCES       = test_strljustify_s.c $(SAFEC_TEST_COMMON)
t_strncat_s_SOURCES           = test_strncat_s.c $(SAFEC_TEST_COMMON)
t_strncpy_s_SOURCES           = test_strncpy_s.c $(SAFEC_TEST_COMMON)
t_strnlen_s_SOURCES           = test_strnlen_s.c $(SAFEC_TEST_COMMON)
t_strnterminate_s_SOURCES     = test_strnterminate_s.c $(SAFEC_TEST_COMMON)
t_strpbrk_s_SOURCES           = test_strpbrk_s.c $(SAFEC_TEST_COMMON)
t_strprefix_s_SOURCES         = test_strprefix_s.c $(SAFEC_TEST_COMMON)
t_strremovews_s_SOURCES       = test_strremovews_s.c $(SAFEC_TEST_COMMON)
t_strspn_s_SOURCES            = test_strspn_s.c $(SAFEC_TEST_COMMON)
t_strstr_s_SOURCES            = test_strstr_s.c $(SAFEC_TEST_COMMON)
t_strtok_s_SOURCES            = test_strtok_s.c $(SAFEC_TEST_COMMON)
t_strtolowercase_s_SOURCES    = test_strtolowercase_s.c $(SAFEC_TEST_COMMON)
t_strtouppercase_s_SOURCES    = test_strtouppercase_s.c $(SAFEC_TEST_COMMON)
t_strzero_s_SOURCES           = test_strzero_s.c $(SAFEC_TEST_COMMON)

CLEANFILES  = *~
