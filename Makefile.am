# Makefile.am - Process with automake to produce Makefile.in
#
# 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
#
# Copyright (c) 2012, 2013 Cisco Systems
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

# Additional flags to pass to aclocal when it is invoked automatically at
# make time. This allows libtool to install its m4 macros.
# The ${ACLOCAL_FLAGS} variable is picked up from the environment
# to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = -I m4 --install ${ACLOCAL_FLAGS}

# Sub-directories to preform recursive make in
SUBDIRS = src tests

# Library header files
pkginclude_HEADERS    = \
	include/safe_compile.h  \
	include/safe_lib.h     \
	include/safe_mem_lib.h \
	include/safe_str_lib.h

# generated package headers that need to be generated every time
dist_pkginclude_HEADERS = \
	include/safe_config.h   \
	include/safe_types.h   \
	include/safe_lib_errno.h

if ENABLE_U8
pkginclude_HEADERS    += \
	include/safe_u8_lib.h
endif

# Support files
SAFEC_INFRA = \
	$(top_srcdir)/README.md                  \
	README.rst                               \
	${top_srcdir}/build-aux/autogen.sh       \
	${top_srcdir}/build-aux/msys2.bat        \
	${top_srcdir}/build-aux/smoke.sh         \
	$(top_srcdir)/build-aux/version-gen.sh   \
	$(top_srcdir)/doc/libc-overview.md     	   \
	$(top_srcdir)/scripts                      \
	$(top_srcdir)/.version                     \
	$(top_srcdir)/libsafec.pc.in

SAFEC_GENHEADERS = \
	$(top_srcdir)/include/safe_config.h.in     \
	$(top_srcdir)/include/safe_lib_errno.h.in  \
	$(top_srcdir)/include/safe_types.h.in

SAFEC_SLKM = \
	$(top_srcdir)/tests/test_slkm.c            \
	$(top_srcdir)/src/slkm                     \
	$(top_srcdir)/Kbuild                       \
	$(top_srcdir)/Makefile.kernel

bin_SCRIPTS = \
	$(top_srcdir)/scripts/check_for_unsafe_apis

# Additional things to distribute
EXTRA_DIST = $(SAFEC_INFRA) $(SAFEC_GENHEADERS) $(SAFEC_SLKM)

BUILT_SOURCES = $(top_srcdir)/.version

# Install the generated pkg-config file (.pc) into the expected location for
# architecture-dependent package configuration information.  Occasionally,
# pkg-config files are also used for architecture-independent data packages,
# in which case the correct install location would be $(datadir)/pkgconfig.
#      pkgconfigdir = $(libdir)/pkgconfig
#      pkgconfig_DATA = safec-$(SAFEC_API_VERSION).pc
# We need to be architecture-dependent:
pcdatadir = $(libdir)/pkgconfig
pcdata_DATA = libsafec.pc

CLEANFILES = *~
LIBTOOL_DISTCLEAN_FILES = \
	m4/libtool.m4 m4/ltoptions.m4 m4/ltsugar.m4 m4/ltversion.m4 m4/lt~obsolete.m4 \
	build-aux/ar-lib \
	build-aux/compile \
	build-aux/config.guess \
	build-aux/config.sub \
	build-aux/depcomp \
	build-aux/install-sh \
	build-aux/ltmain.sh \
	build-aux/missing \
	build-aux/test-driver
DISTCLEANFILES = .version m4/pkg.m4

if ENABLE_DOC
MAN1_SOURCES = doc/man/man1/check_for_unsafe_apis.1
dist_man_MANS =
if HAVE_POD2MAN
dist_man_MANS += $(MAN1_SOURCES)

$(MAN1_SOURCES) :: $(srcdir)/scripts/check_for_unsafe_apis
	-mkdir -p doc/man/man1
	-$(POD2MAN) $(srcdir)/scripts/check_for_unsafe_apis >$@

