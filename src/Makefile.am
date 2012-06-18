## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES      = libsafec-@SAFEC_API_VERSION@.la

include_HEADERS      = safec.h

EXTRA_DIST           = safec.h.in

#AM_CFLAGS            = @GCOV_CFLAGS@

CFILES = \
	strisdigit_s.c        \
	strzero_s.c           \
	strisalphanumeric_s.c \
	safe_str_constraint.c \
	strcmp_s.c            \
	strfirstsame_s.c      \
	strstr_s.c            \
	strcpy_s.c            \
	strcpyfld_s.c         \
	strtolowercase_s.c    \
	strspn_s.c            \
	strnlen_s.c           \
	strisascii_s.c        \
	strcspn_s.c           \
	strlastchar_s.c       \
	strfirstdiff_s.c      \
	strljustify_s.c       \
	strlastsame_s.c       \
	strcmpfld_s.c         \
	strnterminate_s.c     \
	strfirstchar_s.c      \
	strishex_s.c          \
	strpbrk_s.c           \
	strcpyfldin_s.c       \
	strncat_s.c           \
	strremovews_s.c       \
	strcasestr_s.c        \
	strisuppercase_s.c    \
	strcasecmp_s.c        \
	strispassword_s.c     \
	strtouppercase_s.c    \
	strprefix_s.c         \
	strncpy_s.c           \
	strcpyfldout_s.c      \
	strismixedcase_s.c    \
	strislowercase_s.c    \
	strcat_s.c            \
	strlastdiff_s.c       \
	strtok_s.c            \
	safe_mem_constraint.c \
	mem_primitives_lib.c  \
	memset_s.c            \
	memset16_s.c          \
	memset32_s.c          \
	memcmp_s.c            \
	memcmp16_s.c          \
	memcmp32_s.c          \
	memmove_s.c           \
	memmove16_s.c         \
	memmove32_s.c         \
	memzero_s.c           \
	memzero32_s.c         \
	memzero16_s.c         \
	memcpy_s.c            \
	memcpy16_s.c          \
	memcpy32_s.c

HFILES = \
	safec.h               \
	safe_lib_errno.h      \
	mem_primitives_lib.h  \
	safe_mem_lib.h        \
	safe_lib.h            \
	safe_limits.h         \
	safe_types.h          \
	safe_str_lib.h        \
	safe_str_constraint.h \
	safe_mem_constraint.h

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

#EXPORT_SYM      = exported.sym
#$(EXPORT_SYM): safec.h.in
#        sed -n -e 's/^..*SAFEC_EXPORT[[:space:]][[:space:]]*\([[:alnum:]_][[:alnum:]_]*\)..*$$/\1/p' @top_srcdir@/src/safec.h.in > $@

#libsafec_@SAFEC_API_VERSION@_la_DEPENDENCIES = $(EXPORT_SYM)
libsafec_@SAFEC_API_VERSION@_la_LDFLAGS      = -version-info $(SAFEC_SO_VERSION) \
	                                       -no-undefined
#	                                       -export-symbols $(EXPORT_SYM)
libsafec_@SAFEC_API_VERSION@_la_SOURCES      = $(CFILES) $(HFILES)
#libsafec_@SAFEC_API_VERSION@_la_LIBADD       = @GCOV_LIBS@

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
