# configure.ac - Process with autoconf to produce a configure script
#
# 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
# 2017  Reini Urban <rurban@cpan.org>
#
# Copyright (c) 2012, 2013 Cisco Systems
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
#                                               -*- Autoconf -*-

# Autoconf requirements
AC_PREREQ([2.69])

# information on the package
AC_INIT([Safe C Library],
        [m4_esyscmd(build-tools/version-gen.sh .tarball-version)],
        [https://github.com/rurban/safeclib/issues],
        [libsafec],
        [http://github.com/rurban/safeclib/])

# Configure the configure script
# ===============================================

# unique source file --- primitive safety check
AC_CONFIG_SRCDIR([src/abort_handler_s.c])

# extra autoconf macros are kept here
AC_CONFIG_MACRO_DIR([m4])

# place to put some extra build scripts
AC_CONFIG_AUX_DIR([build-tools])

# Other tool initializations
# ===============================================

# really severe build strictness also place generated object files (.o)
# into the same directory as their source files, in order to avoid
# collisions when non-recursive make is used.
AM_INIT_AUTOMAKE([1.10 no-define foreign subdir-objects dist-bzip2 dist-xz])

# Check if automake supports 'pretty' builds, if so enable them by default
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# this is missing with mingw 1.0
m4_pattern_allow([AM_PROG_AR])
AM_PROG_AR

# Enable LibTool as we are building a library
#LT_PREREQ([2.2.6]) # TODO: don't really know what to put here so leave
                    # it out for now
LT_INIT


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
#	AC_DEFINE([SAFECLIB_DISABLE_EXTENSIONS], [ 1 ],
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
		       [Null out the remaining part of a string if it is
			not completely used @<:@default=yes@:>@]),
	[case "${enableval}" in
	yes) enable_nullslack=true ;;
	no)  enable_nullslack=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --disable-nullslack]) ;;
	esac], [enable_nullslack=true ])
AC_MSG_CHECKING([for --disable-nullslack])
if test "x$enable_nullslack" = "xtrue" ; then
	AC_MSG_RESULT([no (default)])
	INSERT_NULLSLACK="#define SAFECLIB_STR_NULL_SLACK 1"
	# AC_DEFINE([SAFECLIB_STR_NULL_SLACK], [ 1 ],
	# 	  [Define to 1 to null out the remaining part of a string
	# 	   buffer if it is not completely used])
else
	AC_MSG_RESULT([yes])
	INSERT_NULLSLACK="#undef SAFECLIB_STR_NULL_SLACK"
fi
AC_SUBST(INSERT_NULLSLACK)