endif
if HAVE_DOXYGEN
# missing doc/man/man3/RSIZE_MAX_WSTR.3
dist_man_MANS += \
	doc/man/man3/EOK.3	\
	doc/man/man3/ESEMPTY.3	\
	doc/man/man3/ESLAST.3	\
	doc/man/man3/ESLEMAX.3	\
	doc/man/man3/ESLEMIN.3	\
	doc/man/man3/ESLEWRNG.3	\
	doc/man/man3/ESNODIFF.3	\
	doc/man/man3/ESNOSPC.3	\
	doc/man/man3/ESNOTFND.3	\
	doc/man/man3/ESNULLP.3	\
	doc/man/man3/ESOVRLP.3	\
	doc/man/man3/ESUNTERM.3	\
	doc/man/man3/ESZEROL.3	\
	doc/man/man3/L_tmpnam_s.3	\
	doc/man/man3/RSIZE_MAX.3	\
	doc/man/man3/RSIZE_MAX_MEM.3	\
	doc/man/man3/RSIZE_MAX_MEM16.3	\
	doc/man/man3/RSIZE_MAX_MEM32.3	\
	doc/man/man3/RSIZE_MAX_STR.3	\
	doc/man/man3/RSIZE_MAX_WSTR.3	\
	doc/man/man3/RSIZE_MIN_STR.3	\
	doc/man/man3/SAFECLIB_STR_NULL_SLACK.3	\
	doc/man/man3/SAFE_STR_MIN_LOWERCASE.3	\
	doc/man/man3/SAFE_STR_MIN_NUMBERS.3	\
	doc/man/man3/SAFE_STR_MIN_SPECIALS.3	\
	doc/man/man3/SAFE_STR_MIN_UPPERCASE.3	\
	doc/man/man3/SAFE_STR_PASSWORD_MAX_LENGTH.3	\
	doc/man/man3/SAFE_STR_PASSWORD_MIN_LENGTH.3	\
	doc/man/man3/STRTOK_DELIM_MAX_LEN.3	\
	doc/man/man3/TMP_MAX_S.3	\
	doc/man/man3/abort_handler_s.3	\
	doc/man/man3/abort_handler_s.c.3\
	doc/man/man3/asctime_s.3	\
	doc/man/man3/asctime_s.c.3	\
	doc/man/man3/bsearch_s.3	\
	doc/man/man3/bsearch_s.c.3	\
	doc/man/man3/constraint_handler_t.3	\
	doc/man/man3/ctime_s.3		\
	doc/man/man3/ctime_s.c.3	\
	doc/man/man3/fopen_s.3		\
	doc/man/man3/fopen_s.c.3	\
	doc/man/man3/fprintf_s.3	\
	doc/man/man3/fprintf_s.c.3	\
	doc/man/man3/freopen_s.3	\
	doc/man/man3/freopen_s.c.3	\
	doc/man/man3/fscanf_s.3		\
	doc/man/man3/fscanf_s.c.3	\
	doc/man/man3/getenv_s.3		\
	doc/man/man3/getenv_s.c.3	\
	doc/man/man3/gets_s.3		\
	doc/man/man3/gets_s.c.3		\
	doc/man/man3/gmtime_s.3		\
	doc/man/man3/gmtime_s.c.3	\
	doc/man/man3/ignore_handler_s.3	\
	doc/man/man3/ignore_handler_s.c.3	\
	doc/man/man3/invoke_safe_mem_constraint_handler.3	\
	doc/man/man3/invoke_safe_str_constraint_handler.3	\
	doc/man/man3/localtime_s.3	\
	doc/man/man3/localtime_s.c.3	\
	doc/man/man3/mem_handler.3	\
	doc/man/man3/mem_prim_move.3	\
	doc/man/man3/mem_prim_move16.3	\
	doc/man/man3/mem_prim_move32.3	\
	doc/man/man3/mem_prim_move8.3	\
	doc/man/man3/mem_prim_set.3	\
	doc/man/man3/mem_prim_set16.3	\
	doc/man/man3/mem_prim_set32.3	\
	doc/man/man3/mem_primitives_lib.c.3	\
	doc/man/man3/mem_primitives_lib.h.3	\
	doc/man/man3/memccpy_s.3	\
	doc/man/man3/memccpy_s.c.3	\
	doc/man/man3/memchr_s.3		\
	doc/man/man3/memchr_s.c.3	\
	doc/man/man3/memcmp16_s.3	\
	doc/man/man3/memcmp16_s.c.3	\
	doc/man/man3/memcmp32_s.3	\
	doc/man/man3/memcmp32_s.c.3	\
	doc/man/man3/memcmp_s.3		\
	doc/man/man3/memcmp_s.c.3	\
	doc/man/man3/memcpy16_s.3	\
	doc/man/man3/memcpy16_s.c.3	\
	doc/man/man3/memcpy32_s.3	\
	doc/man/man3/memcpy32_s.c.3	\
	doc/man/man3/memcpy_s.3		\
	doc/man/man3/memcpy_s.c.3	\
	doc/man/man3/memmove16_s.3	\
	doc/man/man3/memmove16_s.c.3	\
	doc/man/man3/memmove32_s.3	\
	doc/man/man3/memmove32_s.c.3	\
	doc/man/man3/memmove_s.3	\
	doc/man/man3/memmove_s.c.3	\
	doc/man/man3/memrchr_s.3	\
	doc/man/man3/memrchr_s.c.3	\
	doc/man/man3/memset16_s.3	\
	doc/man/man3/memset16_s.c.3	\
	doc/man/man3/memset32_s.3	\
	doc/man/man3/memset32_s.c.3	\
	doc/man/man3/memset_s.3		\
	doc/man/man3/memset_s.c.3	\
	doc/man/man3/memzero16_s.3	\
	doc/man/man3/memzero16_s.c.3	\
	doc/man/man3/memzero32_s.3	\
	doc/man/man3/memzero32_s.c.3	\
	doc/man/man3/memzero_s.3	\
	doc/man/man3/memzero_s.c.3	\
	doc/man/man3/printf_s.3		\
	doc/man/man3/printf_s.c.3	\
	doc/man/man3/qsort_s.3		\
	doc/man/man3/qsort_s.c.3	\
	doc/man/man3/rsize_t.3		\
	doc/man/man3/safe_compile.h.3	\
	doc/man/man3/safe_config.h.3	\
	doc/man/man3/safe_lib.h.3	\
	doc/man/man3/safe_lib_errno.h.3	\
	doc/man/man3/safe_mem_constraint.c.3	\
	doc/man/man3/safe_mem_constraint.h.3	\
	doc/man/man3/safe_mem_lib.h.3		\
	doc/man/man3/safe_str_constraint.c.3	\
	doc/man/man3/safe_str_constraint.h.3	\
	doc/man/man3/safe_str_lib.h.3	\
	doc/man/man3/safe_types.h.3	\
	doc/man/man3/scanf_s.3		\
	doc/man/man3/scanf_s.c.3	\
	doc/man/man3/set_mem_constraint_handler_s.3	\
	doc/man/man3/set_str_constraint_handler_s.3	\
	doc/man/man3/snprintf_s.3	\
	doc/man/man3/snprintf_s.c.3	\
	doc/man/man3/sprintf_s.3	\
	doc/man/man3/sprintf_s.c.3	\
	doc/man/man3/sscanf_s.3		\
	doc/man/man3/sscanf_s.c.3	\
	doc/man/man3/str_handler.3	\
	doc/man/man3/strcasecmp_s.3	\
	doc/man/man3/strcasecmp_s.c.3	\
	doc/man/man3/strcasestr_s.3	\
	doc/man/man3/strcasestr_s.c.3	\
	doc/man/man3/strcat_s.3	\
	doc/man/man3/strcat_s.c.3	\
	doc/man/man3/strchr_s.3	\
	doc/man/man3/strchr_s.c.3	\
	doc/man/man3/strcmp_s.3	\
	doc/man/man3/strcmp_s.c.3	\
	doc/man/man3/strcmpfld_s.3	\
	doc/man/man3/strcmpfld_s.c.3	\
	doc/man/man3/strcoll_s.3	\
	doc/man/man3/strcoll_s.c.3	\
	doc/man/man3/strcpy_s.3	\
	doc/man/man3/strcpy_s.c.3	\
	doc/man/man3/strcpyfld_s.3	\
	doc/man/man3/strcpyfld_s.c.3	\
	doc/man/man3/strcpyfldin_s.3	\
	doc/man/man3/strcpyfldin_s.c.3	\
	doc/man/man3/strcpyfldout_s.3	\
	doc/man/man3/strcpyfldout_s.c.3	\
	doc/man/man3/strcspn_s.3	\
	doc/man/man3/strcspn_s.c.3	\
	doc/man/man3/strerror_s.3	\
	doc/man/man3/strerror_s.c.3	\
	doc/man/man3/strerror_s.h.3	\
	doc/man/man3/strerrorlen_s.3	\
	doc/man/man3/strfirstchar_s.3	\
	doc/man/man3/strfirstchar_s.c.3	\
	doc/man/man3/strfirstdiff_s.3	\
	doc/man/man3/strfirstdiff_s.c.3	\
	doc/man/man3/strfirstsame_s.3	\
	doc/man/man3/strfirstsame_s.c.3	\
	doc/man/man3/strisalphanumeric_s.3	\
	doc/man/man3/strisalphanumeric_s.c.3	\
	doc/man/man3/strisascii_s.3	\
	doc/man/man3/strisascii_s.c.3	\
	doc/man/man3/strisdigit_s.3	\
	doc/man/man3/strisdigit_s.c.3	\
	doc/man/man3/strishex_s.3	\
	doc/man/man3/strishex_s.c.3	\
	doc/man/man3/strislowercase_s.3	\
	doc/man/man3/strislowercase_s.c.3	\
	doc/man/man3/strismixedcase_s.3		\
	doc/man/man3/strismixedcase_s.c.3	\
	doc/man/man3/strispassword_s.3		\
	doc/man/man3/strispassword_s.c.3	\
	doc/man/man3/strisuppercase_s.3		\
	doc/man/man3/strisuppercase_s.c.3	\
	doc/man/man3/strlastchar_s.3	\
	doc/man/man3/strlastchar_s.c.3	\
	doc/man/man3/strlastdiff_s.3	\
	doc/man/man3/strlastdiff_s.c.3	\
	doc/man/man3/strlastsame_s.3	\
	doc/man/man3/strlastsame_s.c.3	\
	doc/man/man3/strljustify_s.3	\
	doc/man/man3/strljustify_s.c.3	\
	doc/man/man3/strlwr_s.3		\
	doc/man/man3/strnatcasecmp_s.3	\
	doc/man/man3/strnatcmp_s.3	\
	doc/man/man3/strnatcmp_s.c.3	\
	doc/man/man3/strncat_s.3	\
	doc/man/man3/strncat_s.c.3	\
	doc/man/man3/strncpy_s.3	\
	doc/man/man3/strncpy_s.c.3	\
	doc/man/man3/strnlen_s.3	\
	doc/man/man3/strnlen_s.c.3	\
	doc/man/man3/strnset_s.3	\
	doc/man/man3/strnset_s.c.3	\
	doc/man/man3/strnterminate_s.3	\
	doc/man/man3/strnterminate_s.c.3	\
	doc/man/man3/strpbrk_s.3	\
	doc/man/man3/strpbrk_s.c.3	\
	doc/man/man3/strprefix_s.3	\
	doc/man/man3/strprefix_s.c.3	\
	doc/man/man3/strrchr_s.3	\
	doc/man/man3/strrchr_s.c.3	\
	doc/man/man3/strremovews_s.3	\
	doc/man/man3/strremovews_s.c.3	\
	doc/man/man3/strset_s.3		\
	doc/man/man3/strset_s.c.3	\
	doc/man/man3/strspn_s.3		\
	doc/man/man3/strspn_s.c.3	\
	doc/man/man3/strstr_s.3		\
	doc/man/man3/strstr_s.c.3	\
	doc/man/man3/strtok_s.3		\
	doc/man/man3/strtok_s.c.3	\
	doc/man/man3/strtolowercase_s.3		\
	doc/man/man3/strtolowercase_s.c.3	\
	doc/man/man3/strtouppercase_s.3		\
	doc/man/man3/strtouppercase_s.c.3	\
	doc/man/man3/strupr_s.3		\
	doc/man/man3/strzero_s.3	\
	doc/man/man3/strzero_s.c.3	\
	doc/man/man3/timingsafe_bcmp.3	\
	doc/man/man3/timingsafe_bcmp.c.3	\
	doc/man/man3/timingsafe_memcmp.3	\
	doc/man/man3/timingsafe_memcmp.c.3	\
	doc/man/man3/tmpfile_s.3	\
	doc/man/man3/tmpfile_s.c.3	\
	doc/man/man3/tmpnam_s.3		\
	doc/man/man3/tmpnam_s.c.3	\
	doc/man/man3/vfprintf_s.3	\
	doc/man/man3/vfprintf_s.c.3	\
	doc/man/man3/vfscanf_s.3	\
	doc/man/man3/vfscanf_s.c.3	\
	doc/man/man3/vprintf_s.3	\
	doc/man/man3/vprintf_s.c.3	\
	doc/man/man3/vscanf_s.3	\
	doc/man/man3/vscanf_s.c.3	\
	doc/man/man3/vsnprintf_s.3	\
	doc/man/man3/vsnprintf_s.c.3	\
	doc/man/man3/vsprintf_s.3	\
	doc/man/man3/vsprintf_s.c.3	\
	doc/man/man3/vsscanf_s.3	\
	doc/man/man3/vsscanf_s.c.3
