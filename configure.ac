# configure.ac - Process with autoconf to produce a configure script
#
# 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
# 2017  Reini Urban <rurban@cpan.org>
#
# Copyright (c) 2012, 2013 Cisco Systems
# Copyright (c) 2017, 2018, 2019, 2020 Reini Urban
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
#                                               -*- Autoconf -*-

# Autoconf requirements
AC_PREREQ([2.69])

# information on the package
#AC_SUBST([PACKAGE_VERSION], [3.7])
AC_INIT([Safe C Library],
	m4_esyscmd([build-aux/version-gen.sh .tarball-version]),
        [https://github.com/rurban/safeclib/issues],
        [safeclib], [http://github.com/rurban/safeclib/])

# Configure the configure script
# ===============================================

# unique source file --- primitive safety check
AC_CONFIG_SRCDIR([src/abort_handler_s.c])

# extra autoconf macros are kept here
AC_CONFIG_MACRO_DIR([m4])

# place to put some extra build scripts
AC_CONFIG_AUX_DIR([build-aux])

# Other tool initializations
# ===============================================

# really severe build strictness also place generated object files (.o)
# into the same directory as their source files, in order to avoid
# collisions when non-recursive make is used.
AM_INIT_AUTOMAKE([1.10 no-define foreign subdir-objects dist-bzip2 dist-xz])

# Check if automake supports 'pretty' builds, if so enable them by default
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Define these substitutions here to keep all version information in
# one place. For information on how to properly maintain the library
# version information, refer to the libtool manual, section "Updating
# library version information":
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
AC_SUBST([SAFEC_SO_VERSION], [3:7:0])
AC_SUBST([SAFEC_API_VERSION], [3.7.0])

# this is missing with mingw 1.0
# m4_pattern_allow([AM_PROG_AR])
# AM_PROG_AR
AC_CHECK_TOOLS([AR], [gcc-ar ar])
AC_CHECK_TOOLS([RANLIB], [gcc-ranlib ranlib])
AC_CHECK_TOOLS([NM], [gcc-nm nm])

# Enable LibTool as we are building a library
#LT_PREREQ([2.2.6]) # TODO: don't really know what to put here so leave
                    # it out for now
LT_INIT

# Initialize pkg-config since we are installing a .pc file
PKG_INSTALLDIR


# Global Variable & Defaults
# ===============================================
# Keep this in sync with what is passed in AC_INIT
TARBALL_VERSION_FILE=".tarball-version"

# Initial values for automake variables AM_*
AM_CPPFLAGS="-I\$(top_srcdir)/include"

dnl we manually export the symbols via dllexport, we only use shared
dnl no libtool -DDLL_EXPORT support yet
dnl case $host_os in
dnl mingw*|msys*)
dnl         AC_MSG_NOTICE([--disable-static on mingw])
dnl         lt_prog_compiler_static=
dnl         enable_static=no
dnl         ;;
dnl esac

AC_MSG_CHECKING([package version])
AC_MSG_RESULT($PACKAGE_VERSION)
AC_MSG_CHECKING([so version-info])
AC_MSG_RESULT($SAFEC_SO_VERSION)
AC_MSG_CHECKING([api version-info])
AC_MSG_RESULT($SAFEC_API_VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

AC_CANONICAL_HOST

# Configure options
# ===============================================

# User switches
AC_ARG_ENABLE(gcov,
	AS_HELP_STRING([--enable-gcov],
		       [turn on test coverage @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_gcov=true ; gcov=gcov ;;
	no)  enable_gcov=false ;;
	*)   enable_gcov=true ; gcov="${enableval}" ;;
	esac], [enable_gcov=false ])

AC_MSG_CHECKING([for --enable-gcov])
if test "x$enable_gcov" = "xtrue" ; then
	dnl if test "$GCC" = yes; then
	dnl 	AC_MSG_ERROR([gcov only works if gcc is used])
	dnl fi

        dnl You might need the compiler-specific gcov: e.g. gcov-mp-6
	GCOV="$gcov"
	AC_MSG_RESULT([gcov=$gcov])
	AC_SUBST(GCOV)

	GCOV_CFLAGS="-fprofile-arcs -ftest-coverage"
	AC_SUBST(GCOV_CFLAGS)

	dnl libtool 1.5.22 and lower strip -fprofile-arcs from the flags
	dnl passed to the linker, which is a bug; -fprofile-arcs implicitly
	dnl links in -lgcov, so we do it explicitly here for the same effect
	GCOV_LIBS=-lgcov
	AC_SUBST(GCOV_LIBS)
else
	AC_MSG_RESULT([no (default)])
fi
AM_CONDITIONAL(ENABLE_GCOV, test "x$enable_gcov" = "xtrue")

AC_ARG_ENABLE(debug,
	AS_HELP_STRING([--enable-debug],
	               [compile library w/ debugging symbols (-g)
	                @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_debug=true ;;
	no)  enable_debug=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
	esac], [enable_debug=false ])
AC_MSG_CHECKING([for --enable-debug])
if test "x$enable_debug" = "xtrue" ; then
        if test "x$enable_gcov" = "xtrue" ; then
	   DEBUG_CFLAGS="-g"
        else
	   DEBUG_CFLAGS="-g -O2"
        fi
        if test "x`uname -o`" = "xDarwin" -a x$cross_compiling = xno; then
            enable_shared=no
            enable_shared_with_static_runtimes=no
        fi
        AC_MSG_RESULT([yes CFLAGS = $DEBUG_CFLAGS])
	AM_CPPFLAGS="${AM_CPPFLAGS} -DDEBUG"
else
        if test "x$enable_gcov" = "xtrue"; then
	   DEBUG_CFLAGS="-g"
        else
           # default release optimization
	   DEBUG_CFLAGS="-O2"
           if test x$GCC = xyes; then
	      DEBUG_CFLAGS="-O2 -D_FORTIFY_SOURCE=2"
           fi
        fi
        AC_MSG_RESULT([no CFLAGS = $DEBUG_CFLAGS])
fi

if test "x$CFLAGS" = "x-g -O2" -o "x$CFLAGS" = "x-g"; then
	# by doing it this way we suppress automake's assumption that if
	# CFLAGS is empty lets compile the code with '-g -O2'.
	CFLAGS="${DEBUG_CFLAGS}"
else
	AC_MSG_WARN([CFLAGS is not empty, will attempt to apply optimization
	             flags to 'AM_CFLAGS' but depending on the contents of
	             CFLAGS these may be overridden.
	             CFLAGS = '$CFLAGS'])
fi
AM_CONDITIONAL(ENABLE_DEBUG, test "x$enable_debug" = "xtrue")

AC_ARG_ENABLE(extensions,
	AS_HELP_STRING([--disable-extensions],
          [This library contains additional functions not defined
	   in the C11 specification. Disable to omit these.
	   @<:@default=no@:>@]),
       [case "${enableval}" in
	yes) enable_extensions=true ;;
	no)  enable_extensions=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --disable-extensions]) ;;
	esac], [enable_extensions=true])