AC_ARG_ENABLE(unsafe,
	AS_HELP_STRING([--enable-unsafe],
                [Include unsafe std C11 functions: snprintf_s, vsnprintf_s,
                tmpnam_s @<:@default=no@:>@]),
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
	AC_DEFINE([HAVE_NORM_COMPAT], [ 1 ],
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
	AC_DEFINE([HAVE_WARN_DMAX], [ 1 ],
            [Define to 1 to enable dmax checks against __builtin_object_size(dest)])
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
        if test "x$enable_warn_dmax" = "xfalse" ; then
            AC_DEFINE([HAVE_WARN_DMAX], [ 1 ],
                [Define to 1 to enable dmax checks against __builtin_object_size(dest)])
        fi
	AC_DEFINE([HAVE_ERROR_DMAX], [ 1 ],
            [Define to 1 to make HAVE_WARN_DMAX fatal])
else
	AC_MSG_RESULT([no (default)])
fi

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
else
   AC_DEFINE([HAVE_C99], [1],
        [Defined to 1 when the compiler supports c99, mostly (...) macros])
   if test "x$ac_cv_prog_cc_c99" != "x"; then
      AC_MSG_RESULT([added $ac_cv_prog_cc_c99 to CFLAGS])
      AM_CFLAGS="$AM_CFLAGS $ac_cv_prog_cc_c99"
   fi
fi
dnl AM_CONDITIONAL([HAVE_C99], [test "x$ac_cv_prog_cc_c99=no" != "xno"])
AC_SUBST(HAVE_C99)

if test "x$ac_cv_prog_cc_c11" != "xno"; then
   AC_DEFINE([HAVE_C11], [1],
        [Defined to 1 when the compiler supports c11])
   if test "x$ac_cv_prog_cc_c11" != "x"; then
      AC_MSG_RESULT([added $ac_cv_prog_cc_c11 to CFLAGS])
      AM_CFLAGS="$AM_CFLAGS $ac_cv_prog_cc_c11"
   fi
fi
AC_SUBST(HAVE_C11)

dnl c++ needs less warnings
dnl AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
dnl   [#ifndef __cplusplus
dnl    #error "no C++"
dnl    #endif]])],
dnl   [ax_compiler_cxx=yes;],
dnl   [ax_compiler_cxx=no;])
dnl if test "$ax_compiler_cxx" = "yes" ; then
dnl   AC_DEFINE([HAVE_CXX], [1],
dnl         [Defined to 1 when the compiler is C++])
dnl   ax_cv_check_cflags__Wnested_externs=no
dnl   ax_cv_check_cflags__Wmissing_prototypes=no
dnl   ax_cv_check_cflags__Wstrict_prototypes=no
dnl   ax_cv_check_cflags__Wimplicit_function_declaration=no
dnl   ax_cv_check_cflags__Wdeclaration_after_statement=no
dnl   ax_cv_check_cflags__Wold_style_definition=no
dnl   ax_cv_check_cflags__Wjump_misses_init=no
dnl fi

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
dnl windows mult. decl are not properly sorted out yet
case $host_os in
  mingw*|cygwin*|msys*)
    AX_CHECK_COMPILE_FLAG([-Werror=unknown-warning-option],
        [ax_ccf_err="-Werror=unknown-warning-option"],[ax_ccf_err=""])
    dnl gcc 3.4 cannot do -Wno-attributes
    AX_APPEND_COMPILE_FLAGS([-Wno-attributes -Wno-discarded-qualifiers],
                            [WARN_CFLAGS],[$ax_ccf_err])
    dnl AM_LDFLAGS=`echo $AM_LDFLAGS|sed -e"s/-Wl,--as-needed -Wl,--no-as-needed//"`
    ;;
esac
AX_GCC_BUILTIN(__builtin_object_size)
dnl against -Werror,-Wgcc-compat fortify errors, only on clang
INSERT_OBJECT_SIZE="#undef HAVE___BUILTIN_OBJECT_SIZE"
if test x$ax_cv_have___builtin_object_size = xyes; then
    AX_CHECK_COMPILE_FLAG([-Werror=unknown-warning-option],
        [ax_ccf_err="-Werror=unknown-warning-option"],[ax_ccf_err=""])
    AX_CHECK_COMPILE_FLAG([-Wuser-defined-warnings],
        [have_user_defined_warnings="yes"],[have_user_defined_warnings="no"])
    AX_CHECK_COMPILE_FLAG([$ax_ccf_err -Wgcc-compat],
        [WARN_CFLAGS="$WARN_CFLAGS -Wno-gcc-compat"],[])
    if test $have_user_defined_warnings = yes; then
        AX_APPEND_COMPILE_FLAGS([-Wno-error=user-defined-warnings],
            [WARN_CFLAGS],[$ax_ccf_err])
        AC_DEFINE([HAVE_USER_DEFINED_WARNINGS], [1], [Uses -Wuser-defined-warnings])
    else
        AC_DEFINE([HAVE_USER_DEFINED_WARNINGS], [0], [Uses -Wuser-defined-warnings])
    fi
    INSERT_OBJECT_SIZE="# define HAVE___BUILTIN_OBJECT_SIZE 1"
else
    if test "x$enable_warn_dmax" = "xtrue" ; then
        AC_DEFINE([HAVE_WARN_DMAX], [ 0 ])
        AC_MSG_RESULT([disable-warn-dmax])
    fi
fi
AC_SUBST(INSERT_OBJECT_SIZE)

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
      AC_DEFINE([HAVE_ATTRIBUTE_DIAGNOSE_IF], [1],
                [Defined to 1 when the compiler supports attribute diagnose_if, since clang-5])
    ],
    [ AC_MSG_RESULT([no])
      have_attribute_diagnose_if=no
      AC_DEFINE([HAVE_ATTRIBUTE_DIAGNOSE_IF], [0],
                [Defined to 1 when the compiler supports attribute diagnose_if, since clang-5])
    ],
    [ AC_MSG_RESULT([no (cross)])
      have_attribute_diagnose_if=no
      AC_DEFINE([HAVE_ATTRIBUTE_DIAGNOSE_IF], [0],
                [Defined to 1 when the compiler supports attribute diagnose_if, since clang-5])
    ])
