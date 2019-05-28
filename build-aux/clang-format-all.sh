#!/bin/bash
#
# clang-format-all: a tool to run clang-format on an entire project
# Copyright (C) 2016 Evan Klitzke <evan@eklitzke.org>
# Copyright (C) 2019 Reini Urban <rurban@cpan.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# call with bash build-aux/clang-format-all.sh
dirs="src include tests"

# Variable that will hold the name of the clang-format command
FMT=""

# Some distros just call it clang-format. Others (e.g. Ubuntu) are insistent
# that the version number be part of the command. We prefer clang-format if
# that's present, otherwise we work backwards from highest version to lowest
# version. macports adds an -mp suffix and has newer versions, so prefer that.
for clangfmt in clang-format{-mp,}{-{devel,8,7,6,5},-{8,7,6,5,4,3}.{9,8,7,6,5,4,3,2,1,0},}; do
    if which "$clangfmt" &>/dev/null; then
        FMT="$clangfmt"
        break
    fi
done

# Check if we found a working clang-format
if [ -z "$FMT" ]; then
    echo "failed to find clang-format"
    exit 1
fi

# Find a dominating file, starting from a given directory and going up.
find-dominating-file() {
    if [ -r "$1"/"$2" ]; then
        return 0
    fi
    if [ "$1" = "/" ]; then
        return 1
    fi
    find-dominating-file "$(realpath "$1"/..)" "$2"
    return $?
}

pushd tests; grep -l ' EXPECT_BOS.* EXPECT_BOS' test_*.c > .format-ignore; popd

# Run clang-format -i on all of the things
for dir in $dirs; do
    pushd "${dir}"
    if ! find-dominating-file . .clang-format; then
        echo "Failed to find dominating .clang-format starting at $PWD"
        continue
    fi
    # skip tests/.format-ignore (no spaces in filenames allowed)
    ign=
    if [ -f .format-ignore ]; then
        ign=`perl -n00 -e'print q(-o ),join(q( -o ),split/\n/,$_)' .format-ignore`
    fi
    find . \
         \( -name '*.c' \
         -o -name '*.h' \) \
         -a \! \( -name 'unw*.h' \
               -o -name 'hangul.h' $ign \) \
         -exec "${FMT}" -i -verbose '{}' \;
    echo "post clang-format fixups (clang-format bugs)"
    sed -i -e's,IGNORE(-Wcast - align),IGNORE(-Wcast-align),;' \
           -e's,IGNORE(-Wuser - defined - warnings),IGNORE(-Wuser-defined-warnings),;' \
        `git grep -l 'DIAG_IGNORE' . | grep -v all.sh`
    popd &>/dev/null
done

rm tests/.format-ignore
