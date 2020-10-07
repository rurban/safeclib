#!/bin/sh
#
# version-gen.sh - figure out package version number
#
# 2012 Jonathan Toppins <jtoppins@users.sourceforge.net>
#
# Copyright (c) 2012 by Cisco Systems, Inc
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

# Get latest version number of the code, either from the repository or
# from a file stored in the source distribution. Prefer the repository
# generated version over the version stored in the file and print the
# version to stdout.
#
# Parameters
# ----------
# tarball-version   - file name of the version file released with a source
#		      distribution.
# sed-normalization - sed normalization script passed to sed via '-e'
#
# Version Numbering
# -----------------
# Version number format: Major.Minor.Patch[_gCOMMITHASH[_dirty]]
# where _dirty denotes uncommitted changes.
#
# Detailed Description
# --------------------
# Calculates the current version number.  If possible, this is the
# output of 'git describe', modified to conform with the versioning
# scheme specified above.  If 'git describe' returns an error
# (most likely because we're in an unpacked copy of a release tarball,
# rather than in a git working copy), then we fall back on reading the
# contents of the tarball-version file passed in via the command line.
#
# Note that tarball-version file should *not* be checked into git;
# please add it to your top-level .gitignore file.
#
# To have this script work with autotools you will want to add the following.
# To you configure.ac file:
#
#	AC_INIT([project-name],
#		m4_esyscmd([path/to/version-gen.sh .tarball-version]),
#		[bug-email@example.com])
#
# Add the following to your Makefile.am, so that .version will be present
# for dependencies, and so that .version and .tarball-version will exist
# in distribution tarballs.
#
#	EXTRA_DIST = $(top_srcdir)/.version
#	BUILT_SOURCES = $(top_srcdir)/.version
#	$(top_srcdir)/.version:
#		echo $(VERSION) > $@-t && mv $@-t $@
#	dist-hook:
#		echo $(VERSION) > $(distdir)/.tarball-version
#

# Unit Tests:
# Note: Tests must be preformed with a clean work tree otherwise not all
# tests will pass.
#
# Test 1
# > git tag -a -m "test" "test1.2" HEAD && \
#	test "1.2" = "$(version-gen.sh --prefix test .test-version)" && \
#	echo "Test 1 - PASSED" || echo "Test 1 - FAILED"; git tag -d "test1.2"
#
# Test 2
# > git tag -a -m "test" "test1.2" HEAD^ && \
#	version-gen.sh --prefix test .test-version | grep -E "^1.2.1-.*$" | \
#	grep -E -v "dirty$" && \
#	echo "Test 2 - PASSED" || echo "Test 2 - FAILED"; git tag -d "test1.2"
#
# Test 3
# > git tag -a -m "test" "test1.2" HEAD^ && \
#	echo "test" >>README && \
#	version-gen.sh --prefix test .test-version | \
#	grep -E "^1.2.1-.*-dirty$" && \
#	echo "Test 3 - PASSED" || echo "Test 3 - FAILED"; \
#	git tag -d "test1.2"; git checkout -- README
#

SCRIPTVERSION=2020-10-07

prog=$0

version="version-gen.sh v$SCRIPTVERSION

Copyright (c) 2012 by Cisco Systems, Inc
All rights reserved. There is NO warranty. You may redistribute this
software under the terms of the MIT License.
For more information about these matters, see the file name COPYING."

usage="
Usage: $prog [OPTIONS] \$srcdir/.tarball-version [TAG-NORMALIZATION-SED-SCRIPT]

Print a version string.

Options:

    --prefix        prefix of git tags (default 'v')
    --abbrev        value passed to the --abbrev option of git describe
                    (default 6)

    --help          this output and exit
    --version       the version of this script and exit
"

prefix=v
abbrev=6

while test $# -gt 0; do
	case $1 in
	--help|-h)     echo "$usage"; exit 0;;
	--version|-v)  echo "$version"; exit 0;;
	--prefix)      shift; prefix="$1";;
	--abbrev)      shift; abbrev="$1";;
	-*)
		echo "$prog: Unknown option '$1'." >&2
		echo "$prog: Try '--help' for more information." >&2
		exit 1;;
	*)
		if test -z "$tarball_version_file"; then
			tarball_version_file="$1"
		elif test -z "$tag_sed_script"; then
			tag_sed_script="$1"
		else
			echo "$prog: extra non-option argument '$1'." >&2
			exit 1
		fi;;
	esac
	shift
done

if test -z "$tarball_version_file"; then
	echo "$usage"
	exit 1
fi

#export GIT_DIR=$(dirname $tarball_version_file)/.git
tag_sed_script="${tag_sed_script:-s/\(.?*\)-\(.*\)-\(g.*\)/\1_\2_\3/;s/-/_/g}"
nl='
'
v=

if test -f "$tarball_version_file"; then
	v=$(cat $tarball_version_file) || v=''
	case $v in
	*$nl*) v= ;; # reject multi-line output
	[0-9]*) ;;
	*) v= ;;
	esac
	test -z "$v" \
		&& echo "$prog: WARNING: $tarball_version_file is missing or damaged" 1>&2
fi

# read git version.
# if the repo has uncommitted changes, append '_dirty' to version.
if test -z "$v"; then
	v=$(git describe --tags --always --match "$prefix*" --abbrev=$abbrev \
		--dirty 2>/dev/null) || v=UNKNOWN
	v=$(echo "$v" | sed "$tag_sed_script" | sed "s/^$prefix//")
fi

echo "$v" | tr -d "$nl"
test "UNKNOWN" = "$v" && exit 1
exit 0