if ENABLE_WCHAR
dist_man_MANS += \
	doc/man/man3/RSIZE_MAX_WMEM.3	\
	doc/man/man3/fwprintf_s.3	\
	doc/man/man3/fwprintf_s.c.3	\
	doc/man/man3/fwscanf_s.3	\
	doc/man/man3/fwscanf_s.c.3	\
	doc/man/man3/mbsrtowcs_s.3	\
	doc/man/man3/mbsrtowcs_s.c.3	\
	doc/man/man3/mbstowcs_s.3	\
	doc/man/man3/mbstowcs_s.c.3	\
	doc/man/man3/snwprintf_s.3	\
	doc/man/man3/snwprintf_s.c.3	\
	doc/man/man3/swprintf_s.3	\
	doc/man/man3/swprintf_s.c.3	\
	doc/man/man3/swscanf_s.3	\
	doc/man/man3/swscanf_s.c.3	\
	doc/man/man3/towctrans.c.3	\
	doc/man/man3/towfc_s.3		\
	doc/man/man3/towfc_s.c.3	\
	doc/man/man3/towlower.3		\
	doc/man/man3/towupper.3		\
	doc/man/man3/unw16ifcan.h.3	\
	doc/man/man3/unw16ifcmb.h.3	\
	doc/man/man3/unw16ifcmp.h.3	\
	doc/man/man3/unw16ifcpt.h.3	\
	doc/man/man3/unw16ifexc.h.3	\
	doc/man/man3/unwifcan.h.3	\
	doc/man/man3/unwifcmb.h.3	\
	doc/man/man3/unwifcmp.h.3	\
	doc/man/man3/unwifcpt.h.3	\
	doc/man/man3/unwifexc.h.3	\
	doc/man/man3/vfwprintf_s.3	\
	doc/man/man3/vfwprintf_s.c.3	\
	doc/man/man3/vfwscanf_s.3	\
	doc/man/man3/vfwscanf_s.c.3	\
	doc/man/man3/vsnwprintf_s.3	\
	doc/man/man3/vsnwprintf_s.c.3	\
	doc/man/man3/vswprintf_s.3	\
	doc/man/man3/vswprintf_s.c.3	\
	doc/man/man3/vswscanf_s.3	\
	doc/man/man3/vswscanf_s.c.3	\
	doc/man/man3/vwprintf_s.3	\
	doc/man/man3/vwprintf_s.c.3	\
	doc/man/man3/vwscanf_s.3	\
	doc/man/man3/vwscanf_s.c.3 	\
	doc/man/man3/wcrtomb_s.3	\
	doc/man/man3/wcrtomb_s.c.3	\
	doc/man/man3/wcscat_s.3		\
	doc/man/man3/wcscat_s.c.3	\
	doc/man/man3/wcscmp_s.3		\
	doc/man/man3/wcscmp_s.c.3	\
	doc/man/man3/wcscoll_s.3	\
	doc/man/man3/wcscoll_s.c.3	\
	doc/man/man3/wcscpy_s.3		\
	doc/man/man3/wcscpy_s.c.3	\
	doc/man/man3/wcsfc_s.3		\
	doc/man/man3/wcsfc_s.c.3	\
	doc/man/man3/wcsicmp_s.3	\
	doc/man/man3/wcsicmp_s.c.3	\
	doc/man/man3/wcslwr_s.3		\
	doc/man/man3/wcslwr_s.c.3	\
	doc/man/man3/wcsnatcmp_s.3	\
	doc/man/man3/wcsnatcmp_s.c.3	\
	doc/man/man3/wcsncat_s.3	\
	doc/man/man3/wcsncat_s.c.3	\
	doc/man/man3/wcsncmp_s.3	\
	doc/man/man3/wcsncmp_s.c.3	\
	doc/man/man3/wcsncpy_s.3	\
	doc/man/man3/wcsncpy_s.c.3	\
	doc/man/man3/wcsnlen_s.3	\
	doc/man/man3/wcsnlen_s.c.3	\
	doc/man/man3/wcsnorm_compose_s.3	\
	doc/man/man3/wcsnorm_decompose_s.3	\
	doc/man/man3/wcsnorm_reorder_s.3	\
	doc/man/man3/wcsnorm_s.3	\
	doc/man/man3/wcsnorm_s.c.3	\
	doc/man/man3/wcsnset_s.3	\
	doc/man/man3/wcsnset_s.c.3	\
	doc/man/man3/wcsrtombs_s.3	\
	doc/man/man3/wcsrtombs_s.c.3	\
	doc/man/man3/wcsset_s.3		\
	doc/man/man3/wcsset_s.c.3	\
	doc/man/man3/wcsstr.3		\
	doc/man/man3/wcsstr.c.3		\
	doc/man/man3/wcsstr_s.3		\
	doc/man/man3/wcsstr_s.c.3	\
	doc/man/man3/wcstok_s.3		\
	doc/man/man3/wcstok_s.c.3	\
	doc/man/man3/wcstombs_s.3	\
	doc/man/man3/wcstombs_s.c.3	\
	doc/man/man3/wcsupr_s.3		\
	doc/man/man3/wcsupr_s.c.3	\
	doc/man/man3/wctomb_s.3		\
	doc/man/man3/wctomb_s.c.3	\
	doc/man/man3/wmemcmp_s.3	\
	doc/man/man3/wmemcmp_s.c.3	\
	doc/man/man3/wmemcpy_s.3	\
	doc/man/man3/wmemcpy_s.c.3	\
	doc/man/man3/wmemmove_s.3	\
	doc/man/man3/wmemmove_s.c.3	\
	doc/man/man3/wprintf_s.3	\
	doc/man/man3/wprintf_s.c.3	\
	doc/man/man3/wscanf_s.3		\
	doc/man/man3/wscanf_s.c.3
