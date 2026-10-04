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

# Sub-directories to perform recursive make in
if ENABLE_TESTS
  TEST_DIR = tests
endif
SUBDIRS = src $(TEST_DIR)

# Library header files
pkginclude_HEADERS    = \
	include/safec.h  \
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
	$(top_srcdir)/doc/libc-overview.md     	 \
	$(top_srcdir)/scripts                    \
	$(top_srcdir)/mknorm.pl                  \
	$(top_srcdir)/.version                   \
	$(top_srcdir)/libsafec.pc.in

SAFEC_GENHEADERS = \
	$(top_srcdir)/include/safe_config.h.in     \
	$(top_srcdir)/include/safe_lib_errno.h.in  \
	$(top_srcdir)/include/safe_types.h.in
if ENABLE_U8
SAFEC_GENHEADERS += \
	$(top_srcdir)/src/extu8/un8ifcan.h \
	$(top_srcdir)/src/extu8/un8ifcmb.h \
	$(top_srcdir)/src/extu8/un8ifcmp.h \
	$(top_srcdir)/src/extu8/un8ifcpt.h
endif
if ENABLE_WCHAR
SAFEC_GENHEADERS += \
	$(top_srcdir)/src/extwchar/hangul.h \
	$(top_srcdir)/src/extwchar/unw16ifcan.h \
	$(top_srcdir)/src/extwchar/unw16ifcpt.h \
	$(top_srcdir)/src/extwchar/unw16ifcmb.h \
	$(top_srcdir)/src/extwchar/unw16ifexc.h \
	$(top_srcdir)/src/extwchar/unw16ifcmp.h \
	$(top_srcdir)/src/extwchar/unwifcmb.h \
	$(top_srcdir)/src/extwchar/unwifexc.h \
	$(top_srcdir)/src/extwchar/unwifcmp.h \
	$(top_srcdir)/src/extwchar/unwifcan.h \
	$(top_srcdir)/src/extwchar/unwifcpt.h
endif

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