AC_MSG_CHECKING([for --disable-extensions])
if test "x$enable_extensions" = "xfalse" ; then
	AC_MSG_RESULT([yes])
	INSERT_EXTS="#define SAFECLIB_DISABLE_EXTENSIONS 1"
else
	AC_MSG_RESULT([no (default)])
	INSERT_EXTS="#undef SAFECLIB_DISABLE_EXTENSIONS"
fi
#	AC_DEFINE([SAFECLIB_DISABLE_EXTENSIONS], 1,
#		  [Define to 1 to disable additional functions not defined
#		   in the C11 appendix K specification])
AC_SUBST(INSERT_EXTS)
AM_CONDITIONAL(ENABLE_EXTS, test "x$enable_extensions" = "xtrue")

AC_ARG_ENABLE(memmax,
	AS_HELP_STRING([--enable-memmax],
		       [specify the largest object size allowed for the
			mem* functions @<:@default=256MB@:>@]),
	[case "${enableval}" in
	@<:@0-9@:>@*) enable_memmax=${enableval} ;;
	*)      AC_MSG_ERROR([bad value ${enableval} for --enable-memmax]) ;;
	esac], [enable_memmax="(256UL << 20)  /* 256MB */" ])
AC_MSG_CHECKING([for --enable-memmax])
AC_MSG_RESULT([$enable_memmax])
# AC_DEFINE([RSIZE_MAX_MEM], [ $enable_memmax ],
#	  [Maximum object size the mem* functions will allow,
#	   default is 256MB])
RSIZE_MAX_MEM=$enable_memmax
AC_SUBST(RSIZE_MAX_MEM)

AC_ARG_ENABLE(strmax,
	AS_HELP_STRING([--enable-strmax],
		       [specify the largest object size allowed for the
			str* functions @<:@default=4KB@:>@]),
	[case "${enableval}" in
	@<:@0-9@:>@*) enable_strmax=${enableval} ;;
	*)      AC_MSG_ERROR([bad value ${enableval} for --enable-strmax]) ;;
	esac], [enable_strmax="(4UL << 10) /* 4KB */" ])

AC_MSG_CHECKING([for --enable-strmax])
AC_MSG_RESULT([$enable_strmax])
RSIZE_MAX_STR=$enable_strmax
AC_SUBST(RSIZE_MAX_STR)

AC_ARG_ENABLE(nullslack,
	AS_HELP_STRING([--disable-nullslack],
		       [Do not null out the remaining part of a string if it is
			not completely used @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_nullslack=true ;;
	no)  enable_nullslack=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --disable-nullslack]) ;;
	esac], [enable_nullslack=true ])
AC_MSG_CHECKING([for --disable-nullslack])
if test "x$enable_nullslack" = "xtrue" ; then
	AC_MSG_RESULT([no (default)])
	INSERT_NULLSLACK="#define SAFECLIB_STR_NULL_SLACK 1"
	# AC_DEFINE([SAFECLIB_STR_NULL_SLACK], 1,
	# 	  [Define to 1 to null out the remaining part of a string
	# 	   buffer if it is not completely used])
else
	AC_MSG_RESULT([yes])
	INSERT_NULLSLACK="#undef SAFECLIB_STR_NULL_SLACK"
fi
AC_SUBST(INSERT_NULLSLACK)

AC_ARG_ENABLE(constraint-handler,
	AS_HELP_STRING([--disable-constraint-handler],
          [Disable C11 invoke_safe_{str,mem}_constraint_handler
          for performance, smaller size and less flexibility.
	   @<:@default=no@:>@]),
       [case "${enableval}" in
	yes) enable_constraint_handler=true ;;
	no)  enable_constraint_handler=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --disable-constraint-handler]) ;;
	esac], [enable_constraint_handler=true])
AC_MSG_CHECKING([for --disable-constraint-handler])
if test "x$enable_constraint_handler" = "xfalse" ; then
	AC_MSG_RESULT([yes])
	INSERT_CONSTRAINT_HANDLER="#define SAFECLIB_DISABLE_CONSTRAINT_HANDLER 1"
else
	AC_MSG_RESULT([no (default)])
	INSERT_CONSTRAINT_HANDLER="#undef SAFECLIB_DISABLE_CONSTRAINT_HANDLER"
fi
AC_SUBST(INSERT_CONSTRAINT_HANDLER)

AC_ARG_ENABLE(unsafe,
	AS_HELP_STRING([--enable-unsafe],
                [Include unsafe std C11 functions: tmpnam_s @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_unsafe=true ;;
	no)  enable_unsafe=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-unsafe]) ;;
	esac], [enable_unsafe=no ])
AC_MSG_CHECKING([for --enable-unsafe])
if test "x$enable_unsafe" = "xtrue" ; then
	AC_MSG_RESULT([yes])
	INSERT_UNSAFE="#define SAFECLIB_ENABLE_UNSAFE 1"
