# Makefile.am - Process with automake to produce Makefile.in
#
# 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
# 2017-2018 Reini Urban
#
# Copyright (c) 2012, 2013 Cisco Systems
# Copyright (c) 2017, 2019 Reini Urban
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
	mem/mem_primitives_lib.h  \
	mem/mem_primitives_lib.c

CORE_FILES = \
	safeclib_private.h    \
	abort_handler_s.c     \
	ignore_handler_s.c

STD_MEM_FILES = \
	mem/safe_mem_constraint.h \
	mem/safe_mem_constraint.c \
	mem/memset_s.c            \
	mem/memcpy_s.c            \
	mem/memmove_s.c

EXT_MEM_FILES = \
	extmem/memset16_s.c     \
	extmem/memset32_s.c     \
	extmem/memcpy16_s.c     \
	extmem/memcpy32_s.c     \
	extmem/memmove16_s.c    \
	extmem/memmove32_s.c    \
	extmem/memcmp_s.c       \
	extmem/memcmp16_s.c     \
	extmem/memcmp32_s.c     \
	extmem/memzero_s.c      \
	extmem/memzero16_s.c    \
	extmem/memzero32_s.c	\
	extmem/memchr_s.c	\
	extmem/memrchr_s.c	\
	extmem/memccpy_s.c      \
	extmem/timingsafe_memcmp.c \
	extmem/timingsafe_bcmp.c

STD_STR_FILES = \
	str/safe_str_constraint.h \
	str/safe_str_constraint.c \
	str/sprintf_s.c 	\
	str/strcat_s.c          \
	str/strcpy_s.c          \
	str/strncat_s.c         \
	str/strncpy_s.c         \
	str/strnlen_s.c         \
	str/strtok_s.c          \
	str/strerror_s.h        \
	str/strerror_s.c        \
	str/vsprintf_s.c	\
	str/snprintf_s.c	\
	str/vsnprintf_s.c

# but they are in the C11 standard
STD_UNSAFE_FILES = \
	io/tmpnam_s.c

EXT_STR_FILES = \
	extstr/stpcpy_s.c            \
	extstr/stpncpy_s.c           \
	extstr/strcasecmp_s.c        \
	extstr/strcasestr_s.c        \
	extstr/strcmp_s.c            \
	extstr/strcmpfld_s.c         \
	extstr/strcpyfld_s.c         \
	extstr/strcpyfldin_s.c       \
	extstr/strcpyfldout_s.c      \
	extstr/strcspn_s.c           \
	extstr/strfirstchar_s.c      \
	extstr/strfirstdiff_s.c      \
	extstr/strfirstsame_s.c      \
	extstr/strisalphanumeric_s.c \
	extstr/strisascii_s.c        \
	extstr/strisdigit_s.c        \
	extstr/strishex_s.c          \
	extstr/strislowercase_s.c    \
	extstr/strismixedcase_s.c    \
	extstr/strispassword_s.c     \
	extstr/strisuppercase_s.c    \
	extstr/strlastchar_s.c       \
	extstr/strlastdiff_s.c       \
	extstr/strlastsame_s.c       \
	extstr/strljustify_s.c       \
	extstr/strnatcmp_s.c         \
	extstr/strnterminate_s.c     \
	extstr/strpbrk_s.c           \
	extstr/strprefix_s.c         \
	extstr/strremovews_s.c       \
	extstr/strspn_s.c            \
	extstr/strchr_s.c            \
	extstr/strrchr_s.c           \
	extstr/strstr_s.c            \
	extstr/strtolowercase_s.c    \
	extstr/strtouppercase_s.c    \
	extstr/strzero_s.c	     \
	extstr/strset_s.c            \
	extstr/strnset_s.c	     \
	extstr/strcoll_s.c

