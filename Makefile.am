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
	include/safe_lib.h     \
	include/safe_mem_lib.h \
	include/safe_str_lib.h

# generated package headers that need to be generated every time
dist_pkginclude_HEADERS = \
	include/safe_config.h   \
	include/safe_types.h   \
	include/safe_lib_errno.h

# Support files
SAFEC_INFRA = \
	${top_srcdir}/build-tools/autogen.sh       \
	$(top_srcdir)/build-tools/version-gen.sh   \
	$(top_srcdir)/doc                          \
	$(top_srcdir)/scripts                      \
	$(top_srcdir)/.version                     \
	$(top_srcdir)/safec.pc.in

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
pcdata_DATA = safec-$(SAFEC_API_VERSION).pc

CLEANFILES = *~
LIBTOOL_DISTCLEAN_FILES = \
	m4/libtool.m4 m4/ltoptions.m4 m4/ltsugar.m4 m4/ltversion.m4 m4/lt~obsolete.m4 \
	build-tools/ar-lib \
	build-tools/compile \
	build-tools/config.guess \
	build-tools/config.sub \
	build-tools/depcomp \
	build-tools/install-sh \
	build-tools/ltmain.sh \
	build-tools/missing \
	build-tools/test-driver
DISTCLEANFILES = .version CaseFolding.txt DerivedGeneralCategory.txt

MAN1_SOURCES = doc/man/man1/check_for_unsafe_apis.1
dist_man_MANS =
if HAVE_POD2MAN
dist_man_MANS += $(MAN1_SOURCES)

$(MAN1_SOURCES): $(top_srcdir)/scripts/check_for_unsafe_apis
	-mkdir -p doc/man/man1
	-$(POD2MAN) $(top_srcdir)/scripts/check_for_unsafe_apis >$@

endif
if HAVE_DOXYGEN
dist_man_MANS += \
	doc/man/man3/STRTOK_DELIM_MAX_LEN.3 \
	doc/man/man3/abort_handler_s.3 \
	doc/man/man3/abort_handler_s.c.3 \
	doc/man/man3/ignore_handler_s.3 \
	doc/man/man3/ignore_handler_s.c.3 \
	doc/man/man3/invoke_safe_mem_constraint_handler.3 \
	doc/man/man3/invoke_safe_str_constraint_handler.3 \
	doc/man/man3/mem_handler.3 \
	doc/man/man3/mem_prim_move.3 \
	doc/man/man3/mem_prim_move16.3 \
	doc/man/man3/mem_prim_move32.3 \
	doc/man/man3/mem_prim_move8.3 \
	doc/man/man3/mem_prim_set.3 \
	doc/man/man3/mem_prim_set16.3 \
	doc/man/man3/mem_prim_set32.3 \
	doc/man/man3/mem_primitives_lib.c.3 \
	doc/man/man3/memcmp16_s.3 \
	doc/man/man3/memcmp16_s.c.3 \
	doc/man/man3/memcmp32_s.3 \
	doc/man/man3/memcmp32_s.c.3 \
	doc/man/man3/memcmp_s.3 \
	doc/man/man3/memcmp_s.c.3 \
	doc/man/man3/memcpy16_s.3 \
	doc/man/man3/memcpy16_s.c.3 \
	doc/man/man3/memcpy32_s.3 \
	doc/man/man3/memcpy32_s.c.3 \
	doc/man/man3/memcpy_s.3 \
	doc/man/man3/memcpy_s.c.3 \
	doc/man/man3/memmove16_s.3 \
	doc/man/man3/memmove16_s.c.3 \
	doc/man/man3/memmove32_s.3 \
	doc/man/man3/memmove32_s.c.3 \
	doc/man/man3/memmove_s.3 \
	doc/man/man3/memmove_s.c.3 \
	doc/man/man3/memset16_s.3 \
	doc/man/man3/memset16_s.c.3 \
	doc/man/man3/memset32_s.3 \
	doc/man/man3/memset32_s.c.3 \
	doc/man/man3/memset_s.3 \
	doc/man/man3/memset_s.c.3 \
	doc/man/man3/memzero16_s.3 \
	doc/man/man3/memzero16_s.c.3 \
	doc/man/man3/memzero32_s.3 \
	doc/man/man3/memzero32_s.c.3 \
	doc/man/man3/memzero_s.3 \
	doc/man/man3/memzero_s.c.3 \
	doc/man/man3/safe_mem_constraint.c.3 \
	doc/man/man3/safe_str_constraint.c.3 \
	doc/man/man3/set_mem_constraint_handler_s.3 \
	doc/man/man3/set_str_constraint_handler_s.3 \
	doc/man/man3/sprintf_s.3 \
	doc/man/man3/sprintf_s.c.3 \
	doc/man/man3/str_handler.3 \
	doc/man/man3/strcasecmp_s.3 \
	doc/man/man3/strcasecmp_s.c.3 \
	doc/man/man3/strcasestr_s.3 \
	doc/man/man3/strcasestr_s.c.3 \
	doc/man/man3/strcat_s.3 \
	doc/man/man3/strcat_s.c.3 \
	doc/man/man3/strcmp_s.3 \
	doc/man/man3/strcmp_s.c.3 \
	doc/man/man3/strcmpfld_s.3 \
	doc/man/man3/strcpy_s.3 \
	doc/man/man3/strcpy_s.c.3 \
	doc/man/man3/strcpyfld_s.3 \
	doc/man/man3/strcpyfldin_s.3 \
	doc/man/man3/strcpyfldin_s.c.3 \
	doc/man/man3/strcpyfldout_s.3 \
	doc/man/man3/strcpyfldout_s.c.3 \
	doc/man/man3/strcspn_s.3 \
	doc/man/man3/strcspn_s.c.3 \
	doc/man/man3/strfirstchar_s.3 \
	doc/man/man3/strfirstchar_s.c.3 \
	doc/man/man3/strfirstdiff_s.3 \
	doc/man/man3/strfirstdiff_s.c.3 \
	doc/man/man3/strfirstsame_s.3 \
	doc/man/man3/strfirstsame_s.c.3 \
	doc/man/man3/strisalphanumeric_s.3 \
	doc/man/man3/strisalphanumeric_s.c.3 \
	doc/man/man3/strisascii_s.3 \
	doc/man/man3/strisascii_s.c.3 \
	doc/man/man3/strisdigit_s.3 \
	doc/man/man3/strisdigit_s.c.3 \
	doc/man/man3/strishex_s.3 \
	doc/man/man3/strishex_s.c.3 \
	doc/man/man3/strislowercase_s.3 \
	doc/man/man3/strislowercase_s.c.3 \
	doc/man/man3/strismixedcase_s.3 \
	doc/man/man3/strismixedcase_s.c.3 \
	doc/man/man3/strispassword_s.3 \
	doc/man/man3/strispassword_s.c.3 \
	doc/man/man3/strisuppercase_s.3 \
	doc/man/man3/strisuppercase_s.c.3 \
	doc/man/man3/strlastchar_s.3 \
	doc/man/man3/strlastchar_s.c.3 \
	doc/man/man3/strlastdiff_s.3 \
	doc/man/man3/strlastdiff_s.c.3 \
	doc/man/man3/strlastsame_s.3 \
	doc/man/man3/strlastsame_s.c.3 \
	doc/man/man3/strljustify_s.3 \
	doc/man/man3/strljustify_s.c.3 \
	doc/man/man3/strncat_s.3 \
	doc/man/man3/strncat_s.c.3 \
	doc/man/man3/strncpy_s.3 \
	doc/man/man3/strncpy_s.c.3 \
	doc/man/man3/strnlen_s.3 \
	doc/man/man3/strnlen_s.c.3 \
	doc/man/man3/strnterminate_s.3 \
	doc/man/man3/strnterminate_s.c.3 \
	doc/man/man3/strpbrk_s.3 \
	doc/man/man3/strpbrk_s.c.3 \
	doc/man/man3/strprefix_s.3 \
	doc/man/man3/strprefix_s.c.3 \
	doc/man/man3/strremovews_s.3 \
	doc/man/man3/strremovews_s.c.3 \
	doc/man/man3/strspn_s.3 \
	doc/man/man3/strspn_s.c.3 \
	doc/man/man3/strstr_s.3 \
	doc/man/man3/strstr_s.c.3 \
	doc/man/man3/strtok_s.3 \
	doc/man/man3/strtok_s.c.3 \
	doc/man/man3/strtolowercase_s.3 \
	doc/man/man3/strtolowercase_s.c.3 \
	doc/man/man3/strtouppercase_s.3 \
	doc/man/man3/strtouppercase_s.c.3 \
	doc/man/man3/strzero_s.3 \
	doc/man/man3/strzero_s.c.3