else
	AC_MSG_RESULT([no])
	INSERT_UNSAFE="#undef SAFECLIB_ENABLE_UNSAFE"
fi
AC_SUBST(INSERT_UNSAFE)
AM_CONDITIONAL([ENABLE_UNSAFE], test "x$enable_unsafe" = "xtrue")

AC_ARG_ENABLE(norm-compat,
	AS_HELP_STRING([--enable-norm-compat],
		       [Enable NFKC and NFKD modes for wcsnorm. @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_norm_compat=true ;;
	no)  enable_norm_compat=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-norm-compat]) ;;
	esac], [enable_norm_compat=false ])
AC_MSG_CHECKING([for --enable-norm-compat])
if test "x$enable_norm_compat" = "xtrue" ; then
	AC_MSG_RESULT([yes])
	AC_DEFINE([HAVE_NORM_COMPAT], 1,
	 	  [Define to 1 to enable the compat normalization modes for wcsnorm_s
                  NFKC and NFKD])
else
	AC_MSG_RESULT([no (default)])
fi

AC_ARG_ENABLE(warn-dmax,
	AS_HELP_STRING([--enable-warn-dmax],
		       [Enable dmax checks against __builtin_object_size(dest). @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_warn_dmax=true ;;
	no)  enable_warn_dmax=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-warn-dmax]) ;;
	esac], [enable_warn_dmax=false ])
AC_MSG_CHECKING([for --enable-warn-dmax])
if test "x$enable_warn_dmax" = "xtrue" ; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no (default)])
fi

AC_ARG_ENABLE(error-dmax,
	AS_HELP_STRING([--enable-error-dmax],
		       [Make --enable-warn-dmax fatal. @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_error_dmax=true ;;
	no)  enable_error_dmax=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-error-dmax]) ;;
	esac], [enable_error_dmax=false ])
AC_MSG_CHECKING([for --enable-error-dmax])
if test "x$enable_error_dmax" = "xtrue" ; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no (default)])
fi

AC_ARG_ENABLE(doc,
	AS_HELP_STRING([--disable-doc],
	               [disable documentation @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_doc=true ;;
	no)  enable_doc=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-doc]) ;;
	esac], [enable_doc=true])
AM_CONDITIONAL(ENABLE_DOC, test "x$enable_doc" = "xtrue")

dnl for windows dllimport. checking pic_flag DLL_EXPORT would be better,
dnl but this is only enabled for the shared objs, and we need it in the config
dnl for our tests.
if test "x$enable_shared" = "xno" ; then
	INSERT_DISABLE_DLLIMPORT="#define DISABLE_DLLIMPORT 1"
else
	INSERT_DISABLE_DLLIMPORT="#undef DISABLE_DLLIMPORT"
fi
AC_SUBST(INSERT_DISABLE_DLLIMPORT)

# Maintainer switches
AC_ARG_ENABLE(debug-build,
	AS_HELP_STRING([--enable-debug-build],
	               [turn on build debugging, generally is not needed
	                unless you are the maintainer @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_debug_build=true ;;
	no)  enable_debug_build=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug-build]) ;;
	esac], [enable_debug_build=false ])
AM_CONDITIONAL(ENABLE_DEBUG_BUILD, test "x$enable_debug_build" = "xtrue")


AC_MSG_NOTICE([Check programs])
# ===============================================
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AM_PROG_CC_C_O
AC_PROG_CC_C99
AC_PROG_CC_C11

if test "x$ac_cv_prog_cc_c99" = "xno"; then
   dnl we really should ERROR, lets see how far can come without
   AC_MSG_WARN([This compiler ${CC} has no c99 support])
   INSERT_SAFECLIB_HAVE_C99="#undef SAFECLIB_HAVE_C99"
else
   AC_DEFINE([SAFECLIB_HAVE_C99], 1,
        [Defined to 1 when the compiler supports c99, mostly (...) macros])
   if test "x$ac_cv_prog_cc_c99" != "x"; then
      AC_MSG_RESULT([added $ac_cv_prog_cc_c99 to CFLAGS])
      AM_CFLAGS="$AM_CFLAGS $ac_cv_prog_cc_c99"
   fi
   INSERT_SAFECLIB_HAVE_C99="#define SAFECLIB_HAVE_C99 1"
fi
dnl AM_CONDITIONAL([SAFECLIB_HAVE_C99], [test "x$ac_cv_prog_cc_c99=no" != "xno"])
AC_SUBST(SAFECLIB_HAVE_C99)
AC_SUBST(INSERT_SAFECLIB_HAVE_C99)

if test "x$ac_cv_prog_cc_c11" != "xno"; then
   AC_DEFINE([HAVE_C11], 1,
        [Defined to 1 when the compiler supports c11])
   if test "x$ac_cv_prog_cc_c11" != "x"; then
      AC_MSG_RESULT([added $ac_cv_prog_cc_c11 to CFLAGS])
      AM_CFLAGS="$AM_CFLAGS $ac_cv_prog_cc_c11"
   fi
fi
AC_SUBST(HAVE_C11)

dnl c++ needs less warnings
if test -z $ax_compiler_cxx; then
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
    [#ifndef __cplusplus
     #error "no C++"
     #endif]])],
    [ax_compiler_cxx=yes;],
    [ax_compiler_cxx=no;])
fi
if test "$ax_compiler_cxx" = "yes" ; then
  AC_DEFINE([HAVE_CXX], 1,
        [Defined to 1 when the compiler is C++])
dnl   ax_cv_check_cflags__Wnested_externs=no
dnl   ax_cv_check_cflags__Wmissing_prototypes=no
dnl   ax_cv_check_cflags__Wstrict_prototypes=no
dnl   ax_cv_check_cflags__Wimplicit_function_declaration=no
dnl   ax_cv_check_cflags__Wdeclaration_after_statement=no
dnl   ax_cv_check_cflags__Wold_style_definition=no
dnl   ax_cv_check_cflags__Wjump_misses_init=no
fi

