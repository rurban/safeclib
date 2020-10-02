# Makefile.am - Process with automake to produce Makefile.in
#
# 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
# September 2017, Reini Urban
#
# Copyright (c) 2012 Cisco Systems
# Copyright (c) 2017 Reini Urban
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
	t_memcpy_s      \
	t_memmove_s     \
	t_memset_s      \
	t_sprintf_s     \
	t_strcat_s      \
	t_strcpy_s      \
	t_strncat_s     \
	t_strncpy_s     \
	t_strnlen_s     \
	t_strtok_s      \
	t_vsprintf_s	\
	t_sscanf_s	\
	t_strerror_s	\
	t_strerrorlen_s

if ENABLE_WCHAR
SAFEC_BUILT_TESTS += \
	t_mbstowcs_s    \
	t_mbsrtowcs_s	\
	t_wcsrtombs_s   \
	t_wcstombs_s	\
	t_wcrtomb_s	\
	t_wctomb_s	\
	t_wcsnlen_s	\
	t_wcscpy_s	\
	t_wcsncpy_s	\
	t_wcscat_s	\
	t_wcsncat_s	\
	t_wmemcpy_s	\
	t_wmemmove_s	\
	t_wcstok_s	\
	t_vswprintf_s	\
	t_swprintf_s	\
	t_fwprintf_s	\
	t_vfwprintf_s	\
	t_swscanf_s	\
	t_vswscanf_s	\
	t_vwscanf_s	\
	t_fwscanf_s	\
	t_vfwscanf_s	\
	t_wscanf_s
if ENABLE_EXTS
SAFEC_BUILT_TESTS += \
	t_wmemcmp_s	\
	t_wcscmp_s	\
	t_wcsncmp_s	\
	t_wcsicmp_s	\
	t_wcsnatcmp_s	\
	t_wcsstr_s	\
	t_wcsset_s	\
	t_wcsnset_s	\
	t_wcscoll_s	\
	t_wcslwr_s	\
	t_wcsupr_s	\
	t_towfc_s	\
	t_towlower	\
	t_towupper	\
	t_wcsfc_s	\
	t_wcsnorm_s
endif
endif

if ENABLE_UNSAFE
SAFEC_BUILT_TESTS += \
	t_tmpnam_s
if ENABLE_WCHAR
SAFEC_BUILT_TESTS += \
	t_snprintf_s	\
	t_vsnprintf_s   \
	t_snwprintf_s	\
	t_vsnwprintf_s
endif
endif

if ENABLE_EXTS
SAFEC_BUILT_TESTS += \
	t_memcpy32_s          \
	t_memcpy16_s          \
	t_memcmp_s            \
	t_memcmp32_s          \
	t_memcmp16_s          \
	t_memmove32_s         \
	t_memmove16_s         \
	t_memset32_s          \
	t_memset16_s          \
	t_memzero_s           \
	t_memzero32_s         \
	t_memzero16_s         \
	t_strcasecmp_s        \
	t_strcasestr_s        \
	t_strcmp_s            \
	t_strcmpfld_s         \
	t_strcpyfldin_s       \
	t_strcpyfldout_s      \
	t_strcpyfld_s         \
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
	t_strnatcmp_s         \
	t_strnterminate_s     \
	t_strpbrk_s           \
	t_strprefix_s         \
	t_strremovews_s       \
	t_strspn_s            \
	t_strchr_s            \
	t_memchr_s            \
	t_strrchr_s           \
	t_memrchr_s           \
	t_strstr_s            \
	t_strtolowercase_s    \
	t_strtouppercase_s    \
	t_strset_s            \
	t_strnset_s           \
	t_strzero_s	      \
	t_strcoll_s	      \
	t_timingsafe_memcmp   \
	t_timingsafe_bcmp     \
	t_memccpy_s
endif

# 	t_vsscanf_s
SAFEC_BUILT_TESTS += 	\
	t_fprintf_s	\
	t_vprintf_s	\
	t_printf_s	\
	t_vfprintf_s	\
	t_tmpfile_s	\
	t_gets_s	\
	t_fopen_s	\
	t_freopen_s	\
	t_asctime_s	\
	t_ctime_s	\
	t_gmtime_s	\
	t_localtime_s	\
	t_getenv_s	\
	t_bsearch_s	\
	t_qsort_s

# unstable tests
if ENABLE_DEBUG
SAFEC_BUILT_TESTS += 	\
	t_scanf_s	\
	t_fscanf_s	
# 	t_vfscanf_s	\
# 	t_vscanf_s
if ENABLE_WCHAR
SAFEC_BUILT_TESTS += 	\
	t_wprintf_s	\
	t_vwprintf_s
endif
endif

SAFEC_BUILT_PERFS =  p_memcpy_s p_memset_s p_strcpy_s
if ENABLE_EXTS
SAFEC_BUILT_PERFS += p_memcpy32_s
endif

SAFEC_TEST_COMMON = \
	test_private.h
SAFEC_TEST_MSVCRT = \
	test_msvcrt.h \
	test_msvcrt.c

UNIDATA = CaseFolding.txt DerivedGeneralCategory.txt

# WARNING: tests won't build with -Werror right now. A few minor warnings
#AM_CFLAGS += -Werror
AM_CFLAGS  += -I$(top_builddir)/include -I$(srcdir)
# pedantic complains about __FUNCTION__
#if !HAVE_MINGW
#AM_CFLAGS += -pedantic
#endif
# WARN_CFLAGS works mostly, but too strict with gnu extensions
AM_CFLAGS += $(WARN_CFLAGS_TESTS)

LDADD      = $(top_builddir)/src/libsafec.la

TESTS = $(SAFEC_BUILT_TESTS)
check_PROGRAMS = $(SAFEC_BUILT_TESTS)
noinst_PROGRAMS = $(SAFEC_BUILT_PERFS)
noinst_DATA = $(UNIDATA)
DISTCLEANFILES = $(UNIDATA)

# installed on demand by the tests
$(UNIDATA):

$(noinst_DATA):

if HAVE_ATTRIBUTE_DIAGNOSE_IF
# TODO via EXTRA_TESTS. use a specific .bos extension for those test binaries
tests: tests-bos $(TESTS)

tests-bos:
	-rm test_*.o* 2>/dev/null
	+$(MAKE) -j1 $(TESTS) DEFS="$(DEFS) -DTEST_BOS" 2>&1 | tee tests-bos.log | grep warning:
	perl -n tests-bos.pl tests-bos.log
	-rm test_*.o* 2>/dev/null

#.c.obos:
#	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
#	$(COMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -o $@ $<
#.obos.bos:
#	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
#	$(COMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -o $@ $<
else
tests: $(TESTS)
endif

.c.i:
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	$(COMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -E -o $@ $<

.c.ii:  # no linemarkers
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	$(COMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -E -P -o $@ $<

if ENABLE_GCOV
AM_CFLAGS += @GCOV_CFLAGS@
LIBS      += @GCOV_LIBS@
#  -e's,$(EXEEXT),,'
gcov: $(TESTS)
	-test -f gcov.log && rm gcov.log
	for f in $(TESTS); do \
	  c="`echo $$f|sed -e's,t_,test_,'`.c"; \
	  echo $(GCOV) $$c; test -e $$c && $(GCOV) $$c >>gcov.log; done
endif

# problematic with asan
# if ENABLE_DEBUG
# if HAVE_DSYMUTIL
# LIBS += && $(DSYMUTIL) $@
# endif
# endif

p_memcpy_s_SOURCES            = perf_memcpy_s.c $(SAFEC_TEST_COMMON) perf_private.h
p_memset_s_SOURCES            = perf_memset_s.c $(SAFEC_TEST_COMMON) perf_private.h
p_memset_s_CFLAGS             = $(CFLAGS) $(AM_CFLAGS) -I$(top_srcdir)/src
# needed for windows
p_memset_s_LDFLAGS            = $(top_builddir)/src/mem/mem_primitives_lib.lo $(LDADD)
p_strcpy_s_SOURCES            = perf_strcpy_s.c $(SAFEC_TEST_COMMON) perf_private.h

t_memcpy_s_SOURCES            = test_memcpy_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT) test_expmem.h
t_memmove_s_SOURCES           = test_memmove_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT) test_expmem.h
t_memset_s_SOURCES            = test_memset_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_sprintf_s_SOURCES           = test_sprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_strcat_s_SOURCES            = test_strcat_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_strcpy_s_SOURCES            = test_strcpy_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_strcspn_s_SOURCES           = test_strcspn_s.c $(SAFEC_TEST_COMMON)
t_strncat_s_SOURCES           = test_strncat_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_strncpy_s_SOURCES           = test_strncpy_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_strnlen_s_SOURCES           = test_strnlen_s.c $(SAFEC_TEST_COMMON)
t_strtok_s_SOURCES            = test_strtok_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_vsprintf_s_SOURCES          = test_vsprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_sscanf_s_SOURCES            = test_sscanf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_scanf_s_SOURCES             = test_scanf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_fscanf_s_SOURCES            = test_fscanf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
#t_vfscanf_s_SOURCES           = test_vfscanf_s.c $(SAFEC_TEST_COMMON)
#t_vscanf_s_SOURCES            = test_vscanf_s.c $(SAFEC_TEST_COMMON)
#t_vsscanf_s_SOURCES           = test_vsscanf_s.c $(SAFEC_TEST_COMMON)
t_printf_s_SOURCES            = test_printf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_vfprintf_s_SOURCES          = test_vfprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_vprintf_s_SOURCES           = test_vprintf_s.c $(SAFEC_TEST_COMMON)
t_fprintf_s_SOURCES           = test_fprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_strerror_s_SOURCES          = test_strerror_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_strerrorlen_s_SOURCES       = test_strerrorlen_s.c $(SAFEC_TEST_COMMON)
t_tmpfile_s_SOURCES           = test_tmpfile_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_gets_s_SOURCES              = test_gets_s.c $(SAFEC_TEST_COMMON)
t_fopen_s_SOURCES             = test_fopen_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_freopen_s_SOURCES           = test_freopen_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_asctime_s_SOURCES           = test_asctime_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_ctime_s_SOURCES             = test_ctime_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_gmtime_s_SOURCES            = test_gmtime_s.c $(SAFEC_TEST_COMMON)
t_localtime_s_SOURCES         = test_localtime_s.c $(SAFEC_TEST_COMMON)
t_getenv_s_SOURCES            = test_getenv_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_bsearch_s_SOURCES           = test_bsearch_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_qsort_s_SOURCES             = test_qsort_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)