CLEANFILES += Doxyfile doc/footer README.md

doc/man/man3/STRTOK_DELIM_MAX_LEN.3: Doxyfile README.md doc/footer \
  include/safe_config.h
	-rm $@
	doxygen

docs man: Doxyfile doc/footer $(BUILT_SOURCES) README.md \
   doc/man/man1/check_for_unsafe_apis.1 doc/man/man3/STRTOK_DELIM_MAX_LEN.3
	-rm -rf doc/man/man3/d[0-9a-f]
	-rm doc/man/man3/_*.3
	-rm doc/man/man3/slk_*.3
	-rm doc/man/man3/module_*.3
	-rm doc/man/man3/{wmask,wsize,likely,unlikely}.3
	-rm doc/man/man3/DRV*.3
	-rm doc/man/man3/{INLINE,EXPORT_SYMBOL}.3
	-rm doc/man/man3/MODULE*.3
	-rm doc/man/man3/README*
README.md: README
	cp README README.md
all-local: Doxyfile doc/footer
clean-local:
	rm -rf $(top_srcdir)/docs/html
	rm -rf $(top_srcdir)/docs/man
endif

if ENABLE_GCOV
gcov: check
	$(MAKE) -C tests gcov
	$(MAKE) -C src gcov
	gcov2perl src/*.gcov src/*/*.gcov
	cover -no-gcov
	build-tools/gcov-upd.pl src/gcov.log
endif

# Additional make rules
# ===============================================

.PHONY: gcov docs man perf gh-pages release check-log
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
release: docs dist
	git checkout master
	autoreconf
	./configure
	$(MAKE)
	$(MAKE) check
	$(MAKE) docs
	$(MAKE) dist
	build-tools/smoke.sh
	git tag v`date +%d%m%Y`
	git push
	git push --tags
	./configure --enable-unsafe
	$(MAKE)
	$(MAKE) docs
	$(MAKE) gh-pages
	git commit -m"Update to v`date +%d%m%Y`"
	git push
	git checkout master
	build-tools/autogen.sh