compile_commands.json: Makefile $(srcdir)/config.h \
	$(dist_pkginclude_HEADERS)
	-intercept-build $(MAKE)

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
dist_man_MANS += $(wildcard doc/man/man3/*.3)
endif
CLEANFILES += Doxyfile doc/footer
MAINTAINERCLEANFILES = *~ README.rst

# avoid parallel doxygen
if HAVE_DOXYGEN
$(dist_man_MANS) ::
doc/man/man3/EOK.3 :: Doxyfile README.rst doc/footer include/safe_config.h
	@if test -f $@; then rm -f $@; fi
	$(DOXYGEN)
docs man: Doxyfile doc/footer $(BUILT_SOURCES) README.rst \
   doc/man/man1/check_for_unsafe_apis.1 doc/man/man3/EOK.3
	-rm -rf doc/man/man3/d[0-9a-f]
	-rm doc/man/man3/_*.3
	-rm doc/man/man3/v.3
	-rm doc/man/man3/_*_chk.3
	-rm doc/man/man3/slk_*.3
	-rm doc/man/man3/BOS*.3
	-rm doc/man/man3/SAFECLIB_HAVE*.3
	-rm doc/man/man3/HAVE___BUILTIN_CONSTANT_P.3
	-rm doc/man/man3/HAVE___BUILTIN_OBJECT_SIZE.3
	-rm doc/man/man3/UNWIF_*.3
	-rm doc/man/man3/UN8IF_*.3
	-rm doc/man/man3/TBL.3
	-rm doc/man/man3/VAL_OVR2*
	-rm doc/man/man3/{wmask,wsize}.3
	-rm doc/man/man3/ASM_VOLATILE.3
	-rm doc/man/man3/CASE*.3
	-rm doc/man/man3/COMPILER*.3
	-rm doc/man/man3/DRV*.3
	-rm doc/man/man3/DISABLE_DLLIMPORT.3
	-rm doc/man/man3/EXTERN.3
	-rm doc/man/man3/errno_t.3
	-rm doc/man/man3/Hangul*.3
	-rm doc/man/man3/hangul.h.3
	-rm doc/man/man3/seqindex_*
	-rm doc/man/man3/*case_seqindex.3
	-rm doc/man/man3/HAVE_LOCALE_TR.3
	-rm doc/man/man3/UINT16_MAX.3
	-rm doc/man/man3/README*
	-rm doc/man/man3/__STDC_WANT_LIB_EXT1__.3
	-rm doc/man/man3/__WORDSIZE.3
	-rm doc/man/man3/MODULE*.3
	-rm doc/man/man3/NORMALIZE_IND_TBL.3
	-rm doc/man/man3/is*.3
	-rm doc/man/man3/beg.3
	-rm doc/man/man3/buf*.3
	-rm doc/man/man3/char8_t.3
	-rm doc/man/man3/cnt.3
	-rm doc/man/man3/compar.3
	-rm doc/man/man3/cookie.3
	-rm doc/man/man3/ctx.3
	-rm doc/man/man3/debruijn32.3
	-rm doc/man/man3/dec_utf8.3
	-rm doc/man/man3/enc_utf8.3
	-rm doc/man/man3/end.3
	-rm doc/man/man3/errmsgs_s.3
	-rm doc/man/man3/f.3
	-rm doc/man/man3/FLOCK.3
	-rm doc/man/man3/FALSE.3
	-rm doc/man/man3/FLAGS_*.3
	-rm doc/man/man3/FUNLOCK.3
	-rm doc/man/man3/from.3
	-rm doc/man/man3/gbreak.3
	-rm doc/man/man3/get.3
	-rm doc/man/man3/last.3
	-rm doc/man/man3/lead.3
	-rm doc/man/man3/length.3
	-rm doc/man/man3/lim.3
	-rm doc/man/man3/lock.3
	-rm doc/man/man3/L_tmpnam_s.3
	-rm doc/man/man3/mask.3
	-rm doc/man/man3/casemaps*.3
	-rm doc/man/man3/CC_SEQ_SIZE.3
	-rm doc/man/man3/CC_SEQ_STEP.3
	-rm doc/man/man3/STRTOK_DELIM_MAX_LEN.3
	-rm doc/man/man3/nextchar.3
	-rm doc/man/man3/noname
	-rm doc/man/man3/ntz.3
	-rm doc/man/man3/md_doc_2libc-overview.3
	-rm doc/man/man3/pairs.3
	-rm doc/man/man3/pntz.3
	-rm doc/man/man3/pos.3
	-rm doc/man/man3/qsort_musl.3
	-rm doc/man/man3/rend.3
	-rm doc/man/man3/rpos.3
	-rm doc/man/man3/shl.3
	-rm doc/man/man3/shr.3
	-rm doc/man/man3/read.3
	-rm doc/man/man3/remaining.3
	-rm doc/man/man3/RESTRICT.3
	-rm doc/man/man3/MEMORY_BARRIER.3
	-rm doc/man/man3/sl_default_handler.3
	-rm doc/man/man3/slkm_init.c.3
	-rm doc/man/man3/module_{exit,init}.3
	-rm doc/man/man3/safec_arg*.3
	-rm doc/man/man3/safec_ascii*.3
	-rm doc/man/man3/safec_*scan.3
	-rm doc/man/man3/safec_ntoa*.3
	-rm doc/man/man3/safec_in_wset.3
	-rm doc/man/man3/safec_is_digit.3
	-rm doc/man/man3/scan_*.3
	-rm doc/man/man3/setlim.3
	-rm doc/man/man3/sift.3
	-rm doc/man/man3/sf.3
	-rm doc/man/man3/sh*.3
	-rm doc/man/man3/str.3
	-rm doc/man/man3/STDCHAR.3
	-rm doc/man/man3/SIZE_*.3
	-rm doc/man/man3/v.3
	-rm doc/man/man3/tmp.3
	-rm doc/man/man3/TRUE.3
	-rm doc/man/man3/trinkle.3
	-rm doc/man/man3/todo.3
	-rm doc/man/man3/tbl2.3
	-rm doc/man/man3/tbl3.3
	-rm doc/man/man3/*UNICODE_VERSION*.3
	-rm doc/man/man3/u8_comp*.3
	-rm doc/man/man3/u8_cpsize.3
	-rm doc/man/man3/u8_is*.3
	-rm doc/man/man3/u8_seqlen.3
	-rm doc/man/man3/u8fc_iter_t.3
	-rm doc/man/man3/u8fc_next.3
	-rm doc/man/man3/U8_*.3
	-rm doc/man/man3/UTF*.3
	-rm doc/man/man3/utf.3
	-rm doc/man/man3/utf8_len.3
	-rm doc/man/man3/unget.3
	-rm doc/man/man3/unsafe_get_property.3
	-rm doc/man/man3/unw16if*.h.3
	-rm doc/man/man3/unwif*.h.3
	-rm doc/man/man3/val_of.3
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

if HAVE_CPPCHECK
cppcheck:
	cppcheck -Iinclude -Isrc --suppress=unknownMacro --suppress=syntaxError src/
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

.PHONY: cppcheck gcov docs man perf gh-pages release check-log check-wine
$(top_srcdir)/.version:
	echo $(PACKAGE_VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(PACKAGE_VERSION) > $(distdir)/$(TARBALL_VERSION_FILE)
check-log:
	$(MAKE) check || (cat tests/test-suite.log && false)
perf:
	tests/p_memcpy_s
	tests/p_memcpy32_s
	tests/p_memset_s
	tests/p_strcpy_s
pkg:
	-rm -rf inst
	$(MAKE) install DESTDIR="`pwd`/inst" prefix=/$(PACKAGE_TARNAME)-$(PACKAGE_VERSION)
if HAVE_MINGW
	(cd inst && zip ../$(PACKAGE_TARNAME)-$(PACKAGE_VERSION)-$(host_triplet).zip -r $(PACKAGE_TARNAME)-$(PACKAGE_VERSION) && cd -)
else
	tar cfz $(PACKAGE_TARNAME)-$(PACKAGE_VERSION)-$(host_triplet).tar.gz -C inst $(PACKAGE_TARNAME)-$(PACKAGE_VERSION)
endif
	rm -rf inst
gh-pages: docs
	echo ensure ./configure --enable-unsafe --enable-wchar
	ls doc/man/man3/tmpnam_s.3
	cp -f doc/libc-overview.md doc/libc-overview.md.master
	git checkout gh-pages
	-mv doc/libc-overview.md.master doc/libc-overview.md
	rm -rf doc/man
	rm -rf doc/safec-$(SAFEC_API_VERSION)
	mkdir doc/safec-$(SAFEC_API_VERSION)
	mv doc/html/* doc/safec-$(SAFEC_API_VERSION)/
	rm -rf doc/html
	sed -i -e"s,3\...,$(SAFEC_API_VERSION),g" index.html
	git add index.html doc/libc-overview.md doc/safec-$(SAFEC_API_VERSION)
release: docs distcheck
	git branch |grep '^* master'
	-git tag -f v$(PACKAGE_VERSION)
	build-aux/autogen.sh
	./configure
	$(MAKE)
	$(MAKE) check
	$(MAKE) docs
	$(MAKE) dist
	echo build-aux/smoke.sh
	git push
	git push --tags
	./configure --enable-unsafe --enable-wchar
	$(MAKE)
	$(MAKE) docs
	$(MAKE) gh-pages
	git branch |grep '^* gh-pages'
	git commit -m"Update to v$(PACKAGE_VERSION)"
	git push
	git checkout master