STD_WCHAR_FILES = \
	wchar/mbsrtowcs_s.c	\
	wchar/mbstowcs_s.c	\
	wchar/wcsrtombs_s.c	\
	wchar/wcstombs_s.c	\
	wchar/wcrtomb_s.c	\
	wchar/wctomb_s.c	\
	wchar/wcsnlen_s.c	\
	wchar/wcscpy_s.c	\
	wchar/wcsncpy_s.c	\
	wchar/wcscat_s.c	\
	wchar/wcsncat_s.c	\
	wchar/wmemcpy_s.c	\
	wchar/wmemmove_s.c	\
	wchar/wcstok_s.c	\
	wchar/vswprintf_s.c	\
	wchar/swprintf_s.c	\
	wchar/vfwprintf_s.c	\
	wchar/vwprintf_s.c	\
	wchar/wprintf_s.c	\
	wchar/fwprintf_s.c	\
	wchar/swscanf_s.c	\
	wchar/vswscanf_s.c	\
	wchar/wscanf_s.c	\
	wchar/vfwscanf_s.c	\
	wchar/fwscanf_s.c	\
	wchar/vwscanf_s.c	\
	wchar/snwprintf_s.c

if !HAVE_VSNWPRINTF_S
STD_WCHAR_FILES += \
	wchar/vsnwprintf_s.c
endif

if !HAVE_WCSSTR
STD_WCHAR_FILES += \
	extwchar/wcsstr.c
endif

if ENABLE_EXTS
STD_WCHAR_FILES += \
	extwchar/wcsstr_s.c     \
	extwchar/wmemcmp_s.c	\
	extwchar/wcscmp_s.c	\
	extwchar/wcsncmp_s.c	\
	extwchar/wcsicmp_s.c	\
	extwchar/wcsnatcmp_s.c  \
	extwchar/wcsset_s.c	\
	extwchar/wcsnset_s.c	\
	extwchar/wcscoll_s.c	\
	extwchar/wcslwr_s.c	\
	extwchar/wcsupr_s.c	\
	extwchar/towfc_s.c	\
	extwchar/towctrans.c	\
	extwchar/wcsfc_s.c	\
	extwchar/hangul.h	\
	extwchar/unw16ifcan.h	\
	extwchar/unw16ifcpt.h	\
	extwchar/unw16ifcmb.h	\
	extwchar/unw16ifcmp.h	\
	extwchar/unw16ifexc.h	\
	extwchar/unwifcan.h	\
	extwchar/unwifcmb.h	\
	extwchar/unwifcmp.h	\
	extwchar/unwifcpt.h	\
	extwchar/unwifexc.h	\
	extwchar/wcsnorm_s.c
endif

if ENABLE_U8
EXT_U8_FILES = \
	extu8/u8cat_s.c        \
	extu8/u8icat_s.c       \
	extu8/u8norm_s.c       \
	extu8/u8script.c       \
	extu8/u8scripts.h       \
	extu8/u8cpy_s.c        \
	extu8/u8icpy_s.c       \
	extu8/u8ncpy_s.c       \
	extu8/u8nlen_s.c
endif

STD_IO_FILES = \
	io/sscanf_s.c	\
	io/fscanf_s.c	\
	io/scanf_s.c	\
	io/vfscanf_s.c	\
	io/vsscanf_s.c	\
	io/vscanf_s.c	\
	io/printf_s.c	\
	io/fprintf_s.c	\
	io/tmpfile_s.c	\
	io/vfprintf_s.c	\
	io/vprintf_s.c	\
	io/fopen_s.c    \
	io/freopen_s.c  \
	io/gets_s.c

STD_MISC_FILES = \
	misc/bsearch_s.c \
	misc/qsort_s.c

STD_OS_FILES = \
	os/gmtime_s.c    \
	os/localtime_s.c \
	os/asctime_s.c   \
	os/ctime_s.c     \
	os/getenv_s.c

ALL_SRC_FILES =
	$(STD_MEM_FILES)    \
	$(STD_STR_FILES)    \
	$(EXT_MEM_FILES)    \
	$(EXT_STR_FILES)    \
	$(EXT_U8_FILES)     \
	$(STD_WCHAR_FILES)  \
	$(STD_IO_FILES)     \
	$(STD_OS_FILES)     \
	$(STD_MISC_FILES)

PERL = @PERL@
if ENABLE_U8
EXTRA_DIST = extu8/gen-u8scripts.pl
endif

