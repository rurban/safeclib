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

MEM_PRIM_FILES = \
	safeclib/mem_primitives_lib.h  \
	safeclib/mem_primitives_lib.c

CORE_FILES = \
	safeclib/safeclib_private.h    \
	safeclib/abort_handler_s.c     \
	safeclib/ignore_handler_s.c

STD_MEM_FILES = \
	safeclib/safe_mem_constraint.h \
	safeclib/safe_mem_constraint.c \
	safeclib/memset_s.c            \
	safeclib/memcpy_s.c            \
	safeclib/memmove_s.c

EXT_MEM_FILES = \
	safeclib/memset16_s.c          \
	safeclib/memset32_s.c          \
	safeclib/memcpy16_s.c          \
	safeclib/memcpy32_s.c          \
	safeclib/memmove16_s.c         \
	safeclib/memmove32_s.c         \
	safeclib/memcmp_s.c            \
	safeclib/memcmp16_s.c          \
	safeclib/memcmp32_s.c          \
	safeclib/memzero_s.c           \
	safeclib/memzero16_s.c         \
	safeclib/memzero32_s.c

STD_STR_FILES = \
	safeclib/safe_str_constraint.h \
	safeclib/safe_str_constraint.c \
	safeclib/strnlen_s.c           \
	safeclib/strcat_s.c            \
	safeclib/strncat_s.c           \
	safeclib/strcpy_s.c            \
	safeclib/strncpy_s.c           \
	safeclib/strtok_s.c

EXT_STR_FILES = \
	safeclib/strcmp_s.c            \
	safeclib/strcmpfld_s.c         \
	safeclib/strspn_s.c            \
	safeclib/strcspn_s.c           \
	safeclib/strstr_s.c            \
	safeclib/strpbrk_s.c           \
	safeclib/strnterminate_s.c     \
	safeclib/strprefix_s.c         \
	safeclib/strremovews_s.c       \
	safeclib/strcasecmp_s.c        \
	safeclib/strcasestr_s.c        \
	safeclib/strzero_s.c           \
	safeclib/strisalphanumeric_s.c \
	safeclib/strisascii_s.c        \
	safeclib/strisdigit_s.c        \
	safeclib/strishex_s.c          \
	safeclib/strislowercase_s.c    \
	safeclib/strismixedcase_s.c    \
	safeclib/strispassword_s.c     \
	safeclib/strisuppercase_s.c    \
	safeclib/strfirstchar_s.c      \
	safeclib/strfirstdiff_s.c      \
	safeclib/strfirstsame_s.c      \
	safeclib/strcpyfld_s.c         \
	safeclib/strcpyfldin_s.c       \
	safeclib/strcpyfldout_s.c      \
	safeclib/strtolowercase_s.c    \
	safeclib/strtouppercase_s.c    \
	safeclib/strlastchar_s.c       \
	safeclib/strlastdiff_s.c       \
	safeclib/strlastsame_s.c       \
	safeclib/strljustify_s.c

# Not all the code can support -Werror yet so we specifically enable it
# for where it works
AM_CFLAGS += -Werror

noinst_LTLIBRARIES = \
	libmemprims.la   \
	libsafeccore.la
#	libsafecmem.la   \
#	libsafecstr.la
libmemprims_la_SOURCES  = $(MEM_PRIM_FILES)
libsafeccore_la_SOURCES = $(CORE_FILES)
#libsafecmem_la_SOURCES  = $(STD_MEM_FILES) $(EXT_MEM_FILES)
#libsafecstr_la_SOURCES  = $(STD_STR_FILES) $(EXT_STR_FILES)

#EXPORT_SYM      = exported.sym
#$(EXPORT_SYM): safec.h.in
#        sed -n -e 's/^..*SAFEC_EXPORT[[:space:]][[:space:]]*\([[:alnum:]_][[:alnum:]_]*\)..*$$/\1/p' @top_srcdir@/src/safec.h.in > $@

lib_LTLIBRARIES      = libsafec-@SAFEC_API_VERSION@.la
libsafec_@SAFEC_API_VERSION@_la_SOURCES      = $(STD_MEM_FILES) $(EXT_MEM_FILES) \
					       $(STD_STR_FILES) $(EXT_STR_FILES)
libsafec_@SAFEC_API_VERSION@_la_LIBADD       = libmemprims.la libsafeccore.la
libsafec_@SAFEC_API_VERSION@_la_LDFLAGS      = -version-info $(SAFEC_SO_VERSION) \
	                                       -no-undefined
#	                                       -export-symbols $(EXPORT_SYM)
#libsafec_@SAFEC_API_VERSION@_la_DEPENDENCIES = $(EXPORT_SYM)


CLEANFILES      = *~ *.gcno $(EXPORT_SYM)

## LCOV_INPUT      = $(CFILES:%.c=.libs/%.gcda)
## LCOV_OUTPUT     = lcov.info
## LCOV_HTML       = lcov_html
## LCOV_LCOV       = @LCOV@
## LCOV_GENHTML    = @GENHTML@
##
## lcov: $(LCOV_HTML)
##
## $(LCOV_INPUT): libcheck.la libcheckinternal.la
##         @$(MAKE) -C $(top_builddir)/tests check
##
## $(LCOV_OUTPUT): $(LCOV_INPUT)
##         $(LCOV_LCOV) --capture --directory . --base-directory . --output-file $@
##
## $(LCOV_HTML): $(LCOV_OUTPUT)
##         -$(RM) -r $@
##         LANG=C $(LCOV_GENHTML) --output-directory $@ --title "Check Code Coverage" --show-details $<
##         @echo "Point a web browser at $(LCOV_HTML)/index.html to see results."
##
## clean-local: lcov-clean
##
## .PHONY: lcov-clean
## lcov-clean:
##         -$(RM) -r $(LCOV_HTML) $(LCOV_OUTPUT)


## Define the list of public header files and their install location.  The
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  This only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.
## @PACKAGE_TARNAME@_includedir = $(includedir)/@PACKAGE_TARNAME@-$(SAFEC_API_VERSION)
## nobase_@PACKAGE_TARNAME@_include_HEADERS = safec.h

## The generated configuration header is installed in its own subdirectory of
## $(libdir).  The reason for this is that the configuration information put
## into this header file describes the target platform the installed library
## has been built for.  Thus the file must not be installed into a location
## intended for architecture-independent files, as defined by the Filesystem
## Hierarchy Standard (FHS).
## The nodist_ prefix instructs Automake to not generate rules for including
## the listed files in the distribution on 'make dist'.  Files that are listed
## in _HEADERS variables are normally included in the distribution, but the
## configuration header file is generated at configure time and should not be
## shipped with the source tarball.
## @PACKAGE_TARNAME@_libincludedir = $(libdir)/@PACKAGE_TARNAME@-$(SAFEC_API_VERSION)/include
## nodist_@PACKAGE_TARNAME@_libinclude_HEADERS = @PACKAGE_TARNAME@config.h