dnl we still have some empty src files
ax_cv_check_ldflags___Wl___fatal_warnings=no
ax_cv_check_ldflags___Wl__fatal_warnings=no
dnl sprintf_s passing through to vsnprintf_s
ax_cv_check_cflags__Wformat_2=no
ax_cv_check_cflags__Wformat_nonliteral=no
dnl windows mult. decl are not properly sorted out yet
dnl const -Wdiscarded-qualifiers is stricter on mingw
case $host_os in
  mingw*|cygwin*|msys*)
    ax_cv_check_cflags__Wredundant_decls=no
    ax_enable_compile_warnings=yes
    ;;
esac
dnl gcc 4.2 fails with -Wmissing-format-attribute
dnl https://gcc.gnu.org/bugzilla/show_bug.cgi?id=28492
if test "x$GCC" != "x" ; then
  AC_MSG_CHECKING([for -Wmissing-format-attribute usability])
  old_cflags="$CFLAGS"
  CFLAGS="-Wmissing-format-attribute -Werror"
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[
#include <stdio.h>
#include <stdarg.h>

void myvsprintf(char *dest, const char *fmt, va_list ap);
int mysprintf(char *dest, const char *fmt, ...);

int myvsprintf(char *dest, const char *fmt, va_list ap) {
    return vsprintf(dest, fmt, ap);
}
int mysprintf(char *dest, const char *fmt, ...) {
    va_list ap;
    int ret;
    va_start(ap, fmt);
    ret = myvsprintf(dest, fmt, ap);
    va_end(ap);
    return ret;
}
    ]],[[
      const char* fmt = "%s";
      char dest[12];
      return mysprintf(dest, fmt) ? 0: 1;
      ]]
    )],
    [ AC_MSG_RESULT([yes])],
    [ AC_MSG_RESULT([no])
      ax_cv_check_cflags__Winline=no
      ax_cv_check_cflags__Wmissing_format_attribute=no
    ],
    [ AC_MSG_RESULT([no (cross)])
      ax_cv_check_cflags__Winline=no
      ax_cv_check_cflags__Wmissing_format_attribute=no
    ])
  CFLAGS="$old_cflags"
fi
AX_COMPILER_FLAGS
case $host_os in
  mingw*|cygwin*)
    WARN_CFLAGS="-Wall -Wextra"
    ;;
esac
dnl older non-c99 g++ complain about __VA_ARGS__, for();
if test "x$ac_cv_prog_cc_c99" = "xno" -a x$ax_compiler_cxx = xyes; then
    WARN_CFLAGS=`echo $WARN_CFLAGS | sed -e"s,-Werror ,,"`
fi

AX_CHECK_COMPILE_FLAG([-Werror=unknown-warning-option],
   [ax_ccf_err="-Werror=unknown-warning-option"],[ax_ccf_err=""])
# Try Intel's libmpx dynamic pointer bounds checker,
# available for gcc-5+ and icc-15.0+, but disabled on gcc by default. --enable-libmpx
# https://gcc.gnu.org/wiki/Intel%20MPX%20support%20in%20the%20GCC%20compiler
# and https://intel-mpx.github.io/design/
# See also https://github.com/google/sanitizers/wiki/AddressSanitizerIntelMemoryProtectionExtensions
dnl gcc: slows down the optimizer with gcc-5.0. 5.3 should be better.
dnl AX_CHECK_COMPILE_FLAG([-fcheck-pointer-bounds -mmpx],
dnl    [CFLAGS="$CFLAGS -fcheck-pointer-bounds -mmpx" LDFLAGS="$LDFLAGS -lmpx -lmpxwrappers"],
dnl    [],[$ax_ccf_err])
#AX_APPEND_COMPILE_FLAGS(["-fcheck-pointer-bounds -mmpx"],
#   [CFLAGS],[$ax_ccf_err])
dnl icc:
if test x$ac_compiler_gnu = xno -a -n "$ax_ccf_err"; then
    AX_CHECK_COMPILE_FLAG([-check-pointers-mpx=rw],
       [CFLAGS="$CFLAGS -check-pointers-mpx=rw" LIBS="$LIBS -lmpx -lchkp"],
       [],[$ax_ccf_err])
    #AX_APPEND_COMPILE_FLAGS(["-check-pointers-mpx=rw"],
    #   [CFLAGS],[$ax_ccf_err])
fi

dnl windows mult. decl are not properly sorted out yet
case $host_os in
  mingw*|cygwin*|msys*)
    dnl gcc 3.4 cannot do -Wno-attributes
    AX_APPEND_COMPILE_FLAGS([-Wno-attributes -Wno-discarded-qualifiers],
                            [WARN_CFLAGS],[$ax_ccf_err])
    dnl AM_LDFLAGS=`echo $AM_LDFLAGS|sed -e"s/-Wl,--as-needed -Wl,--no-as-needed//"`
    dnl x86 needs that for _mm_mfence
    AX_CHECK_COMPILE_FLAG([-msse2],
       [CFLAGS="$CFLAGS -msse2" LDFLAGS="$LDFLAGS -msse2"],
       [],[$ax_ccf_err])
    ;;
esac
AX_APPEND_COMPILE_FLAGS([-fstack-protector-strong -fstack-clash-protection -fcf-protection],
                         [WARN_CFLAGS],[$ax_ccf_err])
AX_APPEND_LINK_FLAGS([-fstack-protector-strong -fstack-clash-protection -fcf-protection],
                         [WARN_LDFLAGS],[$ax_ccf_err])

if test x$ax_cv_check_cflags__Wrestrict = xyes; then
   AC_DEFINE([HAVE_WARNING_RESTRICT], 1, [Have -Wrestrict])
fi
PEDANTIC="-pedantic"
WARN_CFLAGS_TEST="-Wall -Wextra"
if test x$ac_compiler_gnu = xno; then
    AX_CHECK_COMPILE_FLAG([-pedantic],
       [PEDANTIC="-pedantic"],
       [PEDANTIC=""
        WARN_CFLAGS_TEST=""],[$ax_ccf_err])
    AX_CHECK_COMPILE_FLAG([-Werror],
       [],
       [WARN_CFLAGS=""
        WARN_CFLAGS_TEST=""],[$ax_ccf_err])