# Not all the code can support -Werror yet so we specifically enable it
# for where it works
# wcsnorm_CFLAGS = -fbracket-depth=512
AM_CFLAGS += -I$(top_builddir)/include -I$(srcdir)
if !HAVE_MINGW
AM_CFLAGS += @PEDANTIC@
endif
AM_CFLAGS += $(WARN_CFLAGS)

.c.i:
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	$(COMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -E -o $@ $<

.c.ii:  # no linemarkers
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	$(COMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -E -P -o $@ $<

noinst_LTLIBRARIES = \
	libmemprims.la   \
	libsafeccore.la
#	libsafecmem.la   \
#	libsafecstr.la
libmemprims_la_SOURCES  = $(MEM_PRIM_FILES)
libsafeccore_la_SOURCES = $(CORE_FILES)
#libsafecmem_la_SOURCES  = $(STD_MEM_FILES) $(EXT_MEM_FILES)
#libsafecstr_la_SOURCES  = $(STD_STR_FILES) $(EXT_STR_FILES)
if ENABLE_UNSAFE
noinst_LTLIBRARIES += libstdunsafe.la
libstdunsafe_la_SOURCES = $(STD_UNSAFE_FILES)
endif

#EXPORT_SYM      = exported.sym
#$(EXPORT_SYM): safec.h.in
#        sed -n -e 's/^..*SAFEC_EXPORT[[:space:]][[:space:]]*\([[:alnum:]_][[:alnum:]_]*\)..*$$/\1/p' @top_srcdir@/src/safec.h.in > $@

lib_LTLIBRARIES  = libsafec.la
libsafec_la_SOURCES = \
	$(STD_MEM_FILES)    \
	$(STD_STR_FILES)    \
	$(STD_IO_FILES)     \
	$(STD_OS_FILES)     \
	$(STD_MISC_FILES)
if ENABLE_WCHAR
libsafec_la_SOURCES += \
	$(STD_WCHAR_FILES)
endif
if ENABLE_EXTS
libsafec_la_SOURCES += \
	$(EXT_MEM_FILES) \
	$(EXT_STR_FILES)
endif
if ENABLE_U8
libsafec_la_SOURCES += \
	$(EXT_U8_FILES)
lib_LTLIBRARIES += libsafecu8.la
libsafecu8_la_SOURCES = $(EXT_U8_FILES)
endif

libsafec_la_LIBADD = \
	libmemprims.la  \
	libsafeccore.la
if ENABLE_UNSAFE
libsafec_la_LIBADD += \
	libstdunsafe.la
endif
libsafec_la_LDFLAGS = \
	-version-info $(SAFEC_SO_VERSION) \
	-no-undefined \
	$(LDFLAGS) $(AM_LDFLAGS)
#	-export-symbols $(EXPORT_SYM)
#libsafec_la_DEPENDENCIES = $(EXPORT_SYM)

if ENABLE_U8
extu8/u8scripts.h: extu8/gen-u8scripts.pl Scripts.txt
	$(PERL) extu8/gen-u8scripts.pl
endif

# emacs flymake-mode
check-syntax:
	test -n "$(CHK_SOURCES)" && \
	  $(COMPILE) -o /dev/null -S $(CHK_SOURCES)
.PHONY: check-syntax

if ENABLE_GCOV
AM_CFLAGS += @GCOV_CFLAGS@
LIBS      += @GCOV_LIBS@
gcov:
	-test -f $(builddir)/gcov.log && rm $(builddir)/gcov.log
	for c in $(libsafec_la_SOURCES); do \
	  dir="`dirname $$c`"; base="`basename $$c .c`"; \
	  if test -e "$$dir/$$base.gcno" -a -e $$c; then \
	    $(GCOV) -s $$dir $$c | tee -a $(builddir)/gcov.log; \
	    mv $$base.c.gcov $$dir/ ; \
	  fi; \
	done
endif

CLEANFILES   = */*~ *~ *.i $(EXPORT_SYM)
CLEANFILES  += */*.gcno */*.gcda *.gcno *.gcda
if ENABLE_U8
DISTCLEANFILES = Scripts.txt
endif
if ENABLE_GCOV
CLEANFILES  += */*.gcov *.gcov gcov.log
endif

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