#	doc/man/man3/WCSNORM_FCC.3      \
#	doc/man/man3/WCSNORM_FCD.3      \
#	doc/man/man3/WCSNORM_NFC.3      \
#	doc/man/man3/WCSNORM_NFD.3      \
#	doc/man/man3/WCSNORM_NFKC.3     \
#	doc/man/man3/WCSNORM_NFKD.3
endif
CLEANFILES += Doxyfile doc/footer
MAINTAINERCLEANFILES = *~ README.rst

# avoid parallel doxygen
if HAVE_DOXYGEN
$(dist_man_MANS) ::
doc/man/man3/EOK.3 :: Doxyfile README.rst doc/footer include/safe_config.h
	-rm $@
	$(DOXYGEN)
docs man: Doxyfile doc/footer $(BUILT_SOURCES) README.rst \
   doc/man/man1/check_for_unsafe_apis.1 doc/man/man3/EOK.3
	-rm -rf doc/man/man3/d[0-9a-f]
	-rm doc/man/man3/_*.3
	-rm doc/man/man3/v.3
	-rm doc/man/man3/_*_chk.3
	-rm doc/man/man3/slk_*.3
	-rm doc/man/man3/BOS*.3
	-rm doc/man/man3/HAVE___BUILTIN_CONSTANT_P.3
	-rm doc/man/man3/HAVE___BUILTIN_OBJECT_SIZE.3
	-rm doc/man/man3/UNWIF_*.3
	-rm doc/man/man3/TBL.3
	-rm doc/man/man3/VAL_OVR2*
	-rm doc/man/man3/{wmask,wsize}.3
	-rm doc/man/man3/ASM_VOLATILE.3
	-rm doc/man/man3/CASE*.3
	-rm doc/man/man3/DRV*.3
	-rm doc/man/man3/DISABLE_DLLIMPORT.3
	-rm doc/man/man3/EXTERN.3
	-rm doc/man/man3/errno_t.3
	-rm doc/man/man3/Hangul*.3
	-rm doc/man/man3/hangul.h.3
	-rm doc/man/man3/seqindex_*
	-rm doc/man/man3/*case_seqindex.3
	-rm doc/man/man3/UINT16_MAX.3
	-rm doc/man/man3/UNWIF_*.3
	-rm doc/man/man3/README*
	-rm doc/man/man3/__STDC_WANT_LIB_EXT1__.3
	-rm doc/man/man3/__WORDSIZE.3
	-rm doc/man/man3/MODULE*.3
	-rm doc/man/man3/NORMALIZE_IND_TBL.3
	-rm doc/man/man3/is*.3
	-rm doc/man/man3/debruijn32.3
	-rm doc/man/man3/errmsgs_s.3
	-rm doc/man/man3/casemaps*.3
	-rm doc/man/man3/CC_SEQ_SIZE.3
	-rm doc/man/man3/CC_SEQ_STEP.3
	-rm doc/man/man3/qsort_musl.3
	-rm doc/man/man3/shl.3
	-rm doc/man/man3/shr.3
	-rm doc/man/man3/nextchar.3
	-rm doc/man/man3/ntz.3
	-rm doc/man/man3/pairs.3
	-rm doc/man/man3/pntz.3
	-rm doc/man/man3/pos.3
	-rm doc/man/man3/sl_default_handler.3
	-rm doc/man/man3/slkm_init.c.3
	-rm doc/man/man3/module_{exit,init}.3
	-rm doc/man/man3/sift.3
	-rm doc/man/man3/v.3
	-rm doc/man/man3/trinkle.3
	-rm doc/man/man3/todo.3
	-rm doc/man/man3/tbl2.3
	-rm doc/man/man3/tbl3.3
	-rm doc/man/man3/unsafe_get_property.3
	-rm doc/man/man3/unw16if*.h.3
	-rm doc/man/man3/unwif*.h.3
	-rm doc/man/man3/wsize.3
	-rm doc/man/man3/md_doc_libc-overview.3
	-rm doc/man/man3/libc-overview.md.3
	-rm doc/man/man3/len{_errmsgs_s,}.3
	-rm doc/man/man3/upper.3
	-rm doc/man/man3/lower{,1,2,3}.3
	-rm doc/man/man3/uint64_t.3
	-rm doc/man/man3/{cc,cp,cycle,cmpfun,composite,a_ctz_l,a_ctz_64,TBL}.3
	-rm doc/man/man3/compare_{left,right}.3
	-rm doc/man/man3/bos_chk_err.3
	-rm doc/man/man3/{size,slen,smax,src,dest,dlen,dmax,name}.3
	-rm doc/man/man3/handle_{,w,mem_}error.3
	-rm doc/man/man3/handle_{str,mem}_bos_*.3
	-rm doc/man/man3/handle_str_src_bos_*.3
endif
if HAVE_PANDOC
README.rst: README.md
	pandoc -o README.rst README.md
endif
all-local: Doxyfile doc/footer
clean-local:
	rm -rf $(builddir)/doc/html
	rm -rf $(builddir)/doc/man
endif
endif

if ENABLE_GCOV
gcov: check
	$(MAKE) -C tests gcov
	$(MAKE) -C src gcov
	gcov2perl src/*.gcov src/*/*.gcov
	cover -no-gcov
	build-aux/gcov-upd.pl src/gcov.log
