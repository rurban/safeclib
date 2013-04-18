# configure.ac - Process with autoconf to produce a configure script
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
#                                               -*- Autoconf -*-

# Autoconf requirements
AC_PREREQ([2.60])

# information on the package
AC_INIT([Safe C Library],
         m4_esyscmd([build-tools/version-gen.sh .tarball-version]),
        [https://sourceforge.net/projects/safeclib/support],
        [libsafec],
        [http://sourceforge.net/projects/safeclib/])

# Configure the configure script
# ===============================================

# unique source file --- primitive safety check
AC_CONFIG_SRCDIR([src/safeclib/memcmp_s.c])

# extra autoconf macros are kept here
AC_CONFIG_MACRO_DIR([m4])

# place to put some extra build scripts
AC_CONFIG_AUX_DIR([build-tools])

# Other tool initializations
# ===============================================

# really severe build strictness also place generated object files (.o)
# into the same directory as their source files, in order to avoid
# collisions when non-recursive make is used.
AM_INIT_AUTOMAKE([1.10 no-define foreign subdir-objects dist-bzip2 -Wall
		  -Werror])

# Check if automake supports 'pretty' builds, if so enable them by default
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Enable LibTool as we are building a library
#LT_PREREQ([2.2.6])    # TODO: don't really know what to put here so leave
		       # it out for now
LT_INIT


# Global Variable & Defaults
# ===============================================

# Keep this in sync with what is passed in AC_INIT
TARBALL_VERSION_FILE=".tarball-version"

# Initial values for automake variables AM_*
AM_CPPFLAGS="-I\$(top_srcdir)/include"
# Not all code supports -Werror so each makefile enables it as appropriate.
# For debugging add -Werror to this and then go fix those errors :)
AM_CFLAGS="-Wall"


# Configure options
# ===============================================

# User switches
# AC_ARG_ENABLE(gcov,
# 	AS_HELP_STRING([--enable-gcov],
# 		       [turn on test coverage @<:@default=no@:>@]),
# 	[case "${enableval}" in
# 	yes) enable_gcov=true ;;
# 	no)  enable_gcov=false ;;
# 	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-gcov]) ;;
# 	esac], [enable_gcov=false ])
#
# if test "x$enable_gcov" = "xtrue" ; then
# 	if test -n "$GCC"; then
# 		AC_MSG_ERROR([gcov only works if gcc is used])
# 	fi
#
# 	GCOV_CFLAGS="-fprofile-arcs -ftest-coverage"
# 	AC_SUBST(GCOV_CFLAGS)
#
# 	dnl libtool 1.5.22 and lower strip -fprofile-arcs from the flags
# 	dnl passed to the linker, which is a bug; -fprofile-arcs implicitly
# 	dnl links in -lgcov, so we do it explicitly here for the same effect
# 	GCOV_LIBS=-lgcov
# 	AC_SUBST(GCOV_LIBS)
# fi
# AM_CONDITIONAL(ENABLE_GCOV, test "x$enable_gcov" = "xtrue")

AC_ARG_ENABLE(debug,
	AS_HELP_STRING([--enable-debug],
	               [compile library w/ debugging symbols (-g)
	                @<:@default=no@:>@]),
	[case "${enableval}" in
	yes) enable_debug=true ;;
	no)  enable_debug=false ;;
	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
	esac], [enable_debug=false ])
if test "x$enable_debug" = "xtrue" ; then
	DEBUG_CFLAGS="-g -O2"
	AM_CPPFLAGS="${AM_CPPFLAGS} -DDEBUG"
else
	# default release optimization
	DEBUG_CFLAGS="-O2"
fi

if test "-g -O2" == "$CFLAGS"; then
	# by doing it this way we suppress automake's assumption that if
	# CFLAGS is empty lets compile the code with '-g -O2'.
	CFLAGS="${DEBUG_CFLAGS}"
else
	AC_MSG_WARN([CFLAGS is not empty, will attempt to apply optimization
	             flags to 'AM_CFLAGS' but depending on the contents of
	             CFLAGS these may be overridden.
	             CFLAGS = '$CFLAGS'])
	AM_CFLAGS="${AM_CFLAGS} ${DEBUG_CFLAGS}"
fi
AM_CONDITIONAL(ENABLE_DEBUG, test "x$enable_debug" = "xtrue")

# AC_ARG_ENABLE(extensions,
# 	AS_HELP_STRING([--enable-extensions],
# 		       [This library contains additional functions not defined
# 			in the C11 specification, enable this to have access
# 			to those functions @<:@default=no@:>@]),
# 	[case "${enableval}" in
# 	yes) enable_extensions=true ;;
# 	no)  enable_extensions=false ;;
# 	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-extensions]) ;;
# 	esac], [enable_extensions=false ])
#
# if test "x$enable_extensions" = "xtrue" ; then
# 	AC_DEFINE([ENABLE_EXTS], [ 1 ],
# 		  [Define to 1 to enable additional functions not defined
# 		   in the C11 appendix K specification])
# fi
# AM_CONDITIONAL(ENABLE_EXTS, test "x$enable_extensions" = "xtrue")
#
# AC_ARG_ENABLE(memmax,
# 	AS_HELP_STRING([--enable-memmax],
# 		       [specify the largest object size allowed for the
# 			mem* functions @<:@default=256MB@:>@]),
# 	[case "${enableval}" in
# 	[0-9]*) enable_memmax=${enableval} ;;
# 	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-memmax]) ;;
# 	esac], [enable_memmax="(256UL << 20)" ])
#
# AC_DEFINE([RSIZE_MAX_MEM], [ $enable_memmax ],
# 	  [Maximum object size the mem* functions will allow,
# 	   default is 256MB])
#
# AC_ARG_ENABLE(strmax,
# 	AS_HELP_STRING([--enable-strmax],
# 		       [specify the largest object size allowed for the
# 			str* functions @<:@default=4KB@:>@]),
# 	[case "${enableval}" in
# 	[0-9]*) enable_memstr=${enableval} ;;
# 	*)   AC_MSG_ERROR([bad value ${enableval} for --enable-memstr]) ;;
# 	esac], [enable_memstr="(4UL << 10)" ])
#
# AC_DEFINE([RSIZE_MAX_STR], [ $enable_memstr ],
# 	  [Maximum object size the str* functions will allow,
# 	   default is 4KB])
#

# This option works however, it doesn't make sense to enable it until the
# header files are built and this value is added to it. That way users of
# the library will know what options have been enabled.
#
# AC_ARG_ENABLE(nullslack,
# 	AS_HELP_STRING([--disable-nullslack],
# 		       [Null out the remaining part of a string if it is
# 			not completely used @<:@default=yes@:>@]),
# 	[case "${enableval}" in
# 	yes) enable_nullslack=true ;;
# 	no)  enable_nullslack=false ;;
# 	*)   AC_MSG_ERROR([bad value ${enableval} for --disable-nullslack]) ;;
# 	esac], [enable_nullslack=true ])
#
# if test "x$enable_nullslack" = "xtrue" ; then
# 	AC_DEFINE([SAFECLIB_STR_NULL_SLACK], [ 1 ],
# 		  [Define to 1 to null out the remaining part of a string
# 		   buffer if it is not completely used])
# fi

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

# Checks for libraries
# ===============================================
# None

AC_MSG_NOTICE([Check header files])
# ===============================================
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_HEADER_TIME
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([stdlib.h     \
                  memory.h     \
                  string.h     \
                  limits.h     \
                  sys/types.h  \
                  inttypes.h   \
                  stdint.h     \
                  errno.h
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
AC_TYPE_UINTPTR_T
AC_CHECK_TYPE([errno_t], [], [], [[#include <errno.h>]])

FALLBACK_ERRNO_T=""
if test "x$ac_cv_type_errno_t" = "xno"; then
	FALLBACK_ERRNO_T="typedef int errno_t;"
fi
AC_SUBST(FALLBACK_ERRNO_T)

AC_MSG_NOTICE([Check library functions])
# ===============================================
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([memset strcasecmp strcspn strpbrk strspn strstr])

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
AC_SUBST([SAFEC_SO_VERSION], [1:0:0])
AC_SUBST([SAFEC_API_VERSION], [1.0])

# Automake variables, these variables get automagically included at the top
# of all automake generated make files. This is why you don't see them
# referenced in Makefile.am files.
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])

# Some build debugging output
if test "x$enable_debug_build" = "xtrue"; then
	AC_MSG_RESULT([AM_CPPFLAGS = $AM_CPPFLAGS])
	AC_MSG_RESULT([AM_CFLAGS = $AM_CFLAGS])
	AC_MSG_RESULT([CFLAGS = $CFLAGS])
fi

# Output files
# ===============================================

# Generate two configuration headers; one for building the library
# itself with an autogenerated template, and a second one that will
# be installed alongside the library.
# AC_CONFIG_HEADERS([config.h safecconfig.h])
AC_CONFIG_HEADERS([config.h])

# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
AC_CONFIG_FILES([safec-${SAFEC_API_VERSION}.pc:safec.pc.in
                 include/safe_lib_errno.h
                 include/safe_types.h
                 Makefile
                 src/Makefile
                 tests/Makefile])
AC_OUTPUT
