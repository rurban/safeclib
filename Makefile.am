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
nodist_pkginclude_HEADERS = \
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
	$(top_srcdir)/include/safe_types.h.in      \
	$(top_srcdir)/include/safe_lib_errno.h.in

SAFEC_SLKM = \
	$(top_srcdir)/tests/test_slkm.c            \
	$(top_srcdir)/src/slkm                     \
	$(top_srcdir)/Kbuild                       \
	$(top_srcdir)/Makefile.kernel


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

# Additional make rules
# ===============================================

$(top_srcdir)/.version:
	echo $(PACKAGE_VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(PACKAGE_VERSION) > $(distdir)/$(TARBALL_VERSION_FILE)