fi
AC_SUBST(PEDANTIC, $PEDANTIC)
AX_GCC_BUILTIN(__builtin_object_size)
dnl against -Werror,-Wgcc-compat fortify errors, only on clang
INSERT_OBJECT_SIZE="#undef HAVE___BUILTIN_OBJECT_SIZE"
INSERT_WARN_DMAX="#undef HAVE_WARN_DMAX"
INSERT_ERROR_DMAX="#undef HAVE_ERROR_DMAX"

if test x$ax_cv_have___builtin_object_size = xyes; then
    AX_CHECK_COMPILE_FLAG([-Wuser-defined-warnings],
        [have_user_defined_warnings="yes"],[have_user_defined_warnings="no"])
    AX_CHECK_COMPILE_FLAG([$ax_ccf_err -Wgcc-compat],
        [WARN_CFLAGS="$WARN_CFLAGS -Wno-gcc-compat"],[])
    if test $have_user_defined_warnings = yes; then
        AX_APPEND_COMPILE_FLAGS([-Wno-error=user-defined-warnings],
            [WARN_CFLAGS],[$ax_ccf_err])
        AC_DEFINE([HAVE_USER_DEFINED_WARNINGS], 1, [Uses -Wuser-defined-warnings])
    fi
    INSERT_OBJECT_SIZE="# define HAVE___BUILTIN_OBJECT_SIZE 1"
    if test "x$enable_warn_dmax" = "xtrue" ; then
        INSERT_WARN_DMAX="# define HAVE_WARN_DMAX 1"
    fi
    if test "x$enable_error_dmax" = "xtrue" ; then
        INSERT_WARN_DMAX="#define HAVE_WARN_DMAX 1"
        INSERT_ERROR_DMAX="#define HAVE_ERROR_DMAX 1"
    fi
else
    if test "x$enable_warn_dmax" = "xtrue" ; then
      AC_MSG_RESULT([warn-dmax ignored, no __builtin_object_size])
    fi
fi

AC_SUBST(INSERT_OBJECT_SIZE)
AC_SUBST(INSERT_WARN_DMAX)
AC_SUBST(INSERT_ERROR_DMAX)

AC_MSG_CHECKING([for diagnose_if attribute])
old_cflags="$CFLAGS"
CFLAGS="-Werror"
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[
#include <stdio.h>

int myfunc(char *ptr)
    __attribute__((diagnose_if(!ptr, "empty ptr", "error")));

int myfunc(char *ptr) {
    return ptr ? 1 : 0;
}
    ]],[[
      char dest[12];
      return myfunc(dest);
      ]]
    )],
    [ AC_MSG_RESULT([yes])
      have_attribute_diagnose_if=yes
      WARN_CFLAGS_TESTS="$WARN_CFLAGS_TESTS -Wno-gcc-compat"
      AC_DEFINE([HAVE_ATTRIBUTE_DIAGNOSE_IF], 1,
                [Defined to 1 when the compiler supports attribute diagnose_if, since clang-5])
    ],
    [ AC_MSG_RESULT([no])
      have_attribute_diagnose_if=no
    ],
    [ AC_MSG_RESULT([no (cross)])
      have_attribute_diagnose_if=no
    ])
CFLAGS="$old_cflags"
AM_CONDITIONAL([HAVE_ATTRIBUTE_DIAGNOSE_IF], [test "x$have_attribute_diagnose_if" = "xyes"])
AC_SUBST(WARN_CFLAGS_TEST, $WARN_CFLAGS_TEST)

AX_ASM_INLINE()


dnl Was needed for wcsnorm_s / mark.h. So far not needed anymore
dnl AX_CHECK_COMPILE_FLAG([-fbracket-depth=512],
dnl    [AM_CFLAGS="$AM_CFLAGS -fbracket-depth=512"], [], [], [])

AM_CONDITIONAL([HAVE_MINGW], [test "x$host_os" = "xmingw32"])
AM_CONDITIONAL([HAVE_MINGW_CROSS],
        [test "x$host_os" = "xmingw32" -a "x$build_os" != "xmingw32"])
if test "x$host_os" = "xmingw32" -a "x$build_os" != "xmingw32"; then
   AC_CHECK_PROGS([WINE], [wine])
   if test -z "$WINE"; then
      AC_MSG_WARN([wine not found - cannot check cross-compiled executables])
   else
      AC_MSG_RESULT([    wine found - test with make check-wine])
      AM_CFLAGS="$AM_CFLAGS -D_WINE_MSVCRT"
   fi
fi

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([doxygen not found - continuing without doxygen support])
fi
AC_CHECK_PROGS([POD2MAN], [pod2man])
if test -z "POD2MAN";
   then AC_MSG_WARN([pod2man not found - continuing without pod2man support])
fi
AC_CHECK_PROGS([PANDOC], [pandoc])
if test -z "$PANDOC";
   then AC_MSG_WARN([pandoc not found - README.rst cannot be generated from README.md])
fi
AM_CONDITIONAL([HAVE_DSYMUTIL], [test -n "$ac_ct_DSYMUTIL"])

case $host_os in
  darwin*)
    enable_valgrind_helgrind=no
    enable_valgrind_drd=no
    ;;
esac
dnl passes only on some systems. --enable-valgrind-sgcheck
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_CHECK()

# Checks for libraries
# ===============================================
# None

AC_MSG_NOTICE([Check header files])
# ===============================================
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_HEADER_TIME
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([stdlib.h	\
                  memory.h	\
                  ctype.h	\
                  malloc.h	\
                  string.h	\
                  limits.h	\
                  sys/types.h	\
                  inttypes.h	\
                  stdint.h	\
                  errno.h	\
                  wchar.h	\
                  langinfo.h    \
                  valgrind/valgrind.h \
                  intrin.h      \
                  xmmintrin.h   \
                  emmintrin.h   \
                  x86intrin.h   \
                  arm_neon.h    \
                  mmintrin.h    \
                  altivec.h     \
                  spe.h         \
                  mbarrier.h    \
                  ])