endif

if HAVE_WINE
check-wine:
	$(MAKE) -C tests
	for t in tests/t_*.exe; do b="`basename $$t .exe`"; \
	    $(WINE) $$t | tee tests/$$b.log; done
endif

# Additional make rules
# ===============================================

.PHONY: gcov docs man perf gh-pages release check-log check-wine
$(top_srcdir)/.version:
	echo $(PACKAGE_VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(PACKAGE_VERSION) > $(distdir)/$(TARBALL_VERSION_FILE)
check-log:
	$(MAKE) check || (cat tests/test-suite.log && false)
perf:
	tests/p_memcpy_s
	tests/p_memcpy32_s
gh-pages: docs
	echo ensure ./configure --enable-unsafe
	ls doc/man/man3/tmpnam_s.3
	git checkout gh-pages
	rm -rf doc/man
	rm -rf doc/safec-$(SAFEC_API_VERSION)
	mkdir doc/safec-$(SAFEC_API_VERSION)
	mv doc/html/* doc/safec-$(SAFEC_API_VERSION)/
	rm -rf doc/html
	git add doc/safec-$(SAFEC_API_VERSION)
release: docs distcheck
	git branch |grep '^* master'
	git tag -f v`date +%d%m%Y`
	git tag -f $(PACKAGE_VERSION)
	build-aux/autogen.sh
	./configure
	$(MAKE)
	$(MAKE) check
	$(MAKE) docs
	$(MAKE) dist
	build-aux/smoke.sh
	git push
	git push --tags
	./configure --enable-unsafe
	$(MAKE)
	$(MAKE) docs
	$(MAKE) gh-pages
	git branch |grep '^* gh-pages'
	git commit -m"Update to $(PACKAGE_VERSION)"
	git push
	git checkout master
