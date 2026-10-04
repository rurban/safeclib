#!/bin/sh
# update-u8-tables.sh -- refresh the u8 Unicode data tables.
#
# Run by `make -C src update-u8-tables` (see src/Makefile.am, under
# ENABLE_U8). Needs network access to fetch the latest UCD files, so
# it's not part of a normal build -- maintainer-only. Extracted out of
# src/Makefile.am's recipe, rather than inline shell, to keep the
# target's body within checkmake's maxbodylength.
set -eu

srcdir="${1:?usage: $0 SRCDIR PERL}"
perl="${2:?usage: $0 SRCDIR PERL}"

cd "$srcdir"
test -e Scripts.txt || wget -q https://www.unicode.org/Public/UCD/latest/ucd/Scripts.txt
test -e GraphemeBreakProperty.txt || wget -q https://www.unicode.org/Public/UCD/latest/ucd/auxiliary/GraphemeBreakProperty.txt
"$perl" extu8/gen-u8scripts.pl
"$perl" extu8/gen-u8gbreaks.pl