INSERT_SYS_TYPES_H=""
if test "x$ac_cv_header_sys_types_h" = "xyes"; then
	INSERT_SYS_TYPES_H="#include <sys/types.h>"
fi
AC_SUBST(INSERT_SYS_TYPES_H)

INSERT_INTTYPES_H=""
if test "x$ac_cv_header_inttypes_h" = "xyes"; then
	INSERT_INTTYPES_H="#include <inttypes.h>"
fi
AC_SUBST(INSERT_INTTYPES_H)

INSERT_STDINT_H=""
if test "x$ac_cv_header_stdint_h" = "xyes"; then
	INSERT_STDINT_H="#include <stdint.h>"
fi
AC_SUBST(INSERT_STDINT_H)

INSERT_ERRNO_H=""
if test "x$ac_cv_header_errno_h" = "xyes"; then
	INSERT_ERRNO_H="#include <errno.h>"
fi
AC_SUBST(INSERT_ERRNO_H)

INSERT_BOOL_SUPPORT="#include <stdbool.h>"
if test "x$ac_cv_header_stdbool_h" = "xno"; then
	INSERT_BOOL_SUPPORT="
# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
"
if test "x$ac_cv_type__bool" = "xno"; then
	INSERT_BOOL_SUPPORT="
#ifdef __cplusplus
typedef bool _Bool;
#else
# define _Bool unsigned char
#endif
$INSERT_BOOL_SUPPORT
"
	fi
fi
AC_SUBST(INSERT_BOOL_SUPPORT)