CFLAGS="$old_cflags"
AM_CONDITIONAL([HAVE_ATTRIBUTE_DIAGNOSE_IF], [test "x$have_attribute_diagnose_if" = "xyes"])


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
                  langinfo.h])

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
AC_TYPE_UINTPTR_T
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

AC_MSG_NOTICE([Check library functions])
# ===============================================
AC_FUNC_MEMCMP
dnl no header needed as is checks if the linker can find it
dnl mbstowcs missing on cygwin64
dnl vsnprintf_s vsnwprintf_s strtok_s wcstok_s deviate in mingw from c11
AC_CHECK_FUNCS([memset strcmp strcasecmp strcasestr strcspn strpbrk strspn \
                strnstr strnlen strrchr memrchr strstr wmemchr wmemcmp wcscmp wcsstr \
                vswprintf vsnwprintf vswscanf mbsrtowcs mbstowcs towlower towupper \
                towctrans bcmp secure_getenv timingsafe_memcmp timingsafe_bcmp \
                asctime_r ctime_r gmtime_r localtime_r memccpy \
                memset_s memcpy_s memmove_s memzero_s memchr_s memrchr_s memccpy_s \
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
		[[
return (towupper(0x1C80) == 0x412) ? 0 : 1;
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
	*)   if test x$ac_cv_type_wchar_t = xyes -a x$ac_cv_func_vswprintf = xyes
             then
               enable_wchar=true
             else
               AC_MSG_WARN([no proper wchar support])
               enable_wchar=false
             fi ;;
	esac], [enable_wchar=true])
AC_MSG_CHECKING([for --disable-wchar or broken wchar usability])
dnl catch mingw32 1.0 with crippled wchar
if test x$enable_wchar = xtrue -a x$ac_cv_func_vswprintf = xyes \
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

# Define these substations here to keep all version information in
# one place. For information on how to properly maintain the library
# version information, refer to the libtool manual, section "Updating
# library version information":
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
AC_SUBST([SAFEC_SO_VERSION], [3:3:0])
AC_SUBST([SAFEC_API_VERSION], [3.3])

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
fi

# Output files
# ===============================================

# Generate two configuration headers; one for building the library
# itself with an autogenerated template, and a second one that will
# be installed alongside the library.
# This does the #define/#undef magic
AC_CONFIG_HEADERS([config.h])

# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
# This does the @MACRO@ expansion
AC_CONFIG_FILES([safec-${SAFEC_API_VERSION}.pc:safec.pc.in
		 include/safe_config.h
                 include/safe_lib_errno.h
                 include/safe_types.h
                 Makefile
                 src/Makefile
                 tests/Makefile])

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_CONDITIONAL([HAVE_POD2MAN], [test -n "POD2MAN"])
AM_CONDITIONAL([HAVE_WINE], [test -n "$WINE"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([Doxyfile])])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([doc/footer])])

AC_OUTPUT