if ENABLE_WCHAR
t_mbstowcs_s_SOURCES          = test_mbstowcs_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_mbsrtowcs_s_SOURCES         = test_mbsrtowcs_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wcsrtombs_s_SOURCES         = test_wcsrtombs_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wcstombs_s_SOURCES          = test_wcstombs_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wcsnlen_s_SOURCES           = test_wcsnlen_s.c $(SAFEC_TEST_COMMON)
t_wcscpy_s_SOURCES            = test_wcscpy_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wcsncpy_s_SOURCES           = test_wcsncpy_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wcscat_s_SOURCES            = test_wcscat_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wcsncat_s_SOURCES           = test_wcsncat_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wmemcpy_s_SOURCES           = test_wmemcpy_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT) test_expmem.h
t_wmemmove_s_SOURCES          = test_wmemmove_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT) test_expmem.h
t_wcrtomb_s_SOURCES           = test_wcrtomb_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wctomb_s_SOURCES            = test_wctomb_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wcstok_s_SOURCES            = test_wcstok_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_vswprintf_s_SOURCES         = test_vswprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_swprintf_s_SOURCES          = test_swprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wprintf_s_SOURCES           = test_wprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_fwprintf_s_SOURCES          = test_fwprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_vfwprintf_s_SOURCES         = test_vfwprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_vwprintf_s_SOURCES          = test_vwprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_swscanf_s_SOURCES           = test_swscanf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_vswscanf_s_SOURCES          = test_vswscanf_s.c $(SAFEC_TEST_COMMON)
t_vwscanf_s_SOURCES           = test_vwscanf_s.c $(SAFEC_TEST_COMMON)
t_fwscanf_s_SOURCES           = test_fwscanf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_vfwscanf_s_SOURCES          = test_vfwscanf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_wscanf_s_SOURCES            = test_wscanf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
if ENABLE_EXTS
t_wmemcmp_s_SOURCES           = test_wmemcmp_s.c $(SAFEC_TEST_COMMON)
t_wcscmp_s_SOURCES            = test_wcscmp_s.c $(SAFEC_TEST_COMMON)
t_wcsncmp_s_SOURCES           = test_wcsncmp_s.c $(SAFEC_TEST_COMMON)
t_wcsicmp_s_SOURCES           = test_wcsicmp_s.c $(SAFEC_TEST_COMMON)
t_wcsnatcmp_s_SOURCES         = test_wcsnatcmp_s.c $(SAFEC_TEST_COMMON)
t_wcsstr_s_SOURCES            = test_wcsstr_s.c $(SAFEC_TEST_COMMON)
t_wcsset_s_SOURCES            = test_wcsset_s.c $(SAFEC_TEST_COMMON)
t_wcsnset_s_SOURCES           = test_wcsnset_s.c $(SAFEC_TEST_COMMON)
t_wcscoll_s_SOURCES           = test_wcscoll_s.c $(SAFEC_TEST_COMMON)
t_wcslwr_s_SOURCES            = test_wcslwr_s.c $(SAFEC_TEST_COMMON)
t_wcsupr_s_SOURCES            = test_wcsupr_s.c $(SAFEC_TEST_COMMON)
t_towfc_s_SOURCES             = test_towfc_s.c $(SAFEC_TEST_COMMON)
t_towlower_SOURCES            = test_towlower.c $(SAFEC_TEST_COMMON)
t_towupper_SOURCES            = test_towupper.c $(SAFEC_TEST_COMMON)
t_wcsfc_s_SOURCES             = test_wcsfc_s.c $(SAFEC_TEST_COMMON)
t_wcsnorm_s_SOURCES           = test_wcsnorm_s.c $(SAFEC_TEST_COMMON)
endif
endif

if ENABLE_EXTS
p_memcpy32_s_SOURCES          = perf_memcpy32_s.c $(SAFEC_TEST_COMMON)
t_memcpy32_s_SOURCES          = test_memcpy32_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_memcpy16_s_SOURCES          = test_memcpy16_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_memcmp_s_SOURCES            = test_memcmp_s.c $(SAFEC_TEST_COMMON)
t_memcmp32_s_SOURCES          = test_memcmp32_s.c $(SAFEC_TEST_COMMON)
t_memcmp16_s_SOURCES          = test_memcmp16_s.c $(SAFEC_TEST_COMMON)
t_memmove32_s_SOURCES         = test_memmove32_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_memmove16_s_SOURCES         = test_memmove16_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_memset32_s_SOURCES          = test_memset32_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_memset16_s_SOURCES          = test_memset16_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_memzero_s_SOURCES           = test_memzero_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_memzero32_s_SOURCES         = test_memzero32_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_memzero16_s_SOURCES         = test_memzero16_s.c $(SAFEC_TEST_COMMON) test_expmem.h
t_strcasecmp_s_SOURCES        = test_strcasecmp_s.c $(SAFEC_TEST_COMMON)
t_strcasestr_s_SOURCES        = test_strcasestr_s.c $(SAFEC_TEST_COMMON)
t_strcmpfld_s_SOURCES         = test_strcmpfld_s.c $(SAFEC_TEST_COMMON)
t_strcmp_s_SOURCES            = test_strcmp_s.c $(SAFEC_TEST_COMMON)
t_strcpyfldin_s_SOURCES       = test_strcpyfldin_s.c $(SAFEC_TEST_COMMON)
t_strcpyfldout_s_SOURCES      = test_strcpyfldout_s.c $(SAFEC_TEST_COMMON)
t_strcpyfld_s_SOURCES         = test_strcpyfld_s.c $(SAFEC_TEST_COMMON)
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
t_strnatcmp_s_SOURCES         = test_strnatcmp_s.c $(SAFEC_TEST_COMMON)
t_strnterminate_s_SOURCES     = test_strnterminate_s.c $(SAFEC_TEST_COMMON)
t_strpbrk_s_SOURCES           = test_strpbrk_s.c $(SAFEC_TEST_COMMON)
t_strprefix_s_SOURCES         = test_strprefix_s.c $(SAFEC_TEST_COMMON)
t_strremovews_s_SOURCES       = test_strremovews_s.c $(SAFEC_TEST_COMMON)
t_strspn_s_SOURCES            = test_strspn_s.c $(SAFEC_TEST_COMMON)
t_strchr_s_SOURCES            = test_strchr_s.c $(SAFEC_TEST_COMMON)
t_memchr_s_SOURCES            = test_memchr_s.c $(SAFEC_TEST_COMMON)
t_strrchr_s_SOURCES           = test_strrchr_s.c $(SAFEC_TEST_COMMON)
t_memrchr_s_SOURCES           = test_memrchr_s.c $(SAFEC_TEST_COMMON)
t_strstr_s_SOURCES            = test_strstr_s.c $(SAFEC_TEST_COMMON)
t_strtolowercase_s_SOURCES    = test_strtolowercase_s.c $(SAFEC_TEST_COMMON)
t_strtouppercase_s_SOURCES    = test_strtouppercase_s.c $(SAFEC_TEST_COMMON)
t_strzero_s_SOURCES           = test_strzero_s.c $(SAFEC_TEST_COMMON)
t_strset_s_SOURCES            = test_strset_s.c $(SAFEC_TEST_COMMON)
t_strnset_s_SOURCES           = test_strnset_s.c $(SAFEC_TEST_COMMON)
t_strcoll_s_SOURCES           = test_strcoll_s.c $(SAFEC_TEST_COMMON)
t_timingsafe_memcmp_SOURCES   = test_timingsafe_memcmp.c $(SAFEC_TEST_COMMON)
t_timingsafe_bcmp_SOURCES     = test_timingsafe_bcmp.c $(SAFEC_TEST_COMMON)
t_memccpy_s_SOURCES           = test_memccpy_s.c $(SAFEC_TEST_COMMON) test_expmem.h
endif

if ENABLE_UNSAFE
t_tmpnam_s_SOURCES            = test_tmpnam_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
if ENABLE_WCHAR
t_snprintf_s_SOURCES          = test_snprintf_s.c $(SAFEC_TEST_COMMON)
t_vsnprintf_s_SOURCES         = test_vsnprintf_s.c $(SAFEC_TEST_COMMON) \
	                        $(SAFEC_TEST_MSVCRT)
t_snwprintf_s_SOURCES         = test_snwprintf_s.c $(SAFEC_TEST_COMMON)
t_vsnwprintf_s_SOURCES        = test_vsnwprintf_s.c $(SAFEC_TEST_COMMON)
endif
endif

@VALGRIND_CHECK_RULES@
#VALGRIND_SUPPRESSIONS_FILES = $(PACKAGE).supp
#EXTRA_DIST = $(PACKAGE).supp

# emacs flymake-mode
check-syntax:
	test -n "$(CHK_SOURCES)" && \
	  $(COMPILE) -o /dev/null -S $(CHK_SOURCES)
if HAVE_ATTRIBUTE_DIAGNOSE_IF
.PHONY: check-syntax tests-bos
else
.PHONY: check-syntax
endif

if HAVE_DSYMUTIL
clean-local:
	rm -rf *.dSYM
endif
CLEANFILES  = *~ *.lo *.o *.i t_*.log t_*.trs *.exe test-fc.pl test-upr.pl
CLEANFILES  += *.gcno *.gcda
if ENABLE_GCOV
CLEANFILES  += *.gcov gcov.log
endif
if HAVE_ATTRIBUTE_DIAGNOSE_IF
CLEANFILES  += tests-bos.log tests-bos.err
endif