AC_MSG_NOTICE([Check typedefs, structures, and compiler characteristics])
# ===============================================
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_INT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINTPTR_T
AC_CHECK_SIZEOF(size_t)
AC_CHECK_TYPE([errno_t],
        [FALLBACK_ERRNO_T=""],
        [FALLBACK_ERRNO_T="typedef int errno_t;"],
        [[#include <errno.h>]])
AC_SUBST(FALLBACK_ERRNO_T)
AC_TYPE_MBSTATE_T
AC_CHECK_MEMBER(struct tm.tm_gmtoff, 
  [AC_DEFINE(HAVE_TM_GMTOFF, 1, 
     [Define if struct tm has the tm_gmtoff member.])], 
     , 
     [#include <time.h>])
AX_COMPILE_CHECK_SIZEOF(time_t, [#include <time.h>])
AC_CHECK_TYPE([wchar_t],[],[],[[#include <wchar.h>]])

AX_GCC_BUILTIN(__builtin_ctz)
AX_GCC_BUILTIN(__builtin_constant_p)
INSERT_CONSTANT_P="#undef HAVE___BUILTIN_CONSTANT_P"
if test x$ax_cv_have___builtin_constant_p = xyes; then
  INSERT_CONSTANT_P="#define HAVE___BUILTIN_CONSTANT_P 1"
fi
AC_SUBST(INSERT_CONSTANT_P)

AX_GCC_FUNC_ATTRIBUTE(format)
AX_GCC_FUNC_ATTRIBUTE(malloc)
AX_GCC_FUNC_ATTRIBUTE(returns_nonnull)
#not yet supported, planned since 2008
AX_GCC_FUNC_ATTRIBUTE(format_wprintf)
AX_GCC_FUNC_ATTRIBUTE(format_wscanf)

if test x$ax_cv_have_func_attribute_format_wprintf = xyes; then
  HAVE_ATTRIBUTE_FORMAT_WPRINTF=1
else
  HAVE_ATTRIBUTE_FORMAT_WPRINTF=0
fi
if test x$ax_cv_have_func_attribute_format_wscanf = xyes; then
  HAVE_ATTRIBUTE_FORMAT_WSCANF=1
else
  HAVE_ATTRIBUTE_FORMAT_WSCANF=0
fi
AC_SUBST(HAVE_ATTRIBUTE_FORMAT_WPRINTF)
AC_SUBST(HAVE_ATTRIBUTE_FORMAT_WSCANF)

builtins_chk="memcpy memmove memset strcpy strncpy strcat strncat
              printf sprintf snprintf swprintf
              vfprintf vfwprintf vsprintf vsnprintf"
#AX_GCC_BUILTIN(__builtin___memcpy_chk)
#AX_GCC_BUILTIN(__builtin___memmove_chk)
#AX_GCC_BUILTIN(__builtin___memset_chk)
#AX_GCC_BUILTIN(__builtin___strcpy_chk)
#AX_GCC_BUILTIN(__builtin___strncpy_chk)
#AX_GCC_BUILTIN(__builtin___strcat_chk)
#AX_GCC_BUILTIN(__builtin___strncat_chk)
#AX_GCC_BUILTIN(__builtin___printf_chk)
#AX_GCC_BUILTIN(__builtin___sprintf_chk)
#AX_GCC_BUILTIN(__builtin___snprintf_chk)
#AX_GCC_BUILTIN(__builtin___swprintf_chk)
#AX_GCC_BUILTIN(__builtin___vfprintf_chk)
#AX_GCC_BUILTIN(__builtin___vfwprintf_chk)
#AX_GCC_BUILTIN(__builtin___vsprintf_chk)
#AX_GCC_BUILTIN(__builtin___vsnprintf_chk)

builtins_bounds="narrow_ptr_bounds copy_ptr_bounds init_ptr_bounds
        set_ptr_bounds null_ptr_bounds
        chk_ptr_lbounds chk_ptr_ubounds chk_ptr_bounds get_ptr_lbound get_ptr_ubound"
#for b in $builtins_bounds; do
#    AX_GCC_BUILTIN([__builtin___bnd_$b])
#done
#AX_GCC_BUILTIN(__builtin___bnd_narrow_ptr_bounds)
#AX_GCC_BUILTIN(__builtin___bnd_init_ptr_bounds)
#AX_GCC_BUILTIN(__builtin___bnd_copy_ptr_bounds)
AX_GCC_BUILTIN(__builtin___bnd_set_ptr_bounds)
AX_GCC_BUILTIN(__builtin___bnd_null_ptr_bounds)
#AX_GCC_BUILTIN(__builtin___bnd_chk_ptr_lbounds)
#AX_GCC_BUILTIN(__builtin___bnd_chk_ptr_ubounds)
AX_GCC_BUILTIN(__builtin___bnd_chk_ptr_bounds)
#AX_GCC_BUILTIN(__builtin___bnd_get_ptr_lbound)
#AX_GCC_BUILTIN(__builtin___bnd_get_ptr_ubound)

AC_MSG_NOTICE([Check library functions])
# ===============================================
# dynamic checks if registered via malloc (e.g. libmpx)
AC_CHECK_FUNCS([__bnd_chk_ptr_bounds __bnd_set_ptr_bounds __bnd_null_ptr_bounds])
dnl bos enabled chk functions (but not yet usable)
#for b in $builtins_chk; do
AC_CHECK_FUNCS([__memcpy_chk __memmove_chk __memset_chk \
        __strcpy_chk __strncpy_chk __strcat_chk __strncat_chk \
        __printf_chk __sprintf_chk __snprintf_chk __swprintf_chk \
        __vfprintf_chk __vfwprintf_chk __vsprintf_chk __vsnprintf_chk \
        __vsscanf_chk __vswscanf_chk ])
#done
dnl They need to include the relevant intrin header first
dnl TODO: probe for MEMORY_BARRIER and COMPILER_BARRIER here
dnl AC_CHECK_FUNCS([ __sync_synchronize MemoryBarrier _mm_mfence _mm_sfence \
dnl        __machine_rw_barrier ])
AC_FUNC_MEMCMP
dnl no header needed as is checks if the linker can find it
dnl mbstowcs missing on cygwin64
AC_CHECK_FUNCS([ memset strcmp strcasecmp strcasestr strcspn strpbrk strspn \
        strnstr strnlen strrchr memrchr strstr wmemchr wmemcmp wcscmp wcsstr \
        vswprintf vsnwprintf vswscanf mbsrtowcs mbstowcs iswdigit iswspace \
        towlower towupper towctrans bcmp secure_getenv timingsafe_memcmp \
        timingsafe_bcmp explicit_bzero explicit_memset \
        asctime_r ctime_r gmtime_r localtime_r memccpy ])

dnl add the extensions, pretending we want them
$as_echo '#define __STDC_WANT_LIB_EXT1__  1' >>confdefs.h
dnl native safec shadowing functions (should be none)
AC_CHECK_FUNCS([_memcpy_s_chk _memmove_s_chk _memset_s_chk _memcmp_s_chk \
        _strcpy_s_chk _strncpy_s_chk _strcat_s_chk _strncat_s_chk _strnlen_s_chk \
        _printf_s_chk _sprintf_s_chk _snprintf_s_chk _swprintf_s_chk \
        _vfprintf_s_chk _vfwprintf_s_chk _vsprintf_s_chk _vsnprintf_s_chk ])
dnl native C11 Annex K functions
dnl vsnprintf_s vsnwprintf_s strtok_s wcstok_s deviate in mingw from c11
AC_CHECK_FUNCS(
      [ memset_s memcpy_s memmove_s memzero_s memchr_s memrchr_s memccpy_s \
        sprintf_s strcat_s strcpy_s strncat_s strncpy_s \
        strnlen_s strtok_s strerror_s vsprintf_s tmpnam_s snprintf_s \
        vsnprintf_s snwprintf_s \
        strspn_s strset_s strchr_s strrchr_s strstr_s strzero_s strnset_s \
        mbsrtowcs_s mbstowcs_s wcsrtombs_s wcstombs_s \
        wcrtomb_s wctomb_s wcsnlen_s wcscpy_s wcsncpy_s wcscat_s wcsncat_s \
        wmemcpy_s wmemmove_s wcstok_s vswprintf_s swprintf_s vfwprintf_s \
        vwprintf_s wprintf_s fwprintf_s swscanf_s vswscanf_s wscanf_s \
        vfwscanf_s fwscanf_s vwscanf_s vsnwprintf_s wcsstr_s wmemcmp_s \
        wcscmp_s wcsncmp_s wcsicmp_s wcsset_s wcsnset_s wcscoll_s wcslwr_s \
        wcsupr_s towfc_s wcsfc_s sscanf_s fscanf_s scanf_s vfscanf_s \
        vsscanf_s vscanf_s printf_s fprintf_s tmpfile_s vfprintf_s vprintf_s \
        fopen_s freopen_s gets_s bsearch_s qsort_s gmtime_s localtime_s \
        asctime_s ctime_s getenv_s ])
dnl but delete it, because we override it case by case
$GREP -v '__STDC_WANT_LIB_EXT1__' confdefs.h > confdefs.h.tmp
mv confdefs.h.tmp confdefs.h
        
AM_CONDITIONAL([HAVE_WCSSTR], [test "x$ac_cv_func_wcsstr" = "xyes"])
AM_CONDITIONAL([HAVE_MBSTOWCS], [test "x$ac_cv_func_mbstowcs" = "xyes"])
AM_CONDITIONAL([HAVE_MEMCPY_S], [test "x$ac_cv_func_memcpy_s" = "xyes"])
AM_CONDITIONAL([HAVE_VSNWPRINTF_S], [test "x$ac_cv_func_vsnwprintf_s" = "xyes"])
AM_CONDITIONAL([HAVE_TIMINGSAFE_BCMP],
   [test "x$ac_cv_func_timingsafe_memcmp" = "xyes" -a "x$ac_cv_func_timingsafe_bcmp" = "xyes" ])

if test "x$ac_cv_func_towupper" = "xyes"; then
  AC_MSG_CHECKING([for towupper usability])
  AC_RUN_IFELSE( [AC_LANG_PROGRAM([[
#include <wctype.h>
]],
dnl musl fails on this CYRILLIC SMALL LETTER ROUNDED VE, need at least Unicode 9
dnl windows has not yet updated to 12.1: the micro sign
		[[
return (towupper(0x1C80) == 0x412
     && towupper(0xB5) == 0x3BC
     && towupper(0x432) == 0x1C80)
       ? 0 : 1;
]])],
        [ AC_MSG_RESULT([yes])
          ac_cv_towupper_ok=yes
          AC_DEFINE_UNQUOTED([HAVE_TOWUPPER_OK], 1, [Define to 1 if 'towupper' is usable.])
	],
	[ AC_MSG_RESULT([no]) ],
	[ AC_MSG_RESULT([no (cross)]) ])
fi

dnl cygwin had a broken strnstr until Aug 2017
if test "x$ac_cv_func_strnstr" = "xyes"; then
  AC_MSG_CHECKING([for strnstr usability])
  AC_RUN_IFELSE( [AC_LANG_PROGRAM([[
#include <string.h>
]],
        [[
return strnstr("%s %n", "%n", 6) ? 0 : 1;
]])],
        [ AC_MSG_RESULT([yes])
          ac_cv_strnstr_ok=yes
          AC_DEFINE_UNQUOTED([HAVE_STRNSTR_OK], 1, [Define to 1 if 'strnstr' is usable.])
	],
	[ AC_MSG_RESULT([no]) ],
	[ AC_MSG_RESULT([no (cross)]) ])
fi

dnl this user choice needs to probe first for the default
AC_ARG_ENABLE(wchar,
	AS_HELP_STRING([--disable-wchar],
		       [Disable multibyte and wchar support. @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_wchar=true ;;
	no)  enable_wchar=false ;;
        dnl early mingw32 have only crippled wchar support: only wcscmp, wcslen
	*)   if test x$ac_cv_type_wchar_t = xyes -a x$ac_cv_func_vswprintf = xyes \
                  -a x$ac_cv_header_wchar_h = xyes
             then
               enable_wchar=true
             else
               AC_MSG_WARN([no proper wchar support])
               enable_wchar=false
             fi ;;
	esac], [enable_wchar=true])
AC_MSG_CHECKING([for --disable-wchar or broken wchar usability])
dnl catch mingw32 1.0 with crippled wchar


if test x$ac_cv_header_wchar_h = xyes -a x$enable_wchar = xtrue \
   -a x$ac_cv_func_vswprintf = xyes \
   -a x$ac_cv_func_mbsrtowcs = xyes -a x$ac_cv_func_vswscanf = xyes
then
  enable_wchar=true
else
  enable_wchar=false
fi
if test "x$enable_wchar" = "xtrue" ; then
	AC_MSG_RESULT([no (default)])
        INSERT_DISABLE_WCHAR="#undef SAFECLIB_DISABLE_WCHAR"
        AX_COMPILE_CHECK_SIZEOF(wchar_t, [#include <wchar.h>])
else
	AC_MSG_RESULT([yes])
        INSERT_DISABLE_WCHAR="#define SAFECLIB_DISABLE_WCHAR 1"
        ac_cv_header_wchar_h=no
fi
AM_CONDITIONAL([ENABLE_WCHAR], [test "x$enable_wchar" = "xtrue"])
AC_SUBST(INSERT_DISABLE_WCHAR)


# Checks for system services
# ===============================================
# None

# Output to downstream tools, like automake
# ===============================================
# This section emits all variables that are expected/needed by other tools
# mainly automake.

AH_TOP([
#ifndef __SAFECLIB_CONF_H__
#define __SAFECLIB_CONF_H__
])

AH_BOTTOM([
#endif /* __SAFECLIB_CONF_H__ */
])

AC_SUBST([TARBALL_VERSION_FILE])

# Automake variables, these variables get automagically included at the top
# of all automake generated make files. This is why you don't see them
# referenced in Makefile.am files.
AM_LDFLAGS="$AM_LDFLAGS $WARN_LDFLAGS"
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

# Some build debugging output
if test "x$enable_debug_build" = "xtrue"; then
	AC_MSG_RESULT([AM_CPPFLAGS = $AM_CPPFLAGS])
	AC_MSG_RESULT([AM_CFLAGS = $AM_CFLAGS])
	AC_MSG_RESULT([AM_LDFLAGS = $AM_LDFLAGS])
	AC_MSG_RESULT([CFLAGS = $CFLAGS])
	AC_MSG_RESULT([WARN_CFLAGS = $WARN_CFLAGS])
	AC_MSG_RESULT([WARN_CFLAGS_TESTS = $WARN_CFLAGS_TESTS])
	AC_MSG_RESULT([PEDANTIC = $PEDANTIC])
fi

# Output files
# ===============================================

# Generate two configuration headers; one for building the library
# itself with an autogenerated template, and a second one that will
# be installed alongside the library.
# This does the #define/#undef magic
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([libsafec.pc
                 Makefile
                 src/Makefile
                 tests/Makefile])
AC_CONFIG_FILES([include/safe_config.h], [chmod -w include/safe_config.h])
AC_CONFIG_FILES([include/safe_lib_errno.h], [chmod -w include/safe_lib_errno.h])
AC_CONFIG_FILES([include/safe_types.h], [chmod -w include/safe_types.h])

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_CONDITIONAL([HAVE_POD2MAN], [test -n "$POD2MAN"])
AM_CONDITIONAL([HAVE_PANDOC], [test -n "$PANDOC"])
AM_CONDITIONAL([HAVE_WINE], [test -n "$WINE"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([Doxyfile])])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([doc/footer])])

AC_OUTPUT

dnl Local Variables:
dnl comment-start: "dnl "
dnl comment-end: ""
dnl comment-start-skip: "\\bdnl\\b\\s *"
dnl compile-command: "autoreconf --warnings=all"
dnl End:
